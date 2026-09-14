#!/usr/bin/env python3
"""Generate a PGUI wrapper for a WinRT projected type, straight from Windows.winmd.

    python genwrapper.py Visual
    python genwrapper.py Windows.UI.Composition.SpriteVisual
    python genwrapper.py Visual --ixx
    python genwrapper.py Visual -o D:\\Code\\CPP\\PosGUI\\PosGUI

    python genwrapper.py --status

Emits the .ixx following PGUI conventions: Result<T> + CallProjected, Get/Set pairs,
the Enable/Disable/Is triple for booleans, and a //TODO line for anything whose type
has no PGUI mapping yet. Every wrapper is a template, so there is no .cpp to emit.

Which types already have wrappers is discovered by scanning modules/UI/VL rather than
kept in a table here, matching each wrapper to its winrt type by the projected type in
its base specifier rather than by file or class name, which drift. --status reports
done / stub / missing for the whole namespace.

Requires: pip install dnfile
"""
import argparse
import os
import re
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
    "Windows.Foundation.Numerics.Quaternion": ("Quaternion", True),
    "Windows.Foundation.Point": ("Point2F", True),
    "Windows.Foundation.Size": ("SizeF", True),
    "Windows.Foundation.Rect": ("RectF", True),
    "Windows.Foundation.TimeSpan": ("std::chrono::milliseconds", True),
    "Windows.UI.Color": ("RGBA", True),
    "Windows.Graphics.PointInt32": ("Point2I", True),
    "Windows.Graphics.SizeInt32": ("SizeI", True),
    "Windows.Graphics.RectInt32": ("RectI", True),
}

# PGUI type -> the module it has to be imported from, when it is not a VL wrapper
TYPE_MODULE = {
    "Point2F": "PGUI.Shape", "Point2I": "PGUI.Shape", "Point3F": "PGUI.Shape",
    "SizeF": "PGUI.Shape", "SizeI": "PGUI.Shape", "RectF": "PGUI.Shape", "RectI": "PGUI.Shape",
    "Matrix3x2": "PGUI.Shape", "Matrix4x4": "PGUI.Shape", "Quaternion": "PGUI.Shape",
    "RGBA": "PGUI.UI.Color",
}

# property names whose float2 means extent, not position
SIZE_LIKE = ("Size", "SizeAdjustment")

SCALARS = {"bool", "float", "double", "std::int32_t", "std::uint32_t",
           "std::int64_t", "std::uint64_t"}

# Filled by discover(); see there for the shapes each one holds.
WRAPPER_TYPES = {}      # winrt metadata name -> PGUI class, for finished class wrappers only
WRAPPER_MODULE = {}     # PGUI class -> module to import it from
WRAPPER_DIR = {}        # PGUI class -> directory holding it, relative to modules/UI/VL
KNOWN_WRAPPERS = {}     # winrt metadata name -> (PGUI alias, needs explicit construction)
STUBS = {}              # winrt metadata name -> module of a file that exists but is empty
NAMESPACES = {}         # winrt metadata name -> module covering it as a namespace, not a class

# files under modules/UI/VL that are not type wrappers and never will be
NOT_WRAPPERS = {"VL", "VLEnums"}

MODULE_RE = re.compile(r"^\s*export\s+module\s+([\w.]+)\s*;", re.M)
ALIAS_RE = re.compile(r"^\s*namespace\s+(\w+)\s*=\s*([\w:]+)\s*;", re.M)
NS_MODULE_RE = re.compile(r"^\s*export\s+namespace\s+PGUI::UI::VL::(\w+)", re.M)
CLASS_RE = re.compile(
    r"^[ \t]*(?:template[ \t]*<(?P<targs>[^\n]*)>[ \t]*\r?\n)?"
    r"[ \t]*class[ \t]+(?P<name>\w+)[ \t]*:[ \t]*public[ \t]+"
    r"(?P<base>[\w:]+)[ \t]*<[ \t]*(?P<arg>[\w:]+)[ \t]*>",
    re.M)
TARG_DEFAULT_RE = re.compile(r"=\s*([\w:]+)\s*$")

