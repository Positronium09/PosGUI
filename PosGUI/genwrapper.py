#!/usr/bin/env python3
"""Generate a PGUI wrapper for a WinRT projected type, straight from Windows.winmd.

    python genwrapper.py Visual
    python genwrapper.py Windows.UI.Composition.SpriteVisual
    python genwrapper.py Visual --ixx
    python genwrapper.py Visual -o D:\\Code\\CPP\\PosGUI\\PosGUI

Emits the .ixx declarations and the .cpp definitions following PGUI conventions:
Result<T> + CallProjected, Get/Set pairs, the Enable/Disable/Is triple for booleans,
and a //TODO line for anything whose type has no PGUI mapping yet.

Requires: pip install dnfile
"""
import argparse
import os
import sys

try:
    import dnfile
except ImportError:
    sys.exit("dnfile is required:  pip install dnfile")

DEFAULT_WINMD = r"C:\Program Files (x86)\Windows Kits\10\UnionMetadata\10.0.26100.0\Windows.winmd"
DEFAULT_NAMESPACE = "Windows.UI.Composition"

# ---------------------------------------------------------------- type mapping
# winrt metadata type -> (PGUI type, needs explicit construction in the getter)
TYPE_MAP = {
    "bool": ("bool", False),
    "float": ("float", False),
    "double": ("double", False),
    "int32": ("std::int32_t", False),
    "uint32": ("std::uint32_t", False),
    "int64": ("std::int64_t", False),
    "uint64": ("std::uint64_t", False),
    "string": ("std::wstring", True),
    "Windows.Foundation.Numerics.Vector2": ("Point2F", True),
    "Windows.Foundation.Numerics.Vector3": ("Point3F", True),
    "Windows.Foundation.Numerics.Matrix3x2": ("Matrix3x2", True),
    "Windows.Foundation.Numerics.Matrix4x4": ("Matrix4x4", True),
    "Windows.Foundation.TimeSpan": ("std::chrono::milliseconds", True),
}

# property names whose float2 means extent, not position
SIZE_LIKE = ("Size", "SizeAdjustment")

# winrt runtime classes that have a PGUI wrapper: passed as `const X&`, unwrapped with .Get()
WRAPPER_TYPES = {
    "Windows.UI.Composition.CompositionObject": "CompositionObject",
    "Windows.UI.Composition.Visual": "Visual",
    "Windows.UI.Composition.ContainerVisual": "ContainerVisual",
    "Windows.UI.Composition.VisualCollection": "VisualCollection",
}

# winrt enums / classes already wrapped in PGUI, keyed by metadata name
KNOWN_WRAPPERS = {
    "Windows.UI.Composition.CompositionBackfaceVisibility": ("BackfaceVisibility", False),
    "Windows.UI.Composition.CompositionBorderMode": ("BorderMode", False),
    "Windows.UI.Composition.CompositionCompositeMode": ("CompositeMode", False),
}

SCALARS = {"bool", "float", "double", "std::int32_t", "std::uint32_t",
           "std::int64_t", "std::uint64_t"}

# only CompositionObject is templated on the projected type; every other PGUI
# wrapper base is a plain class. Bases that do not live in their own module:
BASE_MODULE = {}

ELEM = {
    0x01: "void", 0x02: "bool", 0x03: "char16", 0x04: "int8", 0x05: "uint8",
    0x06: "int16", 0x07: "uint16", 0x08: "int32", 0x09: "uint32",
    0x0A: "int64", 0x0B: "uint64", 0x0C: "float", 0x0D: "double",
    0x0E: "string", 0x1C: "object",
}


def decompress_uint(data, i):
    b = data[i]
    if b & 0x80 == 0:
        return b, i + 1
    if b & 0xC0 == 0x80:
        return ((b & 0x3F) << 8) | data[i + 1], i + 2
    return (((b & 0x1F) << 24) | (data[i + 1] << 16) | (data[i + 2] << 8) | data[i + 3]), i + 4


