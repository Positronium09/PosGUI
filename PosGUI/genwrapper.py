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
WRAPPER_TEMPLATED = set()   # PGUI classes declared as a class template (XT<Projected>, aliased X)
KNOWN_WRAPPERS = {}     # winrt metadata name -> (PGUI alias, needs explicit construction)
STUBS = {}              # winrt metadata name -> module of a file that exists but is empty
NAMESPACES = {}         # winrt metadata name -> module covering it as a namespace, not a class
INTERFACE_TYPES = set()     # winrt interfaces in the namespace - collection elements fall back to Wrapper<>


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
    """(metadata name, PGUI name, is a class template) read from the first wrapper class's
    base specifier.

    A plain class names the projection directly - CompositionObjectT<WUC::CompositionDrawingSurface>.
    A class template passes its own parameter down to the base, so the projection comes from
    that parameter's default instead - 'Projected = WUC::CompositionObject'.
    """
    m = CLASS_RE.search(text)
    if m is None:
        return None, None, False

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
        return None, pgui, templated
    return resolve_projected(projected, aliases, namespace), pgui, templated


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

            wrapped, pgui, templated = wrapped_type_of(text, aliases, namespace)
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
            if templated:
                WRAPPER_TEMPLATED.add(pgui)

    enums_path = os.path.join(vl_root, "VLEnums.ixx")
    if os.path.isfile(enums_path):
        with open(enums_path, encoding="utf-8", errors="replace") as f:
            text = f.read()
            ns_aliases = {a: n.replace("::", ".")[len("winrt."):]
                          for a, n in re.findall(r"^\s*namespace\s+(\w+)\s*=\s*([\w:]+)\s*;", text, re.M)}
            for alias, ns_alias, winrt_name in re.findall(
                    r"^\s*using\s+(\w+)\s*=\s*(\w+)::(\w+)\s*;", text, re.M):
                enum_ns = ns_aliases.get(ns_alias, namespace)
                KNOWN_WRAPPERS[f"{enum_ns}.{winrt_name}"] = (alias, False)


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


LOSSY_WRAP = {
    "Windows.Foundation.TimeSpan": "std::chrono::duration_cast<std::chrono::milliseconds>({})",
}


def wrap_expr(winrt_type, pgui, inner):
    """TimeSpan is 100 ns ticks; milliseconds{ ticks } is rejected as lossy, so it needs a cast."""
    if winrt_type in LOSSY_WRAP:
        return LOSSY_WRAP[winrt_type].format(inner)
    return f"{pgui}{{ {inner} }}"


IREFERENCE = "Windows.Foundation.IReference`1"

ARITY_RE = re.compile(r"`\d+")

# winrt metadata type -> its C++/WinRT spelling, for the T of an IReference<T>
PROJECTED_SPELLING = {
    "bool": "bool",
    "float": "float",
    "double": "double",
    "int32": "std::int32_t",
    "uint32": "std::uint32_t",
    "int64": "std::int64_t",
    "uint64": "std::uint64_t",
    "Windows.Foundation.Numerics.Vector2": "WFN::float2",
    "Windows.Foundation.Numerics.Vector3": "WFN::float3",
    "Windows.Foundation.Numerics.Vector4": "WFN::float4",
    "Windows.Foundation.Numerics.Matrix3x2": "WFN::float3x2",
    "Windows.Foundation.Numerics.Matrix4x4": "WFN::float4x4",
    "Windows.Foundation.Numerics.Quaternion": "WFN::quaternion",
    "Windows.Foundation.Point": "WF::Point",
    "Windows.Foundation.Size": "WF::Size",
    "Windows.Foundation.Rect": "WF::Rect",
    "Windows.Foundation.TimeSpan": "WF::TimeSpan",
    "Windows.UI.Color": "winrt::Windows::UI::Color",
}


def optional_of(winrt_type, prop_name):
    """IReference<T> -> (PGUI T, projected T, pass-through, inner winrt T), or None.
    Pass-through means the PGUI type IS the projected type, so the IReference <-> std::optional
    conversions cppwinrt provides apply directly; otherwise an optional of the projected type
    is built first and converted."""
    base, targs = split_generic(winrt_type)
    if base != IREFERENCE or len(targs) != 1:
        return None
    inner = targs[0]
    pgui, needs_wrap = map_type(inner, prop_name)
    if pgui is None or inner in WRAPPER_TYPES:
        return None
    if inner in KNOWN_WRAPPERS:
        return pgui, projected_name(inner), True, inner
    projected = PROJECTED_SPELLING.get(inner)
    if projected is None:
        return None
    return pgui, projected, not needs_wrap, inner


