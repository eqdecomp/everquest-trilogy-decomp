// unit_1002902B.c - eqgfx_dx7.dll, the address run 0x1002902B..0x1002EA9F (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>

#include "trilogy/eqaddr.h"

#define ftell_guess EQ_CDECL(long, 0x100693F7, (FILE *))
#define FUN_10069984_guess EQ_CDECL(double, 0x10069984, (double))
#define t3dWriteBytes_guess EQ_CDECL(unsigned int, 0x1002ECA4, (ActorSave_guess *, void *, size_t))
#define t3dWriteVarInt_guess EQ_CDECL(void, 0x1002EAB3, (void *, int))
#define t3dWriteTaggedFloat_guess EQ_CDECL(void, 0x1002EB4A, (void *, float))
#define t3dWriteTag4Data_guess EQ_CDECL(void, 0x1002EB6B, (void *, const char *, unsigned int))
#define t3dGet2DSpriteDefinitionTag_guess EQ_CDECL(void, 0x1001B098, (T3DObject_guess *, char *))
#define t3dGet2DSpriteDefinitionTagLength_guess EQ_CDECL(unsigned int, 0x1000D7BE, (T3DObject_guess *))
#define t3dGetActorWorldY_guess EQ_CDECL(float, 0x10018E89, (T3DObject_guess *))
#define g_binaryFileSignature_guess EQ_GLOBAL(unsigned char, 0x100762B8)
#define FUN_1002dd6f_call EQ_CDECL(unsigned int, 0x1002DD6F, (int))
#define FUN_1002dd0f_call EQ_CDECL(unsigned int, 0x1002DD0F, (int))
#define t3dAllocateMemory_call EQ_CDECL(void *, 0x10050C48, (size_t))
#define EQGFX_T3D_FLOAT_SQRT EQ_CDECL(float, 0x1004EF68, (float))
#define _t3dIsNonzeroVector4_call EQ_CDECL(int, 0x1002E649, (float *))
#define t3dCalculateNormalToEdgePolygon_call EQ_CDECL(int, 0x10011AEA, (void *))
#define logParserError_call EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *, ...))
#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define s_CreateEdgePolygon_indexOutOfRange ((char *)0x100a13d4)
#define s_CreateEdgePolygon_normalFailure ((char *)0x100a13a4)
#define s_AddRegion_dictionaryFailure ((char *)0x100a1634)
#define s_AddRegion_tagInUse ((char *)0x100a166c)
#define s_AddRegion_listFailure ((char *)0x100a168c)
#define s_CreateSphereList_noDefinition ((char *)0x100a1610)
#define s_CreateSphereList_createFailure ((char *)0x100a15f0)
#define s_CreateSphereList_tagFailure ((char *)0x100a15d4)
#define s_CreateSphereList_scaleFailure ((char *)0x100a15b0)
#define t3dGetActorDefinition_call EQ_CDECL(T3DObject_guess *, 0x1000ACC3, (T3DObject_guess *))
#define t3dIsActorAIActive_call EQ_CDECL(int, 0x1000ACF5, (T3DObject_guess *))
#define t3dGetActorCurrentAction_call EQ_CDECL(int, 0x1002F47D, (T3DObject_guess *))
#define t3dGetActorLocation_call EQ_CDECL(void, 0x1000A663, (T3DObject_guess *, float *))
#define s3dGetDMSpriteCollisionVolume_call EQ_CDECL(T3DObject_guess *, 0x10010703, (T3DObject_guess *))
#define t3dGetActorRenderVolume_call EQ_CDECL(T3DObject_guess *, 0x1000A9FF, (T3DObject_guess *))
#define t3dGetActorUserData_call EQ_CDECL(void *, 0x1000A65B, (T3DObject_guess *))
#define t3dSendMessageToActor_call EQ_CDECL(void, 0x1000A553, (unsigned int, T3DObject_guess *, ActorMessage_guess *))
#define EQGFX_T3D_FREE_MEMORY_GUESS EQ_CDECL(void, 0x10050CBB, (void *))
#define t3dWriteTaggedInteger_call EQ_CDECL(void, 0x1002EB07, (void *, int))
#define t3dWriteSizeTaggedData_call EQ_CDECL(void, 0x1002EB8D, (void *, void *, unsigned int))
#define t3dSaveActorVolume_call EQ_CDECL(void, 0x1002E907, (void *, T3DObject_guess *))
#define t3dSetActorSaveStream_call EQ_CDECL(int, 0x1002EA85, (ActorSave_guess *, FILE *))
#define _t3dWriteActorSaveBytes_call EQ_CDECL(void, 0x1002EA9F, (ActorSave_guess *))
#define t3dWriteMarker3_call EQ_CDECL(void, 0x1002EBC2, (ActorSave_guess *))
#define t3dSaveActorInstance_call EQ_CDECL(unsigned int, 0x1002E737, (void *, ActorInstance_guess *))
#define t3dSaveAllActorInstances_call EQ_CDECL(int, 0x1002E6C4, (FILE *, ActorList_guess *))
#define _t3dAddItemToList_call EQ_CDECL(int, 0x1001564B, (void *, Region_guess *))
#define t3dGetPointerFromDictionary_call EQ_CDECL(void *, 0x1000E7FE, (void *, unsigned int))
#define t3dAddEntryToDictionary_call EQ_CDECL(int, 0x1000DC08, (void *, Region_guess *))
#define t3dCreatePolyhedron_call EQ_CDECL(void *, 0x10018CF8, (void *, void *))
#define t3dSetPolyhedronTag_call EQ_CDECL(int, 0x10018DEB, (void *, void *, char *))
#define t3dSetPolyhedronDefinitionBoundingRadius_call EQ_CDECL(int, 0x1001B0BB, (void *, float))
#define t3dCreateSphereList_call EQ_CDECL(void *, 0x1001B047, (void *, void *))
#define ACTOR_OBJECT_KIND 0x18

typedef struct ActorInstance_guess
{
    unsigned int kind00;
    unsigned char _04[0x24];
    struct ActorInstance_guess *primaryInstance28;
    unsigned char _2c[0x38];
    struct ActorInstance_guess *definition64;
} ActorInstance_guess;

typedef struct ActorList_guess
{
    unsigned char _00[0x170];
    unsigned int actorCount170;
    unsigned char _174[8];
    ActorInstance_guess **actors17c;
} ActorList_guess;

typedef struct EdgePolygonEdge_guess
{
    unsigned int kind00;
    float *firstVertex04;
    float *secondVertex08;
    float length0c;
    unsigned int unused10;
} EdgePolygonEdge_guess;

typedef struct EdgePolygon_guess
{
    unsigned int kind00;
    int edgeCount04;
    EdgePolygonEdge_guess *edges08;
    float normalX0c;
    float normalY10;
    float normalZ14;
    float distance18;
    int dominantAxis1c;
} EdgePolygon_guess;

#define EDGE_POLYGON_KIND 0x33
#define EDGE_POLYGON_EDGE_KIND 0x32
#define EDGE_POLYGON_EDGE_BYTES 0x14

typedef struct FUN_1002dce0_object_guess
{
    unsigned int value00;
    unsigned int value04;
    unsigned int value08;
    void *allocation0c;
} FUN_1002dce0_object_guess;

typedef struct RenderListEntry_guess
{
    unsigned char _00[0x50];
} RenderListEntry_guess;

typedef union ParserValue_guess
{
    int i;
    float f;
    char *s;
    void *p;
    FUN_1002dce0_object_guess *list;
    int word[4];
    float real[4];
    void *pointer[4];
    short h[8];
    unsigned char b[16];
} ParserValue_guess;

typedef struct ParserObject_guess
{
    void *world;
    int bitmapFileKind04;
    int errorCount;
    ParserValue_guess tokenValue0c;
    int sourceOffset;
    int missingCallbacks20;
    unsigned char _24[0x08];
    void *palette2c;
    unsigned char _30[0x04];
    void (__cdecl *progress34)(void *, int, int, int);
    void *progressContext38;
    int progressCount3c;
} ParserObject_guess;

typedef struct Lexer_guess
{
    int (__cdecl *nextToken)(struct Lexer_guess *lexer);
    int (__cdecl *command04)(struct Lexer_guess *lexer, char *text);
} Lexer_guess;

typedef struct Region_guess
{
    unsigned int value00;
    unsigned char _04[0x04];
    unsigned int tag08;
} Region_guess;

typedef struct T3DPolygon_guess
{
    unsigned char _00[0x04];
    unsigned int vertexCount;
    int *vertexIndices;
    float *vertexData;
    float normalX;
    float normalY;
    float normalZ;
    float planeConstant;
} T3DPolygon_guess;

typedef struct ActorSave_guess
{
    ActorList_guess *list00;
    FILE *stream;
    long streamPos;
} ActorSave_guess;

typedef struct TransElem_guess
{
    unsigned int words[13];
} TransElem_guess;

typedef struct FUN_1002e2b2_entry_guess
{
    unsigned char _00[8];
    void *allocation08;
    unsigned char _0c[0x14];
} FUN_1002e2b2_entry_guess;

typedef struct DictionaryEntry_guess
{
    unsigned char _00[0x10];
    unsigned int value10;
} DictionaryEntry_guess;

typedef struct Dim129_guess
{
    unsigned char _00[0x1c];
    unsigned int width;
    unsigned int height;
} Dim129_guess;

typedef struct Slot129_guess
{
    Dim129_guess *dims;
} Slot129_guess;

typedef struct Link129_guess
{
    Slot129_guess *slot;
} Link129_guess;

typedef struct Mid129_guess
{
    unsigned char _00[0x18];
    Link129_guess *link;
} Mid129_guess;

typedef struct Root129_guess
{
    unsigned char _00[0x28];
    Mid129_guess *mid;
} Root129_guess;

typedef struct GfxObj129_guess
{
    unsigned int kind00;
    unsigned int value04;
    float value08;
    float value0c;
    unsigned char _10[0x0a];
    unsigned char flag1a;
    unsigned char _1b[0x01];
    Root129_guess *root;
    unsigned int offset20[3];
    float u0;
    float u1;
    float u2;
    float v0;
    float v1;
    float v2;
    float *pairs;
    unsigned int value48;
    unsigned int value4c;
} GfxObj129_guess;

typedef struct T3DObject_guess
{
    unsigned int kind00;
    void *sharedObject04;
    unsigned int value08;
    unsigned int value0c;
    unsigned char _10[4];
    unsigned int value14;
    unsigned int value18;
    unsigned int value1c;
} T3DObject_guess;

typedef struct GraphicsObject_guess
{
    unsigned int kind00;
    void *sharedObject04;
    unsigned int value08;
    unsigned int value0c;
    unsigned int value10;
    unsigned int value14;
    unsigned int value18;
    unsigned int value1c;
    unsigned int value20;
} GraphicsObject_guess;

typedef struct ActorMessage_guess
{
    unsigned int messageType00;
    void *buffer04;
    unsigned int size08;
} ActorMessage_guess;

typedef struct World_guess
{
    unsigned char _00[0x0c];
    void *worldNodes0c;
    unsigned char _10[0x344];
    void *regionInstances354;
} World_guess;

typedef struct RegionEntry_guess
{
    int kind00;
    int value04;
    int value08;
    int value0c;
    int bounds10[5];
    T3DObject_guess *object24;
    void *collision28;
    void *zone2c;
    int value30;
    unsigned char _34[0x04];
} RegionEntry_guess;

typedef struct DMSpriteDefinition_guess
{
    unsigned char _00[0x0e];
    unsigned char flags0e;
} DMSpriteDefinition_guess;

typedef struct DMTrackDefinition_guess
{
    unsigned char _00[0x0e];
    unsigned char flags0e;
} DMTrackDefinition_guess;

typedef struct MaterialDefinition_guess
{
    unsigned char _00[0x14];
    int value14;
    unsigned char _18[0x10];
    int value28;
    unsigned char value2c;
} MaterialDefinition_guess;

typedef struct TrackInstance_guess
{
    unsigned char _00[0x08];
    int tag08;
} TrackInstance_guess;

typedef union ParserRecord_guess
{
    int word[0x14];
    float real[0x14];
    void *pointer[0x14];
    short h[0x28];
} ParserRecord_guess;

typedef float (__cdecl *ParserFloatFunction_guess)(float);

// FUNCTION: EQGFX 0x1002902B
// Reports world-file parsing errors with the source file and line when available.
void __cdecl logParserError_guess(ParserObject_guess *parser, char *format, ...)
{
    va_list arguments;
    char *displayMessage;
    char *formattedMessage;
    int sourceOffset;
    int sourceObject;
    char *sourceName;
    int lineNumber;

    displayMessage = (char *)_alloca(0x100);
    va_start(arguments, format);
    EQ_CDECL(int, 0x1006991D, (char *, unsigned int, char *, va_list))
        (displayMessage, 0x100, format, arguments);
    va_end(arguments);
    if (parser != 0)
    {
        sourceOffset = parser->sourceOffset;
        if (sourceOffset != 0)
        {
            sourceObject = EQ_CDECL(int, 0x10028F2E, (int))(sourceOffset);
            sourceName = EQ_CDECL(char *, 0x1002655D, (int))(sourceObject);
            lineNumber = EQ_CDECL(int, 0x10028F39, (int))(sourceOffset);
            formattedMessage = displayMessage;
            displayMessage = (char *)_alloca(0x100);
            if (lineNumber != 0)
            {
                EQ_CDECL(int, 0x100698CC, (char *, unsigned int, const char *, ...))
                    (displayMessage, 0x100, (const char *)0x100a02e4,
                    formattedMessage, sourceName, lineNumber);
            }
            else
            {
                EQ_CDECL(int, 0x100698CC, (char *, unsigned int, const char *, ...))
                    (displayMessage, 0x100, (const char *)0x100a02fc,
                    formattedMessage, sourceName);
            }
        }
        parser->errorCount++;
    }
    EQ_CDECL(void, 0x10026280, (ParserObject_guess *, char *))(parser, displayMessage);
    return;
}

#define parseStateActions EQ_ARRAY(short, 0x1009E8C4)
#define parseActionTable EQ_ARRAY(short, 0x1009E184)
#define parseDefaultRules EQ_ARRAY(short, 0x1009D9C4)
#define parseExceptionTable EQ_ARRAY(short, 0x1009DBD4)
#define parseRuleLengths EQ_ARRAY(short, 0x1009FE24)
#define parseRuleGotos EQ_ARRAY(short, 0x1009F968)
#define parseGotoTable EQ_ARRAY(short, 0x1009EEC8)
#define parseStackSize EQ_GLOBAL(int, 0x100A02E0)
#define parseColorScale EQ_GLOBAL(float, 0x100762A8)
#define t3d_float_ratio_max_guess EQ_GLOBAL(float, 0x10076294)
#define EQGFX_FLOAT_ONE EQ_GLOBAL(float, 0x10076280)
#define parseInverseLength EQ_GLOBAL(ParserFloatFunction_guess, 0x100FC77C)
#define parseDefaultObject ((void *)0x100f401c)
#define PARSE_STACK_LIMIT 150
#define PARSE_FIRST_REDUCE_STATE 0x301
#define PARSE_REDUCE_STATE_BIAS 0x277
#define PARSE_LAST_SHIFT_STATE 0x108
#define PARSE_ERROR_TOKEN 0x545f4433
#define parseListCount EQ_CDECL(int, 0x1002DD6F, (FUN_1002dce0_object_guess *))
#define parseListData EQ_CDECL(void *, 0x1002DD0F, (FUN_1002dce0_object_guess *))
#define parseListAppend EQ_CDECL(void *, 0x1002DD34, (FUN_1002dce0_object_guess *))
#define parseListElement EQ_CDECL(void *, 0x1002DD17, (FUN_1002dce0_object_guess *, int))
#define parseListInit EQ_CDECL(int, 0x1002DC37, (FUN_1002dce0_object_guess *, int, int))
#define parseListFree EQ_CDECL(void, 0x1002DCE0, (FUN_1002dce0_object_guess *))
#define parseListReset EQ_CDECL(void, 0x1002DCFD, (FUN_1002dce0_object_guess *))
#define parseMemset EQ_CDECL(void *, 0x100682D0, (void *, int, size_t))
#define parseLookup EQ_CDECL(void *, 0x1000E7FE, (void *, char *))
#define parseAddToDictionary EQ_CDECL(int, 0x1000DC08, (void *, void *))
#define parseSetObjectTag EQ_CDECL(int, 0x100174B8, (void *, void *, char *))
#define parseRegionFromNumber EQ_CDECL(void *, 0x10019346, (void *, int))
#define parseSetSkipFrames EQ_CDECL(void, 0x10020666, (void *, int))
#define parseAddNames EQ_CDECL(int, 0x1002DB83, (ParserObject_guess *, FUN_1002dce0_object_guess *, void *))
#define parseAddMipFile EQ_CDECL(int, 0x1002DBDD, (ParserObject_guess *, FUN_1002dce0_object_guess *, char *))
#define parseAddRenderEntry EQ_CDECL(int, 0x1002DAC7, (ParserObject_guess *, FUN_1002dce0_object_guess *, void *))
#define parseAddLodSprite EQ_CDECL(int, 0x1002DB29, (ParserObject_guess *, FUN_1002dce0_object_guess *, char *))
#define parseSetObjectPosition EQ_CDECL(void, 0x1002E67B, (ParserObject_guess *, void *, int))
#define parseLoadRegion EQ_CDECL(int, 0x1002E530, (ParserObject_guess *, char *))
#define parseCallbackValue EQ_CDECL(int, 0x1002DAAF, (void *, char *))
#define t3dDefineLight_parse EQ_CDECL(void *, 0x10013D6D, (void *, char *, int, void *, int, int, int))
#define t3dSetLightColors_parse EQ_CDECL(int, 0x10014080, (void *, void *))
#define s3dSetWorldConstantAmbient_parse EQ_CDECL(void, 0x100202E2, (void *, ParserValue_guess *))
#define _t3dCalculateRadius_parse EQ_CDECL(float, 0x1002E58C, (ParserValue_guess *, ParserValue_guess *, float))
#define t3dDefine2DSprite_parse EQ_CDECL(void *, 0x10007681, (void *, char *, int, int, void *, float, float, float, \
    int, int, int, int, ParserValue_guess *, void *, float))
#define t3dDefineBMInfo_parse EQ_CDECL(void *, 0x1000B550, (void *, char *, char *, int))
#define t3dDefineBMInfoMIPMapped_parse EQ_CDECL(void *, 0x1000B5D6, (void *, int, char *, int, void *, int, int))
#define _t3dScaleSpriteCoordinates_parse EQ_CDECL(void, 0x1002E129, (GfxObj129_guess *, int))
#define t3dPaletteClosestPen_parse EQ_CDECL(unsigned char, 0x10018586, (void *, unsigned char, unsigned char, \
    unsigned char))
#define t3dCreateSimpleSprite_parse EQ_CDECL(void *, 0x1001B824, (void *, int, int, void *))
#define t3dSetActorDefinitionUserData_parse EQ_CDECL(void, 0x1000A99B, (void *, int))
#define t3dDefineBlitSprite_parse EQ_CDECL(void *, 0x1000CDC8, (void *, char *, int, void *))
#define t3dDefineSimpleSprite_parse EQ_CDECL(void *, 0x1001B5BD, (void *, int, char *, int, void *, int, int, int))
#define t3dDefineGeometryResource_parse EQ_CDECL(void *, 0x10007F69, (void *, char *, int, void *, int, void *, int, \
    ParserValue_guess *, void *, void *, float))
#define t3dCompute3DSpritePlane_parse EQ_CDECL(int, 0x1002DFF0, (ParserObject_guess *, void *))
#define t3dCreate2DSprite_parse EQ_CDECL(void *, 0x10007C86, (void *, void *, int))
#define t3dCreateBlitSprite_parse EQ_CDECL(void *, 0x1000CFBF, (void *, void *))
#define t3dCreate3DSprite_parse EQ_CDECL(void *, 0x100084FF, (void *, void *, int))
#define t3dCreateCompositeSprite_parse EQ_CDECL(void *, 0x1000D4E2, (void *, void *, int))
#define t3dCreateHierarchicalSprite_parse EQ_CDECL(void *, 0x10012A60, (void *, void *, int))
#define _t3dTransposeElementGrid_parse EQ_CDECL(void, 0x1002E218, (void *, int, int))
#define t3dDefineCompositeSprite_parse EQ_CDECL(void *, 0x1000D159, (void *, char *, int, int, void *, int, int, int, void *, \
    ParserValue_guess *, void *, float))