# ------------------------------------------------------------------- metadata
class Winmd:
    def __init__(self, path):
        self.t = dnfile.dnPE(path).net.mdtables
        self.by_name = {}
        for idx, row in enumerate(self.t.TypeDef.rows):
            self.by_name[self.full(row)] = (idx + 1, row)

    @staticmethod
    def full(row):
        ns = getattr(row, "TypeNamespace", None)
        return f"{ns}.{row.TypeName}" if ns else row.TypeName

    def resolve_name(self, name):
        if name in self.by_name:
            return name
        if "." not in name:
            cand = f"{DEFAULT_NAMESPACE}.{name}"
            if cand in self.by_name:
                return cand
            hits = [k for k in self.by_name if k.rsplit(".", 1)[-1] == name]
            if len(hits) == 1:
                return hits[0]
            if hits:
                sys.exit("ambiguous type name; candidates:\n  " + "\n  ".join(sorted(hits)[:20]))
        sys.exit(f"type not found in metadata: {name}")

    def _coded_name(self, tag, rid):
        if tag == 0:
            return self.full(self.t.TypeDef.rows[rid - 1])
        if tag == 1:
            return self.full(self.t.TypeRef.rows[rid - 1])
        return f"<typespec#{rid}>"

    def parse_type(self, data, i):
        et = data[i]
        i += 1
        if et in ELEM:
            return ELEM[et], i
        if et in (0x11, 0x12):
            coded, i = decompress_uint(data, i)
            return self._coded_name(coded & 0x3, coded >> 2), i
        if et == 0x1D:
            inner, i = self.parse_type(data, i)
            return f"{inner}[]", i
        if et == 0x15:
            i += 1
            coded, i = decompress_uint(data, i)
            base = self._coded_name(coded & 0x3, coded >> 2)
            argc, i = decompress_uint(data, i)
            args = []
            for _ in range(argc):
                a, i = self.parse_type(data, i)
                args.append(a)
            return f"{base}<{', '.join(args)}>", i
        return f"<elem 0x{et:02x}>", i

    def property_type(self, prop_row):
        blob = prop_row.Type
        data = bytes(getattr(blob, "value", None) or getattr(blob, "raw_data", b""))
        i = 1
        _, i = decompress_uint(data, i)
        while i < len(data) and data[i] in (0x1F, 0x20):
            i += 1
            _, i = decompress_uint(data, i)
        return self.parse_type(data, i)[0]

    def base_of(self, full_name):
        _, row = self.by_name[full_name]
        ext = getattr(row, "Extends", None)
        if ext is None or ext.row is None or not hasattr(ext.row, "TypeName"):
            return None
        return self.full(ext.row)

    def ancestors(self, full_name):
        """Base runtime classes, nearest first, excluding System.Object."""
        out, cur = [], self.base_of(full_name)
        while cur and cur != "System.Object" and cur in self.by_name:
            out.append(cur)
            cur = self.base_of(cur)
        return out

    def interfaces_of(self, full_name):
        rid, _ = self.by_name[full_name]
        out = []
        for ii in self.t.InterfaceImpl.rows:
            if ii.Class.row_index != rid:
                continue
            r = ii.Interface.row
            if r is not None and hasattr(r, "TypeName"):
                out.append(self.full(r))
        return out

    def properties_of(self, full_name):
        entry = self.by_name.get(full_name)
        if entry is None:
            return []
        rid = entry[0]
        # index MethodSemantics by property rid once
        if not hasattr(self, "_sem"):
            self._sem = {}
            for ms in self.t.MethodSemantics.rows:
                a = ms.Association
                if a.table is None or a.table.name != "Property":
                    continue
                sem = ms.Semantics
                g, s = self._sem.get(a.row_index, (False, False))
                self._sem[a.row_index] = (g or sem.msGetter, s or sem.msSetter)
        out = []
        for pm in self.t.PropertyMap.rows:
            if pm.Parent.row_index != rid:
                continue
            for pidx in pm.PropertyList:
                if pidx.row is None:
                    continue
                g, s = self._sem.get(pidx.row_index, (False, False))
                out.append((str(pidx.row.Name), self.property_type(pidx.row), g, s))
        return out

    def method_sig(self, m_row):
        blob = m_row.Signature
        data = bytes(getattr(blob, "value", None) or getattr(blob, "raw_data", b""))
        i = 0
        flags = data[i]
        i += 1
        if flags & 0x10:                      # GENERIC
            _, i = decompress_uint(data, i)
        argc, i = decompress_uint(data, i)
        ret, i = self.parse_type(data, i)
        params = []
        for _ in range(argc):
            while i < len(data) and data[i] in (0x1F, 0x20):
                i += 1
                _, i = decompress_uint(data, i)
            p, i = self.parse_type(data, i)
            params.append(p)
        return ret, params

    def methods_of(self, full_name):
        """[(name, return_type, [(param_type, param_name)])] excluding property/event accessors."""
        entry = self.by_name.get(full_name)
        if entry is None:
            return []
        out = []
        for midx in entry[1].MethodList:
            m = midx.row
            if m is None:
                continue
            name = str(m.Name)
            if name.startswith(("get_", "put_", "add_", "remove_", ".")):
                continue
            ret, ptypes = self.method_sig(m)
            names = [str(p.row.Name) for p in m.ParamList
                     if p.row is not None and p.row.Sequence != 0]
            while len(names) < len(ptypes):
                names.append(f"arg{len(names)}")
            out.append((name, ret, list(zip(ptypes, names))))
        return out

    def class_methods(self, full_name):
        seen, out = set(), []
        for iface in self.interfaces_of(full_name):
            for m in self.methods_of(iface):
                if m[0] not in seen:
                    seen.add(m[0])
                    out.append(m)
        return sorted(out, key=lambda m: m[0])

    def class_properties(self, full_name):
        seen, out = set(), []
        for iface in self.interfaces_of(full_name):
            for name, ty, g, s in self.properties_of(iface):
                if name not in seen:
                    seen.add(name)
                    out.append((name, ty, g, s))
        return sorted(out, key=lambda p: p[0])