ELEM = {
    0x01: "void", 0x02: "bool", 0x03: "char16", 0x04: "int8", 0x05: "uint8",
    0x06: "int16", 0x07: "uint16", 0x08: "int32", 0x09: "uint32",
    0x0A: "int64", 0x0B: "uint64", 0x0C: "float", 0x0D: "double",
    0x0E: "string", 0x1C: "object",
}


def name_candidates(*names):
    """Both spellings, both directions: ColorBrush <-> CompositionColorBrush."""
    out = set(names)
    out |= {"Composition" + n for n in list(out)}
    out |= {n[len("Composition"):] for n in list(out)
            if n.startswith("Composition") and len(n) > len("Composition")}
    return out


def resolve_projected(qualified, aliases, namespace=DEFAULT_NAMESPACE):
    """'WUC::SpriteVisual' -> 'Windows.UI.Composition.SpriteVisual' via the file's aliases."""
    if "::" not in qualified:
        return None
    alias, _, name = qualified.rpartition("::")
    return f"{aliases.get(alias, namespace)}.{name}"


def wrapped_type_of(text, aliases, namespace=DEFAULT_NAMESPACE):
    """(metadata name, PGUI name) read from the first wrapper class's base specifier.

    A plain class names the projection directly - CompositionObjectT<WUC::CompositionDrawingSurface>.
    A class template passes its own parameter down to the base, so the projection comes from
    that parameter's default instead - 'Projected = WUC::CompositionObject'.
    """
    m = CLASS_RE.search(text)
    if m is None:
        return None, None

    cls, targs = m.group("name"), m.group("targs")
    templated = targs is not None
    if templated:
        default = TARG_DEFAULT_RE.search(targs)
        projected = default.group(1) if default else None
    else:
        projected = m.group("arg")

    pgui = cls
    if templated:
        alias = re.search(rf"^\s*using\s+(\w+)\s*=\s*{re.escape(cls)}\s*<\s*>\s*;", text, re.M)
        pgui = alias.group(1) if alias else (cls[:-1] if cls.endswith("T") else cls)

    if projected is None:
        return None, pgui
    return resolve_projected(projected, aliases, namespace), pgui


def discover(vl_root, namespace=DEFAULT_NAMESPACE):
    """Scan modules/UI/VL for wrappers instead of trusting a hardcoded table.

    A wrapper is matched to its winrt type by the PROJECTED TYPE in its base specifier,
    not by the file or class name - those drift, and the base is the one spelling that
    cannot: SurfaceInterop.ixx holds a CompositionDrawingSurface, ColorBrush.ixx a
    CompositionColorBrush. See wrapped_type_of for the two shapes it reads.

    Name matching survives only as the fallback for a class whose base cannot be parsed,
    and for a stub, which declares no class to read a base from. A file whose export
    namespace carries a fourth segment (PGUI::UI::VL::Compositor) is namespace-style: it
    covers its type without a class, so it is recorded apart from WRAPPER_TYPES and never
    offered to the generator as a constructible type.
    """
    if not os.path.isdir(vl_root):
        print(f"warning: no VL directory at {vl_root}; nothing discovered", file=sys.stderr)
        return

    for dirpath, _, filenames in os.walk(vl_root):
        for fn in filenames:
            if not fn.endswith(".ixx"):
                continue
            stem = fn[:-4]
            if stem in NOT_WRAPPERS:
                continue
            path = os.path.join(dirpath, fn)
            with open(path, encoding="utf-8", errors="replace") as f:
                text = f.read()

            m = MODULE_RE.search(text)
            if m is None:
                print(f"warning: {path} has no export module declaration", file=sys.stderr)
                continue
            module = m.group(1)
            if module.rsplit(".", 1)[-1] in NOT_WRAPPERS:
                continue
            if not module.startswith("PGUI.UI.VL."):
                print(f"warning: {path} exports '{module}', expected a PGUI.UI.VL.* module",
                      file=sys.stderr)

            last = module.rsplit(".", 1)[-1]
            aliases = {a: ns.removeprefix("winrt::").replace("::", ".")
                       for a, ns in ALIAS_RE.findall(text)}

            ns_module = NS_MODULE_RE.search(text)
            if ns_module is not None:
                for name in name_candidates(ns_module.group(1)):
                    NAMESPACES.setdefault(f"{namespace}.{name}", module)
                continue

            wrapped, pgui = wrapped_type_of(text, aliases, namespace)
            if pgui is None:
                for name in name_candidates(stem, last):
                    STUBS.setdefault(f"{namespace}.{name}", module)
                continue

            if wrapped is not None:
                WRAPPER_TYPES[wrapped] = pgui
            else:
                print(f"warning: {path} declares {pgui} but its projected type could not be "
                      f"read from the base specifier; falling back to name matching",
                      file=sys.stderr)
                for name in name_candidates(stem, last, pgui):
                    WRAPPER_TYPES[f"{namespace}.{name}"] = pgui
            WRAPPER_MODULE[pgui] = module
            WRAPPER_DIR[pgui] = os.path.relpath(dirpath, vl_root)

    enums_path = os.path.join(vl_root, "VLEnums.ixx")
    if os.path.isfile(enums_path):
        with open(enums_path, encoding="utf-8", errors="replace") as f:
            for alias, winrt_name in re.findall(r"^\s*using\s+(\w+)\s*=\s*\w+::(\w+)\s*;", f.read(), re.M):
                KNOWN_WRAPPERS[f"{namespace}.{winrt_name}"] = (alias, False)