def is_scalar(pgui):
    return pgui in SCALARS or pgui in (v[0] for v in KNOWN_WRAPPERS.values())


# PGUI type -> the view type a parameter of that type is taken as, by const value
PARAM_VIEW = {"std::wstring": "std::wstring_view"}


def param_decl(pgui, arg):
    """(declaration, capture) for a non-wrapper parameter."""
    if pgui in PARAM_VIEW:
        return f"const {PARAM_VIEW[pgui]} {arg}", f"&{arg}"
    if is_scalar(pgui):
        return f"const {pgui} {arg}", arg
    return f"const {pgui}& {arg}", f"&{arg}"


def gen_free_functions(entries, indent="\t"):
    """Accessors whose return type is the class itself (or a descendant) cannot be members -
    std::expected needs a complete type. Emitted after the class, where the type is complete."""
    out = []
    for owner, name, wtype, g, s, owner_templated in entries:
        pgui = WRAPPER_TYPES[wtype]
        arg = owner[0].lower() + owner[1:]
        owner_type = f"{owner}T<Projected>" if owner_templated else owner
        head = [f"{indent}template <HandleDerivedFrom<WUC::{owner}> Projected>"] if owner_templated else []
        if g:
            out += head
            out.append(f"{indent}[[nodiscard]] auto Get{name}Of(const {owner_type}& {arg}) noexcept -> Result<{pgui}>")
            out.append(f"{indent}{{")
            out.append(f"{indent}\treturn CallProjected([&{arg}]")
            out.append(f"{indent}\t{{")
            out.append(f"{indent}\t\treturn {pgui}{{ {arg}.Get().{name}() }};")
            out.append(f"{indent}\t}});")
            out.append(f"{indent}}}")
        if s:
            value = arg_name(name)
            out += head
            out.append(f"{indent}[[nodiscard]] auto Set{name}Of(const {owner_type}& {arg}, const {pgui}& {value}) noexcept -> Result<void>")
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


def gen_members(props, indent="\t\t", force_todo=frozenset(), self_get="this->Get()"):
    """Declarations with inline bodies. self_get is 'this->Get()' in a class template, where the
    base is dependent, and plain 'Get()' in a non-templated class."""
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
        opt = None if name in force_todo else optional_of(wtype, name)
        if opt is not None:
            inner_pgui, projected, passthrough, inner = opt
            if g:
                sig = f"[[nodiscard]] auto Get{name}() const noexcept -> Result<std::optional<{inner_pgui}>>"
                if passthrough:
                    block(sig, ("[this]", f"return std::optional<{projected}>{{ {self_get}.{name}() }};"))
                else:
                    out += [
                        f"{indent}{sig}",
                        f"{indent}{{",
                        f"{indent}\treturn CallProjected([this]",
                        f"{indent}\t{{",
                        f"{indent}\t\treturn std::optional<{projected}>{{ {self_get}.{name}() }}.transform([](const auto& value)",
                        f"{indent}\t\t{{",
                        f"{indent}\t\t\treturn {wrap_expr(inner, inner_pgui, 'value')};",
                        f"{indent}\t\t}});",
                        f"{indent}\t}});",
                        f"{indent}}}",
                    ]
            if s:
                a = arg_name(name)
                if passthrough:
                    param, cap, value = f"const std::optional<{inner_pgui}> {a}", a, a
                else:
                    param, cap, value = (f"const std::optional<{inner_pgui}>& {a}", f"&{a}",
                                         f"std::optional<{projected}>{{ {a} }}")
                block(f"[[nodiscard]] auto Set{name}({param}) const noexcept -> Result<void>",
                      (f"[this, {cap}]", f"{self_get}.{name}({value});"))
            out.append("")
            continue

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
                  ("[this, enabled]", f"{self_get}.{name}(enabled);"))
            out.append(f"{indent}[[nodiscard]] auto {dis}() const noexcept -> Result<void> {{ return {en}(false); }}")
            block(f"[[nodiscard]] auto {is_}() const noexcept -> Result<bool>",
                  ("[this]", f"return {self_get}.{name}();"))
            out.append("")
            continue
        if g:
            expr = wrap_expr(wtype, pgui, f"{self_get}.{name}()") if needs_wrap else f"{self_get}.{name}()"
            block(f"[[nodiscard]] auto Get{name}() const noexcept -> Result<{pgui}>",
                  ("[this]", f"return {expr};"))
        if s:
            a = arg_name(name)
            param, cap = param_decl(pgui, a)
            value = f"{a}.Get()" if wtype in WRAPPER_TYPES else a
            block(f"[[nodiscard]] auto Set{name}({param}) const noexcept -> Result<void>",
                  (f"[this, {cap}]", f"{self_get}.{name}({value});"))
        out.append("")
    while out and out[-1] == "":
        out.pop()
    return out


