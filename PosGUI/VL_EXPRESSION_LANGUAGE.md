# VL Expression Language Reference

Operators, keywords, and functions available in ExpressionAnimation strings
(CreateExpressionAnimation, InsertExpressionKeyFrame, inertia modifier
conditions).

## Verification status

- Tier 1 - runtime-verified on this machine by VLTest.cpp demos:
  Scale(Vector3, scalar), Vector3 +/-, scalar arithmetic, mod/clamp/floor/ceil,
  comparison operators in inertia modifier conditions, ColorRGB(...),
  SetScalarParameter / SetReferenceParameter, vector swizzles (all 4
  two-channel Vector2 swizzles, all 9 two-channel and all 27 three-channel
  Vector3 swizzles), and the absence of Color channel accessors.
- Tier 2 - from the Windows Community Toolkit ExpressionBuilder source
  (ExpressionNodes/*.cs, ExpressionFunctions.cs). The toolkit's operator
  overloads encode what the engine accepts, but the toolkit has no tests
  running expressions against a real compositor - believed-correct, widely
  used since 2017, not CI-verified. Combos the toolkit does not expose (V/S,
  S*M32, S*Q, matrix remainder) might still parse in the engine - test
  empirically before banning them in the EDSL.
- MS docs never published a per-type operator matrix; the class docs only say
  "typical mathematical operators, C# precedence/associativity" plus the
  function lists below.
- Where the docs and the toolkit disagree on a spelling, both are given and
  the entry is marked unverified.

## Keywords

| Keyword | Meaning |
| --- | --- |
| this.Target | the CompositionObject the animation is bound to (resolves per attachment - the same animation instance can be shared, each StartAnimation site supplies its own Target) |
| this.StartingValue | original value of the animated property when the animation started |
| this.CurrentValue | currently known value of the animated property |
| this.FinalValue | final value of the animation - only meaningful for implicit animations (the newly assigned value); in explicit animations it behaves exactly like this.StartingValue |
| Pi | pi |
| True / False | boolean literals |

All keywords are reserved - cannot be used as parameter names.

this.FinalValue (the keyword) is unrelated to the FinalValue property on
ScalarNaturalMotionAnimation / Vector2NaturalMotionAnimation /
Vector3NaturalMotionAnimation, which is a settable API property.

## Operators

Types: S = Scalar, V2/V3/V4 = Vector2/3/4, M32 = Matrix3x2, M44 = Matrix4x4,
Q = Quaternion, C = Color, B = Boolean.

Every combination below has a toolkit operator overload (Tier 2) unless stated
otherwise. A `-` cell means the toolkit has no overload for it; the engine may
still accept it.

### Addition and subtraction: `a + b`, `a - b`

Both operands must be the same type; the result is that type.

| Operand type | Supported |
| --- | --- |
| S, V2, V3, V4, M32, M44 | yes |
| Q, C, B | no |

### Negation and not: `-a`, `!a`

| Operator | Operand | Result |
| --- | --- | --- |
| `-a` | S, V2, V3, V4, M32, M44 | same type |
| `!a` | B | B |

### Multiplication: `a * b`

Rows are the left operand, columns the right operand, cells the result.

| left \ right | S | V2 | V3 | V4 | M32 | M44 | Q |
| --- | --- | --- | --- | --- | --- | --- | --- |
| **S** | S | V2 | V3 | V4 | - | M44 | - |
| **V2** | V2 | V2 | - | - | - | - | - |
| **V3** | V3 | - | V3 | - | - | - | - |
| **V4** | V4 | - | - | V4 | - | - | - |
| **M32** | M32 | - | - | - | M32 | - | - |
| **M44** | M44 | - | - | - | - | M44 | - |
| **Q** | Q | - | - | - | - | - | Q |

- Vector * vector is componentwise and matrix * matrix is the matrix product,
  following System.Numerics - neither source states it for the engine.
- Quaternion * quaternion is undocumented. In System.Numerics it is the
  Hamilton product (rotation composition), not componentwise. For documented
  composition use Concatenate(q1, q2).
- S * M32 and S * Q have no overload although M32 * S and Q * S do - a toolkit
  asymmetry, likely an oversight. Put the scalar on the right.
- There is no matrix * vector; use Transform (see Special cases).

### Division: `a / b`

| left | right | Result |
| --- | --- | --- |
| S | S | S |
| V2 / V3 / V4 | same type | same type (componentwise) |
| Q | Q | Q |

- No vector / scalar: write `v * (1 / s)`.
- No matrix division of any kind.
- Quaternion / quaternion is undocumented (System.Numerics: q1 * inverse(q2)).

### Remainder: `Mod(a, b)`

| left | right | Result |
| --- | --- | --- |
| S | S | S |
| V2 / V3 / V4 | same type | same type (componentwise) |

- The toolkit's `%` overload never emits an infix `%` - it emits the function
  call `mod(a, b)`. Whether the engine parses infix `%` is unverified; write
  `Mod(a, b)`.
- The docs list Mod only for Scalar. Vector Mod comes from the toolkit alone.
- Mod takes the sign of the dividend (like C fmod), so negative inputs do not
  wrap into [0, b).

### Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`

| Operator | Operands | Result |
| --- | --- | --- |
| `==`, `!=` | two operands of the same type: S, V2, V3, V4, M32, M44, Q, C or B | B |
| `<`, `<=`, `>`, `>=` | S and S only | B |

No ordering for vectors, matrices, quaternions or colors.

### Logical: `&&`, `||`

| Operator | Operands | Result |
| --- | --- | --- |
| `a && b`, `a \|\| b` | B and B | B |

The toolkit declares these as the C# operators `&` and `|`, but emits `&&`
and `||`.

### Conditional: `condition ? a : b`

- `condition` is B.
- `a` and `b` are the same type, one of S, V2, V3, V4, M32, M44, Q or C; the
  result is that type.
- The toolkit has no overload with B branches - likely an oversight.
- Ternaries nest (documented).

## Special cases

- No Matrix*Vector operator - transforms are functions:
  Transform(Vector2, Matrix3x2)->Vector2, Transform(Vector4, Matrix4x4)->Vector4
  (no Vector3 variant).
- Color has no arithmetic (only `==` / `!=`) - use the ColorLerp functions.
- Color has no channel accessors (Tier 1): `.A`, `.R`, `.G`, `.B` are rejected
  by the engine, and neither the docs nor the toolkit list any. A color can be
  compared, interpolated or built with ColorRGB / ColorHsl, never taken apart -
  keep channels as scalars (or a Vector4) until the final ColorRGB(a, r, g, b).
- Quaternion has no `+`, `-` or remainder; see the `*` and `/` notes. The
  toolkit exposes no Quaternion channel accessors either (untested).
- Swizzles:
  - Vectors: `.X`, `.Y`, `.Z`, `.W` in any order, with repeats. The channel
    count sets the result type, the channels themselves can be any the source
    vector has.
    - Vector2 (Tier 1): all 4 two-channel swizzles - `.XX`, `.XY`, `.YX`, `.YY`.
    - Vector3 (Tier 1): all 9 two-channel (`.XX` .. `.ZZ`) and all 27
      three-channel (`.XXX` .. `.ZZZ`) swizzles.
    - Vector4 (presumed, not tested): all 16 two-channel, all 64 three-channel
      and all 256 four-channel swizzles, by the same rule.
    - Tested only as values read inside an expression; see the sub-channel
      target note below for StartAnimation targets.
  - Matrices: the toolkit emits `.Channel11` .. `.Channel44`, concatenated for
    multi-channel swizzles (`.Channel11Channel22`). The `._11` spelling appears
    in neither source and is unverified.
  - 1/2/3/4/6/16 channels yield S/V2/V3/V4/M32/M44 respectively.
- Sub-channel animation targets work for vectors: StartAnimation("Offset.X",
  ...), "Offset.XY" (documented). A reordered or repeated target
  ("Offset.YX") and a matrix channel as a StartAnimation target are unverified.
- Function names are case-insensitive in practice (toolkit emits lowercase,
  docs use PascalCase, both work).
- The toolkit fully parenthesizes: binary operators as `(a op b)`, `!` as
  `( ! a )`, negation as a function call `-(a)`, conditionals as
  `((c) ? (a) : (b))`. C# precedence rules only matter for hand-written strings.
- If an expression resolves to NaN (0/0), the system keeps the previously
  calculated value instead of erroring.
- An expression's result type must match the animated property's type, or the
  error surfaces when the expression is first calculated.

## Functions per type

From the ExpressionAnimation class docs (see Sources). Written as C++
signatures (Scalar = float); names are the expression-language identifiers.

### Scalar

```cpp
auto Abs(const Scalar value) -> Scalar;
auto Clamp(const Scalar value, const Scalar min, const Scalar max) -> Scalar;
auto Max(const Scalar value1, const Scalar value2) -> Scalar;
auto Min(const Scalar value1, const Scalar value2) -> Scalar;
auto Lerp(const Scalar value1, const Scalar value2, const Scalar progress) -> Scalar;
auto Slerp(const Scalar value1, const Scalar value2, const Scalar progress) -> Scalar;
auto Mod(const Scalar value1, const Scalar value2) -> Scalar;
auto Ceil(const Scalar value) -> Scalar;
auto Floor(const Scalar value) -> Scalar;
auto Sqrt(const Scalar value) -> Scalar;
auto Square(const Scalar value) -> Scalar;
auto Sin(const Scalar value) -> Scalar;
auto Asin(const Scalar value) -> Scalar;
auto Cos(const Scalar value) -> Scalar;
auto ACos(const Scalar value) -> Scalar;
auto Tan(const Scalar value) -> Scalar;
auto ATan(const Scalar value) -> Scalar;
auto Round(const Scalar value) -> Scalar;
auto Log10(const Scalar value) -> Scalar;
auto Ln(const Scalar value) -> Scalar;
auto Pow(const Scalar value, const Scalar power) -> Scalar;
auto ToDegrees(const Scalar radians) -> Scalar;
auto ToRadians(const Scalar degrees) -> Scalar;
```

### Vector2

```cpp
auto Abs(const Vector2 value) -> Vector2;
auto Clamp(const Vector2 value1, const Vector2 min, const Vector2 max) -> Vector2;
auto Max(const Vector2 value1, const Vector2 value2) -> Vector2;
auto Min(const Vector2 value1, const Vector2 value2) -> Vector2;
auto Lerp(const Vector2 value1, const Vector2 value2, const Scalar progress) -> Vector2;
auto Scale(const Vector2 value, const Scalar factor) -> Vector2;
auto Transform(const Vector2 value, const Matrix3x2 matrix) -> Vector2;
auto Length(const Vector2 value) -> Scalar;
auto LengthSquared(const Vector2 value) -> Scalar;
auto Distance(const Vector2 value1, const Vector2 value2) -> Scalar;
auto DistanceSquared(const Vector2 value1, const Vector2 value2) -> Scalar;
auto Normalize(const Vector2 value) -> Vector2;
auto Vector2(const Scalar x, const Scalar y) -> Vector2;
```

### Vector3

```cpp
auto Abs(const Vector3 value) -> Vector3;
auto Clamp(const Vector3 value1, const Vector3 min, const Vector3 max) -> Vector3;
auto Max(const Vector3 value1, const Vector3 value2) -> Vector3;
auto Min(const Vector3 value1, const Vector3 value2) -> Vector3;
auto Lerp(const Vector3 value1, const Vector3 value2, const Scalar progress) -> Vector3;
auto Scale(const Vector3 value, const Scalar factor) -> Vector3;
auto Length(const Vector3 value) -> Scalar;
auto LengthSquared(const Vector3 value) -> Scalar;
auto Distance(const Vector3 value1, const Vector3 value2) -> Scalar;
auto DistanceSquared(const Vector3 value1, const Vector3 value2) -> Scalar;
auto Normalize(const Vector3 value) -> Vector3;
auto Vector3(const Scalar x, const Scalar y, const Scalar z) -> Vector3;
```

(no Transform for Vector3)

### Vector4

```cpp
auto Abs(const Vector4 value) -> Vector4;
auto Clamp(const Vector4 value1, const Vector4 min, const Vector4 max) -> Vector4;
auto Max(const Vector4 value1, const Vector4 value2) -> Vector4;
auto Min(const Vector4 value1, const Vector4 value2) -> Vector4;
auto Lerp(const Vector4 value1, const Vector4 value2, const Scalar progress) -> Vector4;
auto Scale(const Vector4 value, const Scalar factor) -> Vector4;
auto Transform(const Vector4 value, const Matrix4x4 matrix) -> Vector4;
auto Length(const Vector4 value) -> Scalar;
auto LengthSquared(const Vector4 value) -> Scalar;
auto Distance(const Vector4 value1, const Vector4 value2) -> Scalar;
auto DistanceSquared(const Vector4 value1, const Vector4 value2) -> Scalar;
auto Normalize(const Vector4 value) -> Vector4;
auto Vector4(const Scalar x, const Scalar y, const Scalar z, const Scalar w) -> Vector4;
```

(the docs write Scale's parameter and result, and Abs's result, as Vector3 in
the Vector4 table - a copy-paste error in the docs; Vector4 is the only
sensible reading)

### Matrix3x2

```cpp
auto Scale(const Matrix3x2 value, const Scalar factor) -> Matrix3x2;
auto Inverse(const Matrix3x2 value) -> Matrix3x2;
auto Matrix3x2(const Scalar m11, const Scalar m12,
               const Scalar m21, const Scalar m22,
               const Scalar m31, const Scalar m32) -> Matrix3x2;
auto Matrix3x2::CreateFromScale(const Vector2 scale) -> Matrix3x2;
auto Matrix3x2::CreateFromTranslation(const Vector2 translation) -> Matrix3x2;
auto Matrix3x2::CreateSkew(const Scalar x, const Scalar y, const Vector2 centerpoint) -> Matrix3x2;
auto Matrix3x2::CreateRotation(const Scalar radians) -> Matrix3x2;
auto Matrix3x2::CreateTranslation(const Vector2 translation) -> Matrix3x2;
auto Matrix3x2::CreateScale(const Vector2 scale) -> Matrix3x2;
```

(CreateScale / CreateTranslation are documented as identical to the CreateFrom
forms; the toolkit only emits CreateScale / CreateTranslation)

### Matrix4x4

```cpp
auto Scale(const Matrix4x4 value, const Scalar factor) -> Matrix4x4;
auto Inverse(const Matrix4x4 value) -> Matrix4x4;
auto Matrix4x4(const Scalar m11, const Scalar m12, const Scalar m13, const Scalar m14,
               const Scalar m21, const Scalar m22, const Scalar m23, const Scalar m24,
               const Scalar m31, const Scalar m32, const Scalar m33, const Scalar m34,
               const Scalar m41, const Scalar m42, const Scalar m43, const Scalar m44) -> Matrix4x4;
auto Matrix4x4(const Matrix3x2 matrix) -> Matrix4x4;
auto Matrix4x4::CreateFromScale(const Vector3 scale) -> Matrix4x4;
auto Matrix4x4::CreateFromTranslation(const Vector3 translation) -> Matrix4x4;
auto Matrix4x4::CreateTranslation(const Vector3 translation) -> Matrix4x4;
auto Matrix4x4::CreateScale(const Vector3 scale) -> Matrix4x4;
auto Matrix4x4::CreateFromAxisAngle(const Vector3 axis, const Scalar angle) -> Matrix4x4;
```

### Quaternion

```cpp
auto Slerp(const Quaternion value1, const Quaternion value2, const Scalar progress) -> Quaternion;
auto Concatenate(const Quaternion value1, const Quaternion value2) -> Quaternion;
auto Length(const Quaternion value) -> Scalar;
auto LengthSquared(const Quaternion value) -> Scalar;
auto Normalize(const Quaternion value) -> Quaternion;
auto Quaternion::CreateFromAxisAngle(const Vector3 axis, const Scalar angle) -> Quaternion;
auto Quaternion(const Scalar x, const Scalar y, const Scalar z, const Scalar w) -> Quaternion;
```

### Color

```cpp
auto ColorLerp(const Color colorTo, const Color colorFrom, const Scalar progress) -> Color;
auto ColorLerpRGB(const Color colorTo, const Color colorFrom, const Scalar progress) -> Color;
auto ColorLerpHSL(const Color colorTo, const Color colorFrom, const Scalar progress) -> Color;
auto ColorRGB(const Scalar a, const Scalar r, const Scalar g, const Scalar b) -> Color;
auto ColorHsl(const Scalar h, const Scalar s, const Scalar l) -> Color;
```

- ColorRGB takes ARGB order, components 0-255. ColorHsl hue is 0..2pi.
- Spelling conflict, unverified: the docs name the lerps ColorLerpRGB and
  ColorLerpHSL, but the toolkit emits `colorrgblerp` and `colorhsllerp`
  (ColorRgbLerp / ColorHslLerp). Case-insensitivity does not reconcile these -
  the word order differs. Test both before the EDSL commits to one.

## Sources

- https://learn.microsoft.com/en-us/uwp/api/windows.ui.composition.expressionanimation?view=winrt-28000
- https://github.com/CommunityToolkit/WindowsCommunityToolkit/tree/main/Microsoft.Toolkit.Uwp.UI.Animations/Expressions/ExpressionNodes
- https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/main/Microsoft.Toolkit.Uwp.UI.Animations/Expressions/ExpressionFunctions.cs