def split_generic(name):
    """'NS.IMap`2<A, NS.IKeyValuePair`2<B, C>>' -> ('NS.IMap`2', ['A', 'NS.IKeyValuePair`2<B, C>'])."""
    lt = name.find("<")
    if lt < 0 or not name.endswith(">"):
        return name, []
    args, depth, start = [], 0, lt + 1
    for i in range(lt + 1, len(name) - 1):
        ch = name[i]
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif ch == "," and depth == 0:
            args.append(name[start:i].strip())
            start = i + 1
    args.append(name[start:-1].strip())
    return name[:lt], args


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

    def parse_type(self, data, i, targs=()):
        """targs substitutes VAR (0x13) - the generic parameters of the interface being read."""
        et = data[i]
        i += 1
        if et in ELEM:
            return ELEM[et], i
        if et in (0x11, 0x12):
            coded, i = decompress_uint(data, i)
            return self._coded_name(coded & 0x3, coded >> 2), i
        if et == 0x13:
            idx, i = decompress_uint(data, i)
            return (targs[idx] if idx < len(targs) else f"<var {idx}>"), i
        if et == 0x1D:
            inner, i = self.parse_type(data, i, targs)
            return f"{inner}[]", i
        if et == 0x15:
            i += 1
            coded, i = decompress_uint(data, i)
            base = self._coded_name(coded & 0x3, coded >> 2)
            argc, i = decompress_uint(data, i)
            args = []
            for _ in range(argc):
                a, i = self.parse_type(data, i, targs)
                args.append(a)
            return f"{base}<{', '.join(args)}>", i
        return f"<elem 0x{et:02x}>", i

    def property_type(self, prop_row, targs=()):
        blob = prop_row.Type
        data = bytes(getattr(blob, "value", None) or getattr(blob, "raw_data", b""))
        i = 1
        _, i = decompress_uint(data, i)
        while i < len(data) and data[i] in (0x1F, 0x20):
            i += 1
            _, i = decompress_uint(data, i)
        return self.parse_type(data, i, targs)[0]

    def base_of(self, full_name):
        _, row = self.by_name[full_name]
        ext = getattr(row, "Extends", None)
        if ext is None or ext.row is None or not hasattr(ext.row, "TypeName"):
            return None
        return self.full(ext.row)

    def is_interface(self, full_name):
        entry = self.by_name.get(full_name)
        if entry is None:
            return False
        flags = getattr(entry[1], "Flags", None)
        return bool(flags is not None and getattr(flags, "tdInterface", False))

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
            if r is None:
                continue
            if hasattr(r, "TypeName"):
                out.append(self.full(r))
            elif hasattr(r, "Signature"):
                blob = r.Signature
                data = bytes(getattr(blob, "value", None) or getattr(blob, "raw_data", b""))
                out.append(self.parse_type(data, 0)[0])
        return out

    def properties_of(self, full_name):
        base, targs = split_generic(full_name)
        entry = self.by_name.get(base)
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
                out.append((str(pidx.row.Name), self.property_type(pidx.row, targs), g, s))
        return out

    def method_sig(self, m_row, targs=()):
        blob = m_row.Signature
        data = bytes(getattr(blob, "value", None) or getattr(blob, "raw_data", b""))
        i = 0
        flags = data[i]
        i += 1
        if flags & 0x10:                      # GENERIC
            _, i = decompress_uint(data, i)
        argc, i = decompress_uint(data, i)
        ret, i = self.parse_type(data, i, targs)
        params = []
        for _ in range(argc):
            while i < len(data) and data[i] in (0x1F, 0x20):
                i += 1
                _, i = decompress_uint(data, i)
            p, i = self.parse_type(data, i, targs)
            params.append(p)
        return ret, params

    def methods_of(self, full_name):
        """[(name, return_type, [(param_type, param_name)])] excluding property/event accessors."""
        base, targs = split_generic(full_name)
        entry = self.by_name.get(base)
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
            ret, ptypes = self.method_sig(m, targs)
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
                key = (m[0], tuple(t for t, _ in m[2]))
                if key not in seen:
                    seen.add(key)
                    out.append(m)
        return sorted(out, key=lambda m: (m[0], len(m[2])))

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
    """Last name segment without the `N arity; generic arguments shortened recursively."""
    base, targs = split_generic(winrt_type)
    name = base.rsplit(".", 1)[-1].split("`", 1)[0]
    return f"{name}<{', '.join(short(a) for a in targs)}>" if targs else name