#define t3dCoefficientsFromHeadingPitchRoll_parse EQ_CDECL(int, 0x1004F2DD, (float, float, float, float *))
#define DefineTrack_parse EQ_CDECL(void *, 0x1001C7CC, (void *, char *, int, void *))
#define t3dCreateTrack_parse EQ_CDECL(TrackInstance_guess *, 0x1001CC30, (void *, int, int, void *))
#define t3dSetTrackDefaultSpeed_parse EQ_CDECL(void, 0x1001D243, (void *, float))
#define t3dSetTrackDefaultReverse_parse EQ_CDECL(void, 0x1001D1A9, (void *, int))
#define t3dSetTrackDefaultInterpolate_parse EQ_CDECL(void, 0x1001D1DE, (void *, int))
#define _t3dForwardWorldArguments_parse EQ_CDECL(int, 0x1002E417, (ParserObject_guess *, char *, int, void *, ParserValue_guess *, \
    void *, float, int, void *, void *))
#define s3dIsDMSpriteDefinitionValid_parse EQ_CDECL(int, 0x1000EED2, (void *))
#define s3dCreateDMSprite_parse EQ_CDECL(void *, 0x1001034C, (void *, void *, int))
#define t3dInitializeRecord_parse EQ_CDECL(void, 0x10013676, (void *, int, int, void *, int, void *))
#define t3dEulerToQuaternion_parse EQ_CDECL(void, 0x1004F89A, (float, float, float, void *))
#define t3dCreatePointLightEx_parse EQ_CDECL(void *, 0x10014356, (void *, int, ParserValue_guess *, float, int))
#define t3dSetLightStaticFlag_parse EQ_CDECL(int, 0x10014628, (void *))
#define t3dSetPointLightStaticInfluence_parse EQ_CDECL(int, 0x10014641, (void *))
#define t3dAddPointLightToRegion_parse EQ_CDECL(int, 0x10014673, (void *, void *, int))
#define t3dSetLightDynamicSceneInfluence_parse EQ_CDECL(void, 0x1001464C, (void *))
#define t3dSetLightTag_parse EQ_CDECL(int, 0x100142A1, (void *, void *, char *))
#define t3dIsLightDefinitionValid_parse EQ_CDECL(int, 0x100140E1, (void *))
#define t3dCreateLight_parse EQ_CDECL(void *, 0x100140F2, (void *, void *))
#define t3dCreateDirectionalLight_parse EQ_CDECL(void *, 0x10014B20, (void *, int, ParserValue_guess *))
#define t3dAddDirectionalLightToRegion_parse EQ_CDECL(int, 0x10014D38, (void *, void *, int))
#define t3dSetRegionAmbientLight_parse EQ_CDECL(int, 0x100194A2, (void *, void *))
#define createGraphicsObject_parse EQ_CDECL(GraphicsObject_guess *, 0x1002DDB1, (FUN_1002dce0_object_guess *, int, \
    FUN_1002dce0_object_guess *, int, float))
#define t3dInitReferenceList_parse EQ_CDECL(int, 0x10019012, (void *, int))
#define createT3dObject_parse EQ_CDECL(T3DObject_guess *, 0x1002DD77, (int, FUN_1002dce0_object_guess *, int, int))
#define t3dCreateZone_parse EQ_CDECL(void *, 0x1002055A, (void *))
#define t3dAddRegionToZone_parse EQ_CDECL(int, 0x10020674, (void *, void *, void *))
#define _t3dCreateEdgePolygon_parse EQ_CDECL(int, 0x1002DE1A, (ParserObject_guess *, void *, int, FUN_1002dce0_object_guess *, \
    ParserValue_guess *))
#define _t3dDefineSphereList_parse EQ_CDECL(int, 0x1002E44A, (ParserObject_guess *, char *, float, float, int, void *, void *))
#define _t3dCreatePolyhedronFromDefinition_parse EQ_CDECL(int, 0x1002E37C, (ParserObject_guess *, ParserValue_guess *, char *, char *, float))
#define _t3dCreateSphereListFromDefinition_parse EQ_CDECL(int, 0x1002E495, (ParserObject_guess *, ParserValue_guess *, char *, char *, float))
#define _t3dDefinePolyhedron_parse EQ_CDECL(int, 0x1002E32E, (ParserObject_guess *, char *, float, float, int, void *, int, \
    void *))
#define _t3dFreeObjectLists_parse EQ_CDECL(void, 0x1002E2B2, (FUN_1002dce0_object_guess *, FUN_1002dce0_object_guess *))
#define t3dDefineParticleCloud_parse EQ_CDECL(void *, 0x1003FE47, (void *, char *, int, int, int, float, \
    ParserValue_guess *, int, void *, int, int, int, void *, ParserValue_guess *, float, float, int, float, int, \
    float, ParserValue_guess *, int))
#define s3dDefineMaterial_parse EQ_CDECL(MaterialDefinition_guess *, 0x10016CF2, (void *, int, char *))
#define s3dSetMaterialIntensity_parse EQ_CDECL(void, 0x10016EBE, (void *, void *, float))
#define s3dSetMaterialAmbientScalar_parse EQ_CDECL(void, 0x10016EB0, (void *, void *, float))
#define s3dSetMaterialSimpleSprite_parse EQ_CDECL(void, 0x10016E7D, (void *, void *, void *))
#define s3dSetMaterialTwoSided_parse EQ_CDECL(void, 0x10016ECC, (void *, void *, int))
#define s3dSetMaterialUVShift_parse EQ_CDECL(void, 0x10016EDA, (void *, void *, float, float))
#define s3dCreateMaterialPalette_parse EQ_CDECL(void *, 0x10016F39, (void *, int, void *, int))
#define s3dDefineDMSprite_parse EQ_CDECL(DMSpriteDefinition_guess *, 0x1000EEEB, (void *, char *, int, void *, int, \
    void *, int, void *, int, void *, int, void *, int, void *, void *, int, void *, int, int, int, int, int, \
    void *, int, int, ParserValue_guess *, float *, void *, float, void *))
#define s3dComputeDMSpriteDefPlanarEquations_parse EQ_CDECL(void, 0x1000FCC0, (void *, int))
#define t3dCreateSphylinder_parse EQ_CDECL(void *, 0x1001D422, (float, float))
#define t3dGetActorSprite_parse EQ_CDECL(int *, 0x1000A68E, (void *, int, int))
#define t3dCreateActorEx_parse EQ_CDECL(void *, 0x10009A22, (void *, void *, char *, int, void *, void *, float, \
    float, int, int))
#define t3dSetActorDMRGBTrack_parse EQ_CDECL(void, 0x1000B402, (void *, void *))
#define _t3dPostActorCreate_parse EQ_CDECL(int, 0x1002E1C9, (ParserObject_guess *, void *, char *, int, int, int))
#define t3dDestroyPolyhedron_parse EQ_CDECL(void, 0x10018DA3, (void *, void *))
#define GFX_COMPOSITE_DESTROY_VOLUME EQ_CDECL(void, 0x1001D5A1, (void *, void *))
#define t3dDefineActor_parse EQ_CDECL(void *, 0x100093D1, (void *, char *, int, void *, int, int, void *, void *, \
    void *, float, float, int, void *, int))
#define s3dCreateDMTrack_parse EQ_CDECL(void *, 0x10010AD8, (void *, void *, int, float))
#define s3dCreateDMRGBTrack_parse EQ_CDECL(void *, 0x10010E08, (void *, void *, int, float))
#define s3dDefineDMTrack_parse EQ_CDECL(void *, 0x1001075C, (void *, char *, int, int, int, int, void *))
#define s3dDefineDMRGBTrack_parse EQ_CDECL(DMTrackDefinition_guess *, 0x10010C30, (void *, char *, int, int, int, \
    int, void *))
#define _t3dSetDefaultPaletteFile_parse EQ_CDECL(int, 0x1002E2FC, (ParserObject_guess *, char *))
#define t3dSetPCloudFlags_parse EQ_CDECL(void, 0x10040338, (void *, int))