def projected_name(winrt_type):
    """Windows.UI.Composition.X -> WUC::X, Windows.UI.Composition.Desktop.X -> WUC::Desktop::X."""
    return "WUC::" + winrt_type[len(DEFAULT_NAMESPACE) + 1:].replace(".", "::")


def iterable_param_of(winrt_type):
    """IIterable<X> as a PARAMETER -> (PGUI X, projected X, X is a wrapper), or None.
    cppwinrt takes these as param::iterable<X>, which converts from an iterator pair, so a
    span of PGUI values passes as { arg.begin(), arg.end() }. A span of WRAPPERS has to be
    unwrapped into a vector of projections first - a wrapper does not convert to its handle."""
    base, targs = split_generic(winrt_type)
    if base != ITERABLE or len(targs) != 1:
        return None
    elem = targs[0]
    if elem in WRAPPER_TYPES:
        return WRAPPER_TYPES[elem], projected_name(elem), True
    pgui, _ = map_type(elem, "")
    if pgui is None or split_generic(elem)[1]:
        return None
    return pgui, pgui, False


def map_params(params, deferred_params=frozenset()):
    """[(PGUI type or a 'deferred'/'iterable' marker or None, winrt type, name)]."""
    return [(("deferred" if t in deferred_params
              else ("iterable" if iterable_param_of(t) else map_type(t, "")[0])), t, n)
            for t, n in params]


def build_params(mapped, caps):
    """(declarations, call arguments, lambda captures, prologue lines) for one parameter list."""
    decl, args, prologue = [], [], []
    for pgui, wtype, n in mapped:
        arg = arg_name(n)
        if pgui == "iterable":
            elem_pgui, elem_projected, is_wrapper = iterable_param_of(wtype)
            decl.append(f"const std::span<const {elem_pgui}> {arg}")
            caps.append(arg)
            if is_wrapper:
                projected_arg = f"{arg}Projected"
                prologue += [
                    f"std::vector<{elem_projected}> {projected_arg}{{ }};",
                    f"{projected_arg}.reserve({arg}.size());",
                    f"for (const auto& element : {arg})",
                    "{",
                    f"\t{projected_arg}.emplace_back(element.Get());",
                    "}",
                    "",
                ]
                args.append(projected_arg)
            else:
                args.append(f"{{ {arg}.begin(), {arg}.end() }}")
        elif pgui == "deferred":
            decl.append(f"const WrapperDerivedFrom<{projected_name(wtype)}> auto& {arg}")
            args.append(f"{arg}.Get()")
            caps.append(f"&{arg}")
        elif wtype in WRAPPER_TYPES:
            decl.append(f"const {pgui}& {arg}")
            args.append(f"{arg}.Get()")
            caps.append(f"&{arg}")
        else:
            declared, captured = param_decl(pgui, arg)
            decl.append(declared)
            args.append(arg)
            caps.append(captured)
    return decl, args, caps, prologue


