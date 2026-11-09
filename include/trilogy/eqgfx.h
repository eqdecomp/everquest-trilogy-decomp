/*
 * eqgfx.h - the eqgfx_dx7.dll entry points eqgame.exe calls through function pointers.
 *
 * eqgame.exe does not import eqgfx_dx7.dll: at start-up it LoadLibrary()s it and fills one global
 * pointer per export with GetProcAddress(hGfx, "<name>") (the loader also zeroes them all first).
 * Every call site is `call dword ptr [ptr]`, caller-cleaned (cdecl). The renderer is rewritten last
 * (PLAN Phase 1e), so its exports stay a fixed boundary: rewrites call them through these macros,
 * which compile to the same `call dword ptr [addr]`. Each entry is a pointer typedef `<name>_t` plus
 * `#define <name> EQ_GLOBAL(<name>_t, <pointer VA>)`, the eqaddr.h convention for function pointer
 * variables.
 *
 * Names: the GetProcAddress string stored into each pointer (verified against client: the
 * `DAT_xxx = GetProcAddress(..., "name")` assignments in the loader). Ghidra truncates long string
 * labels; truncated names are completed by hand and marked (completed).
 * Argument and return types are from the eqgame.exe call sites only (what is pushed / used), not
 * from the DLL; `void*` = an opaque renderer handle.
 *
 * Add entries in address order. C++ only (VC6); not included from trilogy.h.
 */
#ifndef TRILOGY_EQGFX_H
#define TRILOGY_EQGFX_H

#include "common.h"
#include "eqaddr.h"

typedef int   (__cdecl *s3dGetNumSkinsAttachedToHierarchicalSprite_t)(void* hierSprite);
typedef void* (__cdecl *s3dGetSkinAttachedToHierarchicalSprite_t)(int index, void* hierSprite);
/* nonzero when the segment start..end hits world geometry; the hit point goes to `hit` (EQMisc CastRay) */
typedef int   (__cdecl *s3dRaySegmentHitsWorldSolid_t)(void* world_a, void* world_b, float* start, float* end,
                                                       float* hit);
typedef int   (__cdecl *s3dSetDMSpriteConstantAmbient_t)(void* dmSprite, u8* ambient_rgb);
typedef int   (__cdecl *t3dClearRenderToBlack_t)(int unused_guess);
typedef int   (__cdecl *t3dDeferRect_t)(void* render_guess, u32 rect_guess);
typedef int   (__cdecl *t3dDestroyParticleCloud_t)(void* world, void* cloud);
typedef int   (__cdecl *t3dDestroyPointLight_t)(void* world, void* light);
typedef int   (__cdecl *t3dDeviceSupportsResolution_t)(int modeFlags);
typedef float (__cdecl *t3dFloatSqrt_t)(float value);
typedef int*  (__cdecl *t3dGetActorSprite_t)(void* actor, int unknown1, int unknown2);
typedef float (__cdecl *t3dGetScaledActorBoundingRadius_t)(void* actor);
typedef int   (__cdecl *t3dOverlayDestroy_t)(void* overlay);
typedef int   (__cdecl *t3dSetActorScaleFactor_t)(void* actor, float scale, u8 flag_guess);
typedef int   (__cdecl *t3dSetActorSurfacePitchType_t)(void* actor, int pitchType);
typedef int   (__cdecl *t3dSetActorUserData_t)(void* actor, void* userData);
typedef int   (__cdecl *t3dSetBlitSpriteTint_t)(void* render_guess, int* sprite, void* tint);
typedef int   (__cdecl *t3dSetGammaLevel_t)(float gamma);
typedef int   (__cdecl *t3dSetSkyDualLayer_t)(void* sky, int dual);
typedef int   (__cdecl *t3dUpdateDisplay_t)(void* render_guess);

#define s3dGetNumSkinsAttachedToHierarchicalSprite \
    EQ_GLOBAL(s3dGetNumSkinsAttachedToHierarchicalSprite_t, 0x006E5110)   /* (completed) */
#define s3dGetSkinAttachedToHierarchicalSprite \
    EQ_GLOBAL(s3dGetSkinAttachedToHierarchicalSprite_t, 0x006E5114)       /* (completed) */
