// Member declarations for this original eqgfx address unit. Layouts precede this include.
#ifdef EQGFX_DECLARE_GraphicsObjectMethods100616F5_guess
struct EqGfxVideoMode_guess;
class GraphicsObjectMethods100616F5_guess {
public:
    void t3dCloseMappedFile_guess();
    int thunk_FUN_10061942();
    int t3dGetMappedFileState_guess();
    int getMappedFileField114_guess();
    int getCommittedSize_guess();
    unsigned int openMappedContainerFile_guess(const char *file_name);
    int closeMappedFile_guess();
    int discardMappedFile_guess();
    int isMappedRecordInBounds_guess(unsigned int size);
    int getMappedViewDword_guess(unsigned int offset);
    int getRecordPayloadThunk_guess(unsigned int offset, unsigned int *value_out);
    int findRecordByKey_guess(unsigned int start_offset, int key);
    int findRecordByKeyAndData_guess(unsigned int start_offset, int key, char *data, unsigned int length);
    void t3dReleaseNodeSurface_guess();
    void createPrimarySurface_guess(int three_d, unsigned int backbuffer_count);
    void t3dGetCurrentVideoMode_guess(EqGfxVideoMode_guess *mode);
};
#endif

#ifdef EQGFX_DECLARE_EqGfxResourceListMethods100616F5_guess
class EqGfxResourceListMethods100616F5_guess : public EqGfxResourceList_guess {
public:
    void releaseGraphicsResource_guess();
};
#endif

#ifdef EQGFX_DECLARE_EqGfxDeviceNodeMethods100616F5_guess
struct EqGfxVideoMode_guess;
class EqGfxDeviceNodeMethods100616F5_guess : public EqGfxDeviceNode_guess {
public:
    void setDeviceNodeCooperativeLevel_guess(void *window, unsigned int flags, void *focus_window, int restore_flag);
    void t3dSetDisplayMode_guess(unsigned int width, unsigned int height, unsigned int bits_per_pixel, unsigned int refresh_rate, int flags);
    void obtainCapsAtDisplayMode_guess(EqGfxVideoMode_guess *requested_mode);
};
#endif