# ------------------------------------------------------------------ generation
def map_type(winrt_type, prop_name):
    if winrt_type in WRAPPER_TYPES:
        return (WRAPPER_TYPES[winrt_type], True)
    if winrt_type in KNOWN_WRAPPERS:
        return KNOWN_WRAPPERS[winrt_type]
    if winrt_type == "Windows.Foundation.Numerics.Vector2" and prop_name.endswith(SIZE_LIKE):
        return ("SizeF", True)
    return TYPE_MAP.get(winrt_type, (None, False))


def short(winrt_type):
    return winrt_type.rsplit(".", 1)[-1]


def bool_names(prop_name):
    base = prop_name[2:] if prop_name.startswith("Is") else prop_name
    if base.endswith("Enabled"):
        base = base[: -len("Enabled")]
    return f"Enable{base}", f"Disable{base}", prop_name


def arg_name(prop_name):
    return prop_name[0].lower() + prop_name[1:]


def is_scalar(pgui):
    return pgui in SCALARS or pgui in (v[0] for v in KNOWN_WRAPPERS.values())


def gen_free_functions(entries, indent="\t"):
    """Accessors whose return type is the class itself (or a descendant) cannot be members -
    std::expected needs a complete type. Emitted after the class, where the type is complete."""
    out = []
    for owner, name, wtype, g, s in entries:
        pgui = WRAPPER_TYPES[wtype]
        arg = owner[0].lower() + owner[1:]
        head = f"{indent}template <HandleDerivedFrom<WUC::{owner}> Projected>"
        if g:
            out.append(head)
            out.append(f"{indent}[[nodiscard]] auto Get{name}Of(const {owner}T<Projected>& {arg}) noexcept -> Result<{pgui}>")
            out.append(f"{indent}{{")
            out.append(f"{indent}\treturn CallProjected([&{arg}]")
            out.append(f"{indent}\t{{")
            out.append(f"{indent}\t\treturn {pgui}{{ {arg}.Get().{name}() }};")
            out.append(f"{indent}\t}});")
            out.append(f"{indent}}}")
        if s:
            value = arg_name(name)
            out.append(head)
            out.append(f"{indent}auto Set{name}Of(const {owner}T<Projected>& {arg}, const {pgui}& {value}) noexcept -> Result<void>")
            out.append(f"{indent}{{")
            out.append(f"{indent}\treturn CallProjected([&{arg}, &{value}]")
            out.append(f"{indent}\t{{")
            out.append(f"{indent}\t\t{arg}.Get().{name}({value}.Get());")
            out.append(f"{indent}\t}});")
            out.append(f"{indent}}}")
        out.append("")
    while out and out[-1] == "":
        out.pop()
    return out