def bool_names(prop_name):
    """Enable / Disable / Is triple. The reader has to read as a PREDICATE, so a
    property that is not already an Is... name gets Is<Base>Enabled rather than the
    bare property name: SnapToPixels -> EnableSnapToPixels, DisableSnapToPixels,
    IsSnapToPixelsEnabled. An already-predicate name is kept verbatim so IsVisible
    stays IsVisible instead of becoming IsVisibleEnabled.
    """
    if prop_name.startswith("Is"):
        base = prop_name[2:]
        if base.endswith("Enabled"):
            base = base[: -len("Enabled")]
        return f"Enable{base}", f"Disable{base}", prop_name

    base = prop_name[: -len("Enabled")] if prop_name.endswith("Enabled") else prop_name
    return f"Enable{base}", f"Disable{base}", f"Is{base}Enabled"


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
            out.append(f"{indent}[[nodiscard]] auto Set{name}Of(const {owner}T<Projected>& {arg}, const {pgui}& {value}) noexcept -> Result<void>")
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
            block(f"[[nodiscard]] auto {en}(const bool enabled = true) const noexcept -> Result<void>",
                  ("[this, enabled]", f"this->Get().{name}(enabled);"))
            out.append(f"{indent}[[nodiscard]] auto {dis}() const noexcept -> Result<void> {{ return {en}(false); }}")
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
            block(f"[[nodiscard]] auto Set{name}({param}) const noexcept -> Result<void>",
                  (cap, f"this->Get().{name}({a});"))
        out.append("")
    while out and out[-1] == "":
        out.pop()
    return out


def projected_name(winrt_type):
    """Windows.UI.Composition.X -> WUC::X, Windows.UI.Composition.Desktop.X -> WUC::Desktop::X."""
    return "WUC::" + winrt_type[len(DEFAULT_NAMESPACE) + 1:].replace(".", "::")


