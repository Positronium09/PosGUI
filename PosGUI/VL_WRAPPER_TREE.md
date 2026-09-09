# VL Wrapper Write Order

Write top-to-bottom - every node's parent (and anything above it) already exists
when you write it. Siblings at the same indent level can be done in any order.
Indentation means "parent must exist first". Star markers are points where
something becomes visible on screen or an existing TODO clears - good commit
boundaries.

Already wrapped (not in the tree): Compositor (namespace), DesktopWindowTarget,
CompositionObject, Visual, VisualCollection, ContainerVisual, VLWindow, VLEnums.

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
|-- 5. CompositionClip                      <- CompositionObject
|   |-- 6. InsetClip
|   |-- 7. CompositionRectangleClip
|   `-- 8. CompositionGeometry              <- CompositionObject
|       |-- 9. CompositionEllipseGeometry
|       |-- 9.5. CompositionViewBox         <- geometry -> visual size mapping
|       `-- 10. CompositionGeometricClip    <- Clip + Geometry (+ ViewBox)
|       * clears the Visual.ixx //TODO Clip
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
- 6 InsetClip: insets are absolute pixels (DPI-unaware), negative insets expand
  past the bounds, and it is the only clip that auto-tracks Size changes.
- 10 CompositionGeometricClip: pulls in CompositionViewBox (9.5) - without it
  the geometry stays absolute-size when the visual resizes. Geometry lives in
  pre-transform local space.

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