def gen_members(props, indent="\t\t", force_todo=frozenset()):
    """Declarations with inline bodies - templated wrappers cannot split into a .cpp."""
    out = []

    def block(sig, body):
        out.append(f"{indent}{sig}")
        out.append(f"{indent}{{")
        out.append(f"{indent}\treturn CallProjected({body[0]}")
        out.append(f"{indent}\t{{")
        out.append(f"{indent}\t\t{body[1]}")
        out.append(f"{indent}\t}});")
        out.append(f"{indent}}}")

    for name, wtype, g, s in props:
        pgui, needs_wrap = map_type(wtype, name)
        if name in force_todo:
            pgui = None
        if pgui is None:
            out.append(f"{indent}//TODO {name} {short(wtype)}")
            out.append("")
            continue
        if pgui == "bool":
            en, dis, is_ = bool_names(name)
            block(f"auto {en}(const bool enabled = true) const noexcept -> Result<void>",
                  ("[this, enabled]", f"this->Get().{name}(enabled);"))
            out.append(f"{indent}auto {dis}() const noexcept -> Result<void> {{ return {en}(false); }}")
            block(f"[[nodiscard]] auto {is_}() const noexcept -> Result<bool>",
                  ("[this]", f"return this->Get().{name}();"))
            out.append("")
            continue
        if g:
            expr = f"{pgui}{{ this->Get().{name}() }}" if needs_wrap else f"this->Get().{name}()"
            block(f"[[nodiscard]] auto Get{name}() const noexcept -> Result<{pgui}>",
                  ("[this]", f"return {expr};"))
        if s:
            a = arg_name(name)
            scalar = is_scalar(pgui)
            param = f"const {pgui} {a}" if scalar else f"const {pgui}& {a}"
            cap = f"[this, {a}]" if scalar else f"[this, &{a}]"
            block(f"auto Set{name}({param}) const noexcept -> Result<void>",
                  (cap, f"this->Get().{name}({a});"))
        out.append("")
    while out and out[-1] == "":
        out.pop()
    return out


def gen_definitions(class_name, props, indent="\t"):
    out = []

    def block(sig, body):
        out.append(f"{indent}{sig}")
        out.append(f"{indent}{{")
        out.extend(f"{indent}\t{line}" for line in body)
        out.append(f"{indent}}}")
        out.append("")

    for name, wtype, g, s in props:
        pgui, needs_wrap = map_type(wtype, name)
        if pgui is None:
            continue
        if pgui == "bool":
            en, _, is_ = bool_names(name)
            block(f"auto {class_name}::{en}(const bool enabled) const noexcept -> Result<void>",
                  ["return CallProjected([this, enabled]", "{", f"\tGet().{name}(enabled);", "});"])
            block(f"auto {class_name}::{is_}() const noexcept -> Result<bool>",
                  ["return CallProjected([this]", "{", f"\treturn Get().{name}();", "});"])
            continue
        if g:
            expr = f"{pgui}{{ Get().{name}() }}" if needs_wrap else f"Get().{name}()"
            block(f"auto {class_name}::Get{name}() const noexcept -> Result<{pgui}>",
                  ["return CallProjected([this]", "{", f"\treturn {expr};", "});"])
        if s:
            a = arg_name(name)
            scalar = pgui in SCALARS or pgui in (v[0] for v in KNOWN_WRAPPERS.values())
            param = f"const {pgui} {a}" if scalar else f"const {pgui}& {a}"
            cap = f"[this, {a}]" if scalar else f"[this, &{a}]"
            block(f"auto {class_name}::Set{name}({param}) const noexcept -> Result<void>",
                  [f"return CallProjected({cap}", "{", f"\tGet().{name}({a});", "});"])
    while out and out[-1] == "":
        out.pop()
    return out