def gen_methods(methods, indent="\t\t", deferred_params=frozenset(), deferred_returns=frozenset()):
    """deferred_params: types taken as WrapperDerivedFrom<WUC::X> auto&, so the module never
    imports their wrapper. deferred_returns: types a member cannot return without that import."""
    out = []
    for name, ret, params in methods:
        mapped = [(("deferred" if t in deferred_params else map_type(t, "")[0]), t, n) for t, n in params]
        ret_pgui = "void" if ret == "void" else (None if ret in deferred_returns else map_type(ret, "")[0])

        if ret_pgui is None or any(p is None for p, _, _ in mapped):
            sig = ", ".join(f"{short(t)} {n}" for _, t, n in mapped)
            out.append(f"{indent}//TODO {name}({sig}) -> {short(ret)}")
            out.append("")
            continue

        decl, args, caps = [], [], ["this"]
        for pgui, wtype, n in mapped:
            arg = arg_name(n)
            if pgui == "deferred":
                decl.append(f"const WrapperDerivedFrom<{projected_name(wtype)}> auto& {arg}")
                args.append(f"{arg}.Get()")
                caps.append(f"&{arg}")
            elif wtype in WRAPPER_TYPES:
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
        nodiscard = "[[nodiscard]] "
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


ITERABLE = "Windows.Foundation.Collections.IIterable`1"
KEY_VALUE_PAIR = "Windows.Foundation.Collections.IKeyValuePair`2"


def gen_iterable(element, indent="\t\t"):
    """ToVector + ForEach over an IIterable<element>. A key/value element is unpacked, so
    ForEach takes a (key, value) callable and ToVector yields pairs."""
    def value_of(winrt_type, source):
        pgui, needs_wrap = map_type(winrt_type, "")
        if pgui is None:
            return None, None
        return pgui, (f"{pgui}{{ {source} }}" if needs_wrap else source)

    base, targs = split_generic(element)
    if base == KEY_VALUE_PAIR:
        key, key_expr = value_of(targs[0], "element.Key()")
        value, value_expr = value_of(targs[1], "element.Value()")
        if key is None or value is None:
            return [f"{indent}//TODO ToVector / ForEach over IIterable<{short(element)}>"]
        stored = f"std::pair<{key}, {value}>"
        signature = f"{key}, {value}"
        emplace_args = f"{key_expr}, {value_expr}"
        invoke_args = f"{key_expr}, {value_expr}"
    else:
        stored, expr = value_of(element, "element")
        if stored is None:
            return [f"{indent}//TODO ToVector / ForEach over IIterable<{short(element)}>"]
        signature = stored
        emplace_args = "element"
        invoke_args = expr

    return [
        f"{indent}[[nodiscard]] auto ToVector() const noexcept -> Result<std::vector<{stored}>>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this]",
        f"{indent}\t{{",
        f"{indent}\t\tauto elements = std::vector<{stored}>{{ }};",
        f"{indent}\t\tfor (const auto& element : this->Get())",
        f"{indent}\t\t{{",
        f"{indent}\t\t\telements.emplace_back({emplace_args});",
        f"{indent}\t\t}}",
        "",
        f"{indent}\t\treturn elements;",
        f"{indent}\t}});",
        f"{indent}}}",
        "",
        f"{indent}template <Callable<Result<void>({signature})> F>",
        f"{indent}[[nodiscard]] auto ForEach(F&& f) const noexcept -> Result<void>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this, &f] -> Result<void>",
        f"{indent}\t{{",
        f"{indent}\t\tfor (const auto& [i, element] : this->Get() | std::views::enumerate)",
        f"{indent}\t\t{{",
        f"{indent}\t\t\tif (auto result = std::invoke(f, {invoke_args});",
        f"{indent}\t\t\t\t!result.has_value())",
        f"{indent}\t\t\t{{",
        f"{indent}\t\t\t\treturn Unexpected{{ result.error().AddDetail(L\"iteration\", std::to_wstring(i)) }};",
        f"{indent}\t\t\t}}",
        f"{indent}\t\t}}",
        "",
        f"{indent}\t\treturn EmptyResult;",
        f"{indent}\t}}).and_then(std::identity{{ }});",
        f"{indent}}}",
    ]


def module_of(pgui_name):
    """Discovered module for a wrapper, falling back to the conventional name."""
    return WRAPPER_MODULE.get(pgui_name, f"PGUI.UI.VL.{pgui_name}")


def base_import(base_wrapper):
    if base_wrapper == "ProjectionHolder":
        return "import PGUI.ProjectionHolder;"
    return f"import {module_of(base_wrapper)};"


