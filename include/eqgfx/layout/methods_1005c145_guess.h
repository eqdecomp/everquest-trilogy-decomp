// Member declarations for this original eqgfx address unit. Layouts precede this include.
class EqGfxD3duGlobalsMethods1005C145_guess : public EqGfxD3duGlobals_guess {
public:
    void checkAndMapD3duHandle_guess(unsigned int *handle_ptr);
    void FindBestMatchForHWLevel_guess(int requested_level, int *found);
};

class GraphicsObjectMethods1005C145_guess {
public:
    unsigned int getSurfaceClassFlags_guess(EqGfxPixelFormat_guess *pixel_format, int kind, void *direct3d7, void *ddraw, EqGfxTag16_guess tag);
};

class EqGfxBaseRecordMethods1005C145_guess : public EqGfxBaseRecord_guess {
public:
    void destroyBaseRecord_guess();
    int getBaseRecordOffset5C_guess();
};

class EqGfxKeyListMethods1005C145_guess : public EqGfxKeyList_guess {
public:
    int addKeyToList_guess(int key);
    int containsKey_guess(int key);
    int findNodeByKeyTriple_guess(EqGfxKeyNode_guess **node_out, int *key);
    int addPixelFormatNode_guess(EqGfxPixelFormat_guess *pixel_format, void *owner, void *ddraw, EqGfxTag16_guess tag);
};

class EqGfxDeviceNodeMethods1005C145_guess : public EqGfxDeviceNode_guess {
public:
    void getCurrentVideoMode_guess(EqGfxVideoMode_guess *video_mode);
    void getDeviceCapsForHWLevel_guess(EqGfxVideoMode_guess *requested_mode, int *mode_value_out, unsigned int *caps_value_out, void **device_out, int *mode_record_out);
};