def gen_methods(methods, indent="\t\t"):
    out = []
    for name, ret, params in methods:
        mapped = [(map_type(t, "")[0], t, n) for t, n in params]
        ret_pgui = "void" if ret == "void" else map_type(ret, "")[0]

        if ret_pgui is None or any(p is None for p, _, _ in mapped):
            sig = ", ".join(f"{short(t)} {n}" for _, t, n in mapped)
            out.append(f"{indent}//TODO {name}({sig}) -> {short(ret)}")
            out.append("")
            continue

        decl, args, caps = [], [], ["this"]
        for pgui, wtype, n in mapped:
            arg = arg_name(n)
            if wtype in WRAPPER_TYPES:
                decl.append(f"const {pgui}& {arg}")
                args.append(f"{arg}.Get()")
                caps.append(f"&{arg}")
            elif is_scalar(pgui):
                decl.append(f"const {pgui} {arg}")
                args.append(arg)
                caps.append(arg)
            else:
                decl.append(f"const {pgui}& {arg}")
                args.append(arg)
                caps.append(f"&{arg}")

        call = f"this->Get().{name}({', '.join(args)});"
        result = "void" if ret_pgui == "void" else ret_pgui
        nodiscard = "" if ret_pgui == "void" else "[[nodiscard]] "
        if ret_pgui != "void":
            call = f"return {ret_pgui}{{ this->Get().{name}({', '.join(args)}) }};" \
                if ret in WRAPPER_TYPES or map_type(ret, "")[1] \
                else f"return this->Get().{name}({', '.join(args)});"

        out.append(f"{indent}{nodiscard}auto {name}({', '.join(decl)}) const noexcept -> Result<{result}>")
        out.append(f"{indent}{{")
        out.append(f"{indent}\treturn CallProjected([{', '.join(caps)}]")
        out.append(f"{indent}\t{{")
        out.append(f"{indent}\t\t{call}")
        out.append(f"{indent}\t}});")
        out.append(f"{indent}}}")
        out.append("")
    while out and out[-1] == "":
        out.pop()
    return out


def base_spec(class_name, base_wrapper):
    if base_wrapper in TEMPLATED_BASES:
        return f"{base_wrapper}<WUC::{class_name}>"
    return base_wrapper


def base_import(base_wrapper):
    owner = BASE_MODULE.get(base_wrapper, base_wrapper)
    if base_wrapper == "ProjectionHolder":
        return "import PGUI.ProjectionHolder;"
    return f"import PGUI.UI.VL.{owner};"


def render_ixx(class_name, base_wrapper, props, methods=(), free_fns=(), force_todo=frozenset()):
    """base_wrapper is the PGUI base WITHOUT the T suffix, e.g. 'Visual'."""
    base = "Wrapper<Projected>" if base_wrapper == "ProjectionHolder" else f"{base_wrapper}T<Projected>"
    lines = [
        "module;",
        "#include <winrt/Windows.UI.Composition.h>",
        "",
        f"export module PGUI.UI.VL.{class_name};",
        "",
        "import std;",
        "",
        "import PGUI.ErrorHandling;",
        "import PGUI.ProjectionHolder;",
        "import PGUI.Shape;",
        "import PGUI.Wrapper;",
        base_import(base_wrapper),
        "import PGUI.UI.VL.VLEnums;",
    ]

    referenced = set()
    for _, wtype, _, _ in props:
        if wtype in WRAPPER_TYPES:
            referenced.add(WRAPPER_TYPES[wtype])
    for _, ret, params in methods:
        for t in [ret] + [p for p, _ in params]:
            if t in WRAPPER_TYPES:
                referenced.add(WRAPPER_TYPES[t])
    referenced.discard(class_name)
    referenced.discard(base_wrapper)
    lines += [f"import PGUI.UI.VL.{w};" for w in sorted(referenced)]

    lines += [
        "",
        "namespace WUC = winrt::Windows::UI::Composition;",
        "",
        "export namespace PGUI::UI::VL",
        "{",
        f"\ttemplate <HandleDerivedFrom<WUC::{class_name}> Projected = WUC::{class_name}>",
        f"\tclass {class_name}T : public {base}",
        "\t{",
        "\t\tpublic:",
        f"\t\tusing Base = {base};",
        "\t\tusing Base::Base;",
        "",
    ]
    members = gen_members(props, force_todo=force_todo)
    method_lines = gen_methods(methods)
    if members and method_lines:
        members.append("")
    lines += members + method_lines
    lines += [
        "\t};",
        f"\tusing {class_name} = {class_name}T<>;",
    ]
    if free_fns:
        lines.append("")
        lines += gen_free_functions(free_fns)
    lines += ["}", ""]
    return "\n".join(lines)