def gen_methods(methods, indent="\t\t", deferred_params=frozenset(), deferred_returns=frozenset(),
                self_get="this->Get()", skip=frozenset()):
    """deferred_params: types taken as WrapperDerivedFrom<WUC::X> auto&, so the module never
    imports their wrapper. deferred_returns: types a member cannot return without that import.
    skip: (name, param types) identities emitted by a dedicated generator instead."""
    out = []
    for name, ret, params in methods:
        if (name, tuple(t for t, _ in params)) in skip:
            continue
        mapped = map_params(params, deferred_params)
        ret_pgui = "void" if ret == "void" else (None if ret in deferred_returns else map_type(ret, "")[0])

        if ret_pgui is None or any(p is None for p, _, _ in mapped):
            sig = ", ".join(f"{short(t)} {n}" for _, t, n in mapped)
            out.append(f"{indent}//TODO {name}({sig}) -> {short(ret)}")
            out.append("")
            continue

        decl, args, caps, prologue = build_params(mapped, ["this"])

        call = f"{self_get}.{name}({', '.join(args)});"
        result = "void" if ret_pgui == "void" else ret_pgui
        nodiscard = "[[nodiscard]] "
        if ret_pgui != "void":
            invoke = f"{self_get}.{name}({', '.join(args)})"
            call = f"return {wrap_expr(ret, ret_pgui, invoke)};" \
                if ret in WRAPPER_TYPES or map_type(ret, "")[1] \
                else f"return {self_get}.{name}({', '.join(args)});"

        out.append(f"{indent}{nodiscard}auto {name}({', '.join(decl)}) const noexcept -> Result<{result}>")
        out.append(f"{indent}{{")
        out.append(f"{indent}\treturn CallProjected([{', '.join(caps)}]")
        out.append(f"{indent}\t{{")
        out += [f"{indent}\t\t{line}".rstrip() for line in prologue]
        out.append(f"{indent}\t\t{call}")
        out.append(f"{indent}\t}});")
        out.append(f"{indent}}}")
        out.append("")
    while out and out[-1] == "":
        out.pop()
    return out


ITERABLE = "Windows.Foundation.Collections.IIterable`1"
KEY_VALUE_PAIR = "Windows.Foundation.Collections.IKeyValuePair`2"
IMAP = "Windows.Foundation.Collections.IMap`2"
COMPOSITOR = "Windows.UI.Composition.Compositor"


def statics_of(md, full_name):
    """The type's I<Name>Statics, I<Name>Statics2, ... interfaces. These are NOT in the class's
    InterfaceImpl set, so nothing else in this generator ever sees them - which is how every
    statics-built wrapper ended up with no Create and not even a //TODO."""
    namespace, _, name = full_name.rpartition(".")
    out = []
    for candidate in [f"{namespace}.I{name}Statics"] + \
            [f"{namespace}.I{name}Statics{n}" for n in range(2, 10)]:
        if candidate in md.by_name and md.is_interface(candidate):
            out += md.methods_of(candidate)
    return out


def gen_statics(class_name, projected, full_name, statics, indent="\t\t",
                deferred_params=frozenset(), deferred_returns=frozenset()):
    """Static factories and helpers, called as Projected::Name(...).

    A static whose first parameter is the compositor loses it: the wrapper pulls the
    process-wide one through Compositor::GetCompositor() instead, which is what every
    hand-written Create does. Create<ClassName> contracts to plain Create."""
    out = []
    for name, ret, params in statics:
        mapped = map_params(params, deferred_params)
        ret_pgui = "void" if ret == "void" else (None if ret in deferred_returns else map_type(ret, "")[0])
        takes_compositor = bool(params) and params[0][0] == COMPOSITOR

        if ret_pgui is None or any(p is None for p, _, _ in mapped[1 if takes_compositor else 0:]):
            sig = ", ".join(f"{short(t)} {n}" for _, t, n in mapped)
            out.append(f"{indent}//TODO static {name}({sig}) -> {short(ret)}")
            out.append("")
            continue

        member = "Create" if name == f"Create{short(full_name)}" else name
        decl, args, caps, prologue = build_params(mapped[1:] if takes_compositor else mapped, [])
        if takes_compositor:
            args.insert(0, "compositor")
            caps.insert(0, "&compositor")

        invoke = f"{projected}::{name}({', '.join(args)})"
        if ret_pgui == "void":
            call = f"{invoke};"
        elif ret in WRAPPER_TYPES or ret == full_name or map_type(ret, "")[1]:
            call = f"return {wrap_expr(ret, ret_pgui, invoke)};"
        else:
            call = f"return {invoke};"

        out.append(f"{indent}[[nodiscard]] static auto {member}({', '.join(decl)}) noexcept -> Result<{ret_pgui}>")
        out.append(f"{indent}{{")
        if takes_compositor:
            out += [
                f"{indent}\tconst auto result = Compositor::GetCompositor();",
                f"{indent}\tif (!result.has_value())",
                f"{indent}\t{{",
                f"{indent}\t\treturn Unexpected{{ result.error() }};",
                f"{indent}\t}}",
                f"{indent}\tconst auto& compositor = result.value();",
                "",
            ]
        out.append(f"{indent}\treturn CallProjected([{', '.join(caps)}]")
        out.append(f"{indent}\t{{")
        out += [f"{indent}\t\t{line}".rstrip() for line in prologue]
        out.append(f"{indent}\t\t{call}")
        out.append(f"{indent}\t}});")
        out.append(f"{indent}}}")
        out.append("")
    while out and out[-1] == "":
        out.pop()
    return out