#define s3dRaySegmentHitsWorldSolid     EQ_GLOBAL(s3dRaySegmentHitsWorldSolid_t, 0x006E5128)
#define s3dSetDMSpriteConstantAmbient   EQ_GLOBAL(s3dSetDMSpriteConstantAmbient_t, 0x006E5130)
#define t3dClearRenderToBlack           EQ_GLOBAL(t3dClearRenderToBlack_t, 0x006E5178)
#define t3dDeferRect                    EQ_GLOBAL(t3dDeferRect_t, 0x006E51BC)
#define t3dDestroyParticleCloud         EQ_GLOBAL(t3dDestroyParticleCloud_t, 0x006E5218)
#define t3dDestroyPointLight            EQ_GLOBAL(t3dDestroyPointLight_t, 0x006E521C)
#define t3dDeviceSupportsResolution     EQ_GLOBAL(t3dDeviceSupportsResolution_t, 0x006E5230)
#define t3dFloatSqrt                    EQ_GLOBAL(t3dFloatSqrt_t, 0x006E5258)
#define t3dGetActorSprite               EQ_GLOBAL(t3dGetActorSprite_t, 0x006E5288)
#define t3dGetScaledActorBoundingRadius EQ_GLOBAL(t3dGetScaledActorBoundingRadius_t, 0x006E52F0)
#define t3dOverlayDestroy               EQ_GLOBAL(t3dOverlayDestroy_t, 0x006E5344)
#define t3dSetActorScaleFactor          EQ_GLOBAL(t3dSetActorScaleFactor_t, 0x006E5380)
#define t3dSetActorSurfacePitchType     EQ_GLOBAL(t3dSetActorSurfacePitchType_t, 0x006E5388)
#define t3dSetActorUserData             EQ_GLOBAL(t3dSetActorUserData_t, 0x006E538C)
#define t3dSetBlitSpriteTint            EQ_GLOBAL(t3dSetBlitSpriteTint_t, 0x006E5390)
#define t3dSetGammaLevel                EQ_GLOBAL(t3dSetGammaLevel_t, 0x006E53E4)
#define t3dSetSkyDualLayer              EQ_GLOBAL(t3dSetSkyDualLayer_t, 0x006E5450)
#define t3dUpdateDisplay                EQ_GLOBAL(t3dUpdateDisplay_t, 0x006E5474)

/* EQAnimate / Physics (Phase 1d). */
typedef int*  (__cdecl *t3dGetDagSprite_t)(void* dag);
typedef void* (__cdecl *t3dGetHierarchicalSpriteDefinition_t)(int* sprite);
typedef u32   (__cdecl *t3dGetHierarchicalSpriteDefinitionNumDags_t)(void* definition);
typedef void* (__cdecl *t3dGetHierarchicalSpriteDagByIndex_t)(int* sprite, u32 index);
typedef int   (__cdecl *t3dDagHasPendingTrack_t)(void* dag);
typedef int   (__cdecl *t3dSetDagSpeed_t)(void* dag, float speed);
typedef int   (__cdecl *t3dSetDagReverse_t)(void* dag, int reverse);
typedef int   (__cdecl *t3dSetDagPendingSpeed_t)(void* dag, float speed);
typedef int   (__cdecl *t3dSetDagPendingReverse_t)(void* dag, int reverse);
/* 9 floats, row-major: the rotation for (heading, pitch, roll). */
typedef int   (__cdecl *t3dCalculateAccurateCoefficientsFromHeadingPitchRoll_t)(float heading, float pitch, float roll,
                                                                                 float* matrix);
typedef float (__cdecl *t3dFloatFastCosine_t)(float heading);
typedef float (__cdecl *t3dFloatFastSine_t)(float heading);
/* location = y, x, z, heading, pitch, ... ; nonzero = failure. */
typedef int   (__cdecl *t3dSetActorLocation_t)(void* world, void* actor, float* location);

#define t3dGetDagSprite                           EQ_GLOBAL(t3dGetDagSprite_t, 0x006E52B8)
#define t3dGetHierarchicalSpriteDefinition        EQ_GLOBAL(t3dGetHierarchicalSpriteDefinition_t, 0x006E52CC)
#define t3dGetHierarchicalSpriteDefinitionNumDags EQ_GLOBAL(t3dGetHierarchicalSpriteDefinitionNumDags_t, 0x006E52D8)
#define t3dGetHierarchicalSpriteDagByIndex        EQ_GLOBAL(t3dGetHierarchicalSpriteDagByIndex_t, 0x006E52C4)
#define t3dDagHasPendingTrack                     EQ_GLOBAL(t3dDagHasPendingTrack_t, 0x006E51B0)
#define t3dSetDagSpeed                            EQ_GLOBAL(t3dSetDagSpeed_t, 0x006E53C8)
#define t3dSetDagReverse                          EQ_GLOBAL(t3dSetDagReverse_t, 0x006E53C4)
#define t3dSetDagPendingSpeed                     EQ_GLOBAL(t3dSetDagPendingSpeed_t, 0x006E53C0)
#define t3dSetDagPendingReverse                   EQ_GLOBAL(t3dSetDagPendingReverse_t, 0x006E53BC)
#define t3dCalculateAccurateCoefficientsFromHeadingPitchRoll     EQ_GLOBAL(t3dCalculateAccurateCoefficientsFromHeadingPitchRoll_t, 0x006E5168)
#define t3dFloatFastCosine          EQ_GLOBAL(t3dFloatFastCosine_t, 0x006E524C)
#define t3dFloatFastSine            EQ_GLOBAL(t3dFloatFastSine_t, 0x006E5250)
#define t3dSetActorLocation         EQ_GLOBAL(t3dSetActorLocation_t, 0x006E537C)