def render_cpp(class_name, base_wrapper, props):
    lines = [
        "module;",
        "#include <winrt/Windows.UI.Composition.h>",
        "",
        f"module PGUI.UI.VL.{class_name};",
        "",
        "import std;",
        "",
        "import PGUI.ErrorHandling;",
        "import PGUI.ProjectionHolder;",
        "import PGUI.Shape;",
        base_import(base_wrapper),
        "import PGUI.UI.VL.VLEnums;",
        "",
        "namespace WUC = winrt::Windows::UI::Composition;",
        "",
        "namespace PGUI::UI::VL",
        "{",
    ]
    lines += gen_definitions(class_name, props)
    lines += ["}", ""]
    return "\n".join(lines)


def main():
    ap = argparse.ArgumentParser(description="Generate a PGUI wrapper from WinRT metadata.")
    ap.add_argument("type", help="projected type, e.g. Visual or Windows.UI.Composition.Visual")
    ap.add_argument("--winmd", default=DEFAULT_WINMD)
    ap.add_argument("--base", default=None, help="PGUI base wrapper (default: inferred)")
    ap.add_argument("--ixx", action="store_true", help="emit only the .ixx")
    ap.add_argument("--cpp", action="store_true", help="emit only the .cpp")
    ap.add_argument("-o", "--outdir", default=None,
                    help="write modules/UI/VL/<T>.ixx and src/UI/VL/<T>.cpp under this project root")
    args = ap.parse_args()

    md = Winmd(args.winmd)
    full_name = md.resolve_name(args.type)
    class_name = short(full_name)
    props = md.class_properties(full_name)

    base_meta = md.base_of(full_name)
    base_wrapper = args.base or (short(base_meta) if base_meta and base_meta != "System.Object"
                                 else "ProjectionHolder")

    methods = md.class_methods(full_name)

    # A property whose type is this class, or a class derived from it, cannot be a member:
    # std::expected needs a complete type, and the derived wrapper's module imports this one.
    free_fns, force_todo = [], set()
    for name, wtype, g, s in props:
        if WRAPPER_TYPES.get(wtype) == class_name:
            free_fns.append((class_name, name, wtype, g, s))
            force_todo.add(name)
        elif wtype in WRAPPER_TYPES and full_name in md.ancestors(wtype):
            force_todo.add(name)          # emitted as a free function in the descendant's file

    # ...and inherited properties whose type is THIS class get their free function here,
    # where both the ancestor template and this class are complete.
    for ancestor in md.ancestors(full_name):
        for name, wtype, g, s in md.class_properties(ancestor):
            if WRAPPER_TYPES.get(wtype) == class_name:
                free_fns.append((short(ancestor), name, wtype, g, s))

    ixx = render_ixx(class_name, base_wrapper, props, methods, free_fns, force_todo)

    if args.outdir:
        path = os.path.join(args.outdir, "modules", "UI", "VL", f"{class_name}.ixx")
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w", encoding="utf-8", newline="\r\n") as f:
            f.write(ixx)
        print(f"wrote {path}", file=sys.stderr)
        return

    print(ixx)

    unmapped = [(n, t) for n, t, _, _ in props if map_type(t, n)[0] is None]
    if unmapped:
        print(f"\n// {len(unmapped)} unmapped -> emitted as //TODO:", file=sys.stderr)
        for n, t in unmapped:
            print(f"//   {n}: {t}", file=sys.stderr)


if __name__ == "__main__":
    main()
