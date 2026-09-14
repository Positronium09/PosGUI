# VL Animatable Properties

All properties animatable via CompositionObject.StartAnimation, grouped by
object. There is no complete official list - the StartAnimation remarks table
dates from 1511 and omits Visual.Scale and every 1803+ type - so each entry is
tagged with what confirms it:

- D - Microsoft docs: property page marked "Animatable", or the class remarks list it
- T - Community Toolkit ExpressionBuilder reference nodes
- L - Lottie-Windows translator calls StartAnimation on it in shipped code
- unverified - settable property of an animatable type, nothing above confirms it

Unless noted, a property accepts both keyframe and expression animations.
TransformMatrix (Matrix3x2 and Matrix4x4 alike) is expression-only - there is
no Matrix keyframe animation.

## Visual

Applies to ContainerVisual, SpriteVisual, LayerVisual, ShapeVisual, RedirectVisual.

| Property | Type | Source |
| --- | --- | --- |
| AnchorPoint | Vector2 | D T |
| CenterPoint | Vector3 | D T |
| IsVisible | Boolean | L |
| Offset | Vector3 | D T L |
| Opacity | Scalar | D T L |
| Orientation | Quaternion | D T |
| RelativeOffsetAdjustment | Vector3 | T (named "RelativeOffset" there) |
| RelativeSizeAdjustment | Vector2 | T (named "RelativeSize" there) |
| RotationAngle | Scalar (radians) | D T |
| RotationAngleInDegrees | Scalar | D T L |
| RotationAxis | Vector3 | D T |
| Scale | Vector3 | T L (absent from docs) |
| Size | Vector2 | D T |
| TransformMatrix | Matrix4x4, expression-only | D T |

## Clips

CompositionClip base - applies to InsetClip, RectangleClip, CompositionGeometricClip:

| Property | Type | Source |
| --- | --- | --- |
| AnchorPoint | Vector2 | T |
| CenterPoint | Vector2 | T |
| Offset | Vector2 | T |
| RotationAngle | Scalar | T |
| RotationAngleInDegrees | Scalar | T |
| Scale | Vector2 | T |
| TransformMatrix | Matrix3x2, expression-only | T |

| Object | Properties | Type | Source |
| --- | --- | --- | --- |
| InsetClip | LeftInset, TopInset, RightInset, BottomInset | Scalar | D T |
| RectangleClip | Left, Top, Right, Bottom | Scalar | D |
| RectangleClip | TopLeftRadius, TopRightRadius, BottomLeftRadius, BottomRightRadius | Vector2 | D (class remarks: "fully animatable") |

## Brushes

| Object | Properties | Type | Source |
| --- | --- | --- | --- |
| CompositionColorBrush | Color | Color | D T L |
| CompositionSurfaceBrush | AnchorPoint, CenterPoint, Offset, Scale | Vector2 | T |
| CompositionSurfaceBrush | RotationAngle, RotationAngleInDegrees, HorizontalAlignmentRatio, VerticalAlignmentRatio | Scalar | T |
| CompositionSurfaceBrush | TransformMatrix | Matrix3x2, expression-only | T |
| CompositionNineGridBrush | LeftInset, TopInset, RightInset, BottomInset, LeftInsetScale, TopInsetScale, RightInsetScale, BottomInsetScale | Scalar | T |
| CompositionGradientBrush (base) | AnchorPoint, CenterPoint, Offset, Scale (Vector2); RotationAngle, RotationAngleInDegrees (Scalar); TransformMatrix (Matrix3x2) | - | unverified |
| CompositionLinearGradientBrush | StartPoint, EndPoint | Vector2 | L |
| CompositionRadialGradientBrush | EllipseCenter | Vector2 | L |
| CompositionRadialGradientBrush | EllipseRadius, GradientOriginOffset | Vector2 | unverified |
| CompositionColorGradientStop | Color | Color | L |
| CompositionColorGradientStop | Offset | Scalar | L |

## Geometry

| Object | Properties | Type | Source |
| --- | --- | --- | --- |
| CompositionGeometry (base) | TrimStart, TrimEnd, TrimOffset | Scalar | L |
| CompositionEllipseGeometry | Center, Radius | Vector2 | L |
| CompositionLineGeometry | Start, End | Vector2 | unverified |
| CompositionPathGeometry | Path | Path, keyframe-only | L |
| CompositionRectangleGeometry | Offset, Size | Vector2 | unverified |
| CompositionRoundedRectangleGeometry | CornerRadius, Offset, Size | Vector2 | L |

## Shapes

| Object | Properties | Type | Source |
| --- | --- | --- | --- |
| CompositionShape (base) | Offset, Scale, CenterPoint | Vector2 | L |
| CompositionShape (base) | RotationAngleInDegrees | Scalar | L |
| CompositionShape (base) | RotationAngle | Scalar | unverified |
| CompositionShape (base) | TransformMatrix | Matrix3x2, expression-only | unverified |
| CompositionSpriteShape | StrokeThickness, StrokeDashOffset | Scalar | L |
| CompositionSpriteShape | StrokeMiterLimit | Scalar | unverified |

## CompositionViewBox

| Property | Type | Source |
| --- | --- | --- |
| Offset | Vector2 | unverified |
| Size | Vector2 | unverified |
| HorizontalAlignmentRatio | Scalar | unverified |
| VerticalAlignmentRatio | Scalar | unverified |