def import_rank(module):
    """Foundation modules first, then PGUI.UI.*, then the VL siblings - the order the
    hand-written wrappers already use."""
    if module.startswith("PGUI.UI.VL."):
        return 2, module
    if module.startswith("PGUI.UI."):
        return 1, module
    return 0, module


def target_module(class_name, base_wrapper):
    """A new wrapper belongs in its base's folder, so it inherits the base's module
    prefix: a Visual subclass whose base lives in PGUI.UI.VL.Visual.ContainerVisual
    becomes PGUI.UI.VL.Visual.<class_name>. Falls back to the flat top-level name.
    """
    base_module = WRAPPER_MODULE.get(base_wrapper, "")
    if base_module.startswith("PGUI.UI.VL.") and base_module.count(".") > 3:
        return f"{base_module.rsplit('.', 1)[0]}.{class_name}"
    return f"PGUI.UI.VL.{class_name}"


def target_dir(base_wrapper):
    """Folder for a new wrapper, relative to modules/UI/VL - its base's folder."""
    folder = WRAPPER_DIR.get(base_wrapper, ".")
    return "" if folder == "." else folder


def render_ixx(class_name, base_wrapper, props, methods=(), free_fns=(), force_todo=frozenset(),
               deferred_params=frozenset(), deferred_returns=frozenset(), iterable=None):
    """base_wrapper is the PGUI base WITHOUT the T suffix, e.g. 'Visual'."""
    base = "Wrapper<Projected>" if base_wrapper == "ProjectionHolder" else f"{base_wrapper}T<Projected>"

    body = [
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
    method_lines = gen_methods(methods, deferred_params=deferred_params, deferred_returns=deferred_returns)
    if members and method_lines:
        members.append("")
    iterable_lines = gen_iterable(iterable) if iterable else []
    if (members or method_lines) and iterable_lines:
        method_lines.append("")
    body += members + method_lines + iterable_lines
    body += [
        "\t};",
        f"\tusing {class_name} = {class_name}T<>;",
    ]
    if free_fns:
        body.append("")
        body += gen_free_functions(free_fns)
    body += ["}", ""]
    text = "\n".join(body)

    # Imports are derived from what the body actually names, so a wrapper that uses no
    # Shape type does not import PGUI.Shape and one with no members does not import
    # PGUI.ProjectionHolder. A fixed block is what left unused imports in the VL files.
    referenced = set()
    for _, wtype, _, _ in props:
        if wtype in WRAPPER_TYPES:
            referenced.add(WRAPPER_TYPES[wtype])
    for _, ret, params in methods:
        for t in [ret] + [p for p, _ in params]:
            if t in WRAPPER_TYPES and t not in deferred_params and t not in deferred_returns:
                referenced.add(WRAPPER_TYPES[t])
    referenced.discard(class_name)
    referenced.discard(base_wrapper)

    imports = {"PGUI.ErrorHandling", "PGUI.Wrapper", base_import(base_wrapper)[len("import "):-1]}
    if "CallProjected(" in text:
        imports.add("PGUI.ProjectionHolder")
    if "Callable<" in text:
        imports.add("PGUI.Utils")
    for pgui, module in TYPE_MODULE.items():
        if re.search(rf"\b{re.escape(pgui)}\b", text):
            imports.add(module)
    if any(re.search(rf"\b{re.escape(alias)}\b", text) for alias, _ in KNOWN_WRAPPERS.values()):
        imports.add("PGUI.UI.VL.VLEnums")
    imports |= {module_of(w) for w in referenced}

    lines = [
        "module;",
        "#include <winrt/Windows.UI.Composition.h>",
        "",
    ]
    if iterable:
        lines += ["#include <winrt/Windows.Foundation.Collections.h>", ""]
    lines += [
        f"export module {target_module(class_name, base_wrapper)};",
        "",
    ]
    if "std::" in text:
        lines += ["import std;", ""]
    lines += [f"import {m};" for m in sorted(imports, key=import_rank)]
    lines.append("")

    return "\n".join(lines) + "\n" + text


def report_status(md, namespace=DEFAULT_NAMESPACE):
    """done / stub / missing for every runtime class in the namespace."""
    classes, values = [], []
    for full_name, (_, row) in md.by_name.items():
        if not full_name.startswith(namespace + "."):
            continue
        if "." in full_name[len(namespace) + 1:]:
            continue
        flags = getattr(row, "Flags", None)
        if flags is not None and getattr(flags, "tdInterface", False):
            continue
        name = short(full_name)
        if name.startswith("I") and name[1:2].isupper():
            continue
        # enums extend System.Enum, structs/delegates System.ValueType or MulticastDelegate;
        # only runtime classes get a wrapper, the rest are aliased in VLEnums or used directly
        if md.base_of(full_name) in ("System.Enum", "System.ValueType", "System.MulticastDelegate"):
            values.append(full_name)
        else:
            classes.append(full_name)

    done = sorted(n for n in classes if n in WRAPPER_TYPES)
    namespaced = sorted(n for n in classes if n not in WRAPPER_TYPES and n in NAMESPACES)
    stub = sorted(n for n in classes
                  if n not in WRAPPER_TYPES and n not in NAMESPACES and n in STUBS)
    missing = sorted(set(classes) - set(done) - set(namespaced) - set(stub))
    aliased = sorted(n for n in values if n in KNOWN_WRAPPERS)
    unaliased = sorted(set(values) - set(aliased))

    print(f"{namespace}  -  {len(classes)} runtime classes, {len(values)} enums/structs\n")
    for label, group in (("done", done), ("namespace, not a class", namespaced),
                         ("stub", stub), ("aliased in VLEnums", aliased)):
        if not group:
            continue
        print(f"{label} ({len(group)}):")
        for n in group:
            extra = (WRAPPER_TYPES.get(n) or NAMESPACES.get(n)
                     or KNOWN_WRAPPERS.get(n, (None,))[0] or STUBS.get(n, ""))
            print(f"    {short(n):<44} {extra}")
        print()
    print(f"classes with no wrapper ({len(missing)}):")
    for n in missing:
        print(f"    {short(n)}")
    print(f"\nenums/structs not aliased ({len(unaliased)}) - only needed once something uses them")


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    ap = argparse.ArgumentParser(description="Generate a PGUI wrapper from WinRT metadata.")
    ap.add_argument("type", nargs="?", help="projected type, e.g. Visual or Windows.UI.Composition.Visual")
    ap.add_argument("--winmd", default=DEFAULT_WINMD)
    ap.add_argument("--base", default=None, help="PGUI base wrapper (default: inferred)")
    ap.add_argument("--root", default=here,
                    help="project root holding modules/UI/VL (default: this script's directory)")
    ap.add_argument("--status", action="store_true",
                    help="report which types in the namespace are done, stubbed or missing, then exit")
    ap.add_argument("-o", "--outdir", default=None,
                    help="write modules/UI/VL/<T>.ixx under this project root instead of stdout")
    args = ap.parse_args()

    if not args.status and args.type is None:
        ap.error("a type is required unless --status is given")

    discover(os.path.join(args.root, "modules", "UI", "VL"))

    md = Winmd(args.winmd)

    if args.status:
        report_status(md)
        return

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

    # Method parameters of a descendant type (or an interface, which has no wrapper) are taken
    # through the projected type instead, so this module never imports the descendant's -
    # which imports this one. A descendant RETURN type still needs the complete wrapper.
    def cycles_back(t):
        return t.startswith(DEFAULT_NAMESPACE + ".") and t in md.by_name and (
            full_name in md.ancestors(t) or md.is_interface(t))

    deferred_params = {t for _, _, params in methods for t, _ in params if cycles_back(t)}
    deferred_returns = {ret for _, ret, _ in methods if cycles_back(ret)}

    iterable = next((split_generic(i)[1][0] for i in md.interfaces_of(full_name)
                     if split_generic(i)[0] == ITERABLE), None)

    ixx = render_ixx(class_name, base_wrapper, props, methods, free_fns, force_todo,
                     deferred_params, deferred_returns, iterable)

    if args.outdir:
        path = os.path.join(args.outdir, "modules", "UI", "VL",
                            target_dir(base_wrapper), f"{class_name}.ixx")
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
