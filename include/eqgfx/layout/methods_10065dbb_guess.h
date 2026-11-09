// Member declarations for this original eqgfx address unit. Layouts precede this include.
#ifdef EQGFX_DECLARE_GraphicsObjectMethods10065DBB_guess
struct FormatSource_guess;
struct ResourceCollection_guess;
struct ResourceLoadState_guess;
struct ResourceSourceList_guess;
struct ResourceSource_guess;
class GraphicsObjectMethods10065DBB_guess {
public:
    int loadResourceCollectionSources_guess(ResourceCollection_guess *sourceCollection);
    void clearObjectHolderAfterCallback_guess();
    int hasValidSurfaceBits_guess();
    int matchesKind6Or7_guess();
    int queryObjectDataBuffer_guess(void **outBuf);
    int appendEightByteElement_guess(void *src);
    int ensureRendererDataLoadedFromWindowsDirectory_guess();
    int registerResourceCollectionSources_guess(ResourceCollection_guess *sourceCollection);
    int t3dLoadCachedDeviceCaps_guess(ResourceSource_guess *source, ResourceLoadState_guess *state);
    int t3dBuildModeResourceSet_guess(ResourceSource_guess *source, ResourceLoadState_guess *state);
    int resolveRevisionRecord_guess();
    int getOrCreateRevisionResourceEntry_guess();
    int readPackedResourceRecord_guess(unsigned int resourceOffset, int readContext, int *outputBuffer);
    int t3dCreatePackedResourceRecord_guess(int resourceKey, int readContext, int resourceTextOrOffset);
    int t3dCreateResourceFromSources_guess(int resourceKey, int readContext, int resourceTextOrOffset, ResourceSourceList_guess *sourceList);
    int getOrAddTextResource_guess(char *text);
    int getOrAddFormatResource_guess(FormatSource_guess *formatSource);
};
#endif

