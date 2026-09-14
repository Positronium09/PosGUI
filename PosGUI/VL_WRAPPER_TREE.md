# VL Wrapper Write Order

Write top-to-bottom - every node's parent (and anything above it) already exists
when you write it. Siblings at the same indent level can be done in any order.
Indentation means "parent must exist first". Star markers are points where
something becomes visible on screen or an existing TODO clears - good commit
boundaries.

Already wrapped (not in the tree): Compositor (namespace), DesktopWindowTarget,
CompositionObject, Visual, VisualCollection, ContainerVisual, VLWindow, VLEnums.

[DONE] markers reflect modules/UI/VL as of 2026-08-28.

```
Compositor (namespace, done)
CompositionObject (done)
Visual (done)
|
|-- 1. CompositionBrush [DONE]                   <- CompositionObject
|   |-- 2. CompositionColorBrush [DONE]
|   |-- 3. CompositionSurfaceBrush [DONE]           <- also needs ICompositionSurface (see below, can stub)
|   |
|   `-- [later] CompositionBackdropBrush    <- only needed for acrylic
|
|-- 4. SpriteVisual [DONE]                       <- Visual + CompositionBrush
|       * FIRST VISIBLE MILESTONE: colored rect on screen
|
|-- 5. CompositionClip [DONE]               <- CompositionObject
|   |-- 6. InsetClip [DONE]
|   |-- 7. CompositionRectangleClip [DONE]
|   `-- 8. CompositionGeometry [DONE]       <- CompositionObject
|       |-- 9. CompositionEllipseGeometry [DONE]
|       |-- 9.5. CompositionViewBox [DONE]  <- geometry -> visual size mapping
|       |-- 10. CompositionGeometricClip [DONE] <- Clip + Geometry (+ ViewBox)
|       |-- bonus: CompositionRectangleGeometry, CompositionLineGeometry,
|       |          CompositionRoundedRectangleGeometry [DONE]
|       * cleared: Visual.ixx has GetClip/SetClip
|
|-- 11. CompositionAnimation                <- CompositionObject
|   |-- 12. ExpressionAnimation
|   |-- 13. ScalarKeyFrameAnimation
|   |-- 14. Vector3KeyFrameAnimation
|   |-- 15. ImplicitAnimationCollection     <- Animation
|   `-- [later] SpringVector3NaturalMotionAnimation
|       * unlocks StartAnimation on CompositionObject/Visual
|
|-- 16. CompositionPropertySet              <- CompositionObject (standalone, slot anywhere)
|
|-- 17. CompositionGraphicsDevice [DONE]         <- Compositor
|   |-- 18. CompositionDrawingSurface [DONE]  <- GraphicsDevice
|   |-- 19. CompositionVirtualDrawingSurface (deferrable)
|   `-- 20. ICompositionSurface [DONE]      <- surface types
|       * fills in SurfaceBrush (3) -> real D2D content on visuals
|
|-- 21. CompositionShadow                   <- CompositionObject
|   `-- 22. DropShadow                      <- Shadow (+ ImplicitAnimationCollection for the fancy bits)
|
|-- 23. CompositionLight                    <- CompositionObject
|   |-- 24. PointLight                      <- Light + Visual
|   `-- 25. AmbientLight
|
|-- 26. InteractionTracker                  <- CompositionObject + Animation
|   |-- 27. VisualInteractionSource         <- Visual
|   `-- 28. InteractionTrackerInertiaRestingValue <- ExpressionAnimation
|
`-- 29. CompositionEffectSourceParameter    <- standalone
    `-- 30. D2D1Effect descriptor
        `-- 31. CompositionEffectFactory    <- D2D1Effect + Compositor
            `-- 32. CompositionEffectBrush  <- Brush + Factory (+ BackdropBrush for acrylic)
```

## Soft edges

- CompositionSurfaceBrush (3) can land early with the surface property stubbed
  as `//TODO Surface` - the color-brush path does not need ICompositionSurface.
  Same trick anywhere a property's type is not wrapped yet (genwrapper.py
  already does this).

## Hard ones

- 11 CompositionAnimation: the stringly-typed boundary (StartAnimation /
  SetReferenceParameter / implicit animation keys are raw strings, silent
  failure on drift - the future EDSL territory). Typed SetParameter<T>
  dispatch over the per-type projected overloads is a design decision.
  Duration is TimeSpan <-> std::chrono.
- 3 CompositionSurfaceBrush: surface lifetime vs brush ownership; the
  winrt::impl::as hack from the tests must become a properly wrapped
  ICompositionSurface (attach_abi takes void* and checks nothing).