No source touches this class at all.

## DropShadow

| Property | Type | Source |
| --- | --- | --- |
| BlurRadius | Scalar | D T |
| Color | Color | D T |
| Offset | Vector3 | D T L |
| Opacity | Scalar | D T L |

Not animatable despite the docs: Mask. Its page says "Animatable" but it is a
brush reference - copy-paste error.

## Lights

| Object | Properties | Source |
| --- | --- | --- |
| AmbientLight | Color | D T |
| DistantLight | Color, Direction | T |
| PointLight | Color, Offset, ConstantAttenuation, LinearAttenuation, QuadraticAttenuation | T |
| SpotLight | Offset, Direction, ConstantAttenuation, LinearAttenuation, QuadraticAttenuation, InnerConeAngle, InnerConeAngleInDegrees, OuterConeAngle, OuterConeAngleInDegrees, InnerConeColor, OuterConeColor | D T |

Unverified: Intensity (Ambient, Distant, Point), InnerConeIntensity and
OuterConeIntensity (Spot), MinAttenuationCutoff and MaxAttenuationCutoff (Point,
Spot). The properties exist but nothing marks them animatable.

PointLight Offset/Color animation confirmed by VLTest.cpp LightShineTestWnd.

## Property sets

Every CompositionObject carries a Properties set (CompositionObject.Properties),
not only a standalone CompositionPropertySet - a custom value can be inserted on
any visual, brush or geometry and animated there. Insertable and animatable by
name: Boolean, Scalar, Vector2, Vector3, Vector4, Color, Quaternion, Matrix3x2,
Matrix4x4 (matrices expression-only).

## Effect properties under CompositionEffectBrush (D)

| Effect Name | Property Name | Type |
| --- | --- | --- |
| SaturationEffect | Saturation | Scalar |
| ColorSourceEffect | Color | Vector4 |
| ArithmeticComposite | Offset, Source1Amount, Source2Amount, MultiplyAmount | Scalar |
| Transform2D | TransformMatrix | Matrix3x2 |
| ContrastEffect | Contrast | Scalar |
| ExposureEffect | Exposure | Scalar |
| HueRotationEffect | Angle | Scalar |
| SepiaEffect | Intensity | Scalar |
| TemperatureAndTintEffect | Temperature, Tint | Scalar |
| GammaTransferEffect | RedAmplitude, RedExponent, RedOffset, GreenAmplitude, GreenExponent, GreenOffset, BlueAmplitude, BlueExponent, BlueOffset, AlphaAmplitude, AlphaExponent, AlphaOffset | Scalar |

Two prerequisites before any of these can be animated:

- Declare them up front. Compositor.CreateEffectFactory(effect,
  animatableProperties) takes the "EffectName.Property" strings to be animated;
  a property left out of that list cannot be animated on the resulting brush.
- A Win2D-free descriptor must implement
  IGraphicsEffectD2D1Interop::GetNamedPropertyMapping. That is how Composition
  resolves "Blur.StandardDeviation" to a D2D property index; without it no
  property of the descriptor is reachable by name.

## Not animatable

- Enums: Stretch, BorderMode, CompositeMode, ExtendMode,
  BitmapInterpolationMode, stroke caps/joins, CoordinateSpace
- Object references: Brush, Clip, Geometry, ViewBox, Surface, Mask,
  FillBrush/StrokeBrush
- Collections: ColorStops, StrokeDashArray, Shapes
- InteractionTracker: its properties are read-only - readable from
  expressions, never a StartAnimation target. Animate it through
  TryUpdatePositionWithAnimation / TryUpdateScaleWithAnimation.

## Wrapper-relevant notes

- Sub-channel targeting exists: expression animations can target "Offset.X",
  "TransformMatrix._11" etc., not just whole properties. Property-name
  handling must not assume top-level names only.
- Keyframe animation types are fixed: Scalar, Vector2, Vector3, Vector4,
  Color, Quaternion, Boolean, Path. There is NO Matrix keyframe animation -
  TransformMatrix is expression-animation-only. Natural concept constraint
  for a typed StartAnimation design.
- Natural motion animations exist only for Scalar, Vector2 and Vector3 - no
  spring/decay on Color, Quaternion or Vector4.
- Path keyframes have exactly one built-in target (CompositionPathGeometry.Path);
  Boolean keyframes have one built-in target (Visual.IsVisible) plus property-set
  values.

## Sources

- https://learn.microsoft.com/en-us/uwp/api/windows.ui.composition.compositionobject.startanimation?view=winrt-28000
- https://learn.microsoft.com/en-us/uwp/api/windows.ui.composition.visual?view=winrt-28000
- https://learn.microsoft.com/en-us/uwp/api/windows.ui.composition.rectangleclip?view=winrt-28000
- https://learn.microsoft.com/en-us/uwp/api/windows.ui.composition.compositionsurfacebrush?view=winrt-28000
- https://learn.microsoft.com/en-us/uwp/api/windows.ui.composition.dropshadow?view=winrt-28000
- https://github.com/MicrosoftDocs/winrt-api (windows.ui.composition/ - per-property "Animatable" markers)
- https://github.com/CommunityToolkit/WindowsCommunityToolkit (Microsoft.Toolkit.Uwp.UI.Animations/Expressions/ReferenceNodes)
- https://github.com/CommunityToolkit/Lottie-Windows (source/LottieToWinComp)