// FUNCTION: EQGFX 0x100290CE
// Builds the world's lights, sprites, materials, tracks, actors, regions and zones from a text world file.
// __cdecl. yacc parser of the text world file: tokens come from lexer->nextToken and every reduced rule builds world
// objects through the t3d/s3d calls. Returns 0 when the input is accepted, 1 on any error.
int __cdecl _t3dParseTokenStream(ParserObject_guess *parser, Lexer_guess *lexer)
{
    ParserValue_guess ruleValue;
    void *world;
    int state;
    int token;
    unsigned short *stateTop;
    ParserValue_guess *valueTop;
    FUN_1002dce0_object_guess regionList;
    int objectIndex;
    int uvPairCount;
    int errorFlag;
    int hasRgba;
    unsigned short *stateStack;
    int lodIndex;
    int rule;
    FUN_1002dce0_object_guess vertexList;
    int frameCount;
    int collisionKind;
    int rgbTrackFlag;
    ParserRecord_guess triangleRecord;
    FUN_1002dce0_object_guess nameList;
    FUN_1002dce0_object_guess frameTransformList;
    FUN_1002dce0_object_guess frameList;
    int syntaxErrors;
    FUN_1002dce0_object_guess renderList;
    ParserValue_guess *tokenValue;
    FUN_1002dce0_object_guess colorList;
    FUN_1002dce0_object_guess rgbTrackFrameList;
    FUN_1002dce0_object_guess dmTrackFrameList;
    FUN_1002dce0_object_guess bspNodeList;
    FUN_1002dce0_object_guess actionList;
    FUN_1002dce0_object_guess radiusList;
    FUN_1002dce0_object_guess mipFileList;
    FUN_1002dce0_object_guess uvList;
    FUN_1002dce0_object_guess dagList;
    FUN_1002dce0_object_guess subDagList;
    ParserRecord_guess polygonRecord;
    ParserRecord_guess regionShapeRecord;
    FUN_1002dce0_object_guess visNodeList;
    void *lastActor;
    ParserValue_guess *valueStack;
    FUN_1002dce0_object_guess worldNodeList;
    FUN_1002dce0_object_guess attachedSkinList;
    FUN_1002dce0_object_guess rangeList;
    FUN_1002dce0_object_guess lodSpriteList;
    FUN_1002dce0_object_guess visListList;
    FUN_1002dce0_object_guess componentList;
    FUN_1002dce0_object_guess vertexChainList;
    FUN_1002dce0_object_guess materialGroupList;
    FUN_1002dce0_object_guess skinGroupList;
    FUN_1002dce0_object_guess proximalRegionList;
    GfxObj129_guess spriteRecord;
    FUN_1002dce0_object_guess materialList;
    FUN_1002dce0_object_guess vertexMaterialGroupList;
    FUN_1002dce0_object_guess wallList;
    FUN_1002dce0_object_guess regionEntryList;
    FUN_1002dce0_object_guess faceEntryList;
    FUN_1002dce0_object_guess vertexNormalList;
    FUN_1002dce0_object_guess face2List;
    float center[3];
    ParserRecord_guess collisionRecord;
    ParserRecord_guess boxRecord;
    ParserRecord_guess boundsRecord;
    FUN_1002dce0_object_guess lodList;
    FUN_1002dce0_object_guess attachedSkinLinkList;
    FUN_1002dce0_object_guess colorIntensityList;
    FUN_1002dce0_object_guess faceList;
    FUN_1002dce0_object_guess meshOpList;
    ParserRecord_guess extentRecord;
    ParserRecord_guess zoneRecord;
    FUN_1002dce0_object_guess entryList1c;
    FUN_1002dce0_object_guess frameSetList;
    FUN_1002dce0_object_guess headingList;
    ParserValue_guess savedRuleValue;
    ParserValue_guess savedTokenValue;
    ParserRecord_guess reservedRecord;
    ParserValue_guess *top;
    short *actions;
    short *entry;
    short *gotoEntry;
    short value;
    int length;
    int result;
    char *message;
    char *detail;
    int status;
    int index;
    void *definition;
    void *object;
    void *light;
    void *zone;
    int *element;
    short *shortElement;
    ParserValue_guess *valueElement;
    RegionEntry_guess *regionEntry;
    T3DObject_guess *regionObject;
    void **names;
    float *coefficients;
    float invLength;
    float radius;
    float scale;
    void *scaleTable;
    void *namePointer;
    void *attachment;
    void *volume;
    int *source;
    int frames;
    int components;
    int red;
    int green;
    int blue;
    int alpha;
    DMSpriteDefinition_guess *spriteDefinition;
    DMTrackDefinition_guess *trackDefinition;
    MaterialDefinition_guess *material;
    TrackInstance_guess *track;
    int vertexCount;
    void *vertices;
    int uvCount;
    void *uvs;
    int normalCount;
    void *normals;
    int colorCount;
    void *colors;
    int faceCount;
    void *faces;
    int meshOpCount;
    void *meshOps;
    int skinGroupCount;
    void *skinGroups;
    int groupCount;
    void *groups;
    int extraCountA;
    int extraCountB;
    float spriteScale;
    void *spriteName;

    ruleValue.word[0] = 0;
    ruleValue.word[1] = 0;
    ruleValue.word[2] = 0;
    ruleValue.word[3] = 0;
    vertexList = *(FUN_1002dce0_object_guess *)&ruleValue;
    uvList = *(FUN_1002dce0_object_guess *)&ruleValue;
    vertexNormalList = *(FUN_1002dce0_object_guess *)&ruleValue;
    colorList = *(FUN_1002dce0_object_guess *)&ruleValue;
    vertexChainList = *(FUN_1002dce0_object_guess *)&ruleValue;
    regionList = *(FUN_1002dce0_object_guess *)&ruleValue;
    rangeList = *(FUN_1002dce0_object_guess *)&ruleValue;
    radiusList = *(FUN_1002dce0_object_guess *)&ruleValue;
    frameList = *(FUN_1002dce0_object_guess *)&ruleValue;
    frameSetList = *(FUN_1002dce0_object_guess *)&ruleValue;
    headingList = *(FUN_1002dce0_object_guess *)&ruleValue;
    world = parser->world;
    componentList = *(FUN_1002dce0_object_guess *)&ruleValue;
    frameTransformList = *(FUN_1002dce0_object_guess *)&ruleValue;
    worldNodeList = *(FUN_1002dce0_object_guess *)&ruleValue;
    bspNodeList = *(FUN_1002dce0_object_guess *)&ruleValue;
    faceList = *(FUN_1002dce0_object_guess *)&ruleValue;
    renderList = *(FUN_1002dce0_object_guess *)&ruleValue;
    lodList = *(FUN_1002dce0_object_guess *)&ruleValue;
    visListList = *(FUN_1002dce0_object_guess *)&ruleValue;
    visNodeList = *(FUN_1002dce0_object_guess *)&ruleValue;
    wallList = *(FUN_1002dce0_object_guess *)&ruleValue;
    entryList1c = *(FUN_1002dce0_object_guess *)&ruleValue;
    regionEntryList = *(FUN_1002dce0_object_guess *)&ruleValue;
    actionList = *(FUN_1002dce0_object_guess *)&ruleValue;
    lodSpriteList = *(FUN_1002dce0_object_guess *)&ruleValue;
    proximalRegionList = *(FUN_1002dce0_object_guess *)&ruleValue;
    dagList = *(FUN_1002dce0_object_guess *)&ruleValue;
    subDagList = *(FUN_1002dce0_object_guess *)&ruleValue;
    nameList = *(FUN_1002dce0_object_guess *)&ruleValue;
    mipFileList = *(FUN_1002dce0_object_guess *)&ruleValue;
    colorIntensityList = *(FUN_1002dce0_object_guess *)&ruleValue;
    materialList = *(FUN_1002dce0_object_guess *)&ruleValue;
    skinGroupList = *(FUN_1002dce0_object_guess *)&ruleValue;
    faceEntryList = *(FUN_1002dce0_object_guess *)&ruleValue;
    face2List = *(FUN_1002dce0_object_guess *)&ruleValue;
    dmTrackFrameList = *(FUN_1002dce0_object_guess *)&ruleValue;
    rgbTrackFrameList = *(FUN_1002dce0_object_guess *)&ruleValue;
    attachedSkinList = *(FUN_1002dce0_object_guess *)&ruleValue;
    attachedSkinLinkList = *(FUN_1002dce0_object_guess *)&ruleValue;
    meshOpList = *(FUN_1002dce0_object_guess *)&ruleValue;
    vertexMaterialGroupList = *(FUN_1002dce0_object_guess *)&ruleValue;
    materialGroupList = *(FUN_1002dce0_object_guess *)&ruleValue;
    frameCount = 0;
    uvPairCount = 0;
    lodIndex = 0;
    objectIndex = 0;
    hasRgba = 0;
    rgbTrackFlag = 0;
    parseMemset(&spriteRecord, 0, 0x50);
    parseMemset(&regionShapeRecord, 0, 0x2c);
    parseMemset(&boundsRecord, 0, 0x1c);
    parseMemset(&reservedRecord, 0, 0xc);
    parseMemset(&collisionRecord, 0, 0x14);
    parseMemset(&collisionKind, 0, 4);
    parseMemset(&zoneRecord, 0, 0x40);
    parseMemset(&polygonRecord, 0, 0x10);
    parseMemset(&triangleRecord, 0, 8);
    parseMemset(&boxRecord, 0, 0x18);
    parser->sourceOffset = (int)lexer;
    stateStack = (unsigned short *)t3dAllocateMemory_call(parseStackSize * 2 + 2);
    valueStack = (ParserValue_guess *)t3dAllocateMemory_call((parseStackSize + 1) * 0x10);
    if (stateStack == 0 || valueStack == 0)
    {
        logParserError_call(parser, (char *)0x100a12f4, (char *)0x100a030c);
        return 1;
    }
    token = -1;
    valueTop = valueStack;
    tokenValue = &parser->tokenValue0c;
    syntaxErrors = 0;
    savedTokenValue = *tokenValue;
    savedRuleValue = ruleValue;
    *stateStack = 0;
    stateTop = stateStack + 1;
    errorFlag = 0;
    state = 0;
    if (stateTop > stateStack + PARSE_STACK_LIMIT)
        goto stackOverflow;

pushState:
    valueTop++;
    *stateTop = (unsigned short)state;
    *valueTop = ruleValue;
newState:
    if ((unsigned short)state >= PARSE_FIRST_REDUCE_STATE)
    {
        rule = state - PARSE_REDUCE_STATE_BIAS;
        goto reduceRule;
    }
    actions = &parseActionTable[parseStateActions[(short)state]];
    if (*actions >= 0)
    {
        if (token < 0)
        {
            token = lexer->nextToken(lexer);
            if (token < 0)
            {
                if (token == -2)
                    goto abortParse;
                token = 0;
            }
        }
        entry = actions;
        value = *entry++;
        while ((short)token < value)
            value = *entry++;
        if ((short)token == entry[-1])
        {
            token = -1;
            state = -1 - actions[actions - entry];
            ruleValue = *tokenValue;
            if (errorFlag != 0)
                errorFlag--;
            goto nextState;
        }
    }
    if ((unsigned short)state >= PARSE_LAST_SHIFT_STATE)
        goto parseError;
    rule = parseDefaultRules[(short)state];
    if ((short)rule < 0)
    {
        entry = parseExceptionTable - ((short)rule + 1);
        if (token < 0)
        {
            token = lexer->nextToken(lexer);
            if (token < 0)
            {
                if (token == -2)
                    goto abortParse;
                token = 0;
            }
        }
        while (*entry >= 0 && *entry != token)
            entry += 2;
        rule = entry[1];
    }
reduceRule:
    length = parseRuleLengths[(short)rule];
    top = valueTop;
    valueTop -= length;
    stateTop -= length;
    ruleValue = valueTop[1];
    switch ((short)rule)
    {
    case 1:
        goto parseError;
    case 0x236:
    case 0x237:
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x234:
        if (_t3dSetDefaultPaletteFile_parse(parser, top[0].s) != 0)
            goto abortParse;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x233:
        t3dSetPCloudFlags_parse(world, top[0].i);
        break;
    case 0x217: case 0x218: case 0x219: case 0x21a: case 0x21b: case 0x21c: case 0x21d:
    case 0x21e: case 0x21f: case 0x220: case 0x221: case 0x222: case 0x223: case 0x224:
    case 0x225: case 0x226: case 0x227: case 0x228: case 0x229: case 0x22a: case 0x22b:
    case 0x22c: case 0x22d: case 0x22e: case 0x22f: case 0x230: case 0x231: case 0x232:
        if (parser->progress34 != 0)
        {
            parser->progressCount3c++;
            parser->progress34(parser->progressContext38, 1, parser->progressCount3c, 0);
        }
        break;
    case 0x216:
        status = ((Lexer_guess *)parser->sourceOffset)->command04((Lexer_guess *)parser->sourceOffset, top[0].s);
        if (status != 0)
            goto abortParse;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x215:
        definition = t3dDefineLight_parse(world, top[-6].s, parseListCount(top[-5].list), parseListData(top[-5].list),
            top[-4].i, top[-3].i, top[-2].i);
        if (definition == 0)
        {
            message = (char *)0x100a1260;
            goto failWithMessage;
        }
        if (top[-1].i > 0)
        {
            if (top[-1].i != parseListCount(top[-5].list))
            {
                message = (char *)0x100a1228;
                goto failWithMessage;
            }
            if (t3dSetLightColors_parse(definition, parseListData(&colorIntensityList)) != 0)
            {
                message = (char *)0x100a1200;
                goto failWithMessage;
            }
        }
        parseListFree(&colorIntensityList);
        parseListFree(top[-5].list);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-6].s);
        break;
    case 0x213:
        if (parseListInit(&radiusList, 4, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&colorIntensityList, 0xc, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 3:
        if (top[-3].i != parseListCount(&radiusList))
        {
            message = (char *)0x100a0dfc;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &radiusList;
        break;
    case 0x212:
        element = (int *)parseListAppend(&radiusList);
        *element = top[0].i;
        ruleValue.word[0] = 1;
        break;
    case 0x99: case 0xc2: case 0xd6: case 0xdd: case 0xe2: case 0x113: case 0x122: case 0x125: case 0x128:
    case 0x12d: case 0x133: case 0x137: case 0x13e: case 0x149: case 0x14e: case 0x162: case 0x169: case 0x17e:
    case 0x17f: case 0x180: case 0x195: case 0x19d: case 0x1a2: case 0x1a5: case 0x1aa: case 0x1ab: case 0x1b1:
    case 0x1b4: case 0x1d2: case 0x1d9: case 0x1f9: case 0x202: case 0x204:
        ruleValue.word[0] = 1;
        break;
    case 0x211:
        element = (int *)parseListAppend(&radiusList);
        *element = top[0].i;
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x20f:
        element = (int *)parseListAppend(&colorIntensityList);
        element[0] = top[0].word[0];
        element[1] = top[0].word[1];
        element[2] = top[0].word[2];
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0xc1: case 0x112: case 0x132: case 0x136: case 0x148: case 0x151: case 0x161: case 0x166: case 0x194:
    case 0x19c: case 0x1a1: case 0x1a4: case 0x1b0: case 0x1b3:
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x20e:
        ruleValue.real[0] = top[-2].real[0];
        ruleValue.word[1] = top[-1].word[0];
        ruleValue.word[2] = top[0].word[0];
        if (ruleValue.real[0] < 0.0f || ruleValue.real[1] < 0.0f || ruleValue.real[2] < 0.0f)
        {
            message = (char *)0x100a11e0;
            goto failWithMessage;
        }
        break;
    case 0x20d:
        s3dSetWorldConstantAmbient_parse(world, top);
        break;
    case 0x20c:
        frameCount = top[0].i;
        break;
    case 0x20b:
        if (parseListInit(&headingList, 0x10, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&nameList, 4, 10) != 0)
            goto outOfMemory;
        break;
    case 0x20a:
        namePointer = parseDefaultObject;
        if (top[-7].i != parseListCount(top[-5].list))
        {
            message = (char *)0x100a11c8;
            goto failWithMessage;
        }
        if (top[-3].p != 0)
            namePointer = top[-3].p;
        if (top[-2].real[0] == 0.0f)
            radius = _t3dCalculateRadius_parse(&top[-15], &top[-9], top[-8].real[0]);
        else
            radius = top[-2].real[0];
        definition = t3dDefine2DSprite_parse(world, top[-16].s, top[-14].i, parseListCount(top[-5].list),
            parseListData(top[-5].list), top[-9].real[0], top[-9].real[1], top[-8].real[0], top[-4].i, top[-12].i,
            top[-11].i, top[-10].i, &top[-15], namePointer, radius);
        if (definition == 0)
        {
            message = (char *)0x100a11a0;
            goto failWithMessage;
        }
        names = (void **)parseListData(&nameList);
        index = 0;
        if (parseListCount(&nameList) > 0)
        {
            do
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(*names);
                names++;
                index++;
            } while (index < parseListCount(&nameList));
        }
        parseListFree(&nameList);
        parseListReset(top[-5].list);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-16].s);
        break;
    case 7:
        ruleValue.word[0] = -1;
        break;
    case 8:
        ruleValue.real[0] = 1.0f;
        break;
    case 0x1f6:
    case 0x1fd:
        ruleValue.word[0] = top[-2].i;
        break;
    case 0x1fb:
        if (parseListInit(&frameSetList, 8, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 9:
        valueElement = (ParserValue_guess *)parseListAppend(&headingList);
        valueElement->real[0] = (float)top[-4].i;
        valueElement->word[1] = top[-3].i;
        valueElement->word[2] = top[-2].i;
        if (valueElement->word[2] != parseListCount(top[0].list))
        {
            message = (char *)0x100a1188;
            goto failWithMessage;
        }
        valueElement->pointer[3] = parseListData(top[0].list);
        parseListReset(top[0].list);
        ruleValue.pointer[0] = &headingList;
        break;
    case 0x1f4:
        if (parseListInit(&frameList, 4, frameCount) != 0)
            goto outOfMemory;
        break;
    case 0xb:
        valueElement = (ParserValue_guess *)parseListAppend(&frameSetList);
        valueElement->real[0] = (float)top[-2].i;
        if (frameCount != parseListCount(&frameList))
        {
            message = (char *)0x100a116c;
            goto failWithMessage;
        }
        valueElement->pointer[1] = parseListData(&frameList);
        parseAddNames(parser, &nameList, parseListData(&frameList));
        parseListReset(&frameList);
        ruleValue.pointer[0] = &frameSetList;
        break;
    case 0x1f2:
        element = (int *)parseListAppend(&frameList);
        *element = top[0].i;
        ruleValue.word[0] = 1;
        break;
    case 0x1f1:
        element = (int *)parseListAppend(&frameList);
        *element = top[0].i;
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x1f0:
        ruleValue.pointer[0] = t3dDefineBMInfo_parse(world, top[0].s, top[-1].s, parser->bitmapFileKind04);
        if (ruleValue.pointer[0] == 0)
        {
            logParserError_call(parser, (char *)0x100a1130, top[0].s, top[-1].s,
                parser->bitmapFileKind04 != 0 ? (char *)0x100a1168 : (char *)0x100a1164);
            goto abortParse;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-1].s);
        break;
    case 0x1ef:
        if (parseListInit(&mipFileList, 4, 10) != 0)
            goto outOfMemory;
        break;
    case 0xc:
        ruleValue.pointer[0] = t3dDefineBMInfoMIPMapped_parse(world, 0, top[-2].s, parseListCount(&mipFileList),
            parseListData(&mipFileList), parser->bitmapFileKind04, 1);
        if (ruleValue.pointer[0] == 0)
        {
            logParserError_call(parser, (char *)0x100a110c, top[-2].s,
                parser->bitmapFileKind04 != 0 ? (char *)0x100a1168 : (char *)0x100a1164);
            goto abortParse;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-2].s);
        names = (void **)parseListData(&mipFileList);
        index = 0;
        if (parseListCount(&mipFileList) > 0)
        {
            do
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(*names);
                names++;
                index++;
            } while (index < parseListCount(&mipFileList));
        }
        parseListFree(&mipFileList);
        break;
    case 0x1ee:
        parseAddMipFile(parser, &mipFileList, top[0].s);
        ruleValue.word[0] = 1;
        break;
    case 0x1ed:
        parseAddMipFile(parser, &mipFileList, top[0].s);
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0xd:
        spriteRecord.kind00 = 0x37;
        ruleValue.pointer[0] = &spriteRecord;
        spriteRecord.value08 = 0.0f;
        spriteRecord.flag1a = 0;
        spriteRecord.value04 = top[0].i;
        spriteRecord.value0c = 0.0f;
        spriteRecord.offset20[0] = 0;
        spriteRecord.root = 0;
        spriteRecord.pairs = 0;
        spriteRecord.offset20[1] = 0;
        spriteRecord.value48 = 0;
        spriteRecord.value4c = 0;
        spriteRecord.offset20[2] = 0;
        spriteRecord.u0 = 0.0f;
        spriteRecord.u1 = 0.0f;
        spriteRecord.u2 = 0.0f;
        spriteRecord.v0 = 0.0f;
        spriteRecord.v1 = 0.0f;
        spriteRecord.v2 = 0.0f;
        break;
    case 0x1ec:
        ruleValue.pointer[0] = top[0].p;
        *(int *)top[0].p = 0x37;
        ((int *)ruleValue.pointer[0])[1] = top[-1].i;
        break;
    case 0x1ea:
        ruleValue.word[0] = top[-1].i + top[0].i - 1;
        break;
    case 0x1e9:
        if (uvPairCount == 0)
            break;
        if (parseListInit(&uvList, 8, uvPairCount) != 0)
            goto outOfMemory;
        break;
    case 0x1e8:
        ruleValue.pointer[0] = &spriteRecord;
        spriteRecord.value08 = top[-9].real[0];
        spriteRecord.value0c = top[-8].real[0];
        spriteRecord.flag1a = top[-10].b[0];
        spriteRecord.root = (Root129_guess *)top[-7].pointer[0];
        spriteRecord.offset20[0] = top[-6].word[0];
        spriteRecord.offset20[1] = top[-6].word[1];
        spriteRecord.offset20[2] = top[-6].word[2];
        spriteRecord.u0 = top[-5].real[0];
        spriteRecord.u1 = top[-5].real[1];
        spriteRecord.u2 = top[-5].real[2];
        spriteRecord.v0 = top[-4].real[0];
        spriteRecord.v1 = top[-4].real[1];
        spriteRecord.v2 = top[-4].real[2];
        if (top[-2].i == 0)
        {
            spriteRecord.pairs = 0;
            if (top[-7].pointer[0] != parseDefaultObject)
                _t3dScaleSpriteCoordinates_parse(&spriteRecord, 0);
            if (uvPairCount != 0)
                parseListFree(&uvList);
        }
        else
        {
            if (top[-2].i != uvPairCount)
            {
                message = (char *)0x100a10f0;
                goto failWithMessage;
            }
            ((GfxObj129_guess *)ruleValue.pointer[0])->pairs = (float *)parseListData(&uvList);
            _t3dScaleSpriteCoordinates_parse((GfxObj129_guess *)ruleValue.pointer[0], parseListCount(&uvList));
            parseListReset(&uvList);
        }
        ((GfxObj129_guess *)ruleValue.pointer[0])->value48 = 0;
        uvPairCount = 0;
        ((GfxObj129_guess *)ruleValue.pointer[0])->value4c = top[-1].i;
        break;
    case 0x1e6:
        if (parser->palette2c == 0)
        {
            message = (char *)0x100a10cc;
            goto failWithMessage;
        }
        ruleValue.word[0] = t3dPaletteClosestPen_parse(parser->palette2c, top[-2].b[0], top[-1].b[0], top[0].b[0]);
        break;
    case 0x1e2:
        definition = parseLookup(world, top[-2].s);
        if (definition == 0)
        {
            message = (char *)0x100a10a8;
            detail = top[-2].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = t3dCreateSimpleSprite_parse(world, 0, 0, definition);
        if (ruleValue.pointer[0] != 0 && top[-1].i != -1)
            t3dSetActorDefinitionUserData_parse(ruleValue.pointer[0], top[-1].i);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-2].s);
        break;
    case 0x1dd:
    case 0x1de:
        ruleValue.real[0] = top[-2].real[0] / top[-3].real[0];
        ruleValue.real[1] = top[-1].real[0] / top[-3].real[0];
        ruleValue.real[2] = top[0].real[0] / top[-3].real[0];
        break;
    case 0x1db:
        element = (int *)parseListAppend(&uvList);
        element[0] = top[0].word[0];
        element[1] = top[0].word[1];
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0xc7:
    case 0x1da:
    case 0x201:
        ruleValue.word[0] = top[-1].i;
        ruleValue.word[1] = top[0].i;
        break;
    case 0x1d8:
        definition = parseLookup(world, top[-1].s);
        if (definition == 0)
        {
            message = (char *)0x100a0ff0;
            detail = top[-1].s;
            goto failWithDetail;
        }
        object = t3dCreateSimpleSprite_parse(world, 0, 0, definition);
        if (object == 0)
        {
            message = (char *)0x100a0fd0;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-1].s);
        if (t3dDefineBlitSprite_parse(world, top[-4].s, top[-3].i, object) == 0)
        {
            message = (char *)0x100a1080;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-4].s);
        break;
    case 0x1d7:
        if (parseListInit(&frameList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x1d6:
        if (top[-7].i != parseListCount(&frameList))
        {
            message = (char *)0x100a0dfc;
            goto failWithMessage;
        }
        if (t3dDefineSimpleSprite_parse(world, 0, top[-8].s, parseListCount(&frameList), parseListData(&frameList),
            top[-4].i, top[-3].i, top[-2].i) == 0)
        {
            message = (char *)0x100a1054;
            goto failWithMessage;
        }
        parseListFree(&frameList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-8].s);
        break;
    case 0x1d4:
        parseListData(&bspNodeList);
        parseListData(&renderList);
        scale = 1.0f;
        scaleTable = parseDefaultObject;
        if (top[-3].pointer[0] != 0)
            scaleTable = top[-3].pointer[0];
        if (top[-2].real[0] != 0.0f)
            scale = top[-2].real[0];
        if (t3dDefineGeometryResource_parse(world, top[-8].s, parseListCount(top[-5].list), parseListData(top[-5].list),
            parseListCount(&bspNodeList), parseListData(&bspNodeList), top[-7].i, &top[-6],
            parseListData(&renderList), scaleTable, scale) == 0)
        {
            message = (char *)0x100a102c;
            goto failWithMessage;
        }
        parseListFree(top[-5].list);
        parseListReset(&bspNodeList);
        parseListReset(&renderList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-8].s);
        break;
    case 0x170:
    case 0x176:
    case 0x1d1:
        if (parseListInit(&vertexList, 0xc, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x18:
        if (top[-2].i != parseListCount(&vertexList))
        {
            message = (char *)0x100a084c;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &vertexList;
        break;
    case 0x1cf:
        element = (int *)parseListAppend(&vertexList);
        element[0] = top[0].word[0];
        element[1] = top[0].word[1];
        element[2] = top[0].word[2];
        ruleValue.word[0] = 1;
        break;
    case 0x1ce:
        element = (int *)parseListAppend(&vertexList);
        element[0] = top[0].word[0];
        element[1] = top[0].word[1];
        element[2] = top[0].word[2];
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x1cc:
        if (parseListInit(&bspNodeList, 0x2c, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&renderList, 0x50, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x1b:
        if (top[-2].i != parseListCount(&bspNodeList))
        {
            message = (char *)0x100a1014;
            goto failWithMessage;
        }
        break;
    case 0x1ca:
        ((int *)top[0].pointer[0])[8] = 0;
        ruleValue.word[0] = 1;
        break;
    case 0x1c9:
        ((int *)top[0].pointer[0])[8] = top[-1].i;
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x1c7:
        if (parseAddRenderEntry(parser, &renderList, top[-4].pointer[0]) != 0)
            goto abortParse;
        element = (int *)parseListAppend(&bspNodeList);
        ruleValue.pointer[0] = element;
        element[0] = 0x34;
        ((int *)ruleValue.pointer[0])[1] = parseListCount(top[-6].list);
        ((int *)ruleValue.pointer[0])[2] = (int)parseListData(top[-6].list);
        ((int *)ruleValue.pointer[0])[3] = (int)parseListData(&vertexList);
        if (top[-3].i != 0)
            ((int *)ruleValue.pointer[0])[9] = (int)parseListElement(&bspNodeList, top[-3].i - 1);
        else
            ((int *)ruleValue.pointer[0])[9] = 0;
        if (top[-2].i != 0)
            ((int *)ruleValue.pointer[0])[10] = (int)parseListElement(&bspNodeList, top[-2].i - 1);
        else
            ((int *)ruleValue.pointer[0])[10] = 0;
        if (_t3dIsNonzeroVector4_call((float *)&top[-7]) != 0)
        {
            element = (int *)ruleValue.pointer[0];
            element[4] = top[-7].word[0];
            element[5] = top[-7].word[1];
            element[6] = top[-7].word[2];
            element[7] = top[-7].word[3];
        }
        else if (t3dCompute3DSpritePlane_parse(parser, ruleValue.pointer[0]) != 0)
            goto abortParse;
        parseListReset(top[-6].list);
        break;
    case 0x1c6:
        if (parseListInit(&regionList, 4, top[-1].i) != 0)
            goto outOfMemory;
        break;
    case 0x1c:
        if (top[-3].i != parseListCount(&regionList))
        {
            message = (char *)0x100a084c;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &regionList;
        break;
    case 0x29:
        ruleValue.pointer[0] = &regionList;
        break;
    case 0x187:
    case 0x1c4:
        element = (int *)parseListAppend(&regionList);
        *element = top[0].i - 1;
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x1c1:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a0ff0;
            detail = top[0].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = t3dCreateSimpleSprite_parse(world, 0, 0, definition);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a0fd0;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x1c0:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a0fb0;
            detail = top[0].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = t3dCreate2DSprite_parse(world, definition, 0);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a0f94;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x1bf:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a0658;
            detail = top[0].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = t3dCreateBlitSprite_parse(world, definition);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a0f74;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x1be:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a0f54;
            detail = top[0].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = t3dCreate3DSprite_parse(world, definition, 0);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a0f38;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x1bc:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a0f10;
            detail = top[0].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = t3dCreateCompositeSprite_parse(world, definition, 0);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a0eec;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x1bb:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a0ec0;
            detail = top[0].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = t3dCreateHierarchicalSprite_parse(world, definition, 0);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a0e98;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x11:
    case 0x1ba:
        ruleValue.pointer[0] = parseDefaultObject;
        break;
    case 0x1b8:
        if (parseListInit(&componentList, 0xa0, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&frameTransformList, 0x34, top[-1].i * top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x1b7:
        scale = 1.0f;
        scaleTable = parseDefaultObject;
        if (top[-10].i != parseListCount(&componentList))
        {
            message = (char *)0x100a0e7c;
            goto failWithMessage;
        }
        frames = top[-11].i;
        components = top[-10].i;
        if (frames * components != parseListCount(&frameTransformList))
        {
            message = (char *)0x100a0e58;
            goto failWithMessage;
        }
        if (top[-3].pointer[0] != 0)
            scaleTable = top[-3].pointer[0];
        if (top[-2].real[0] != 0.0f)
            scale = top[-2].real[0];
        _t3dTransposeElementGrid_parse(parseListData(&frameTransformList), frames, components);
        if (t3dDefineCompositeSprite_parse(world, top[-12].s, top[-11].i, parseListCount(&componentList),
            parseListData(&frameTransformList), top[-7].i, top[-6].i, top[-5].i, parseListData(&componentList),
            &top[-4], scaleTable, scale) == 0)
        {
            message = (char *)0x100a0e34;
            goto failWithMessage;
        }
        parseListReset(&componentList);
        parseListFree(&frameTransformList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-12].s);
        break;
    case 0x1b2:
        element = (int *)parseListAppend(&componentList);
        *element = top[-3].i;
        break;
    case 0x1af:
        coefficients = (float *)parseListAppend(&frameTransformList);
        if (t3dCoefficientsFromHeadingPitchRoll_parse((float)top[-5].i, (float)top[-4].i, (float)top[-3].i,
            coefficients) < 0)
        {
            message = (char *)0x100a0e14;
            goto failWithMessage;
        }
        coefficients[0] = coefficients[0] * top[-6].real[0];
        coefficients[1] = top[-6].real[0] * coefficients[1];
        coefficients[2] = top[-6].real[0] * coefficients[2];
        coefficients[3] = top[-6].real[0] * coefficients[3];
        coefficients[4] = top[-6].real[0] * coefficients[4];
        coefficients[5] = top[-6].real[0] * coefficients[5];
        coefficients[6] = top[-6].real[0] * coefficients[6];
        coefficients[7] = top[-6].real[0] * coefficients[7];
        coefficients[8] = top[-6].real[0] * coefficients[8];
        ((int *)coefficients)[9] = top[-2].i;
        ((int *)coefficients)[10] = top[-1].i;
        ((int *)coefficients)[11] = top[0].i;
        coefficients[12] = EQGFX_FLOAT_ONE / (top[-6].real[0] * top[-6].real[0]);
        break;
    case 0x1ae:
        if (parseListInit(&frameTransformList, 0x24, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x1ad:
        if (top[-3].i != parseListCount(&frameTransformList))
        {
            message = (char *)0x100a0dfc;
            goto failWithMessage;
        }
        if (DefineTrack_parse(world, top[-4].s, parseListCount(&frameTransformList),
            parseListData(&frameTransformList)) == 0)
        {
            message = (char *)0x100a0de0;
            detail = top[-4].s;
            goto failWithDetail;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-4].s);
        parseListReset(&frameTransformList);
        break;
    case 0x1ac:
        definition = parseLookup(world, top[-5].s);
        if (definition == 0)
        {
            message = (char *)0x100a03a8;
            detail = top[-5].s;
            goto failWithDetail;
        }
        track = t3dCreateTrack_parse(world, 0, 0, definition);
        if (track == 0)
        {
            message = (char *)0x100a0dc4;
            detail = top[-6].s;
            goto failWithDetail;
        }
        track->tag08 = top[-6].i;
        if (parseAddToDictionary(world, track) < 0)
        {
            message = (char *)0x100a0d8c;
            goto failWithMessage;
        }
        t3dSetTrackDefaultSpeed_parse(track, 1.0f);
        t3dSetTrackDefaultReverse_parse(track, top[-3].i);
        t3dSetTrackDefaultInterpolate_parse(track, top[-2].i);
        parseSetSkipFrames(track, top[-1].i);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-5].s);
        break;
    case 0x1a9:
        if (parseListInit(&dagList, 0x148, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&nameList, 4, 10) != 0)
            goto outOfMemory;
        break;
    case 0x1a8:
        if (top[0].i == 0)
            break;
        if (parseListInit(&attachedSkinList, 4, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&attachedSkinLinkList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x1a7:
        scale = 1.0f;
        namePointer = parseDefaultObject;
        if (top[-9].i != parseListCount(&dagList))
        {
            message = (char *)0x100a0d78;
            goto failWithMessage;
        }
        index = 0;
        if (top[-2].pointer[0] != 0)
            namePointer = top[-2].pointer[0];
        if (top[-1].real[0] != 0.0f)
            scale = top[-1].real[0];
        if (top[-6].i != 0 && top[-6].i != parseListCount(&attachedSkinList))
        {
            message = (char *)0x100a0d48;
            goto failWithMessage;
        }
        if (_t3dForwardWorldArguments_parse(parser, top[-11].s, parseListCount(&dagList), parseListData(&dagList), &top[-3],
            namePointer, scale, top[-6].i, parseListData(&attachedSkinList),
            parseListData(&attachedSkinLinkList)) != 0)
        {
            message = (char *)0x100a0d20;
            goto failWithMessage;
        }
        names = (void **)parseListData(&nameList);
        if (parseListCount(&nameList) > 0)
        {
            do
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(*names);
                names++;
                index++;
            } while (index < parseListCount(&nameList));
        }
        parseListFree(&nameList);
        parseListFree(&attachedSkinList);
        parseListFree(&attachedSkinLinkList);
        parseListReset(&dagList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-11].s);
        break;
    case 0x22:
        ruleValue.pointer[0] = &attachedSkinList;
        break;
    case 0x1a3:
        definition = parseLookup(world, top[-2].s);
        if (definition == 0)
        {
            message = (char *)0x100a0d04;
            detail = top[-2].s;
            goto failWithDetail;
        }
        if (s3dIsDMSpriteDefinitionValid_parse(definition) != 0)
        {
            message = (char *)0x100a0ce4;
            detail = top[-2].s;
            goto failWithDetail;
        }
        object = s3dCreateDMSprite_parse(world, definition, 0);
        if (object == 0)
        {
            message = (char *)0x100a0cc0;
            detail = top[-2].s;
            goto failWithDetail;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-2].s);
        *(void **)parseListAppend(&attachedSkinList) = object;
        *(int *)parseListAppend(&attachedSkinLinkList) = top[0].i - 1;
        ruleValue.pointer[0] = object;
        break;
    case 0x1a0:
        parseListReset(&subDagList);
        if (top[0].i <= 0)
            break;
        if (parseListInit(&subDagList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x19f:
        object = parseListAppend(&dagList);
        definition = parseLookup(world, top[-5].s);
        index = top[-3].i;
        if (index != parseListCount(&subDagList))
        {
            message = (char *)0x100a0ca8;
            goto failWithMessage;
        }
        if (index != 0)
        {
            names = (void **)parseListData(&subDagList);
            parseAddNames(parser, &nameList, parseListData(&subDagList));
        }
        else
            names = 0;
        if (definition == 0)
        {
            message = (char *)0x100a0c88;
            detail = top[-5].s;
            goto failWithDetail;
        }
        t3dInitializeRecord_parse(object, top[-9].i, top[-8].i, definition, top[-3].i, names);
        parseListReset(&subDagList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-5].s);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-9].s);
        break;
    case 0x19b:
        element = (int *)parseListAppend(&frameTransformList);
        t3dEulerToQuaternion_parse((float)top[-4].i, (float)top[-3].i, (float)top[-5].i, element);
        element[4] = top[-2].i;
        element[5] = top[-1].i;
        element[6] = top[0].i;
        element[7] = top[-6].i;
        break;
    case 0x19a:
        element = (int *)parseListAppend(&subDagList);
        *element = (int)parseListElement(&dagList, top[0].i - 1);
        ruleValue.word[0] = 1;
        break;
    case 0x199:
        element = (int *)parseListAppend(&subDagList);
        *element = (int)parseListElement(&dagList, top[0].i - 1);
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x198:
        if (parseListInit(&worldNodeList, 0x20, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x197:
        if (top[-3].i != parseListCount(&worldNodeList))
        {
            message = (char *)0x100a0c6c;
            goto failWithMessage;
        }
        ((World_guess *)world)->worldNodes0c = parseListData(&worldNodeList);
        parseListReset(&worldNodeList);
        break;
    case 0x193:
        element = (int *)parseListAppend(&worldNodeList);
        element[0] = top[-5].word[0];
        element[1] = top[-5].word[1];
        element[2] = top[-5].word[2];
        element[3] = top[-5].word[3];
        element[6] = top[-4].i;
        if (top[-3].i != 0)
            element[4] = (int)parseListElement(&worldNodeList, top[-3].i - 1);
        else
            element[4] = 0;
        if (top[-2].i != 0)
            element[5] = (int)parseListElement(&worldNodeList, top[-2].i - 1);
        else
            element[5] = 0;
        break;
    case 0x192:
        ruleValue.word[0] = top[-3].i;
        ruleValue.word[1] = top[-2].i;
        ruleValue.word[2] = top[-1].i;
        ruleValue.word[3] = top[0].i;
        break;
    case 0x26:
        ruleValue = EQ_GLOBAL(ParserValue_guess, 0x100ABF80);
        break;
    case 0x191:
        ruleValue = top[0];
        break;
    case 0x18f:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
            goto noLightDefinition;
        goto createLight;
    case 0x18e:
        light = t3dCreatePointLightEx_parse(world, top[-6].i, &top[-7], top[-5].real[0], top[-2].i == 0);
        if (light == 0)
        {
            message = (char *)0x100a0c4c;
            goto failWithMessage;
        }
        if (top[-4].i == 1 && t3dSetLightStaticFlag_parse(light) != 0)
        {
            message = (char *)0x100a0c24;
            goto failWithMessage;
        }
        if (top[-3].i == 1 && t3dSetPointLightStaticInfluence_parse(light) != 0)
        {
            message = (char *)0x100a0bf4;
            goto failWithMessage;
        }
        if (top[-2].list != 0)
        {
            index = 0;
            if (parseListCount(top[-2].list) > 0)
            {
                do
                {
                    if (t3dAddPointLightToRegion_parse(world, light,
                        *(int *)parseListElement(top[-2].list, index)) != 0)
                    {
                        message = (char *)0x100a0bd0;
                        goto failWithMessage;
                    }
                    index++;
                } while (index < parseListCount(top[-2].list));
            }
            parseListFree(top[-2].list);
        }
        if (top[-1].i != 0)
            t3dSetLightDynamicSceneInfluence_parse(light);
        if (top[-8].s == 0)
            break;
        if (t3dSetLightTag_parse(world, light, top[-8].s) != 0)
        {
            message = (char *)0x100a0bb0;
            goto failWithMessage;
        }
        if (parseAddToDictionary(world, light) < 0)
        {
            message = (char *)0x100a0b88;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-8].s);
        break;
    case 0x18d:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
            goto noLightDefinition;
        if (t3dIsLightDefinitionValid_parse(definition) != 0)
            goto noLightDefinition;
createLight:
        ruleValue.pointer[0] = t3dCreateLight_parse(world, definition);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a0b70;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
noLightDefinition:
        message = (char *)0x100a0b54;
        detail = top[0].s;
        goto failWithDetail;
    case 0x18a:
        if (top[-3].i != parseListCount(&regionList))
        {
            message = (char *)0x100a06c0;
            goto failWithMessage;
        }
        light = t3dCreateDirectionalLight_parse(world, top[-5].i, &top[-6]);
        if (light == 0)
        {
            message = (char *)0x100a0b30;
            goto failWithMessage;
        }
        if (top[-7].s != 0)
        {
            if (parseSetObjectTag(world, light, top[-7].s) != 0)
            {
                message = (char *)0x100a0b0c;
                goto failWithMessage;
            }
            if (parseAddToDictionary(world, light) < 0)
            {
                message = (char *)0x100a0adc;
                goto failWithMessage;
            }
            EQGFX_T3D_FREE_MEMORY_GUESS(top[-7].s);
        }
        if (top[-4].i == 1 && t3dSetLightStaticFlag_parse(light) != 0)
        {
            message = (char *)0x100a0ab0;
            goto failWithMessage;
        }
        if (top[-3].i == 0)
            break;
        index = 0;
        if (parseListCount(&regionList) > 0)
        {
            do
            {
                if (t3dAddDirectionalLightToRegion_parse(world, light,
                    *(int *)parseListElement(&regionList, index)) != 0)
                {
                    message = (char *)0x100a0a84;
                    goto failWithMessage;
                }
                index++;
            } while (index < parseListCount(&regionList));
        }
        parseListFree(&regionList);
        break;
    case 0x189:
        invLength = parseInverseLength(top[-1].real[0] * top[-1].real[0] + top[-2].real[0] * top[-2].real[0] +
            top[0].real[0] * top[0].real[0]);
        ruleValue.real[0] = invLength * top[-2].real[0];
        ruleValue.real[1] = invLength * top[-1].real[0];
        ruleValue.real[2] = invLength * top[0].real[0];
        break;
    case 0x188:
    case 0x1c5:
        element = (int *)parseListAppend(&regionList);
        *element = top[0].i - 1;
        ruleValue.word[0] = 1;
        break;
    case 0x185:
        light = top[-4].pointer[0];
        if (top[-3].i != parseListCount(&regionList))
        {
            message = (char *)0x100a06c0;
            goto failWithMessage;
        }
        if (top[-5].s != 0)
        {
            if (parseSetObjectTag(world, light, top[-5].s) != 0)
            {
                message = (char *)0x100a0a64;
                goto failWithMessage;
            }
            if (parseAddToDictionary(world, light) < 0)
            {
                message = (char *)0x100a0a38;
                goto failWithMessage;
            }
            EQGFX_T3D_FREE_MEMORY_GUESS(top[-5].s);
        }
        if (top[-3].i == 0)
            break;
        index = 0;
        if (parseListCount(&regionList) > 0)
        {
            do
            {
                object = parseRegionFromNumber(world, *(int *)parseListElement(&regionList, index));
                if (t3dSetRegionAmbientLight_parse(object, light) != 0)
                {
                    message = (char *)0x100a0a10;
                    goto failWithMessage;
                }
                index++;
            } while (index < parseListCount(&regionList));
        }
        parseListFree(&regionList);
        break;
    case 0x184:
        if (parseListInit(&regionEntryList, 0x38, top[0].i) != 0)
            goto outOfMemory;
        ((World_guess *)parser->world)->regionInstances354 = t3dAllocateMemory_call(top[0].i << 2);
        if (((World_guess *)parser->world)->regionInstances354 == 0)
            goto outOfMemory;
        objectIndex = 1;
        break;
    case 0x2a:
        if (top[-2].i != parseListCount(&regionEntryList))
        {
            message = (char *)0x100a06c0;
            goto failWithMessage;
        }
        parseListReset(&regionEntryList);
        break;
    case 0x181:
        if (top[-2].i != parseListCount(&regionList))
        {
            message = (char *)0x100a06c0;
            goto failWithMessage;
        }
        ruleValue.word[0] = top[0].i;
        break;
    case 0x1a: case 0x25: case 0x3c: case 0x8a: case 0x8b: case 0x8d: case 0x8e: case 0x8f: case 0x90:
    case 0xc4: case 0xe5: case 0xe6: case 0xe9: case 0x114: case 0x116: case 0x119: case 0x11d: case 0x11e:
    case 0x11f: case 0x123: case 0x126: case 0x127: case 0x12b: case 0x12c: case 0x134: case 0x138: case 0x139:
    case 0x13d: case 0x14a: case 0x14b: case 0x153: case 0x155: case 0x156: case 0x15e: case 0x163: case 0x168:
    case 0x174: case 0x175: case 0x177: case 0x183: case 0x18c: case 0x190: case 0x196: case 0x19e: case 0x1a6:
    case 0x1b5: case 0x1b6: case 0x1c2: case 0x1c3: case 0x1cb: case 0x1d0: case 0x1d3: case 0x1d5: case 0x1e0:
    case 0x1e1: case 0x1e3: case 0x1e4: case 0x1e5: case 0x1e7: case 0x1eb: case 0x1f3: case 0x1f5: case 0x1f7:
    case 0x1f8: case 0x1fa: case 0x1fc: case 0x1fe: case 0x1ff: case 0x200: case 0x206: case 0x207: case 0x209:
    case 0x214:
        ruleValue.word[0] = top[0].i;
        break;
    case 0x17d:
        ruleValue.word[0] = 1;
        if (parseLoadRegion(parser, top[0].s) != 0)
        {
            result = 1;
            goto finishParse;
        }
        break;
    case 0x17c:
        ruleValue.word[0] = top[-1].i + 1;
        if (parseLoadRegion(parser, top[0].s) != 0)
            goto abortParse;
        break;
    case 0x179:
    case 0x17a:
    case 0x17b:
        objectIndex++;
        ruleValue.word[0] = top[0].i;
        break;
    case 0x178:
        regionEntry = (RegionEntry_guess *)parseListAppend(&regionEntryList);
        ruleValue.pointer[0] = regionEntry;
        regionEntry->kind00 = 0x19;
        ((RegionEntry_guess *)ruleValue.pointer[0])->value04 = 0;
        ((RegionEntry_guess *)ruleValue.pointer[0])->value08 = top[-9].i;
        if (((RegionEntry_guess *)ruleValue.pointer[0])->value08 == 0)
            goto abortParse;
        ((RegionEntry_guess *)ruleValue.pointer[0])->value0c = 0;
        ((RegionEntry_guess *)ruleValue.pointer[0])->object24 = (T3DObject_guess *)createGraphicsObject_parse(&actionList,
            top[-5].i, &renderList, top[-8].i, top[-4].real[0]);
        source = (int *)top[-3].pointer[0];
        if (source != 0)
        {
            regionEntry = (RegionEntry_guess *)ruleValue.pointer[0];
            for (index = 0; index < 5; index++)
                regionEntry->bounds10[index] = source[index];
        }
        ((RegionEntry_guess *)ruleValue.pointer[0])->collision28 = parseDefaultObject;
        ((RegionEntry_guess *)ruleValue.pointer[0])->zone2c = parseDefaultObject;
        ((RegionEntry_guess *)ruleValue.pointer[0])->value30 = top[-2].i;
        parseListReset(top[-7].list);
        parseListReset(&renderList);
        parseListFree(&lodSpriteList);
        parseListReset(&actionList);
        break;
    case 0x2f:
        if (top[-2].i != parseListCount(&vertexList))
        {
            message = (char *)0x100a09f4;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &vertexList;
        break;
    case 0x173:
        ruleValue.pointer[0] = top[0].pointer[0];
        zoneRecord.word[0] = 0x27;
        goto copyZoneRecord;
    case 0x172:
        ruleValue.pointer[0] = top[0].pointer[0];
        zoneRecord.word[0] = 0x29;
copyZoneRecord:
        ((RegionEntry_guess *)ruleValue.pointer[0])->zone2c = t3dAllocateMemory_call(0x40);
        element = (int *)((RegionEntry_guess *)ruleValue.pointer[0])->zone2c;
        for (index = 0; index < 0x10; index++)
            element[index] = zoneRecord.word[index];
        break;
    case 0x171:
        regionEntry = (RegionEntry_guess *)parseListAppend(&regionEntryList);
        ruleValue.pointer[0] = regionEntry;
        regionEntry->kind00 = 0x19;
        parseSetObjectPosition(parser, ruleValue.pointer[0], objectIndex);
        parseMemset(&collisionKind, 0, 4);
        parseMemset(&zoneRecord, 0, 0x40);
        break;
    case 0x16f:
        if (top[-3].i != parseListCount(&vertexList))
        {
            message = (char *)0x100a09f4;
            goto failWithMessage;
        }
        parseListReset(&vertexList);
        break;
    case 0x16e:
        ((RegionEntry_guess *)top[-8].pointer[0])->object24 = createT3dObject_parse(top[0].i, &wallList, top[-3].i,
            top[-7].i);
        break;
    case 0x16d:
        regionObject = ((RegionEntry_guess *)top[-13].pointer[0])->object24;
        if (top[-1].i != 0)
            regionObject->value18 = 1;
        regionObject->value14 = top[0].i;
        regionObject->value08 = top[-2].word[0];
        regionObject->value0c = top[-2].word[1];
        break;
    case 0x16c:
        ruleValue.pointer[0] = top[-22].pointer[0];
        ((RegionEntry_guess *)ruleValue.pointer[0])->value04 = 0;
        ((RegionEntry_guess *)ruleValue.pointer[0])->value08 = top[-23].i;
        if (((RegionEntry_guess *)ruleValue.pointer[0])->value08 == 0)
            goto abortParse;
        ((RegionEntry_guess *)ruleValue.pointer[0])->value0c = 0;
        source = (int *)top[-7].pointer[0];
        if (source != 0)
        {
            regionEntry = (RegionEntry_guess *)ruleValue.pointer[0];
            for (index = 0; index < 5; index++)
                regionEntry->bounds10[index] = source[index];
        }
        ((RegionEntry_guess *)ruleValue.pointer[0])->zone2c = 0;
        ((RegionEntry_guess *)ruleValue.pointer[0])->value30 = top[-3].i;
        parseSetObjectPosition(parser, 0, 0);
        collisionKind = 0x26;
        ((RegionEntry_guess *)ruleValue.pointer[0])->collision28 = t3dAllocateMemory_call(4);
        if (((RegionEntry_guess *)ruleValue.pointer[0])->collision28 == 0)
        {
            message = (char *)0x100a09d0;
            goto failWithMessage;
        }
        *(int *)((RegionEntry_guess *)ruleValue.pointer[0])->collision28 = collisionKind;
        zoneRecord.pointer[3] = t3dAllocateMemory_call(0x40);
        zoneRecord.word[1] = 0;
        zoneRecord.word[2] = 0x10;
        zoneRecord.word[4] = 0;
        zoneRecord.word[5] = 3;
        zoneRecord.pointer[6] = t3dAllocateMemory_call(0xc);
        if (zoneRecord.pointer[6] == 0)
        {
            message = (char *)0x100a0994;
            goto failWithMessage;
        }
        zoneRecord.word[7] = 0;
        zoneRecord.word[8] = 3;
        zoneRecord.word[9] = 0;
        if (t3dInitReferenceList_parse(&zoneRecord.word[10], 3) != 0)
        {
            message = (char *)0x100a0958;
            goto failWithMessage;
        }
        zoneRecord.word[14] = top[-5].i;
        zoneRecord.word[15] = top[-4].i;
        if (top[-2].i != 0)
            ((RegionEntry_guess *)top[-22].pointer[0])->object24->value1c = top[-2].i;
        if (top[-14].i != 0)
            parseListReset(&wallList);
        if (top[-20].list == 0)
            break;
        parseListReset(top[-20].list);
        break;
    case 0x16b:
    case 0x1bd:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a0938;
            detail = top[0].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = s3dCreateDMSprite_parse(world, definition, 0);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a091c;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x16a:
        parseListReset(&proximalRegionList);
        if (top[0].i <= 0)
            break;
        if (parseListInit(&proximalRegionList, 0xc, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x33:
        if (top[-2].i != parseListCount(&proximalRegionList))
        {
            message = (char *)0x100a08fc;
            goto failWithMessage;
        }
        if (top[0].i <= 0)
        {
            ruleValue.word[0] = 0;
            break;
        }
        ruleValue.pointer[0] = &proximalRegionList;
        break;
    case 0x165:
        element = (int *)parseListAppend(&proximalRegionList);
        ruleValue.pointer[0] = element;
        element[0] = 1;
        ((int *)ruleValue.pointer[0])[1] = top[-1].i - 1;
        ((float *)ruleValue.pointer[0])[2] = top[0].real[0];
        break;
    case 0x164:
        if (parseListInit(&renderList, 0x50, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&lodSpriteList, 4, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&actionList, 8, top[0].i) != 0)
            goto outOfMemory;
        lodIndex = 0;
        break;
    case 0x35:
        if (top[-2].i != parseListCount(&actionList))
        {
            message = (char *)0x100a08e4;
            goto failWithMessage;
        }
        if (parseListCount(&lodSpriteList) == parseListCount(&renderList))
        {
            message = (char *)0x100a08bc;
            goto failWithMessage;
        }
        break;
    case 0x160:
        element = (int *)parseListAppend(&actionList);
        element[0] = parseListCount(top[-2].list);
        element[1] = (int)parseListData(top[-2].list);
        parseListReset(top[-2].list);
        break;
    case 0x15f:
        if (parseListInit(&lodList, 0xc, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x36:
        if (top[-2].i != parseListCount(&lodList))
        {
            message = (char *)0x100a089c;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &lodList;
        break;
    case 0x15d:
        *(float *)top[0].pointer[0] = 0.0f;
        ruleValue.word[0] = top[0].i;
        break;
    case 0x15c:
        if (t3d_float_ratio_max_guess == *(float *)top[0].pointer[0])
            *(int *)top[0].pointer[0] = ((int *)top[-1].pointer[0])[1];
        ruleValue.word[0] = top[0].i;
        break;
    case 0x15b:
        element = (int *)parseListAppend(&lodList);
        ruleValue.pointer[0] = element;
        element[0] = top[-3].i;
        ((float *)ruleValue.pointer[0])[1] = top[-2].real[0];
        ((int *)ruleValue.pointer[0])[2] = lodIndex++;
        break;
    case 0x15a:
        if (parseAddRenderEntry(parser, &renderList, top[0].pointer[0]) != 0)
            goto abortParse;
        break;
    case 0x159:
        if (parseAddLodSprite(parser, &lodSpriteList, top[0].s) != 0)
            goto abortParse;
        break;
    case 0x30:
    case 0x37:
    case 0x38:
        ruleValue.word[0] = 0x7149f2ca;
        break;
    case 0x157:
    case 0x158:
        ruleValue.real[0] = top[0].real[0] * top[0].real[0];
        break;
    case 0x154:
        parseListReset(&wallList);
        if (top[0].i == 0)
            break;
        if (parseListInit(&wallList, 0x70, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x39:
        index = top[-2].i;
        if (index != parseListCount(&wallList))
        {
            message = (char *)0x100a0888;
            goto failWithMessage;
        }
        ruleValue.word[0] = index;
        break;
    case 0x150:
    case 0x1c8:
        uvPairCount = parseListCount(top[0].list);
        break;
    case 0x14f:
        parseListReset(top[-4].list);
        break;
    case 0x14d:
        if (parseListInit(&entryList1c, 0x1c, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x14c:
        parseListReset(&entryList1c);
        break;
    case 0x146:
        if (top[-2].i != 0)
            goto otherRegions;
        regionShapeRecord.word[0] = 0xb;
        goto regionShapeRange;
    case 0x145:
        if (top[-2].i != 0)
            goto otherRegions;
        regionShapeRecord.word[0] = 0xa;
        goto regionShapeRange;
    case 0x144:
        if (top[-1].i != 0)
            goto otherRegions;
        regionShapeRecord.word[0] = 0xd;
        regionShapeRecord.word[5] = top[0].i - 1;
        break;
    case 0x143:
        if (top[-2].i != 0)
            goto otherRegions;
        regionShapeRecord.word[0] = 0xc;
regionShapeRange:
        regionShapeRecord.word[5] = top[-1].i - 1;
        regionShapeRecord.word[6] = top[0].i - 1;
        break;
    case 0x142:
        if (top[-1].i != 0)
            goto otherRegions;
        regionShapeRecord.word[0] = 8;
        goto regionShapeIndex;
    case 0x141:
        if (top[-1].i != 0)
            goto otherRegions;
        regionShapeRecord.word[0] = 9;
regionShapeIndex:
        regionShapeRecord.word[6] = 0;
        regionShapeRecord.word[5] = top[0].i - 1;
        break;
    case 0x140:
        if (parseListInit(&regionList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x3d:
        if (top[-5].i != 0)
            goto otherRegions;
        regionShapeRecord.word[0] = 0xe;
        regionShapeRecord.word[7] = top[-4].word[0];
        regionShapeRecord.word[8] = top[-4].word[1];
        regionShapeRecord.word[9] = top[-4].word[2];
        regionShapeRecord.word[10] = top[-4].word[3];
        regionShapeRecord.word[5] = top[-3].i;
        if (top[-3].i != parseListCount(&regionList))
        {
            message = (char *)0x100a084c;
            goto failWithMessage;
        }
        regionShapeRecord.word[6] = (int)parseListData(&regionList);
        parseListReset(&regionList);
        break;
    case 0x13f:
        if (top[-1].i != 0)
            goto otherRegions;
        regionShapeRecord.word[0] = 0x12;
        regionShapeRecord.word[5] = top[0].i;
        break;
otherRegions:
        message = (char *)0x100a0864;
        goto failWithMessage;
    case 0x13c:
        if (parseListInit(&visNodeList, 0x20, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x13b:
        if (parseListInit(&visListList, 8, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x13a:
        if (top[-6].i != parseListCount(&visNodeList))
        {
            message = (char *)0x100a0834;
            goto failWithMessage;
        }
        ruleValue.pointer[1] = parseListData(&visNodeList);
        parseListReset(&visNodeList);
        if (top[-4].i != parseListCount(&visListList))
        {
            message = (char *)0x100a0834;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = parseListData(&visListList);
        parseListReset(&visListList);
        break;
    case 0x135:
        element = (int *)parseListAppend(&visNodeList);
        element[0] = top[-5].word[0];
        element[1] = top[-5].word[1];
        element[2] = top[-5].word[2];
        element[3] = top[-5].word[3];
        if (top[-4].i != 0)
            element[6] = (int)parseListElement(&visListList, top[-4].i - 1);
        else
            element[6] = 0;
        if (top[-3].i != 0)
            element[4] = (int)parseListElement(&visNodeList, top[-3].i - 1);
        else
            element[4] = 0;
        if (top[-2].i != 0)
            element[5] = (int)parseListElement(&visNodeList, top[-2].i - 1);
        else
            element[5] = 0;
        break;
    case 0x131:
        parseListReset(&rangeList);
        if (top[0].i <= 0)
            break;
        if (parseListInit(&rangeList, 2, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x130:
        element = (int *)parseListAppend(&visListList);
        if (top[-4].i > 0)
            element[1] = (int)parseListData(&rangeList);
        else
            element[1] = 0;
        element[0] = top[-4].i;
        if (element[0] != parseListCount(&rangeList))
        {
            message = (char *)0x100a0820;
            goto failWithMessage;
        }
        parseListReset(&rangeList);
        break;
    case 0x12e:
        shortElement = (short *)parseListAppend(&rangeList);
        *shortElement = top[0].h[0] - 1;
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x12a:
        parseListData(&actionList);
        namePointer = (void *)0x100abf60;
        if (top[-4].pointer[0] != 0)
            namePointer = top[-4].pointer[0];
        scaleTable = parseDefaultObject;
        if (top[-3].pointer[0] != 0)
            scaleTable = top[-3].pointer[0];
        if (t3dDefineActor_parse(world, top[-9].s, parseListCount(&actionList), parseListData(&actionList), top[-7].i,
            top[-2].i, namePointer, scaleTable, (void *)0x100abf50, 0.0f, 0.0f, top[-5].i,
            parseListData(&lodSpriteList), top[-8].i) == 0)
        {
            message = (char *)0x100a0808;
            goto failWithMessage;
        }
        parseListFree(&renderList);
        parseListFree(&lodSpriteList);
        parseListReset(&actionList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-9].s);
        break;
    case 0x129:
        definition = parseLookup(world, top[-14].s);
        radius = top[-8].real[1];
        volume = 0;
        if (top[-8].real[0] * radius != 0.0f)
            volume = t3dCreateSphylinder_parse(radius, top[-8].real[0]);
        if (definition == 0)
        {
            message = (char *)0x100a07ec;
            detail = top[-14].s;
            goto failWithDetail;
        }
        if (volume == 0 && (top[-7].real[0] == 0.0f || top[-6].real[0] == 0.0f))
        {
            message = (char *)0x100a07d4;
            goto failWithMessage;
        }
        if (top[-4].pointer[0] != 0 && *t3dGetActorSprite_parse(lastActor, 0, 0) != 0x48)
        {
            message = (char *)0x100a07ac;
            goto failWithMessage;
        }
        object = t3dCreateActorEx_parse(world, definition, top[-13].s, top[-10].i, top[-9].pointer[0], volume,
            top[-6].real[0], top[-7].real[0], top[-3].i, top[-2].i);
        lastActor = object;
        if (object == 0)
        {
            logParserError_call(parser, (char *)0x100a077c, top[-14].s, top[-13].s);
            goto abortParse;
        }
        if (top[-4].pointer[0] != 0)
            t3dSetActorDMRGBTrack_parse(object, top[-4].pointer[0]);
        if (_t3dPostActorCreate_parse(parser, object, top[-13].s, top[-12].i, top[-11].i, top[-5].i) != 0)
            goto abortParse;
        source = (int *)top[-9].pointer[0];
        if (source != 0)
        {
            if (*source == 2)
                t3dDestroyPolyhedron_parse(world, source);
            else if (*source == 4)
                t3dDestroyPolyhedron_parse(world, source);
            else if (*source == 0x2a)
                EQGFX_T3D_FREE_MEMORY_GUESS(source);
        }
        if (volume != 0)
            GFX_COMPOSITE_DESTROY_VOLUME(world, volume);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-14].s);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-13].s);
        if (top[-5].s == 0)
            break;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-5].s);
        break;
    case 0x124:
        ruleValue.word[0] = parseCallbackValue(world, top[0].s);
        if (ruleValue.word[0] == 0 && parser->missingCallbacks20 == 0)
        {
            logParserError_call(parser, (char *)0x100a12b0, top[0].s);
            parser->missingCallbacks20++;
            parser->errorCount--;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0x121:
    case 0x1b9:
        ruleValue.word[0] = top[0].i - 1;
        break;
    case 0x120:
        ruleValue.pointer[0] = &boundsRecord;
        boundsRecord.word[0] = top[-5].i;
        boundsRecord.word[1] = top[-4].i;
        boundsRecord.word[2] = top[-3].i;
        boundsRecord.real[3] = (float)top[-2].i;
        boundsRecord.real[4] = (float)top[-1].i;
        boundsRecord.real[5] = (float)top[0].i;
        boundsRecord.word[6] = top[-6].i - 1;
        break;
    case 0x11c:
        ruleValue.pointer[0] = (void *)0x100a35c8;
        break;
    case 0x11b:
        ruleValue.pointer[0] = t3dAllocateMemory_call(0xc);
        if (ruleValue.pointer[0] == 0)
        {
            message = (char *)0x100a0758;
            goto failWithMessage;
        }
        *(int *)ruleValue.pointer[0] = 0x2a;
        ((float *)ruleValue.pointer[0])[2] = top[0].real[0];
        break;
    case 0x11a:
        collisionRecord.word[0] = 0x2b;
        ruleValue.pointer[0] = &collisionRecord;
        collisionRecord.word[1] = top[-3].i;
        collisionRecord.word[2] = top[-2].i;
        collisionRecord.word[3] = top[-1].i;
        collisionRecord.word[4] = top[0].i;
        break;
    case 0x118:
        status = _t3dDefinePolyhedron_parse(parser, top[-7].s, top[-5].real[0], top[-3].real[0], parseListCount(top[-2].list),
            parseListData(top[-2].list), parseListCount(top[-1].list), parseListData(top[-1].list));
        _t3dFreeObjectLists_parse(top[-2].list, top[-1].list);
        if (status != 0)
            goto abortParse;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-7].s);
        break;
    case 0x117:
        if (_t3dCreatePolyhedronFromDefinition_parse(parser, &ruleValue, top[-3].s, top[-2].s, top[-1].real[0]) != 0)
            goto abortParse;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-3].s);
        if (top[-2].s == 0)
            break;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-2].s);
        break;
    case 0x115:
        if (parseListInit(&faceList, 0x20, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x4d:
        if (top[-2].i != parseListCount(&faceList))
        {
            message = (char *)0x100a0744;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &faceList;
        break;
    case 0x111:
        object = parseListAppend(&faceList);
        if (_t3dCreateEdgePolygon_parse(parser, object, top[-2].i, &vertexList, &top[-3]) != 0)
            goto abortParse;
        parseListFree(top[-2].list);
        break;
    case 0x110:
        if (parseListInit(&vertexList, 0xc, top[0].i) != 0)
            goto outOfMemory;
        if (parseListInit(&radiusList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x10f:
        if (top[-3].i != parseListCount(&vertexList))
        {
            message = (char *)0x100a072c;
            goto failWithMessage;
        }
        if (_t3dDefineSphereList_parse(parser, top[-9].s, top[-7].real[0], top[-5].real[0], top[-3].i, parseListData(&vertexList),
            parseListData(&radiusList)) != 0)
            goto abortParse;
        parseListFree(&vertexList);
        parseListFree(&radiusList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-9].s);
        break;
    case 0x10e:
        element = (int *)parseListAppend(&vertexList);
        source = (int *)parseListAppend(&radiusList);
        element[0] = top[-3].i;
        element[1] = top[-2].i;
        element[2] = top[-1].i;
        *source = top[0].i;
        break;
    case 0x10d:
        if (_t3dCreateSphereListFromDefinition_parse(parser, &ruleValue, top[-3].s, top[-2].s, top[-1].real[0]) != 0)
            goto abortParse;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-3].s);
        if (top[-2].s == 0)
            break;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-2].s);
        break;
    case 0x10c:
        ruleValue.word[0] = top[0].word[0];
        ruleValue.word[1] = top[0].word[1];
        break;
    case 0xed:
    case 0x182:
    case 0x186:
    case 0x18b:
        parseListReset(&regionList);
        if (top[0].i == 0)
            break;
        if (parseListInit(&regionList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0xec:
        zone = t3dCreateZone_parse(world);
        if (zone == 0)
        {
            message = (char *)0x100a0714;
            goto failWithMessage;
        }
        if (top[-5].s != 0)
        {
            if (parseSetObjectTag(world, zone, top[-5].s) != 0)
            {
                message = (char *)0x100a06fc;
                goto failWithMessage;
            }
            if (parseAddToDictionary(world, zone) < 0)
            {
                message = (char *)0x100a06d8;
                goto failWithMessage;
            }
            EQGFX_T3D_FREE_MEMORY_GUESS(top[-5].s);
        }
        index = top[-4].i;
        if (index != parseListCount(&regionList))
        {
            message = (char *)0x100a06c0;
            goto failWithMessage;
        }
        if (index != 0)
        {
            index = 0;
            if (parseListCount(&regionList) > 0)
            {
                do
                {
                    object = parseRegionFromNumber(world, *(int *)parseListElement(&regionList, index));
                    if (t3dAddRegionToZone_parse(world, object, zone) != 0)
                    {
                        message = (char *)0x100a06a0;
                        goto failWithMessage;
                    }
                    index++;
                } while (index < parseListCount(&regionList));
            }
            parseListFree(&regionList);
        }
        if (top[-1].i == 0)
            break;
        parseSetSkipFrames(zone, top[-1].i);
        break;
    case 0xeb:
        if (top[-2].pointer[0] != 0)
            attachment = (char *)top[-2].pointer[0] + 0xc;
        else
            attachment = 0;
        if (top[-15].pointer[0] != 0)
            object = (char *)top[-15].pointer[0] + 0xc;
        else
            object = 0;
        if (t3dDefineParticleCloud_parse(world, top[-27].s, top[-25].i, top[-22].i, top[-20].i, top[-19].real[0],
            &top[-18], top[-2].i, attachment, top[-23].i, top[-16].i, top[-15].i, object, &top[-10],
            top[-14].real[0], top[-13].real[0], top[-11].i, top[-8].real[0], top[-6].i, top[-4].real[0], &top[-3],
            top[-1].i) == 0)
        {
            message = (char *)0x100a067c;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-27].s);
        break;
    case 0xea:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a0658;
            detail = top[0].s;
            goto failWithDetail;
        }
        ruleValue.pointer[0] = definition;
        break;
    case 0xe7:
        extentRecord.word[0] = top[-6].i;
        extentRecord.word[1] = top[-5].i;
        extentRecord.word[2] = top[-4].i;
        extentRecord.word[3] = top[-2].i;
        extentRecord.word[4] = top[-1].i;
        extentRecord.word[5] = top[0].i;
        ruleValue.pointer[0] = &extentRecord;
        break;
    case 0xf: case 0x10: case 0x40: case 0x4b: case 0x4c: case 0x61: case 0x64: case 0x65:
        ruleValue.real[0] = 0.0f;
        break;
    case 0x67:
        ruleValue.b[2] = 0xff;
        ruleValue.b[1] = 0xff;
        ruleValue.b[0] = 0xff;
        break;
    case 0xe3:
        ruleValue.b[2] = top[-2].b[0];
        ruleValue.b[1] = top[-1].b[0];
        ruleValue.b[0] = top[0].b[0];
        break;
    case 0xd4:
    case 0xda:
    case 0xdf:
        ruleValue.word[0] = 4;
        break;
    case 0xdb:
    case 0xe0:
        ruleValue.word[0] = 3;
        break;
    case 0xd9:
        ruleValue.word[0] = 5;
        break;
    case 0xd7:
        ruleValue.word[0] = top[-1].i | top[0].i;
        break;
    case 0xd5:
    case 0xdc:
    case 0xe1:
        ruleValue.word[0] = 2;
        break;
    case 0xd3:
        ruleValue.word[0] = 8;
        break;
    case 0xd2:
        ruleValue.word[0] = 0x10;
        break;
    case 0xd1:
        ruleValue.word[0] = 0x20;
        break;
    case 0xd0:
        ruleValue.word[0] = 0x80;
        break;
    case 0xcf:
        ruleValue.word[0] = 0x100;
        break;
    case 0xce:
        ruleValue.word[0] = 0x400;
        break;
    case 0xcd:
        ruleValue.word[0] = 0x800;
        break;
    case 0xcc:
        ruleValue.word[0] = 0x4000;
        break;
    case 0xcb:
        ruleValue.word[0] = 0x8000;
        break;
    case 0xca:
        ruleValue.word[0] = 0x10000;
        break;
    case 0xc9:
        ruleValue.word[0] = 0x20000;
        break;
    case 0xc8:
        if (parseLookup(world, top[-8].s) != 0)
        {
            EQGFX_T3D_FREE_MEMORY_GUESS(top[-8].s);
            break;
        }
        material = s3dDefineMaterial_parse(world, 0, top[-8].s);
        if (material == 0)
        {
            message = (char *)0x100a0630;
            goto failWithMessage;
        }
        material->value28 = 0;
        material->value2c = top[-6].b[0];
        material->value14 = top[-7].i;
        s3dSetMaterialIntensity_parse(world, material, top[-5].real[0]);
        s3dSetMaterialAmbientScalar_parse(world, material, top[-4].real[0]);
        s3dSetMaterialSimpleSprite_parse(world, material, top[-3].pointer[0]);
        s3dSetMaterialTwoSided_parse(world, material, top[-2].i);
        s3dSetMaterialUVShift_parse(world, material, top[-1].real[0], top[-1].real[1]);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-8].s);
        break;
    case 0x4e:
    case 0x68:
        ruleValue.real[0] = 0.0f;
        ruleValue.real[1] = 0.0f;
        break;
    case 0xc6:
        if (parseListInit(&materialList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x69:
        if (top[0].i != parseListCount(&materialList))
        {
            message = (char *)0x100a0618;
            goto failWithMessage;
        }
        break;
    case 0xc5:
        object = s3dCreateMaterialPalette_parse(world, top[-4].i, parseListData(&materialList), 1);
        parseListFree(&materialList);
        if (top[-5].s == 0)
            break;
        if (parseSetObjectTag(world, object, top[-5].s) != 0)
        {
            message = (char *)0x100a05f4;
            goto failWithMessage;
        }
        if (parseAddToDictionary(world, object) < 0)
        {
            message = (char *)0x100a05c4;
            goto failWithMessage;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-5].s);
        break;
    case 0xc3:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a05a4;
            detail = top[0].s;
            goto failWithDetail;
        }
        *(void **)parseListAppend(&materialList) = definition;
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        break;
    case 0xc0:
        spriteScale = 1.0f;
        vertexCount = 0;
        center[0] = 0.0f;
        center[1] = 0.0f;
        vertices = 0;
        uvCount = 0;
        center[2] = 0.0f;
        uvs = 0;
        normalCount = 0;
        normals = 0;
        colorCount = 0;
        colors = 0;
        faceCount = 0;
        faces = 0;
        meshOpCount = 0;
        meshOps = 0;
        skinGroupCount = 0;
        skinGroups = 0;
        spriteName = parseDefaultObject;
        groupCount = 0;
        groups = 0;
        extraCountA = 0;
        extraCountB = 0;
        attachment = 0;
        if (top[-18].list != 0)
        {
            vertexCount = parseListCount(top[-18].list);
            vertices = parseListData(top[-18].list);
        }
        if (top[-17].list != 0)
        {
            uvCount = parseListCount(top[-17].list);
            uvs = parseListData(top[-17].list);
        }
        if (top[-16].list != 0)
        {
            normalCount = parseListCount(top[-16].list);
            normals = parseListData(top[-16].list);
        }
        if (top[-15].list != 0)
        {
            colorCount = parseListCount(top[-15].list);
            colors = parseListData(top[-15].list);
        }
        if (top[-14].list != 0)
        {
            faceCount = parseListCount(top[-14].list);
            faces = parseListData(top[-14].list);
        }
        if (top[-13].list != 0)
        {
            meshOpCount = parseListCount(top[-13].list);
            meshOps = parseListData(top[-13].list);
        }
        definition = parseLookup(world, top[-11].s);
        if (definition == 0)
        {
            message = (char *)0x100a05a4;
            detail = top[-11].s;
            goto failWithDetail;
        }
        if (top[-10].list != 0)
        {
            skinGroupCount = parseListCount(top[-10].list);
            skinGroups = parseListData(top[-10].list);
        }
        if (top[-9].list != 0)
        {
            groupCount = parseListCount(top[-9].list);
            groups = parseListData(top[-9].list);
        }
        if (top[-6].list != 0)
            extraCountA = parseListCount(top[-6].list);
        if (top[-5].list != 0)
            extraCountB = parseListCount(top[-5].list);
        if (top[-3].pointer[0] != 0)
            spriteName = top[-3].pointer[0];
        if (top[-2].real[0] != 0.0f)
            spriteScale = top[-2].real[0];
        if (top[-1].pointer[0] != 0)
            attachment = top[-1].pointer[0];
        spriteDefinition = s3dDefineDMSprite_parse(world, top[-20].s, vertexCount, vertices, uvCount, uvs,
            normalCount, normals, colorCount, colors, faceCount, faces, skinGroupCount, skinGroups, definition,
            meshOpCount, meshOps, extraCountA, 0, extraCountB, 0, groupCount, groups, top[-8].i, top[-7].i,
            &top[-19], center, spriteName, spriteScale, attachment);
        if (spriteDefinition == 0)
        {
            message = (char *)0x100a057c;
            goto failWithMessage;
        }
        if (top[-15].list != 0 && hasRgba == 1)
            spriteDefinition->flags0e |= 0x20;
        if (top[-8].i == 0 && meshOpCount == 0)
            s3dComputeDMSpriteDefPlanarEquations_parse(spriteDefinition, 1);
        if (top[-18].list != 0)
            parseListFree(top[-18].list);
        if (top[-17].list != 0)
            parseListFree(top[-17].list);
        if (top[-16].list != 0)
            parseListFree(top[-16].list);
        if (top[-15].list != 0)
            parseListFree(top[-15].list);
        if (top[-14].list != 0)
            parseListFree(top[-14].list);
        if (top[-13].list != 0)
            parseListFree(top[-13].list);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-11].s);
        if (top[-10].list != 0)
            parseListFree(top[-10].list);
        if (top[-9].list != 0)
            parseListFree(top[-9].list);
        if (top[-6].list != 0)
            parseListFree(top[-6].list);
        if (top[-5].list != 0)
            parseListFree(top[-5].list);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-20].s);
        break;
    case 0xbf:
        boxRecord.word[0] = top[-6].i;
        boxRecord.word[1] = top[-5].i;
        boxRecord.word[2] = top[-4].i;
        boxRecord.word[3] = top[-2].i;
        boxRecord.word[4] = top[-1].i;
        boxRecord.word[5] = top[0].i;
        ruleValue.pointer[0] = &boxRecord;
        break;
    case 4: case 0x12: case 0x13: case 0x14: case 0x62: case 0x66: case 0x6b: case 0xe4: case 0xe8:
        ruleValue.real[0] = 0.0f;
        ruleValue.real[1] = 0.0f;
        ruleValue.real[2] = 0.0f;
        break;
    case 0xbe:
    case 0x1cd:
    case 0x1df:
    case 0x208:
        ruleValue.word[0] = top[-2].i;
        ruleValue.word[1] = top[-1].i;
        ruleValue.word[2] = top[0].i;
        break;
    case 0xbd:
        if (parseListInit(&uvList, 8, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x6d:
        if (top[0].i != parseListCount(&uvList))
        {
            message = (char *)0x100a0558;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &uvList;
        break;
    case 0xbc:
        if (parseListInit(&vertexNormalList, 0xc, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x6f:
        if (top[0].i != parseListCount(&vertexNormalList))
        {
            message = (char *)0x100a0528;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &vertexNormalList;
        break;
    case 0xbb:
        element = (int *)parseListAppend(&vertexNormalList);
        element[0] = top[0].word[0];
        element[1] = top[0].word[1];
        element[2] = top[0].word[2];
        ruleValue.word[0] = 1;
        break;
    case 0xba:
        element = (int *)parseListAppend(&vertexNormalList);
        element[0] = top[0].word[0];
        element[1] = top[0].word[1];
        element[2] = top[0].word[2];
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x70:
        ruleValue.word[0] = 0;
        hasRgba = 0;
        break;
    case 0xb8:
    case 0xb9:
        if (parseListInit(&colorList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x71:
        if (top[0].i != parseListCount(&colorList))
        {
            message = (char *)0x100a0500;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &colorList;
        hasRgba = 0;
        break;
    case 0x72:
        if (top[0].i != parseListCount(&colorList))
        {
            message = (char *)0x100a04d8;
            goto failWithMessage;
        }
        hasRgba = 1;
        ruleValue.pointer[0] = &colorList;
        break;
    case 0xb3:
    case 0xb6:
        element = (int *)parseListAppend(&colorList);
        *element = top[0].i;
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0xb5:
        red = (int)(top[-2].real[0] * parseColorScale);
        green = (int)(top[-1].real[0] * parseColorScale);
        blue = (int)(top[0].real[0] * parseColorScale);
        if (red < 0)
            red = 0;
        else if (red > 0xff)
            red = 0xff;
        if (green < 0)
            green = 0;
        else if (green > 0xff)
            green = 0xff;
        if (blue < 0)
            blue = 0;
        else if (blue > 0xff)
            blue = 0xff;
        ruleValue.b[3] = 0xff;
        ruleValue.b[2] = (unsigned char)red;
        ruleValue.b[1] = (unsigned char)green;
        ruleValue.b[0] = (unsigned char)blue;
        break;
    case 0xb4:
    case 0xb7:
        element = (int *)parseListAppend(&colorList);
        *element = top[0].i;
        ruleValue.word[0] = 1;
        break;
    case 0xb2:
        red = (int)(top[-3].real[0] * parseColorScale);
        green = (int)(top[-2].real[0] * parseColorScale);
        blue = (int)(top[-1].real[0] * parseColorScale);
        alpha = (int)(top[0].real[0] * parseColorScale);
        if (red < 0)
            red = 0;
        else if (red > 0xff)
            red = 0xff;
        if (green < 0)
            green = 0;
        else if (green > 0xff)
            green = 0xff;
        if (blue < 0)
            blue = 0;
        else if (blue > 0xff)
            blue = 0xff;
        if (alpha < 0)
            alpha = 0;
        else if (alpha > 0xff)
            alpha = 0xff;
        ruleValue.b[1] = (unsigned char)green;
        ruleValue.b[2] = (unsigned char)red;
        ruleValue.b[0] = (unsigned char)blue;
        ruleValue.b[3] = (unsigned char)alpha;
        break;
    case 0xb1:
        if (parseListInit(&vertexChainList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x74:
        if (top[0].i != parseListCount(&vertexChainList))
        {
            message = (char *)0x100a04a8;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &vertexChainList;
        break;
    case 0xb0:
        element = (int *)parseListAppend(&vertexChainList);
        *element = top[0].i;
        ruleValue.word[0] = 1;
        break;
    case 0xaf:
        element = (int *)parseListAppend(&vertexChainList);
        *element = top[0].i;
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0xae:
        ruleValue.b[0] = top[-2].i != 0;
        ruleValue.b[1] = top[-1].b[0] - 1;
        ruleValue.h[1] = top[0].h[0] - 1;
        break;
    case 0xad:
        if (parseListInit(&meshOpList, 0xc, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x76:
        if (top[0].i != top[-2].i)
        {
            message = (char *)0x100a0480;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &meshOpList;
        break;
    case 0xac:
        element = (int *)parseListAppend(&meshOpList);
        element[0] = top[0].word[0];
        element[1] = top[0].word[1];
        element[2] = top[0].word[2];
        ruleValue.word[0] = 1;
        break;
    case 0xab:
        element = (int *)parseListAppend(&meshOpList);
        element[0] = top[0].word[0];
        element[1] = top[0].word[1];
        element[2] = top[0].word[2];
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0xaa:
        ruleValue.word[0] = 2;
        ruleValue.word[1] = top[0].i - 1;
        break;
    case 0xa9:
        ruleValue.word[0] = 1;
        ruleValue.word[1] = top[-2].i - 1;
        ruleValue.h[4] = top[-1].h[0] - 1;
        ruleValue.h[5] = top[0].h[0] - 1;
        break;
    case 0xa8:
        ruleValue.word[0] = 4;
        ruleValue.word[1] = top[0].i;
        break;
    case 0xa7:
        ruleValue.word[0] = 3;
        ruleValue.word[1] = top[0].i - 1;
        break;
    case 0xa6:
        if (parseListInit(&skinGroupList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x78:
        if (top[0].i != parseListCount(&skinGroupList))
        {
            message = (char *)0x100a0460;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &skinGroupList;
        break;
    case 0xa5:
        shortElement = (short *)parseListAppend(&skinGroupList);
        shortElement[0] = top[-1].h[0];
        shortElement[1] = top[0].h[0] - 1;
        ruleValue.word[0] = 1;
        break;
    case 0xa4:
        shortElement = (short *)parseListAppend(&skinGroupList);
        shortElement[0] = top[-1].h[0];
        shortElement[1] = top[0].h[0] - 1;
        ruleValue.word[0] = top[-2].i + 1;
        break;
    case 0xa3:
        if (parseListInit(&materialGroupList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x7a:
        if (top[0].i != parseListCount(&materialGroupList))
        {
            message = (char *)0x100a043c;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &materialGroupList;
        break;
    case 0xa2:
        shortElement = (short *)parseListAppend(&materialGroupList);
        shortElement[0] = top[-1].h[0];
        shortElement[1] = top[0].h[0] - 1;
        ruleValue.word[0] = 1;
        break;
    case 0xa1:
        shortElement = (short *)parseListAppend(&materialGroupList);
        shortElement[0] = top[-1].h[0];
        shortElement[1] = top[0].h[0] - 1;
        ruleValue.word[0] = top[-2].i + 1;
        break;
    case 0xa0:
        if (parseListInit(&vertexMaterialGroupList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x7c:
        if (top[0].i != parseListCount(&vertexMaterialGroupList))
        {
            message = (char *)0x100a043c;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &vertexMaterialGroupList;
        break;
    case 0x9f:
        shortElement = (short *)parseListAppend(&vertexMaterialGroupList);
        shortElement[0] = top[-1].h[0];
        shortElement[1] = top[0].h[0] - 1;
        ruleValue.word[0] = 1;
        break;
    case 0x9e:
        shortElement = (short *)parseListAppend(&vertexMaterialGroupList);
        shortElement[0] = top[-1].h[0];
        shortElement[1] = top[0].h[0] - 1;
        ruleValue.word[0] = top[-2].i + 1;
        break;
    case 0x9d:
        if (parseListInit(&faceEntryList, 0x10, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x7e:
        if (top[0].i != parseListCount(&faceEntryList))
        {
            message = (char *)0x100a0414;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &faceEntryList;
        break;
    case 0x9c:
        element = (int *)parseListAppend(&faceEntryList);
        source = (int *)top[0].pointer[0];
        element[0] = source[0];
        element[1] = source[1];
        element[2] = source[2];
        element[3] = source[3];
        ruleValue.word[0] = 1;
        break;
    case 0x9b:
        element = (int *)parseListAppend(&faceEntryList);
        source = (int *)top[0].pointer[0];
        element[0] = source[0];
        element[1] = source[1];
        element[2] = source[2];
        element[3] = source[3];
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x9a:
        polygonRecord.h[4] = 0;
        ruleValue.pointer[0] = &polygonRecord;
        polygonRecord.word[0] = 0x4b;
        polygonRecord.h[5] = top[-13].h[0] - 1;
        polygonRecord.h[6] = top[-12].h[0] - 1;
        polygonRecord.h[7] = top[-11].h[0] - 1;
        polygonRecord.word[1] = top[-9].i - 1;
        if (top[-1].i == 1)
            polygonRecord.h[4] = 0x10;
        break;
    case 0x80:
        if (parseListInit(&face2List, 8, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x81:
        if (top[0].i != parseListCount(&face2List))
        {
            message = (char *)0x100a03ec;
            goto failWithMessage;
        }
        ruleValue.pointer[0] = &face2List;
        break;
    case 0x82:
        element = (int *)parseListAppend(&face2List);
        source = (int *)top[0].pointer[0];
        element[0] = source[0];
        element[1] = source[1];
        ruleValue.word[0] = 1;
        break;
    case 0x83:
        element = (int *)parseListAppend(&face2List);
        source = (int *)top[0].pointer[0];
        element[0] = source[0];
        element[1] = source[1];
        ruleValue.word[0] = top[-1].i + 1;
        break;
    case 0x84:
        triangleRecord.h[0] = 0;
        ruleValue.pointer[0] = &triangleRecord;
        triangleRecord.h[1] = top[-4].h[0] - 1;
        triangleRecord.h[2] = top[-3].h[0] - 1;
        triangleRecord.h[3] = top[-2].h[0] - 1;
        if (top[-1].i == 1)
            triangleRecord.h[0] = 0x10;
        break;
    case 0x98:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a03a8;
            detail = top[0].s;
            goto failWithDetail;
        }
        object = s3dCreateDMTrack_parse(world, definition, -1, 1.0f);
        goto createdTrack;
    case 0x97:
        if (parseListInit(&dmTrackFrameList, 4, top[0].i) != 0)
            goto outOfMemory;
        break;
    case 0x96:
        if (parseListCount(&dmTrackFrameList) != 0 && top[-4].i != parseListCount(&dmTrackFrameList))
        {
            message = (char *)0x100a035c;
            goto failWithMessage;
        }
        if (s3dDefineDMTrack_parse(world, top[-7].s, top[-5].i, top[-4].i, top[-2].i, top[-1].i,
            parseListCount(&dmTrackFrameList) != 0 ? parseListData(&dmTrackFrameList) : 0) == 0)
        {
            message = (char *)0x100a03c4;
            detail = top[-7].s;
            goto failWithDetail;
        }
        index = 0;
        if (parseListCount(&dmTrackFrameList) > 0)
        {
            do
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(*(void **)parseListElement(&dmTrackFrameList, index));
                index++;
            } while (index < parseListCount(&dmTrackFrameList));
        }
        parseListFree(&dmTrackFrameList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-7].s);
        break;
    case 0x95:
        element = (int *)parseListAppend(&dmTrackFrameList);
        *element = (int)parseListData(top[-1].list);
        parseListReset(&vertexList);
        break;
    case 0x94:
        definition = parseLookup(world, top[0].s);
        if (definition == 0)
        {
            message = (char *)0x100a03a8;
            detail = top[0].s;
            goto failWithDetail;
        }
        object = s3dCreateDMRGBTrack_parse(world, definition, -1, 1.0f);
createdTrack:
        if (object == 0)
        {
            message = (char *)0x100a0384;
            detail = top[0].s;
            goto failWithDetail;
        }
        EQGFX_T3D_FREE_MEMORY_GUESS(top[0].s);
        ruleValue.pointer[0] = object;
        break;
    case 0x93:
        if (parseListInit(&rgbTrackFrameList, 4, top[0].i) != 0)
            goto outOfMemory;
        rgbTrackFlag = 0;
        break;
    case 0x92:
        if (parseListCount(&rgbTrackFrameList) != 0 && top[-4].i != parseListCount(&rgbTrackFrameList))
        {
            message = (char *)0x100a035c;
            goto failWithMessage;
        }
        trackDefinition = s3dDefineDMRGBTrack_parse(world, top[-7].s, top[-5].i, top[-4].i, top[-2].i, top[-1].i,
            parseListCount(&rgbTrackFrameList) != 0 ? parseListData(&rgbTrackFrameList) : 0);
        if (trackDefinition == 0)
        {
            message = (char *)0x100a0330;
            detail = top[-7].s;
            goto failWithDetail;
        }
        if (rgbTrackFlag == 1)
            trackDefinition->flags0e |= 0x80;
        index = 0;
        if (parseListCount(&rgbTrackFrameList) > 0)
        {
            do
            {
                EQGFX_T3D_FREE_MEMORY_GUESS(*(void **)parseListElement(&rgbTrackFrameList, index));
                index++;
            } while (index < parseListCount(&rgbTrackFrameList));
        }
        parseListFree(&rgbTrackFrameList);
        EQGFX_T3D_FREE_MEMORY_GUESS(top[-7].s);
        break;
    case 0x91:
        element = (int *)parseListAppend(&rgbTrackFrameList);
        *element = (int)parseListData(top[-1].list);
        if (hasRgba != 0)
            rgbTrackFlag = 1;
        parseListReset(&colorList);
        break;
    case 5: case 6: case 0xa: case 0xe: case 0x15: case 0x16: case 0x17: case 0x19: case 0x1d: case 0x1e:
    case 0x1f: case 0x20: case 0x21: case 0x23: case 0x24: case 0x27: case 0x28: case 0x2b: case 0x2c: case 0x2d:
    case 0x2e: case 0x31: case 0x32: case 0x34: case 0x3a: case 0x3b: case 0x3e: case 0x3f: case 0x41: case 0x42:
    case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: case 0x48: case 0x49: case 0x4a: case 0x60: case 0x63:
    case 0x6a: case 0x6c: case 0x6e: case 0x73: case 0x75: case 0x77: case 0x79: case 0x7b: case 0x7d: case 0x7f:
    case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0xd8: case 0xde: case 0x12f: case 0x152:
    case 0x167: case 0x1dc: case 0x203: case 0x205: case 0x210:
        ruleValue.word[0] = 0;
        break;
    case 0x235:
        message = (char *)0x100a1284;
        goto failWithMessage;
    case 0:
        result = 0;
        goto finishParse;
    }
    gotoEntry = &parseGotoTable[parseRuleGotos[(short)rule]];
    entry = gotoEntry + 1;
    do
    {
        value = *entry++;
    } while ((short)*stateTop < value);
    entry--;
    if ((short)*stateTop == *entry)
        state = -1 - gotoEntry[gotoEntry - entry];
    else
        state = -1 - *gotoEntry;
nextState:
    stateTop++;
    if (stateTop <= stateStack + PARSE_STACK_LIMIT)
        goto pushState;
stackOverflow:
    logParserError_call(parser, (char *)0x100a12f4, (char *)0x100a1298);
    goto abortParse;

parseError:
    if (errorFlag == 0)
    {
        syntaxErrors++;
        logParserError_call(parser, (char *)0x100a12f4, (char *)0x100a1300);
        if ((short)token == token)
            goto recoverError;
        errorFlag = 1;
        goto newState;
    }
    if (errorFlag <= 0)
        goto abortParse;
    if (errorFlag <= 2)
        goto recoverError;
    if (errorFlag != 3 || token == 0)
        goto abortParse;
    token = -1;
    goto newState;
recoverError:
    errorFlag = 3;
    while (stateTop > stateStack)
    {
        if (*stateTop < PARSE_FIRST_REDUCE_STATE)
        {
            actions = &parseActionTable[parseStateActions[(short)*stateTop]];
            entry = actions;
            do
            {
                value = *entry++;
                if (value == PARSE_ERROR_TOKEN)
                {
                    state = -1 - actions[actions - entry];
                    goto nextState;
                }
            } while (value > 0x100);
        }
        stateTop--;
        valueTop--;
    }
    goto abortParse;

outOfMemory:
    message = (char *)0x100871d0;
failWithMessage:
    logParserError_call(parser, message);
    goto abortParse;
failWithDetail:
    logParserError_call(parser, message, detail);
abortParse:
    result = 1;
finishParse:
    *tokenValue = savedTokenValue;
    ruleValue = savedRuleValue;
    EQGFX_T3D_FREE_MEMORY_GUESS(stateStack);
    EQGFX_T3D_FREE_MEMORY_GUESS(valueStack);
    return result;
}

// FUNCTION: EQGFX 0x1002daaf
// Returns a named world's dictionary value for parser callbacks.
unsigned int __cdecl getDictionaryValueByKey_guess(void *dictionary, const char *key)
{
    DictionaryEntry_guess *entry;

    entry = (DictionaryEntry_guess *)EQ_CDECL(DictionaryEntry_guess *, 0x1000E7FE, (void *, const char *))
        (dictionary, key);
    if (entry == 0)
        return 0;
    return entry->value10;
}

// FUNCTION: EQGFX 0x1002dac7
// Retains a render definition for later drawing of the parsed world.
// __cdecl (callee pops 0 bytes). Copies the 0x50-byte entry into the next slot of renderList. A NULL entry or a failed growth logs through parser and returns 1; otherwise the copy is made and 0 is returned.
int __cdecl parseAddRenderEntry_guess(ParserObject_guess *parser, FUN_1002dce0_object_guess *renderList, RenderListEntry_guess *entry)
{
    RenderListEntry_guess *slot;
    int result;

    if (entry == 0)
    {
        EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, EQ_ARRAY(char, 0x100A1310));
        return 1;
    }
    if (EQ_CDECL(int, 0x1002DCCF, (FUN_1002dce0_object_guess *))(renderList) != 0)
    {
        if (EQ_CDECL(int, 0x1002DC8D, (FUN_1002dce0_object_guess *, int))(renderList,
            EQ_CDECL(unsigned int, 0x1002DD6F, (FUN_1002dce0_object_guess *))(renderList) + 10) != 0)
        {
            EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, EQ_ARRAY(char, 0x100871D0));
            return 1;
        }
    }
    slot = EQ_CDECL(RenderListEntry_guess *, 0x1002DD34, (FUN_1002dce0_object_guess *))(renderList);
    result = 0;
    *slot = *entry;
    return result;
}

// FUNCTION: EQGFX 0x1002db29
// Keeps level-of-detail sprite references for parsed world models.
// __cdecl (callee pops 0 bytes). Appends spriteInst to spriteList. A NULL spriteInst or a failed growth logs through parser and returns 1; otherwise spriteInst goes in the next slot and 0 is returned.
int __cdecl parseAddSpriteInstance_guess(ParserObject_guess *parser, FUN_1002dce0_object_guess *spriteList, int spriteInst)
{
    int growthFailed;
    int *slot;

    if (spriteInst == 0)
    {
        EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, EQ_ARRAY(char, 0x100A1330));
        return 1;
    }
    if (EQ_CDECL(int, 0x1002DCCF, (FUN_1002dce0_object_guess *))(spriteList) != 0)
    {
        growthFailed = EQ_CDECL(int, 0x1002DC8D, (FUN_1002dce0_object_guess *, int))(spriteList,
            EQ_CDECL(unsigned int, 0x1002DD6F, (FUN_1002dce0_object_guess *))(spriteList) + 10);
        if (growthFailed != 0)
        {
            EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, EQ_ARRAY(char, 0x100871D0));
            return 1;
        }
    }
    slot = (int *)EQ_CDECL(int, 0x1002DD34, (FUN_1002dce0_object_guess *))(spriteList);
    *slot = spriteInst;
    return 0;
}

// FUNCTION: EQGFX 0x1002db83
// Keeps parsed frame and sub-DAG name data for later world-building steps.
// __cdecl (callee pops 0 bytes). Same contract as parseAddSpriteInstance_guess: a NULL spriteInst or a failed growth logs through parser and returns 1; otherwise spriteInst goes in the next slot and 0 is returned. Both messages share one logParserError call.
int __cdecl parseAddMemoryBlock_guess(ParserObject_guess *parser, FUN_1002dce0_object_guess *spriteList, int spriteInst)
{
    int growthFailed;
    int *slot;
    char *message;
    int result;

    if (spriteInst == 0)
    {
        message = EQ_ARRAY(char, 0x100A1358);
        goto logFailure;
    }
    if (EQ_CDECL(int, 0x1002DCCF, (FUN_1002dce0_object_guess *))(spriteList) != 0)
    {
        growthFailed = EQ_CDECL(int, 0x1002DC8D, (FUN_1002dce0_object_guess *, int))(spriteList,
            EQ_CDECL(unsigned int, 0x1002DD6F, (FUN_1002dce0_object_guess *))(spriteList) + 10);
        if (growthFailed != 0)
        {
            message = EQ_ARRAY(char, 0x100871D0);
            goto logFailure;
        }
    }
    slot = (int *)EQ_CDECL(int, 0x1002DD34, (FUN_1002dce0_object_guess *))(spriteList);
    *slot = spriteInst;
    result = 0;
    goto done;

logFailure:
    EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, message);
    result = 1;

done:
    return result;
}

// FUNCTION: EQGFX 0x1002dbdd
// Keeps MIP file references for the parsed world's textures.
// Cloned from parseAddSpriteInstance_guess 0x1002DB29 (same original code).
// __cdecl (callee pops 0 bytes). Appends spriteInst to spriteList. A NULL spriteInst or a failed growth logs through parser and returns 1; otherwise spriteInst goes in the next slot and 0 is returned.
int __cdecl parseAddMipFile_guess(ParserObject_guess *parser, FUN_1002dce0_object_guess *spriteList, int spriteInst)
{
    int growthFailed;
    int *slot;

    if (spriteInst == 0)
    {
        EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, EQ_ARRAY(char, 0x100A1380));
        return 1;
    }
    if (EQ_CDECL(int, 0x1002DCCF, (FUN_1002dce0_object_guess *))(spriteList) != 0)
    {
        growthFailed = EQ_CDECL(int, 0x1002DC8D, (FUN_1002dce0_object_guess *, int))(spriteList,
            EQ_CDECL(unsigned int, 0x1002DD6F, (FUN_1002dce0_object_guess *))(spriteList) + 10);
        if (growthFailed != 0)
        {
            EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, EQ_ARRAY(char, 0x100871D0));
            return 1;
        }
    }
    slot = (int *)EQ_CDECL(int, 0x1002DD34, (FUN_1002dce0_object_guess *))(spriteList);
    *slot = spriteInst;
    return 0;
}

// FUNCTION: EQGFX 0x1002dc37
// Prepares parser lists for world geometry, lighting, and animation data.
// __cdecl. Re-inits count/elementSize and allocates the block when a field is 0. Returns 1 when all four fields were already held or the allocation failed, else 0.
int __cdecl initDynamicArray_guess(FUN_1002dce0_object_guess *object, int elementSize, int count)
{
    if (object->value00 == 0 || object->value04 == 0 || object->value08 == 0 || object->allocation0c == 0)
    {
        object->value00 = count;
        object->value04 = elementSize;
        if (count != 0)
            object->allocation0c = t3dAllocateMemory_call(elementSize * count);
        else
            object->allocation0c = 0;
        object->value08 = 0;
        if (object->allocation0c != 0 || count == 0)
            return 0;
    }
    return 1;
}

// FUNCTION: EQGFX 0x1002dc8d
// Expands parser list capacity as more world records are read.
// __cdecl (callee pops 0 bytes). When requiredCount exceeds the count at +0x00, reallocates the block at +0x0c
// to requiredCount elements of +0x04 bytes. Returns 1 when no block is held afterwards, else 0.
int __cdecl reserveDynamicArray_guess(FUN_1002dce0_object_guess *array, int requiredCount)
{
    void *allocation;

    if (requiredCount > (int)array->value00)
    {
        if (array->allocation0c != 0)
        {
            allocation = EQ_CDECL(void *, 0x10050C8C, (void *, size_t))
                (array->allocation0c, array->value04 * requiredCount);
        }
        else
        {
            allocation = t3dAllocateMemory_call(array->value04 * requiredCount);
        }
        array->allocation0c = allocation;
        array->value00 = requiredCount;
    }
    return array->allocation0c == 0;
}

// FUNCTION: EQGFX 0x1002dccf
// Checks whether a parsed world-data list has reached its entry limit.
// __cdecl (callee pops 0 bytes). Returns 1 when the dword at +0x00 is
// less than or equal to the dword at +0x08, else 0 (signed compare).
int __cdecl isDynamicArrayFull_guess(int *array)
{
    return array[2] >= array[0];
}

// FUNCTION: EQGFX 0x1002dce0
// Releases temporary world data after parser lists are no longer needed.
void __cdecl freeDynamicArray_guess(FUN_1002dce0_object_guess *object)
{
    EQGFX_T3D_FREE_MEMORY_GUESS(object->allocation0c);
    object->allocation0c = 0;
    object->value00 = 0;
    object->value04 = 0;
    object->value08 = 0;
}

// FUNCTION: EQGFX 0x1002dcfd
// __cdecl (callee pops 0 bytes). Zeroes the four dwords of the vector.
void __cdecl initializeVector_guess(int *vector)
{
    vector[3] = 0;
    vector[0] = 0;
    vector[1] = 0;
    vector[2] = 0;
}

// FUNCTION: EQGFX 0x1002dd0f
// Provides the stored records for consumers of parsed world lists.
// __cdecl (callee pops 0 bytes; callers pass the object on the stack). Returns
// the 4-byte value at +0x0c of the object.
// NOTE: BUNDLE names this getObjectValue_guess, but unit_1002902B.c already
// defines getObjectValue_guess @ 0x1002dd6f (+0x08); C needs unique symbols so
// this rewrite keeps the same label via the fixture's "rewrite" alias (see
// getObjectValue_1002dd0f.json) with a unique C symbol here.
unsigned int __cdecl getObjectValueAt0C_guess(int graphicsObject)
{
    return *(unsigned int *)(graphicsObject + 0x0c);
}

// FUNCTION: EQGFX 0x1002dd17
// Resolves a parsed node index to its stored world-data entry.
// __cdecl (callee pops 0 bytes). Returns stride * index + base when
// 0 <= index < count, else 0.
int __cdecl getArrayElementOffset_guess(int *arrayInfo, int index)
{
    int temp;
    if (index < 0)
        return 0;
    if (!(index >= *arrayInfo))
        {
    }
    else
        return 0;
    temp = arrayInfo[1];
    return temp * index + arrayInfo[3];
}

// FUNCTION: EQGFX 0x1002dd34
// Provides the next entry position for parsed geometry and animation data.
// __cdecl. Returns the element offset at the cursor (+0x08) and advances the cursor; grows the block first when the cursor reaches the count (returns 0 if growth fails).
int __cdecl appendDynamicArrayElement_guess(FUN_1002dce0_object_guess *array)
{
    int needsGrowth;
    int requiredCount;
    int growthFailed;
    int elementOffset;

    needsGrowth = EQ_CDECL(int, 0x1002DCCF, (FUN_1002dce0_object_guess *))(array);
    if (needsGrowth != 0)
    {
        requiredCount = EQ_CDECL(unsigned int, 0x1002DD6F, (FUN_1002dce0_object_guess *))(array) + 10;
        growthFailed = EQ_CDECL(int, 0x1002DC8D, (FUN_1002dce0_object_guess *, int))(array, requiredCount);
        if (growthFailed != 0)
            return 0;
    }
    elementOffset = EQ_CDECL(int, 0x1002DD17, (FUN_1002dce0_object_guess *, int))(array, array->value08);
    array->value08 = array->value08 + 1;
    return elementOffset;
}

// FUNCTION: EQGFX 0x1002dd6f
// Reports the populated entry count of a parsed world-data list.
// __cdecl (callee pops 0 bytes; callers pass the object on the stack). Returns
// the 4-byte value at +0x08 of the object.
unsigned int __cdecl getObjectValue_guess(int object)
{
    return *(unsigned int *)(object + 8);
}

// FUNCTION: EQGFX 0x1002dd77
// __cdecl. New 0x20-byte object tagged 0x25 that shares sharedObject (the 4th stack arg, +0x10); args 1-3 are unread.
T3DObject_guess *__cdecl createT3dObject_guess(int unusedArg1, int unusedArg2, int unusedArg3, void *sharedObject)
{
    T3DObject_guess *object;

    object = t3dAllocateMemory_call(sizeof(T3DObject_guess));
    object->kind00 = 0x25;
    object->sharedObject04 = sharedObject;
    if (sharedObject != 0)
        EQ_CDECL(void, 0x10017CE8, (void *))(sharedObject);
    object->value08 = 0;
    object->value0c = 0;
    object->value14 = 0;
    object->value18 = 0;
    object->value1c = 0;
    return object;
}

// FUNCTION: EQGFX 0x1002ddb1
// Builds the graphics object attached to a parsed world region.
// __cdecl. New 0x24-byte object tagged 0x28 that shares sharedObject (the 4th stack arg, +0x04); +0x08/+0x0c come from args 1 and 3, +0x10 and +0x20 are args 2 and 5 raw.
GraphicsObject_guess *__cdecl createGraphicsObject_guess(int firstObject, unsigned int middleValue, int secondObject,
    void *sharedObject, unsigned int lastValue)
{
    GraphicsObject_guess *object;

    object = t3dAllocateMemory_call(sizeof(GraphicsObject_guess));
    object->kind00 = 0x28;
    object->sharedObject04 = sharedObject;
    if (sharedObject != 0)
        EQ_CDECL(void, 0x10017CE8, (void *))(sharedObject);
    object->value08 = FUN_1002dd6f_call(firstObject);
    object->value0c = FUN_1002dd0f_call(firstObject);
    object->value10 = middleValue;
    object->value14 = FUN_1002dd6f_call(secondObject);
    object->value18 = FUN_1002dd0f_call(secondObject);
    object->value1c = 0xffffffff;
    object->value20 = lastValue;
    return object;
}

// FUNCTION: EQGFX 0x1002de1a
// Defines the edges and surface orientation of a parsed world polygon.
int __cdecl _t3dCreateEdgePolygon_guess(ParserObject_guess *parser, int *edgePolygonWords, int indexObject,
    int vertexDataObject, float *normal)
{
    EdgePolygon_guess *polygon;
    EdgePolygonEdge_guess *edge;
    int edgeCount;
    int *vertexIndices;
    int vertexDataAddress;
    float *vertexData;
    float *firstVertex;
    float *secondVertex;
    float edgeLength;
    int nextIndex;
    int keepGoing;
    int normalResult;
    char *errorMessage;
    float absNormalX;
    float absNormalY;
    float absNormalZ;

    polygon = (EdgePolygon_guess *)edgePolygonWords;
    edgeCount = FUN_1002dd6f_call(indexObject);
    vertexIndices = (int *)FUN_1002dd0f_call(indexObject);
    vertexDataAddress = FUN_1002dd6f_call(vertexDataObject);
    vertexData = (float *)FUN_1002dd0f_call(vertexDataObject);
    polygon->kind00 = EDGE_POLYGON_KIND;
    polygon->edgeCount04 = edgeCount;
    polygon->edges08 = (EdgePolygonEdge_guess *)t3dAllocateMemory_call(edgeCount * EDGE_POLYGON_EDGE_BYTES);
    if (0 < edgeCount) {
        vertexDataObject = 0;
        indexObject = 1;
        edgePolygonWords = vertexIndices;
        do {
            nextIndex = indexObject;
            if (edgeCount <= indexObject)
                nextIndex = 0;
            if ((vertexDataAddress <= *edgePolygonWords) || (vertexDataAddress <= vertexIndices[nextIndex])) {
                errorMessage = s_CreateEdgePolygon_indexOutOfRange;
                goto reportError;
            }
            edge = (EdgePolygonEdge_guess *)((char *)polygon->edges08 + vertexDataObject);
            edge->kind00 = EDGE_POLYGON_EDGE_KIND;
            edge->firstVertex04 = vertexData + *edgePolygonWords * 3;
            firstVertex = edge->firstVertex04;
            secondVertex = vertexData + vertexIndices[nextIndex] * 3;
            edge->secondVertex08 = secondVertex;
            edgeLength = EQGFX_T3D_FLOAT_SQRT((firstVertex[0] - secondVertex[0]) * (firstVertex[0] - secondVertex[0]) +
                (firstVertex[1] - secondVertex[1]) * (firstVertex[1] - secondVertex[1]) +
                (firstVertex[2] - secondVertex[2]) * (firstVertex[2] - secondVertex[2]));
            vertexDataObject = vertexDataObject + EDGE_POLYGON_EDGE_BYTES;
            edgePolygonWords = edgePolygonWords + 1;
            edge->length0c = edgeLength;
            keepGoing = indexObject < edgeCount;
            indexObject = indexObject + 1;
        } while (keepGoing);
    }
    normalResult = _t3dIsNonzeroVector4_call(normal);
    if (normalResult == 0) {
        normalResult = t3dCalculateNormalToEdgePolygon_call(polygon);
        if (normalResult < 0) {
            errorMessage = s_CreateEdgePolygon_normalFailure;
            goto reportError;
        }
    } else {
        polygon->normalX0c = normal[0];
        polygon->normalY10 = normal[1];
        polygon->normalZ14 = normal[2];
        polygon->distance18 = normal[3];
    }
    absNormalX = polygon->normalX0c;
    if (polygon->normalX0c < EQGFX_ZERO_F_GUESS)
        absNormalX = -polygon->normalX0c;
    absNormalY = polygon->normalY10;
    if (polygon->normalY10 < EQGFX_ZERO_F_GUESS)
        absNormalY = -polygon->normalY10;
    absNormalZ = polygon->normalZ14;
    if (polygon->normalZ14 < EQGFX_ZERO_F_GUESS)
        absNormalZ = -polygon->normalZ14;
    if ((absNormalX < absNormalY) || (absNormalX < absNormalZ)) {
        if ((absNormalY < absNormalX) || (absNormalY < absNormalZ))
            polygon->dominantAxis1c = 1;
        else
            polygon->dominantAxis1c = 3;
    } else {
        polygon->dominantAxis1c = 2;
    }
    return 0;

reportError:
    logParserError_call(parser, errorMessage);
    return 1;
}

// FUNCTION: EQGFX 0x1002dff0
// Calculates the facing plane for a 3D sprite polygon in the world.
unsigned int __cdecl _t3dCompute3DSpritePlane_guess(ParserObject_guess *parser, T3DPolygon_guess *polygon)
{
    int *vertexIndices;
    float *vertexData;
    float *firstVertex;
    float *secondVertex;
    float *thirdVertex;
    float firstX;
    float firstY;
    float firstZ;
    float secondToThirdX;
    float secondToThirdY;
    double firstToSecondX;
    double firstToSecondY;
    double firstToSecondZ;
    double secondToThirdZ;
    double normalX;
    double normalY;
    double normalZ;
    double normalLength;
    char *errorMessage;

    if (polygon->vertexCount < 3)
    {
        errorMessage = EQ_ARRAY(char, 0x100A142C);
    }
    else
    {
        vertexIndices = polygon->vertexIndices;
        vertexData = polygon->vertexData;
        firstVertex = vertexData + vertexIndices[0] * 3;
        firstX = firstVertex[0];
        firstY = firstVertex[1];
        firstZ = firstVertex[2];
        secondVertex = vertexData + vertexIndices[1] * 3;
        firstToSecondX = (double)firstX - (double)secondVertex[0];
        firstToSecondY = (double)firstY - (double)secondVertex[1];
        thirdVertex = vertexData + vertexIndices[2] * 3;
        firstToSecondZ = (double)firstZ - (double)secondVertex[2];
        secondToThirdX = thirdVertex[0] - secondVertex[0];
        secondToThirdY = thirdVertex[1] - secondVertex[1];
        secondToThirdZ = (double)thirdVertex[2] - (double)secondVertex[2];
        normalX = firstToSecondY * secondToThirdZ - firstToSecondZ * (double)secondToThirdY;
        normalY = firstToSecondZ * (double)secondToThirdX - firstToSecondX * secondToThirdZ;
        normalZ = firstToSecondX * (double)secondToThirdY - firstToSecondY * (double)secondToThirdX;
        normalLength = FUN_10069984_guess(normalX * normalX + normalY * normalY + normalZ * normalZ);
        if (normalLength != 0.0)
        {
            polygon->normalX = (float)(normalX / normalLength);
            polygon->normalY = (float)(normalY / normalLength);
            polygon->normalZ = (float)(normalZ / normalLength);
            polygon->planeConstant = (float)-((double)polygon->normalX * (double)firstX +
                                               (double)polygon->normalY * (double)firstY +
                                               (normalZ / normalLength) * (double)firstZ);
            return 0;
        }
        errorMessage = EQ_ARRAY(char, 0x100A13FC);
    }
    EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, errorMessage);
    return 1;
}

// FUNCTION: EQGFX 0x1002e129
// Divides the six u/v floats by the linked dims width/height, then scales the pair list by them.
// __cdecl (callee pops 0 bytes). Normalizes the six floats by the display
// width/height and rescales the pair list back into pixels.
void __cdecl _t3dScaleSpriteCoordinates_guess(GfxObj129_guess *gfxObj, int pairCount)
{
    float width;
    float height;
    int index;

    if (gfxObj->root == 0)
        return;
    index = 0;
    width = (float)gfxObj->root->mid->link->slot->dims->width;
    gfxObj->u0 /= width;
    gfxObj->u1 /= width;
    gfxObj->u2 /= width;
    height = (float)gfxObj->root->mid->link->slot->dims->height;
    gfxObj->v0 /= height;
    gfxObj->v1 /= height;
    gfxObj->v2 /= height;
    if (pairCount > 0)
    {
        do
        {
            gfxObj->pairs[index * 2] *= width;
            gfxObj->pairs[index * 2 + 1] *= height;
            index++;
        } while (index < pairCount);
    }
}

// FUNCTION: EQGFX 0x1002E1C9
// __cdecl (callee pops 0 bytes). Returns 0, or logs a PostActorCreate error and returns 1.
int __cdecl _t3dPostActorCreate_guess(ParserObject_guess *parser, void *actor, unsigned int unusedArg, int appendToList, int action)
{
    void *world;

    world = parser->world;
    if (EQ_CDECL(int, 0x1000A72E, (void *, int))(actor, action) != 0)
    {
        EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, EQ_ARRAY(char, 0x100A1498));
        return 1;
    }
    if (appendToList == 0 || EQ_CDECL(int, 0x10008F98, (void *, void *))(world, actor) >= 0)
        return 0;
    EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, char *))(parser, EQ_ARRAY(char, 0x100A145C));
    return 1;
}

// FUNCTION: EQGFX 0x1002E218
// __cdecl (callee pops 0 bytes). In-place transpose of countY-by-countX 0x34-byte elements.
void __cdecl _t3dTransposeElementGrid_guess(TransElem_guess *items, int countX, int countY)
{
    int total;
    TransElem_guess *tempBase;
    TransElem_guess *srcRow;
    TransElem_guess *tempRow;
    TransElem_guess *srcCur;
    TransElem_guess *dstCur;
    TransElem_guess *dst;
    int outerLeft;
    int innerLeft;
    int left;

    total = countX * countY;
    tempBase = (TransElem_guess *)_alloca(total * 0x34);
    if (countY > 0)
    {
        srcRow = items;
        tempRow = tempBase;
        outerLeft = countY;
        do
        {
            if (countX > 0)
            {
                srcCur = srcRow;
                dstCur = tempRow;
                innerLeft = countX;
                do
                {
                    innerLeft--;
                    *dstCur = *srcCur;
                    dstCur += countY;
                    srcCur++;
                } while (innerLeft != 0);
            }
            srcRow += countX;
            tempRow++;
            outerLeft--;
        } while (outerLeft != 0);
    }
    if (total > 0)
    {
        dst = items;
        left = total;
        do
        {
            *dst = *(tempBase + (dst - items));
            dst++;
            left--;
        } while (left != 0);
    }
}

// FUNCTION: EQGFX 0x1002E2B2
// Releases temporary records accumulated while parsing a world.
void __cdecl _t3dFreeObjectLists_guess(FUN_1002dce0_object_guess *firstObject,
    FUN_1002dce0_object_guess *object)
{
    unsigned int start;
    int count;
    unsigned int end;
    FUN_1002e2b2_entry_guess *entry;

    start = FUN_1002dd0f_call((int)object);
    count = EQ_CDECL(int, 0x1002DD6F, (int))((int)object);
    end = count * 0x20 + start;
    while (start < end)
    {
        end -= 0x20;
        entry = (FUN_1002e2b2_entry_guess *)end;
        EQGFX_T3D_FREE_MEMORY_GUESS(entry->allocation08);
    }
    parseListFree(object);
    parseListFree(firstObject);
    return;
}

// FUNCTION: EQGFX 0x1002E2FC
// __cdecl. Sets the world's default palette file name; on failure logs it on the parser and returns 1, else 0.
int __cdecl _t3dSetDefaultPaletteFile_guess(ParserObject_guess *parser, char *paletteFileName)
{
    void *world;
    int result;

    world = parser->world;
    result = EQ_CDECL(int, 0x1001E1E9, (void *, char *))(world, paletteFileName);
    if (result != 0)
    {
        EQ_CDECL(void, 0x1002902B, (ParserObject_guess *, const char *, ...))
            (parser, (const char *)0x100a14cc, paletteFileName);
        return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1002E32E
// Stores a named polyhedron definition in the world for later geometry creation.
// __cdecl. Defines polyhedron polyhedronName in the parser's world; on failure logs it on the parser, returns 1, else 0.
int __cdecl _t3dDefinePolyhedron_guess(ParserObject_guess *parser, char *polyhedronName, float definitionValue, float entryValue,
                         int dataA, int dataB, int dataC, int dataD)
{
    void *world;
    int result;

    world = parser->world;
    result = EQ_CDECL(int, 0x100188CD, (void *, char *, float, int, int, int, int, float))(
        world, polyhedronName, definitionValue, dataC, dataD, dataA, dataB, entryValue);
    if (result == 0)
    {
        logParserError_call(parser, (char *)0x100a14f0, polyhedronName);
        return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1002E37C
// Creates world geometry from a named polyhedron definition.
// __cdecl. Creates polyhedron definitionName into *polyhedronOut, then sets its tag and bounding radius if given; on failure logs it on the parser, returns 1, else 0.
int __cdecl _t3dCreatePolyhedronFromDefinition_guess(ParserObject_guess *parser, void **polyhedronOut, char *definitionName, char *polyhedronTag,
                         float boundingRadius)
{
    void *world;
    void *definition;
    void *polyhedron;

    world = parser->world;
    definition = t3dGetPointerFromDictionary_call(world, (unsigned int)definitionName);
    if (definition == 0)
    {
        logParserError_call(parser, (char *)0x100a156c, definitionName);
    }
    else
    {
        polyhedron = t3dCreatePolyhedron_call(world, definition);
        *polyhedronOut = polyhedron;
        if (polyhedron == 0)
        {
            logParserError_call(parser, (char *)0x100a1550);
        }
        else if (polyhedronTag == 0 || t3dSetPolyhedronTag_call(world, polyhedron, polyhedronTag) == 0)
        {
            if (0.0f == boundingRadius ||
                t3dSetPolyhedronDefinitionBoundingRadius_call(*polyhedronOut, boundingRadius) == 0)
            {
                return 0;
            }
            logParserError_call(parser, (char *)0x100a1510);
        }
        else
        {
            logParserError_call(parser, (char *)0x100a1534);
        }
    }
    return 1;
}

// FUNCTION: EQGFX 0x1002E417
// __cdecl. Defines a hierarchical sprite in the parser's world from the other arguments; returns 0.
int __cdecl _t3dForwardWorldArguments_guess(ParserObject_guess *parser, unsigned int spriteTag, int dataA, int dataB, int dataC, int dataD,
                         float dataFloat, int dataE, int dataF, int dataG)
{
    EQ_CDECL(int, 0x1001244F, (void *, unsigned int, int, int, int, int, float, int, int, int))(
        parser->world, spriteTag, dataA, dataB, dataC, dataD, dataFloat, dataE, dataF, dataG);
    return 0;
}

// FUNCTION: EQGFX 0x1002E44A
// Adds a named sphere-list definition to the world for later use.
// __cdecl. Defines sphere list sphereListName in the parser's world; on failure logs it on the parser, returns 1, else 0.
int __cdecl _t3dDefineSphereList_guess(ParserObject_guess *parser, char *sphereListName, float definitionValue, float entryValue,
                         int count, void *centerData, void *radiusData)
{
    void *definition;

    definition = EQ_CDECL(void *, 0x1001AD5C, (void *, char *, float, int, void *, void *, float))(
        parser->world, sphereListName, definitionValue, count, centerData, radiusData, entryValue);
    if (definition == 0)
    {
        logParserError_call(parser, (char *)0x100a1590, sphereListName);
        return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1002E495
// Instantiates a sphere list in the world from its named definition.
// __cdecl. Creates sphere list sphereListName, sets its tag and bounding radius when given; on failure logs it on the parser, returns 1, else 0.
int __cdecl _t3dCreateSphereListFromDefinition_guess(ParserObject_guess *parser, void **sphereListOut, char *sphereListName, char *sphereListTag,
                         float boundingRadius)
{
    void *world;
    void *definition;
    void *sphereList;
    char *message;

    world = parser->world;
    definition = t3dGetPointerFromDictionary_call(world, (unsigned int)sphereListName);
    if (definition == 0)
    {
        logParserError_call(parser, s_CreateSphereList_noDefinition, sphereListName);
    }
    else
    {
        sphereList = t3dCreateSphereList_call(world, definition);
        *sphereListOut = sphereList;
        if (sphereList == 0)
        {
            message = s_CreateSphereList_createFailure;
        }
        else if (sphereListTag == 0 || t3dSetPolyhedronTag_call(world, sphereList, sphereListTag) == 0)
        {
            if (boundingRadius == 0.0f ||
                t3dSetPolyhedronDefinitionBoundingRadius_call(*sphereListOut, boundingRadius) == 0)
                return 0;
            message = s_CreateSphereList_scaleFailure;
        }
        else
        {
            message = s_CreateSphereList_tagFailure;
        }
        logParserError_call(parser, message);
    }
    return 1;
}

// FUNCTION: EQGFX 0x1002E530
// __cdecl. Adds region to the parser's world; on failure logs it on the parser and returns 1, else 0.
int __cdecl parAddRegionToWorld(ParserObject_guess *parser, Region_guess *region)
{
    void *world;
    char *message;
    int status;

    world = parser->world;
    if (_t3dAddItemToList_call(world, region) != 0)
    {
        message = s_AddRegion_listFailure;
    }
    else if (t3dGetPointerFromDictionary_call(world, region->tag08) != 0)
    {
        message = s_AddRegion_tagInUse;
    }
    else
    {
        status = t3dAddEntryToDictionary_call(world, region);
        if (status >= 0)
            return 0;
        message = s_AddRegion_dictionaryFailure;
    }
    logParserError_call(parser, message);
    return 1;
}

// FUNCTION: EQGFX 0x1002E58C
// Sums per-axis max squared offsets of vector +/- extents; returns FUN_10069984 of the sum, used as a radius.
double __cdecl _t3dCalculateRadius_guess(float *vector, float *secondVec, float scale)
{
    float firstPairMaxSquare;
    float secondPairMaxSquare;
    float thirdPairMaxSquare;
    float firstVecThirdSquare;
    float baseSum;
    float totalSum;
    float endpointSquare;

    firstPairMaxSquare = (scale + vector[0]) * (scale + vector[0]);
    secondPairMaxSquare = (vector[1] + secondVec[0]) * (vector[1] + secondVec[0]);
    thirdPairMaxSquare = (secondVec[1] + vector[2]) * (secondVec[1] + vector[2]);
    endpointSquare = (vector[0] - scale) * (vector[0] - scale);
    if (endpointSquare > firstPairMaxSquare)
        firstPairMaxSquare = endpointSquare;
    endpointSquare = (vector[1] - secondVec[0]) * (vector[1] - secondVec[0]);
    if (endpointSquare > secondPairMaxSquare)
        secondPairMaxSquare = endpointSquare;
    endpointSquare = (vector[2] - secondVec[1]) * (vector[2] - secondVec[1]);
    if (endpointSquare > thirdPairMaxSquare)
        thirdPairMaxSquare = endpointSquare;
    firstVecThirdSquare = vector[2] * vector[2];
    baseSum = vector[0] * vector[0] + thirdPairMaxSquare;
    totalSum = firstVecThirdSquare + firstPairMaxSquare;
    if (totalSum > baseSum)
        totalSum = firstVecThirdSquare + secondPairMaxSquare + firstPairMaxSquare;
    else
        totalSum = baseSum + secondPairMaxSquare;
    return FUN_10069984_guess((double)totalSum);
}

// FUNCTION: EQGFX 0x1002e649
// __cdecl (callee pops 0 bytes). Returns 0 when all four floats are zero, else 1.
int __cdecl _t3dIsNonzeroVector4_guess(float *vector)
{
    if (vector[0] == 0.0f && vector[1] == 0.0f && vector[2] == 0.0f && vector[3] == 0.0f)
        return 0;
    return 1;
}

// FUNCTION: EQGFX 0x1002e67b
// __cdecl (callee pops 0 bytes). Stores two dwords at +0x24 and +0x28.
void __cdecl setObjectPosition_guess(int graphicsObject, unsigned int posX, unsigned int posY)
{
    *(unsigned int *)(graphicsObject + 0x24) = posX;
    *(unsigned int *)(graphicsObject + 0x28) = posY;
}

// FUNCTION: EQGFX 0x1002e68e
// Includes actor placements in an environment save when actor objects are requested.
// __cdecl (callee pops 0 bytes). Walks kindCount entries of kindList; each entry of kind 0x18 saves all actor instances, and the first nonzero t3dSaveAllActorInstances result is returned, else 0.
int __cdecl t3dSaveEnvironmentFromList(FILE *writer, ActorList_guess *actorList, int *kindList, unsigned short kindCount)
{
    int *kindEntry;
    unsigned short kindIndex;
    int result;

    kindEntry = kindList;
    result = 0;
    kindIndex = 0;
    if (kindCount > 0)
    {
        do
        {
            if (*kindEntry == ACTOR_OBJECT_KIND && (result = t3dSaveAllActorInstances_call(writer, actorList)) != 0)
                return result;
            kindEntry++;
            kindIndex++;
        } while (kindIndex < kindCount);
    }
    return result;
}

// FUNCTION: EQGFX 0x1002e6c4
// Preserves placed actor instances that are separate from their definitions in the environment save.
// __cdecl (callee pops 0 bytes). Saves each actor (kind 0x18) whose definition's primary instance is not itself; returns the first nonzero t3dSaveActorInstance_guess result, else 0.
int __cdecl t3dSaveAllActorInstances(FILE *writer, ActorList_guess *actorList)
{
    ActorSave_guess saveState;
    ActorInstance_guess **actorEntry;
    unsigned int actorIndex;
    ActorInstance_guess *actor;
    int result;

    saveState.list00 = actorList;
    t3dSetActorSaveStream_call(&saveState, writer);
    _t3dWriteActorSaveBytes_call(&saveState);
    actorEntry = actorList->actors17c;
    for (actorIndex = 0; actorIndex < actorList->actorCount170; actorIndex++)
    {
        actor = *actorEntry;
        if (actor->kind00 == ACTOR_OBJECT_KIND && actor != actor->definition64->primaryInstance28)
        {
            result = t3dSaveActorInstance_call(&saveState, actor);
            if (result != 0)
                return result;
        }
        actorEntry++;
    }
    t3dWriteMarker3_call(&saveState);
    return 0;
}

// FUNCTION: EQGFX 0x1002e737
// __cdecl (callee pops 0 bytes). Returns 0xffff3d3e if the user-data buffer cannot be allocated.
unsigned int __cdecl t3dSaveActorInstance_guess(void *writer, T3DObject_guess *actor)
{
    ActorMessage_guess message;
    volatile int sequence;
    float location[6];
    char tag[32];
    unsigned int saveContext;
    T3DObject_guess *definition;
    unsigned int tagLength;
    int actionIndex;
    void *userData;
    void *buffer;

    sequence++;
    saveContext = *(unsigned int *)writer;
    t3dWriteVarInt_guess(writer, 0xaa);
    definition = t3dGetActorDefinition_call(actor);
    t3dGet2DSpriteDefinitionTag_guess(definition, tag);
    t3dWriteVarInt_guess(writer, 0x2d);
    tagLength = t3dGet2DSpriteDefinitionTagLength_guess(definition);
    t3dWriteTag4Data_guess(writer, tag, tagLength);
    t3dGet2DSpriteDefinitionTag_guess(actor, tag);
    if (tag[0] != '\0')
    {
        t3dWriteVarInt_guess(writer, 0x5c);
        tagLength = t3dGet2DSpriteDefinitionTagLength_guess(actor);
        t3dWriteTag4Data_guess(writer, tag, tagLength);
    }
    if (t3dIsActorAIActive_call(actor) != 0)
    {
        t3dWriteVarInt_guess(writer, 0xa6);
    }
    t3dWriteVarInt_guess(writer, 0x2a);
    actionIndex = t3dGetActorCurrentAction_call(actor);
    t3dWriteTaggedInteger_call(writer, actionIndex + 1);
    t3dWriteVarInt_guess(writer, 0x37);
    t3dGetActorLocation_call(actor, location);
    t3dWriteTaggedInteger_call(writer, sequence);
    t3dWriteTaggedFloat_guess(writer, location[0]);
    t3dWriteTaggedFloat_guess(writer, location[1]);
    t3dWriteTaggedFloat_guess(writer, location[2]);
    t3dWriteTaggedInteger_call(writer, (int)location[3]);
    t3dWriteTaggedInteger_call(writer, (int)location[4]);
    t3dWriteTaggedInteger_call(writer, (int)location[5]);
    t3dSaveActorVolume_call(writer, s3dGetDMSpriteCollisionVolume_call(actor));
    t3dSaveActorVolume_call(writer, t3dGetActorRenderVolume_call(actor));
    userData = t3dGetActorUserData_call(actor);
    if (userData != 0)
    {
        message.buffer04 = 0;
        message.messageType00 = 0x42;
        message.size08 = 0x2000;
        t3dSendMessageToActor_call(saveContext, actor, &message);
        if (message.size08 != 0)
        {
            buffer = t3dAllocateMemory_call(message.size08);
            if (buffer == 0)
                return 0xffff3d3e;
            message.size08 = 0;
            message.messageType00 = 0x43;
            message.buffer04 = buffer;
            t3dSendMessageToActor_call(saveContext, actor, &message);
            if (message.size08 != 0)
            {
                t3dWriteSizeTaggedData_call(writer, buffer, message.size08);
            }
            EQGFX_T3D_FREE_MEMORY_GUESS(buffer);
        }
    }
    t3dWriteVarInt_guess(writer, 0xbf);
    return 0;
}

// FUNCTION: EQGFX 0x1002e907
// Preserves an actor's collision and render volume data in the world save.
void __cdecl t3dSaveActorVolume_guess(void *writer, T3DObject_guess *actor)
{
    int kind;
    char tag[32];

    kind = actor->kind00;
    if (kind == 2)
    {
        t3dWriteVarInt_guess(writer, 0xf7);
        t3dWriteVarInt_guess(writer, 0x2d);
        t3dGet2DSpriteDefinitionTag_guess((T3DObject_guess *)actor->value18, tag);
        t3dWriteTag4Data_guess(writer, tag, t3dGet2DSpriteDefinitionTagLength_guess((T3DObject_guess *)actor->value18));
        t3dGet2DSpriteDefinitionTag_guess(actor, tag);
        if (tag[0] != '\0')
        {
            t3dWriteVarInt_guess(writer, 0x5c);
            t3dWriteTag4Data_guess(writer, tag, t3dGet2DSpriteDefinitionTagLength_guess(actor));
        }
        t3dWriteVarInt_guess(writer, 0x57);
        t3dWriteTaggedFloat_guess(writer, t3dGetActorWorldY_guess(actor));
        t3dWriteVarInt_guess(writer, 0xcd);
    }
    else
    {
        if (kind != 4)
        {
            if (kind == 0x2a)
            {
                t3dWriteVarInt_guess(writer, 0x5a);
            }
            else
            {
                if (kind != 0x2c)
                    return;
                t3dWriteVarInt_guess(writer, 0x106);
                t3dWriteTaggedFloat_guess(writer, *(float *)&actor->value0c);
            }
            t3dWriteTaggedFloat_guess(writer, *(float *)&actor->value08);
            return;
        }
        t3dWriteVarInt_guess(writer, 0x11f);
        t3dWriteVarInt_guess(writer, 0x2d);
        t3dGet2DSpriteDefinitionTag_guess((T3DObject_guess *)actor->value18, tag);
        t3dWriteTag4Data_guess(writer, tag, t3dGet2DSpriteDefinitionTagLength_guess((T3DObject_guess *)actor->value18));
        t3dGet2DSpriteDefinitionTag_guess(actor, tag);
        if (tag[0] != '\0')
        {
            t3dWriteVarInt_guess(writer, 0x5c);
            t3dWriteTag4Data_guess(writer, tag, t3dGet2DSpriteDefinitionTagLength_guess(actor));
        }
        t3dWriteVarInt_guess(writer, 0x57);
        t3dWriteTaggedFloat_guess(writer, t3dGetActorWorldY_guess(actor));
        t3dWriteVarInt_guess(writer, 0x120);
    }
    return;
}

// FUNCTION: EQGFX 0x1002ea85
// __cdecl (callee pops 0 bytes). Stores the stream at +0x04 and its position at +0x08.
int __cdecl t3dSetActorSaveStream_guess(ActorSave_guess *state, FILE *stream)
{
    state->stream = stream;
    state->streamPos = ftell_guess(stream);
    return 0;
}

// FUNCTION: EQGFX 0x1002ea9f
// Writes the fixed header for the actor-instance section of an environment save.
void __cdecl _t3dWriteActorSaveBytes_guess(ActorSave_guess *state)
{
    t3dWriteBytes_guess(state, &g_binaryFileSignature_guess, 6);
    return;
}