def collection_element(winrt_type):
    """(PGUI type, needs explicit construction, is a handle) for a collection key/value/element.
    An element whose type is a bare interface has no wrapper class, but Wrapper<WUC::IFoo> is a
    perfectly good handle holder - which is what the hand-written map wrappers already return."""
    pgui, needs_wrap = map_type(winrt_type, "")
    if pgui is not None:
        return pgui, needs_wrap, winrt_type in WRAPPER_TYPES
    if winrt_type in INTERFACE_TYPES:
        return f"Wrapper<{projected_name(winrt_type)}>", True, True
    return None, False, False


def gen_map(key, value, indent="\t\t", self_get="this->Get()"):
    """The IMap<K, V> surface. TryLookup / TryRemove are cppwinrt extensions with no metadata -
    TryLookup yields the nullable projection itself for a reference V and std::optional<V>
    otherwise. GetView cannot hand back an IMapView wrapper (its lifetime is tied to the map and
    every read can throw), so it is snapshotted into a std::map."""
    key_pgui, key_wrap, key_handle = collection_element(key)
    value_pgui, value_wrap, value_handle = collection_element(value)
    if key_pgui is None or value_pgui is None:
        return [f"{indent}//TODO IMap<{short(key)}, {short(value)}> members"]

    if key_handle:
        key_param, key_cap, key_arg = f"const {key_pgui}& key", "&key", "key.Get()"
    else:
        key_param, key_cap = param_decl(key_pgui, "key")
        key_arg = "key"
    if value_handle:
        value_param, value_cap, value_arg = f"const {value_pgui}& value", "&value", "value.Get()"
    else:
        value_param, value_cap = param_decl(value_pgui, "value")
        value_arg = "value"

    def received(source):
        return f"{value_pgui}{{ {source} }}" if value_wrap else source

    out = [
        f"{indent}[[nodiscard]] auto Lookup({key_param}) const noexcept -> Result<{value_pgui}>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this, {key_cap}]",
        f"{indent}\t{{",
        f"{indent}\t\treturn {received(f'{self_get}.Lookup({key_arg})')};",
        f"{indent}\t}});",
        f"{indent}}}",
        "",
    ]
    if value_handle:
        out += [
            f"{indent}[[nodiscard]] auto TryLookup({key_param}) const noexcept -> Result<std::optional<{value_pgui}>>",
            f"{indent}{{",
            f"{indent}\treturn CallProjected([this, {key_cap}] -> std::optional<{value_pgui}>",
            f"{indent}\t{{",
            f"{indent}\t\tconst auto value = {self_get}.TryLookup({key_arg});",
            f"{indent}\t\tif (!value)",
            f"{indent}\t\t{{",
            f"{indent}\t\t\treturn std::nullopt;",
            f"{indent}\t\t}}",
            "",
            f"{indent}\t\treturn {received('value')};",
            f"{indent}\t}});",
            f"{indent}}}",
            "",
        ]
    elif value_wrap:
        out += [
            f"{indent}[[nodiscard]] auto TryLookup({key_param}) const noexcept -> Result<std::optional<{value_pgui}>>",
            f"{indent}{{",
            f"{indent}\treturn CallProjected([this, {key_cap}]",
            f"{indent}\t{{",
            f"{indent}\t\treturn {self_get}.TryLookup({key_arg}).transform([](const auto& value)",
            f"{indent}\t\t{{",
            f"{indent}\t\t\treturn {received('value')};",
            f"{indent}\t\t}});",
            f"{indent}\t}});",
            f"{indent}}}",
            "",
        ]
    else:
        out += [
            f"{indent}[[nodiscard]] auto TryLookup({key_param}) const noexcept -> Result<std::optional<{value_pgui}>>",
            f"{indent}{{",
            f"{indent}\treturn CallProjected([this, {key_cap}]",
            f"{indent}\t{{",
            f"{indent}\t\treturn {self_get}.TryLookup({key_arg});",
            f"{indent}\t}});",
            f"{indent}}}",
            "",
        ]

    stored = f"std::map<{key_pgui}, {value_pgui}>"
    key_received = f"{key_pgui}{{ element.Key() }}" if key_wrap else "element.Key()"
    out += [
        f"{indent}[[nodiscard]] auto HasKey({key_param}) const noexcept -> Result<bool>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this, {key_cap}]",
        f"{indent}\t{{",
        f"{indent}\t\treturn {self_get}.HasKey({key_arg});",
        f"{indent}\t}});",
        f"{indent}}}",
        "",
        f"{indent}[[nodiscard]] auto GetView() const noexcept -> Result<{stored}>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this]",
        f"{indent}\t{{",
        f"{indent}\t\treturn {self_get}.GetView() | std::views::transform([](const auto& element)",
        f"{indent}\t\t{{",
        f"{indent}\t\t\treturn std::pair{{ {key_received}, {received('element.Value()')} }};",
        f"{indent}\t\t}}) | std::ranges::to<std::map>();",
        f"{indent}\t}});",
        f"{indent}}}",
        "",
        f"{indent}[[nodiscard]] auto Insert({key_param}, {value_param}) const noexcept -> Result<bool>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this, {key_cap}, {value_cap}]",
        f"{indent}\t{{",
        f"{indent}\t\treturn {self_get}.Insert({key_arg}, {value_arg});",
        f"{indent}\t}});",
        f"{indent}}}",
        "",
        f"{indent}[[nodiscard]] auto Remove({key_param}) const noexcept -> Result<void>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this, {key_cap}]",
        f"{indent}\t{{",
        f"{indent}\t\t{self_get}.Remove({key_arg});",
        f"{indent}\t}});",
        f"{indent}}}",
        f"{indent}[[nodiscard]] auto TryRemove({key_param}) const noexcept -> Result<bool>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this, {key_cap}]",
        f"{indent}\t{{",
        f"{indent}\t\treturn {self_get}.TryRemove({key_arg});",
        f"{indent}\t}});",
        f"{indent}}}",
        "",
        f"{indent}[[nodiscard]] auto Clear() const noexcept -> Result<void>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this]",
        f"{indent}\t{{",
        f"{indent}\t\t{self_get}.Clear();",
        f"{indent}\t}});",
        f"{indent}}}",
    ]
    return out