- 17 CompositionGraphicsDevice: first cross-world factory (COM ID2D1Device in
  via ICompositorInterop, projection out) and first event
  (RenderingDeviceReplaced - the pattern picked here becomes the template for
  all later VL events). Must be rebuilt when the D2D device is lost/recreated,
  so it couples to DXDevices lifetime.
- 6 InsetClip [DONE]: insets are absolute pixels (DPI-unaware), negative
  insets expand past the bounds, and it is the only clip that auto-tracks Size
  changes.
- 10 CompositionGeometricClip [DONE]: pulls in CompositionViewBox (9.5) -
  without it the geometry stays absolute-size when the visual resizes.
  Geometry lives in pre-transform local space.

## D2D <-> VL interop notes

- Color brushes: trivial (RGBA).
- Gradient/bitmap/image brushes: no VL equivalent - rasterize into a
  CompositionDrawingSurface via BeginDraw, then SurfaceBrush. The surface
  stack IS the brush interop layer.
- Clips: rect / rounded-rect / ellipse map across cleanly. InsetClip is
  VL-only. Path geometry is the hole: CompositionPath needs IGeometrySource2D,
  which without Win2D means implementing it over an ID2D1Geometry sink or
  rasterizing path-clipped content. Verify before committing.
- Design direction: prefer a PGUI-level brush/clip description realized as
  either a D2D brush (in a draw pass) or a VL brush (on a visual), over
  live-object conversion between the two families.

## Enums to add to VLEnums

- CompositionStretch, InteractionSourceMode (WUC / WUC::Interactions)
- DirectXPixelFormat, DirectXAlphaMode (Windows.Graphics.DirectX)

## Importance ranking

Ranked by what they unblock and how central they are to the framework
(not write order - the tree above has that).

### S tier - nothing works without these

1. CompositionBrush [DONE] - base of the whole brush family; gates
   SpriteVisual, DropShadow, everything acrylic.
2. SpriteVisual [DONE] - the entire point of the VL path.
3. CompositionColorBrush [DONE] - the only brush needed for the first
   visible pixel.
4. CompositionClip [DONE] - base of all clips; the Visual.ixx TODO hung on it.
5. CompositionAnimation - base of every animation type; StartAnimation on the
   already-wrapped CompositionObject is dead without it.

### A tier - the framework's actual personality

6. ExpressionAnimation - the VL superpower; most demos' motion runs on it.
7. ImplicitAnimationCollection - what makes UI feel alive without imperative
   animation code.
8. ScalarKeyFrameAnimation / Vector3KeyFrameAnimation - the workhorse pair.
9. CompositionGraphicsDevice [DONE] - the D2D seam.
10. CompositionDrawingSurface [DONE] + ICompositionSurface [DONE] - the
    content pipeline itself.
11. CompositionSurfaceBrush [DONE] - turns the pipeline into something
    visible; also IS the brush interop layer.

### B tier - commonly needed, self-contained

12. InsetClip [DONE] - simplest clip, reveal animations, auto-tracks size.
13. CompositionGeometry -> CompositionEllipseGeometry -> CompositionViewBox
    -> CompositionGeometricClip [ALL DONE] - one chain (plus bonus
    RectangleGeometry, LineGeometry, RoundedRectangleGeometry).
14. CompositionRectangleClip [DONE] - rounded corners, the most UI-typical
    clip.
15. CompositionShadow / DropShadow - depth, one-property visuals.
16. CompositionPropertySet - custom animation parameters.

### C tier - nice, leaf-ward, nothing depends on them

17. CompositionLight / PointLight / AmbientLight.
18. SpringVector3NaturalMotionAnimation - polish for interaction demos.
19. InteractionTracker / VisualInteractionSource /
    InteractionTrackerInertiaRestingValue - big subsystem, pure consumer;
    matters only when scroll/drag interactions land.
20. CompositionVirtualDrawingSurface - huge-canvas scenarios only.

### D tier - defer until acrylic is actually wanted

21. CompositionEffectSourceParameter -> D2D1Effect ->
    CompositionEffectFactory -> CompositionEffectBrush ->
    CompositionBackdropBrush - a five-class chain that exists for one feature
    (acrylic/blur); D2D1Effect is real winrt::implements work.

The two orderings agree on the critical path: brush -> sprite -> clip /
animation -> surface stack. Importance says where to spend care (S/A tiers
deserve hand-review; C/D tiers are genwrapper.py candidates).

Next up by both orderings: the animation core (11-15) - the last remaining
"hard one".