/* Display (Phase 1d wave 3). */
typedef void* (__cdecl *s3dGetDMSpriteMaterialPalette_t)(void* dmSprite);
typedef int   (__cdecl *s3dGetNumMaterialPaletteEntries_t)(void* world, void* palette);
typedef int   (__cdecl *s3dSetCameraActorYon_t)(void* camera, float yon);
typedef int   (__cdecl *s3dSetMaterialPaletteEntryTint_t)(void* world, void* palette, int index, u32* tint);
typedef void* (__cdecl *t3dCreateLight_t)(void* world, void* lightDef);
typedef void* (__cdecl *t3dCreatePalette_t)(void* world, void* a, void* b, int flag);
typedef void* (__cdecl *t3dCreatePointLightEx_t)(void* world, void* light, float* color, float radius, int flag);
/* flags 0x40000003 | blend << 18; color = b, g, r, 0; four (x, y, z) corners, z left unset by DoPoly */
typedef int   (__cdecl *t3dDefer2DQuad_t)(u32 flags, void* color, float* corners);
typedef int   (__cdecl *t3dDestroyPalette_t)(void* world, void* palette);
/* fills 7 floats from y, x, z */
typedef int   (__cdecl *t3dGetActorLocation_t)(void* actor, float* location);
typedef int   (__cdecl *t3dOverlayApply_t)(void* overlay, int x, int y, int arg);
typedef int   (__cdecl *t3dOverlayClear_t)(void* overlay, int arg);
typedef int   (__cdecl *t3dSetActorInvisible_t)(void* actor, int invisible);
/* nonzero = error code */
typedef int   (__cdecl *t3dSetCameraLocation_t)(void* world, void* camera, float* location);
typedef int   (__cdecl *t3dSetCameraRenderWindow_t)(void* camera, int* rect);
typedef int   (__cdecl *t3dSetPCloudDensityMultiplier_t)(float multiplier);
typedef int   (__cdecl *t3dSetPointLightDynamicSceneInfluenceFlag_t)(void* pointLight);
typedef int   (__cdecl *t3dSetPointLightStaticSceneInfluenceFlag_t)(void* pointLight);

#define s3dGetDMSpriteMaterialPalette   EQ_GLOBAL(s3dGetDMSpriteMaterialPalette_t, 0x006E5100)
#define s3dGetNumMaterialPaletteEntries EQ_GLOBAL(s3dGetNumMaterialPaletteEntries_t, 0x006E5108)
#define s3dSetCameraActorYon            EQ_GLOBAL(s3dSetCameraActorYon_t, 0x006E512C)
#define s3dSetMaterialPaletteEntryTint  EQ_GLOBAL(s3dSetMaterialPaletteEntryTint_t, 0x006E5140)
#define t3dCreateLight                  EQ_GLOBAL(t3dCreateLight_t, 0x006E5190)
#define t3dCreatePalette                EQ_GLOBAL(t3dCreatePalette_t, 0x006E5194)
#define t3dCreatePointLightEx           EQ_GLOBAL(t3dCreatePointLightEx_t, 0x006E519C)
#define t3dDefer2DQuad                  EQ_GLOBAL(t3dDefer2DQuad_t, 0x006E51B4)
#define t3dDestroyPalette               EQ_GLOBAL(t3dDestroyPalette_t, 0x006E5214)
#define t3dGetActorLocation             EQ_GLOBAL(t3dGetActorLocation_t, 0x006E5284)
#define t3dOverlayApply                 EQ_GLOBAL(t3dOverlayApply_t, 0x006E5334)
#define t3dOverlayClear                 EQ_GLOBAL(t3dOverlayClear_t, 0x006E5338)
#define t3dSetActorInvisible            EQ_GLOBAL(t3dSetActorInvisible_t, 0x006E5378)
#define t3dSetCameraLocation            EQ_GLOBAL(t3dSetCameraLocation_t, 0x006E539C)
#define t3dSetCameraRenderWindow        EQ_GLOBAL(t3dSetCameraRenderWindow_t, 0x006E53A0)
#define t3dSetPCloudDensityMultiplier   EQ_GLOBAL(t3dSetPCloudDensityMultiplier_t, 0x006E53FC)
#define t3dSetPointLightDynamicSceneInfluenceFlag EQ_GLOBAL(t3dSetPointLightDynamicSceneInfluenceFlag_t, 0x006E5424)
#define t3dSetPointLightStaticSceneInfluenceFlag  EQ_GLOBAL(t3dSetPointLightStaticSceneInfluenceFlag_t, 0x006E542C)
typedef int   (__cdecl *_t3dComputeActorDefinitionBoundingRadius_t)(void* definition, float* radius);
typedef void* (__cdecl *_t3dDefineParticleCloud_t)(void* world, const char* name, int a, int flags, int b, float c,
                                                   float* direction, int d, int e, int f, int g, int h, int i,
                                                   float* velocity, float j, float k, int l, float m, int n, float o,
                                                   u8* tint, void* sprite);