def gen_iterable(element, indent="\t\t", self_get="this->Get()", size_expr=None):
    """ToVector + ForEach over an IIterable<element>. A key/value element is unpacked, so
    ForEach takes a (key, value) callable and ToVector yields pairs. size_expr is the
    collection's own count, when it has one, so ToVector reserves instead of regrowing."""
    def value_of(winrt_type, source):
        pgui, needs_wrap, _ = collection_element(winrt_type)
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

    reserve = [
        f"{indent}\t\telements.reserve({size_expr});",
        "",
    ] if size_expr else []

    return [
        f"{indent}[[nodiscard]] auto ToVector() const noexcept -> Result<std::vector<{stored}>>",
        f"{indent}{{",
        f"{indent}\treturn CallProjected([this]",
        f"{indent}\t{{",
        f"{indent}\t\tauto elements = std::vector<{stored}>{{ }};",
        *reserve,
        f"{indent}\t\tfor (const auto& element : {self_get})",
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
        f"{indent}\t\tfor (const auto& [i, element] : {self_get} | std::views::enumerate)",
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
               deferred_params=frozenset(), deferred_returns=frozenset(), iterable=None,
               templated=False, map_kv=None, skip_methods=frozenset(), statics=(), full_name=None):
    """base_wrapper is the PGUI base WITHOUT the T suffix, e.g. 'Visual'.

    templated=False (the default) emits a plain class over a fixed projection -
        class X : public BaseT<WUC::X> { using BaseT::BaseT; ... };
    templated=True emits the class template a further wrapper can derive from -
        template <HandleDerivedFrom<WUC::X> Projected = WUC::X> class XT : public BaseT<Projected>
    followed by 'using X = XT<>;'.
    """
    base_name = "Wrapper" if base_wrapper == "ProjectionHolder" else f"{base_wrapper}T"
    self_get = "this->Get()" if templated else "Get()"
    full_name = full_name or f"{DEFAULT_NAMESPACE}.{class_name}"
    projected = projected_name(full_name)

    body = [
        "namespace WUC = winrt::Windows::UI::Composition;",
        "",
        "export namespace PGUI::UI::VL",
        "{",
    ]
    if templated:
        base = f"{base_name}<Projected>"
        body += [
            f"\ttemplate <HandleDerivedFrom<{projected}> Projected = {projected}>",
            f"\tclass {class_name}T : public {base}",
            "\t{",
            "\t\tpublic:",
            f"\t\tusing Base = {base};",
            "\t\tusing Base::Base;",
            "",
        ]
    else:
        body += [
            f"\tclass {class_name} : public {base_name}<{projected}>",
            "\t{",
            "\t\tpublic:",
            f"\t\tusing {base_name}::{base_name};",
            "",
        ]
    static_lines = gen_statics(class_name, projected, full_name, statics,
                               deferred_params=deferred_params, deferred_returns=deferred_returns)
    if static_lines:
        static_lines.append("")
    body += static_lines
    members = gen_members(props, force_todo=force_todo, self_get=self_get)
    method_lines = gen_methods(methods, deferred_params=deferred_params, deferred_returns=deferred_returns,
                               self_get=self_get, skip=skip_methods)
    if members and method_lines:
        members.append("")
    map_lines = gen_map(*map_kv, self_get=self_get) if map_kv else []
    if (members or method_lines) and map_lines:
        method_lines.append("")
    # Count is int32 in the Composition collections, Size uint32 - only the signed one
    # needs the cast reserve() would otherwise warn about.
    counter = next(((n, t) for n, t, g, _ in props if g and n in ("Size", "Count")), None)
    size_expr = None
    if counter is not None:
        call = f"{self_get}.{counter[0]}()"
        size_expr = f"static_cast<std::size_t>({call})" if counter[1].startswith("int") else call
    iterable_lines = gen_iterable(iterable, self_get=self_get, size_expr=size_expr) if iterable else []
    if (members or method_lines or map_lines) and iterable_lines:
        (map_lines or method_lines).append("")
    body += members + method_lines + map_lines + iterable_lines
    body.append("\t};")
    if templated:
        body.append(f"\tusing {class_name} = {class_name}T<>;")
    if free_fns:
        body.append("")
        body += gen_free_functions(free_fns)
    body += ["}", ""]
    text = "\n".join(body)
    aliases = []
    if "WUC::Interactions::" in text:
        text = text.replace("WUC::Interactions::", "WUCI::")
        aliases.append("namespace WUCI = winrt::Windows::UI::Composition::Interactions;")
    if "WF::" in text:
        aliases.append("namespace WF = winrt::Windows::Foundation;")
    if "WFN::" in text:
        aliases.append("namespace WFN = winrt::Windows::Foundation::Numerics;")
    if aliases:
        text = text.replace("namespace WUC = winrt::Windows::UI::Composition;",
                            "\n".join(["namespace WUC = winrt::Windows::UI::Composition;"] + aliases), 1)

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
    if "Compositor::GetCompositor()" in text:
        imports.add("PGUI.UI.VL.Compositor")
    if "Callable<" in text:
        imports.add("PGUI.Utils")
    for pgui, module in TYPE_MODULE.items():
        if re.search(rf"\b{re.escape(pgui)}\b", text):
            imports.add(module)
    if any(re.search(rf"\b{re.escape(alias)}\b", text) for alias, _ in KNOWN_WRAPPERS.values()):
        imports.add("PGUI.UI.VL.VLEnums")
    imports |= {module_of(w) for w in referenced}
    # a sibling class sharing this file would otherwise make the module import itself
    imports.discard(target_module(class_name, base_wrapper))
    imports.discard(WRAPPER_MODULE.get(class_name, ""))

    lines = [
        "module;",
        "#include <winrt/Windows.UI.Composition.h>",
        "",
    ]
    if "WUCI::" in text:
        lines += ["#include <winrt/Windows.UI.Composition.Interactions.h>", ""]
    if "std::optional<" in text:
        lines += ["#include <winrt/Windows.Foundation.h>", ""]
    if iterable or map_kv:
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
    ap.add_argument("-t", "--templated", "--template", dest="templated", action="store_true",
                    help="emit a class template XT<Projected> + 'using X = XT<>;' that further "
                         "wrappers can derive from (default: a plain class over WUC::X)")
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
    # the type being generated names itself in its own statics and in self-referential
    # members, so it has to map like any other wrapper even before its file exists
    WRAPPER_TYPES.setdefault(full_name, class_name)
    props = md.class_properties(full_name)

    base_meta = md.base_of(full_name)
    base_wrapper = args.base or (short(base_meta) if base_meta and base_meta != "System.Object"
                                 else "ProjectionHolder")

    if base_wrapper != "ProjectionHolder" and base_wrapper in WRAPPER_MODULE \
            and base_wrapper not in WRAPPER_TEMPLATED:
        print(f"warning: base {base_wrapper} is a plain class, not a template - nothing can derive "
              f"from {base_wrapper}T<WUC::{class_name}>; regenerate {base_wrapper} with --templated",
              file=sys.stderr)

    methods = md.class_methods(full_name)

    # A property whose type is this class, or a class derived from it, cannot be a member:
    # std::expected needs a complete type, and the derived wrapper's module imports this one.
    free_fns, force_todo = [], set()
    for name, wtype, g, s in props:
        if WRAPPER_TYPES.get(wtype) == class_name:
            free_fns.append((class_name, name, wtype, g, s, args.templated))
            force_todo.add(name)
        elif wtype in WRAPPER_TYPES and full_name in md.ancestors(wtype):
            force_todo.add(name)          # emitted as a free function in the descendant's file

    # ...and inherited properties whose type is THIS class get their free function here,
    # where both the ancestor template and this class are complete.
    for ancestor in md.ancestors(full_name):
        for name, wtype, g, s in md.class_properties(ancestor):
            if WRAPPER_TYPES.get(wtype) == class_name:
                owner = WRAPPER_TYPES.get(ancestor, short(ancestor))
                free_fns.append((short(ancestor), name, wtype, g, s,
                                 owner in WRAPPER_TEMPLATED or ancestor not in WRAPPER_TYPES))

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

    INTERFACE_TYPES.update(n for n in md.by_name
                           if n.startswith(DEFAULT_NAMESPACE + ".") and md.is_interface(n))

    map_iface = next((i for i in md.interfaces_of(full_name) if split_generic(i)[0] == IMAP), None)
    map_kv = tuple(split_generic(map_iface)[1]) if map_iface else None
    skip_methods = {(name, tuple(t for t, _ in params))
                    for name, _, params in (md.methods_of(map_iface) if map_iface else [])}

    statics = statics_of(md, full_name)

    ixx = render_ixx(class_name, base_wrapper, props, methods, free_fns, force_todo,
                     deferred_params, deferred_returns, iterable, args.templated,
                     map_kv, skip_methods, statics, full_name)

    unmapped = [(n, t) for n, t, _, _ in props
                if n not in force_todo and map_type(t, n)[0] is None and optional_of(t, n) is None]
    for name, ret, params in list(methods) + [(f"static {n}", r, p) for n, r, p in statics]:
        if (name, tuple(t for t, _ in params)) in skip_methods:
            continue
        for t in [ret] + [p for p, _ in params]:
            if t == "void" or t in deferred_params or t in deferred_returns:
                continue
            # a static's leading compositor is elided, not unmapped
            if t == COMPOSITOR and name.startswith("static "):
                continue
            if map_type(t, "")[0] is None and optional_of(t, "") is None \
                    and iterable_param_of(t) is None:
                unmapped.append((f"{name}()", t))
    def report():
        if not unmapped:
            return
        print(f"\n// {len(unmapped)} unmapped -> emitted as //TODO:", file=sys.stderr)
        for n, t in unmapped:
            readable = ARITY_RE.sub("", t)
            print(f"//   {n}: {readable}", file=sys.stderr)

    if args.outdir:
        path = os.path.join(args.outdir, "modules", "UI", "VL",
                            target_dir(base_wrapper), f"{class_name}.ixx")
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w", encoding="utf-8", newline="\r\n") as f:
            f.write(ixx)
        print(f"wrote {path}", file=sys.stderr)
        report()
        return

    print(ixx, flush=True)
    report()


if __name__ == "__main__":
    main()