typedef void* (__cdecl *s3dCreateStringSprite_t)(void* world, int a, void* font, const char* text, u32 flags);
typedef int   (__cdecl *s3dDestroyStringSprite_t)(void* world, void* sprite);
typedef void* (__cdecl *s3dPickObjectOnScreen_t)(void* world, void* render, void* camera, void* exclude, int x, int y,
                                                 int flag);
typedef int   (__cdecl *s3dSetStringSpriteMaxScaleFactor_t)(void* sprite, int flag, float scale);
typedef int   (__cdecl *s3dSetStringSpriteTint_t)(void* sprite, u8* rgb);
typedef int   (__cdecl *s3dSetStringSpriteYonClip_t)(void* sprite, int flag, float yon);
typedef int   (__cdecl *t3dBindParticleCloudSpawn_t)(void* cloud, void* dag);
typedef void* (__cdecl *t3dCreateParticleCloud_t)(void* world, void* definition, float* location, int a, int b);
typedef int   (__cdecl *t3dDestroyActor_t)(void* world, void* actor);
typedef void* (__cdecl *t3dGetPointerFromDictionary_t)(void* world, const char* name);
typedef int   (__cdecl *t3dSetActorBoundingRadius_t)(void* actor, float radius, int flag);
typedef int   (__cdecl *t3dSetDagSprite_t)(void* dag, void* sprite);
typedef int   (__cdecl *t3dUpdateTouchedRegions_t)(void* world, void* actor, void* region);

#define _t3dComputeActorDefinitionBoundingRadius EQ_GLOBAL(_t3dComputeActorDefinitionBoundingRadius_t, 0x006E50AC)
#define _t3dDefineParticleCloud         EQ_GLOBAL(_t3dDefineParticleCloud_t, 0x006E50B0)
#define s3dCreateStringSprite           EQ_GLOBAL(s3dCreateStringSprite_t, 0x006E50D4)
#define s3dDestroyStringSprite          EQ_GLOBAL(s3dDestroyStringSprite_t, 0x006E50E0)
#define s3dPickObjectOnScreen           EQ_GLOBAL(s3dPickObjectOnScreen_t, 0x006E5124)
#define s3dSetStringSpriteMaxScaleFactor EQ_GLOBAL(s3dSetStringSpriteMaxScaleFactor_t, 0x006E5148)
#define s3dSetStringSpriteTint          EQ_GLOBAL(s3dSetStringSpriteTint_t, 0x006E514C)
#define s3dSetStringSpriteYonClip       EQ_GLOBAL(s3dSetStringSpriteYonClip_t, 0x006E5150)
#define t3dBindParticleCloudSpawn       EQ_GLOBAL(t3dBindParticleCloudSpawn_t, 0x006E5160)
#define t3dCreateParticleCloud          EQ_GLOBAL(t3dCreateParticleCloud_t, 0x006E5198)
#define t3dDestroyActor                 EQ_GLOBAL(t3dDestroyActor_t, 0x006E5208)
#define t3dGetPointerFromDictionary     EQ_GLOBAL(t3dGetPointerFromDictionary_t, 0x006E52E0)
#define t3dSetActorBoundingRadius       EQ_GLOBAL(t3dSetActorBoundingRadius_t, 0x006E5370)
#define t3dSetDagSprite                 EQ_GLOBAL(t3dSetDagSprite_t, 0x006E53CC)
#define t3dUpdateTouchedRegions         EQ_GLOBAL(t3dUpdateTouchedRegions_t, 0x006E5478)

#endif /* TRILOGY_EQGFX_H */
