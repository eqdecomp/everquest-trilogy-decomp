#include "verify_shared.h"

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004dd808
// Creates shared storage for a sized byte buffer used by resources.
PackFSData_guess::PackFSData_guess(unsigned int len)
{
    void *data;
    PackFSShared_guess *shared;
    DWORD idx;
    LPCSTR file;
    LPCSTR msg;

    shared_guess = 0;
    if (len != 0)
    {
        data = new unsigned char[len];
        idx = EQ_GLOBAL(DWORD, 0x006F035C);
        file = (LPCSTR)0x00570990;
        if (idx < 0x40)
        {
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].file_guess = file;
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].line_guess = 0x1b0;
            EQ_GLOBAL(DWORD, 0x006F035C) = idx + 1;
        }
        msg = (LPCSTR)0x00570968;
        if (data == 0)
        {
            EQ_CDECL(void, 0x004F8310, (void *, LPCSTR, LPCSTR))(data, msg, (LPCSTR)0x00570958);
        }

        shared = new PackFSShared_guess(data, len);
        idx = EQ_GLOBAL(DWORD, 0x006F035C);
        if (idx < 0x40)
        {
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].file_guess = file;
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].line_guess = 0x1b2;
            EQ_GLOBAL(DWORD, 0x006F035C) = idx + 1;
        }
        if (shared == 0)
        {
            EQ_CDECL(void, 0x004F8310, (void *, LPCSTR, LPCSTR))(shared, msg, (LPCSTR)0x00570940);
        }
        shared_guess = shared;
        shared->refs_guess++;
    }
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004dde87
// Loads the graphics DLL named by the argument and binds its exports into global function pointers.
// Steps: init 0x004dd9a0, LoadLibraryA into 0x006e547c, resolve the version export (0x006e50a4) and call it, log the
// reported version and set the match flag at 0x006e5480 from it.
// EQGFX_RESOLVE then resolves the export table: a block of 16 only when the flag is 1, then the common set.
// Returns 0 on success; 0xfffffffe if no version is reported; 0xffffffff on load or missing-export failure.
int Init_EQGfx_Dx8_guess(LPCSTR graphicsDllName)
{
    DWORD lastError;
    char *errorText;
    char *versionMatch;

    EQ_CDECL(void, 0x004DD9A0, (void))();
    EQ_GLOBAL(HMODULE, 0x006E547C) = LoadLibraryA(graphicsDllName);
    if (EQ_GLOBAL(HMODULE, 0x006E547C) == NULL)
    {
        goto load_failed;
    }

    EQ_GLOBAL(FARPROC, 0x006E50A4) = GetProcAddress(
        EQ_GLOBAL(HMODULE, 0x006E547C), (LPCSTR)0x00575f94);
    if (EQ_GLOBAL(FARPROC, 0x006E50A4) == NULL)
    {
        errorText = (char *)0x00575f5c;
        goto missing_export;
    }

    ((void (__cdecl *)(int *, char *))EQ_GLOBAL(FARPROC, 0x006E50A4))(
        (int *)0x006e50a0, (char *)0x006e4ea0);
    if (EQ_GLOBAL(int, 0x006E50A0) == 0)
    {
        EQ_CDECL(void, 0x004F8220, (const char *, ...))(
            (const char *)0x00575f38, graphicsDllName);
        return 0xfffffffe;
    }

    EQ_CDECL(void, 0x004F8220, (const char *, ...))(
        (const char *)0x00575f28, graphicsDllName, (char *)0x006e4ea0);
    versionMatch = EQ_CDECL(char *, 0x00522A30, (const char *, const char *))(
        (const char *)0x006e4ea0, (const char *)0x00575f24);
    if (versionMatch == NULL)
    {
        EQ_GLOBAL(int, 0x006E5480) = 0;
    }
    else
    {
        EQ_GLOBAL(int, 0x006E5480) = 1;
        EQ_CDECL(void, 0x004F8220, (const char *, ...))(
            (const char *)0x00575f04);
    }

#define EQGFX_RESOLVE(slotAddress, nameAddress, errorAddress) \
    do \
    { \
        EQ_GLOBAL(FARPROC, slotAddress) = GetProcAddress( \
            EQ_GLOBAL(HMODULE, 0x006E547C), (LPCSTR)(nameAddress)); \
        if (EQ_GLOBAL(FARPROC, slotAddress) == NULL) \
        { \
            errorText = (char *)(errorAddress); \
            goto missing_export; \
        } \
    } while (0)

    if (EQ_GLOBAL(int, 0x006E5480) == 1)
    {
        EQGFX_RESOLVE(0x006E50A8, 0x00575eec, 0x00575eb0);
        EQGFX_RESOLVE(0x006E51EC, 0x00575e94, 0x00575e50);
        EQGFX_RESOLVE(0x006E51F0, 0x00575e38, 0x00575df8);
        EQGFX_RESOLVE(0x006E51F4, 0x00575de0, 0x00575da0);
        EQGFX_RESOLVE(0x006E51BC, 0x00575d90, 0x00575d5c);
        EQGFX_RESOLVE(0x006E51C0, 0x00575d4c, 0x00575d18);
        EQGFX_RESOLVE(0x006E51C4, 0x00575d08, 0x00575cd0);
        EQGFX_RESOLVE(0x006E51C8, 0x00575cc0, 0x00575c8c);
        EQGFX_RESOLVE(0x006E51CC, 0x00575c7c, 0x00575c48);
        EQGFX_RESOLVE(0x006E51D0, 0x00575c38, 0x00575c04);
        EQGFX_RESOLVE(0x006E51D4, 0x00575bf0, 0x00575bb8);
        EQGFX_RESOLVE(0x006E51D8, 0x00575ba8, 0x00575b74);
        EQGFX_RESOLVE(0x006E51DC, 0x00575b64, 0x00575b2c);
        EQGFX_RESOLVE(0x006E51E0, 0x00575b1c, 0x00575ae8);
        EQGFX_RESOLVE(0x006E51E4, 0x00575ad8, 0x00575aa4);
        EQGFX_RESOLVE(0x006E51E8, 0x00575a8c, 0x00575a4c);
    }

    EQGFX_RESOLVE(0x006E51B8, 0x00575a3c, 0x00575a08);
    EQGFX_RESOLVE(0x006E50AC, 0x005759dc, 0x0057598c);
    EQGFX_RESOLVE(0x006E50B0, 0x00575974, 0x00575934);
    EQGFX_RESOLVE(0x006E50B4, 0x00575924, 0x005758f0);
    EQGFX_RESOLVE(0x006E50B8, 0x005758e0, 0x005758ac);
    EQGFX_RESOLVE(0x006E50BC, 0x00575888, 0x00575840);
    EQGFX_RESOLVE(0x006E50C0, 0x00575824, 0x005757e4);
    EQGFX_RESOLVE(0x006E50C4, 0x005757b8, 0x00575764);
    EQGFX_RESOLVE(0x006E50C8, 0x00575748, 0x00575708);
    EQGFX_RESOLVE(0x006E50CC, 0x005756f4, 0x005756bc);
    EQGFX_RESOLVE(0x006E50D0, 0x005756a4, 0x00575668);
    EQGFX_RESOLVE(0x006E50D4, 0x00575650, 0x00575614);
    EQGFX_RESOLVE(0x006E50D8, 0x00575600, 0x005755c4);
    EQGFX_RESOLVE(0x006E50DC, 0x005755ac, 0x00575570);
    EQGFX_RESOLVE(0x006E50E0, 0x00575558, 0x00575518);
    EQGFX_RESOLVE(0x006E50E4, 0x005754f4, 0x005754a8);
    EQGFX_RESOLVE(0x006E50E8, 0x0057548c, 0x0057544c);
    EQGFX_RESOLVE(0x006E50EC, 0x00575434, 0x005753f8);
    EQGFX_RESOLVE(0x006E50F0, 0x005753dc, 0x00575398);
    EQGFX_RESOLVE(0x006E50F4, 0x0057537c, 0x0057533c);
    EQGFX_RESOLVE(0x006E50F8, 0x00575320, 0x005752e0);
    EQGFX_RESOLVE(0x006E50FC, 0x005752c4, 0x00575284);
    EQGFX_RESOLVE(0x006E5100, 0x00575264, 0x00575220);
    EQGFX_RESOLVE(0x006E5104, 0x00575204, 0x005751c0);
    EQGFX_RESOLVE(0x006E5108, 0x005751a0, 0x00575158);
    EQGFX_RESOLVE(0x006E510C, 0x0057513c, 0x005750fc);
    EQGFX_RESOLVE(0x006E5110, 0x005750d0, 0x0057507c);
    EQGFX_RESOLVE(0x006E5114, 0x00575054, 0x00575004);
    EQGFX_RESOLVE(0x006E5118, 0x00574fc8, 0x00574f64);
    EQGFX_RESOLVE(0x006E511C, 0x00574f48, 0x00574f04);
    EQGFX_RESOLVE(0x006E5120, 0x00574eec, 0x00574eb0);
    EQGFX_RESOLVE(0x006E5124, 0x00574e98, 0x00574e5c);
    EQGFX_RESOLVE(0x006E5128, 0x00574e40, 0x00574dfc);
    EQGFX_RESOLVE(0x006E512C, 0x00574de4, 0x00574da8);
    EQGFX_RESOLVE(0x006E5130, 0x00574d88, 0x00574d44);
    EQGFX_RESOLVE(0x006E5134, 0x00574d24, 0x00574ce0);
    EQGFX_RESOLVE(0x006E5138, 0x00574ccc, 0x00574c94);
    EQGFX_RESOLVE(0x006E513C, 0x00574c78, 0x00574c34);
    EQGFX_RESOLVE(0x006E5140, 0x00574c14, 0x00574bcc);
    EQGFX_RESOLVE(0x006E5144, 0x00574bb4, 0x00574b78);
    EQGFX_RESOLVE(0x006E5148, 0x00574b54, 0x00574b0c);
    EQGFX_RESOLVE(0x006E514C, 0x00574af4, 0x00574ab4);
    EQGFX_RESOLVE(0x006E5150, 0x00574a98, 0x00574a58);
    EQGFX_RESOLVE(0x006E5154, 0x00574a3c, 0x005749fc);
    EQGFX_RESOLVE(0x006E5158, 0x005749dc, 0x00574994);
    EQGFX_RESOLVE(0x006E515C, 0x0057497c, 0x0057493c);
    EQGFX_RESOLVE(0x006E5160, 0x00574920, 0x005748e0);
    EQGFX_RESOLVE(0x006E5164, 0x005748c4, 0x00574880);
    EQGFX_RESOLVE(0x006E5168, 0x00574848, 0x005747ec);
    EQGFX_RESOLVE(0x006E516C, 0x005747d0, 0x00574790);
    EQGFX_RESOLVE(0x006E5170, 0x0057477c, 0x00574744);
    EQGFX_RESOLVE(0x006E5174, 0x0057472c, 0x005746ec);
    EQGFX_RESOLVE(0x006E5178, 0x005746d4, 0x00574698);
    EQGFX_RESOLVE(0x006E517C, 0x00574684, 0x0057464c);
    EQGFX_RESOLVE(0x006E5180, 0x00574638, 0x00574600);
    EQGFX_RESOLVE(0x006E5184, 0x005745f0, 0x005745b8);
    EQGFX_RESOLVE(0x006E5188, 0x0057459c, 0x0057455c);
    EQGFX_RESOLVE(0x006E518C, 0x00574540, 0x005744fc);
    EQGFX_RESOLVE(0x006E5190, 0x005744ec, 0x005744b4);
    EQGFX_RESOLVE(0x006E5194, 0x005744a0, 0x00574468);
    EQGFX_RESOLVE(0x006E5198, 0x00574450, 0x00574410);
    EQGFX_RESOLVE(0x006E519C, 0x005743f8, 0x005743bc);
    EQGFX_RESOLVE(0x006E51A0, 0x005743a4, 0x00574364);
    EQGFX_RESOLVE(0x006E51A4, 0x00574354, 0x0057431c);
    EQGFX_RESOLVE(0x006E51A8, 0x0057430c, 0x005742d4);
    EQGFX_RESOLVE(0x006E51AC, 0x005742bc, 0x00574280);
    EQGFX_RESOLVE(0x006E51B0, 0x00574268, 0x0057422c);
    EQGFX_RESOLVE(0x006E51B4, 0x0057421c, 0x005741e4);
    EQGFX_RESOLVE(0x006E51F8, 0x005741d8, 0x005741a4);
    EQGFX_RESOLVE(0x006E51FC, 0x00574194, 0x0057415c);
    EQGFX_RESOLVE(0x006E5200, 0x0057414c, 0x00574114);
    EQGFX_RESOLVE(0x006E5204, 0x00574100, 0x005740c4);
    EQGFX_RESOLVE(0x006E5208, 0x005740b4, 0x0057407c);
    EQGFX_RESOLVE(0x006E520C, 0x00574068, 0x00574030);
    EQGFX_RESOLVE(0x006E5210, 0x00574010, 0x00573fcc);
    EQGFX_RESOLVE(0x006E5214, 0x00573fb8, 0x00573f80);
    EQGFX_RESOLVE(0x006E5218, 0x00573f68, 0x00573f28);
    EQGFX_RESOLVE(0x006E521C, 0x00573f10, 0x00573ed4);
    EQGFX_RESOLVE(0x006E5220, 0x00573ebc, 0x00573e7c);
    EQGFX_RESOLVE(0x006E5224, 0x00573e68, 0x00573e30);
    EQGFX_RESOLVE(0x006E5228, 0x00573e1c, 0x00573de4);
    EQGFX_RESOLVE(0x006E522C, 0x00573dd4, 0x00573d9c);
    EQGFX_RESOLVE(0x006E5230, 0x00573d80, 0x00573d3c);
    EQGFX_RESOLVE(0x006E5234, 0x00573d1c, 0x00573cd4);
    EQGFX_RESOLVE(0x006E5238, 0x00573cbc, 0x00573c7c);
    EQGFX_RESOLVE(0x006E523C, 0x00573c6c, 0x00573c34);
    EQGFX_RESOLVE(0x006E5240, 0x00573c20, 0x00573be8);
    EQGFX_RESOLVE(0x006E5244, 0x00573bd8, 0x00573ba0);
    EQGFX_RESOLVE(0x006E5248, 0x00573b90, 0x00573b58);
    EQGFX_RESOLVE(0x006E524C, 0x00573b44, 0x00573b08);
    EQGFX_RESOLVE(0x006E5250, 0x00573af4, 0x00573abc);
    EQGFX_RESOLVE(0x006E5254, 0x00573aac, 0x00573a78);
    EQGFX_RESOLVE(0x006E5258, 0x00573a68, 0x00573a34);
    EQGFX_RESOLVE(0x006E525C, 0x00573a24, 0x005739f0);
    EQGFX_RESOLVE(0x006E5260, 0x005739e0, 0x005739ac);
    EQGFX_RESOLVE(0x006E5264, 0x0057399c, 0x00573968);
    EQGFX_RESOLVE(0x006E5268, 0x00573950, 0x00573914);
    EQGFX_RESOLVE(0x006E526C, 0x005738f8, 0x005738b4);
    EQGFX_RESOLVE(0x006E5270, 0x00573894, 0x0057384c);
    EQGFX_RESOLVE(0x006E5274, 0x00573834, 0x005737f8);
    EQGFX_RESOLVE(0x006E5278, 0x005737d0, 0x00573784);
    EQGFX_RESOLVE(0x006E527C, 0x00573764, 0x0057371c);
    EQGFX_RESOLVE(0x006E5280, 0x005736f4, 0x005736a4);
    EQGFX_RESOLVE(0x006E5284, 0x00573690, 0x00573654);
    EQGFX_RESOLVE(0x006E5288, 0x00573640, 0x00573608);
    EQGFX_RESOLVE(0x006E528C, 0x005735f4, 0x005735b8);
    EQGFX_RESOLVE(0x006E5290, 0x005735a0, 0x00573564);
    EQGFX_RESOLVE(0x006E5294, 0x00573544, 0x005734fc);
    EQGFX_RESOLVE(0x006E5298, 0x005734dc, 0x00573498);
    EQGFX_RESOLVE(0x006E529C, 0x00573488, 0x00573450);
    EQGFX_RESOLVE(0x006E52A0, 0x00573440, 0x0057340c);
    EQGFX_RESOLVE(0x006E52A4, 0x005733fc, 0x005733c4);
    EQGFX_RESOLVE(0x006E52A8, 0x005733a4, 0x0057335c);
    EQGFX_RESOLVE(0x006E52AC, 0x00573334, 0x005732e4);
    EQGFX_RESOLVE(0x006E52B0, 0x005732d0, 0x00573294);
    EQGFX_RESOLVE(0x006E52B4, 0x00573278, 0x00573238);
    EQGFX_RESOLVE(0x006E52B8, 0x00573228, 0x005731f0);
    EQGFX_RESOLVE(0x006E52BC, 0x005731e0, 0x005731a8);
    EQGFX_RESOLVE(0x006E52C0, 0x00573180, 0x00573130);
    EQGFX_RESOLVE(0x006E52C4, 0x0057310c, 0x005730c0);
    EQGFX_RESOLVE(0x006E52C8, 0x0057309c, 0x00573054);
    EQGFX_RESOLVE(0x006E52CC, 0x00573030, 0x00572fe4);
    EQGFX_RESOLVE(0x006E52D0, 0x00572fb0, 0x00572f58);
    EQGFX_RESOLVE(0x006E52D4, 0x00572f24, 0x00572ecc);
    EQGFX_RESOLVE(0x006E52D8, 0x00572ea0, 0x00572e50);
    EQGFX_RESOLVE(0x006E52DC, 0x00572e40, 0x00572e08);
    EQGFX_RESOLVE(0x006E52E0, 0x00572dec, 0x00572da8);
    EQGFX_RESOLVE(0x006E52E4, 0x00572d94, 0x00572d58);
    EQGFX_RESOLVE(0x006E52E8, 0x00572d3c, 0x00572cf8);
    EQGFX_RESOLVE(0x006E52EC, 0x00572cd4, 0x00572c8c);
    EQGFX_RESOLVE(0x006E52F0, 0x00572c6c, 0x00572c24);
    EQGFX_RESOLVE(0x006E52F4, 0x00572c10, 0x00572bd4);
    EQGFX_RESOLVE(0x006E52F8, 0x00572bb0, 0x00572b68);
    EQGFX_RESOLVE(0x006E52FC, 0x00572b50, 0x00572b14);
    EQGFX_RESOLVE(0x006E5300, 0x00572af0, 0x00572aa4);
    EQGFX_RESOLVE(0x006E5304, 0x00572a8c, 0x00572a50);
    EQGFX_RESOLVE(0x006E5308, 0x00572a38, 0x005729fc);
    EQGFX_RESOLVE(0x006E530C, 0x005729ec, 0x005729b4);
    EQGFX_RESOLVE(0x006E5310, 0x0057298c, 0x00572940);
    EQGFX_RESOLVE(0x006E5314, 0x0057292c, 0x005728f0);
    EQGFX_RESOLVE(0x006E5318, 0x005728c0, 0x0057286c);
    EQGFX_RESOLVE(0x006E531C, 0x00572858, 0x0057281c);
    EQGFX_RESOLVE(0x006E5320, 0x00572804, 0x005727c8);
    EQGFX_RESOLVE(0x006E5324, 0x005727b0, 0x00572774);
    EQGFX_RESOLVE(0x006E5328, 0x00572768, 0x00572734);
    EQGFX_RESOLVE(0x006E532C, 0x00572720, 0x005726e4);
    EQGFX_RESOLVE(0x006E5330, 0x005726c4, 0x00572680);
    EQGFX_RESOLVE(0x006E5334, 0x00572670, 0x00572638);
    EQGFX_RESOLVE(0x006E5338, 0x00572628, 0x005725f0);
    EQGFX_RESOLVE(0x006E533C, 0x005725dc, 0x005725a0);
    EQGFX_RESOLVE(0x006E5340, 0x0057258c, 0x00572554);
    EQGFX_RESOLVE(0x006E5344, 0x00572540, 0x00572508);
    EQGFX_RESOLVE(0x006E5348, 0x005724f8, 0x005724c0);
    EQGFX_RESOLVE(0x006E534C, 0x005724b0, 0x00572478);
    EQGFX_RESOLVE(0x006E5350, 0x00572464, 0x0057242c);
    EQGFX_RESOLVE(0x006E5354, 0x0057240c, 0x005723c8);
    EQGFX_RESOLVE(0x006E5358, 0x005723b0, 0x00572374);
    EQGFX_RESOLVE(0x006E535C, 0x00572360, 0x00572328);
    EQGFX_RESOLVE(0x006E5360, 0x00572310, 0x005722d4);
    EQGFX_RESOLVE(0x006E5364, 0x005722b8, 0x00572278);
    EQGFX_RESOLVE(0x006E5368, 0x00572264, 0x0057222c);
    EQGFX_RESOLVE(0x006E536C, 0x00572210, 0x005721cc);
    EQGFX_RESOLVE(0x006E5370, 0x005721b0, 0x00572170);
    EQGFX_RESOLVE(0x006E5374, 0x00572154, 0x00572110);
    EQGFX_RESOLVE(0x006E5378, 0x005720f8, 0x005720bc);
    EQGFX_RESOLVE(0x006E537C, 0x005720a8, 0x0057206c);
    EQGFX_RESOLVE(0x006E5380, 0x00572054, 0x00572014);
    EQGFX_RESOLVE(0x006E5384, 0x00572000, 0x00571fc8);
    EQGFX_RESOLVE(0x006E5388, 0x00571fac, 0x00571f68);
    EQGFX_RESOLVE(0x006E538C, 0x00571f54, 0x00571f18);
    EQGFX_RESOLVE(0x006E5390, 0x00571f00, 0x00571ec4);
    EQGFX_RESOLVE(0x006E5394, 0x00571ea8, 0x00571e64);
    EQGFX_RESOLVE(0x006E5398, 0x00571e50, 0x00571e18);
    EQGFX_RESOLVE(0x006E539C, 0x00571e00, 0x00571dc4);
    EQGFX_RESOLVE(0x006E53A0, 0x00571da8, 0x00571d68);
    EQGFX_RESOLVE(0x006E53A4, 0x00571d58, 0x00571d20);
    EQGFX_RESOLVE(0x006E53A8, 0x00571cfc, 0x00571cb0);
    EQGFX_RESOLVE(0x006E53AC, 0x00571c98, 0x00571c5c);
    EQGFX_RESOLVE(0x006E53B0, 0x00571c48, 0x00571c10);
    EQGFX_RESOLVE(0x006E53B4, 0x00571bf4, 0x00571bb0);
    EQGFX_RESOLVE(0x006E53B8, 0x00571b94, 0x00571b54);
    EQGFX_RESOLVE(0x006E53BC, 0x00571b3c, 0x00571afc);
    EQGFX_RESOLVE(0x006E53C0, 0x00571ae4, 0x00571aa8);
    EQGFX_RESOLVE(0x006E53C4, 0x00571a94, 0x00571a5c);
    EQGFX_RESOLVE(0x006E53C8, 0x00571a4c, 0x00571a14);
    EQGFX_RESOLVE(0x006E53CC, 0x00571a04, 0x005719cc);
    EQGFX_RESOLVE(0x006E53D0, 0x005719bc, 0x00571984);
    EQGFX_RESOLVE(0x006E53D4, 0x00571954, 0x005718fc);
    EQGFX_RESOLVE(0x006E53D8, 0x005718dc, 0x00571898);
    EQGFX_RESOLVE(0x006E53DC, 0x00571868, 0x00571810);
    EQGFX_RESOLVE(0x006E53E0, 0x00571804, 0x005717d4);
    EQGFX_RESOLVE(0x006E53E4, 0x005717c0, 0x00571788);
    EQGFX_RESOLVE(0x006E53E8, 0x00571754, 0x005716fc);
    EQGFX_RESOLVE(0x006E53EC, 0x005716d0, 0x00571680);
    EQGFX_RESOLVE(0x006E53F0, 0x00571658, 0x00571608);
    EQGFX_RESOLVE(0x006E53F4, 0x005715f0, 0x005715b0);
    EQGFX_RESOLVE(0x006E53F8, 0x0057159c, 0x00571564);
    EQGFX_RESOLVE(0x006E53FC, 0x00571544, 0x00571500);
    // The original looks up t3dSetPCloudDuration twice into the same slot.
    EQGFX_RESOLVE(0x006E5400, 0x005714e8, 0x005714ac);
    EQGFX_RESOLVE(0x006E5400, 0x005714e8, 0x005714ac);
    EQGFX_RESOLVE(0x006E5404, 0x00571498, 0x0057145c);
    EQGFX_RESOLVE(0x006E5408, 0x00571444, 0x00571404);
    EQGFX_RESOLVE(0x006E540C, 0x005713e8, 0x005713a8);
    EQGFX_RESOLVE(0x006E5410, 0x00571390, 0x00571350);
    EQGFX_RESOLVE(0x006E5414, 0x00571338, 0x005712f8);
    EQGFX_RESOLVE(0x006E5418, 0x005712e0, 0x005712a4);
    EQGFX_RESOLVE(0x006E541C, 0x0057128c, 0x0057124c);
    EQGFX_RESOLVE(0x006E5420, 0x00571230, 0x005711f0);
    EQGFX_RESOLVE(0x006E5424, 0x005711c4, 0x00571174);
    EQGFX_RESOLVE(0x006E5428, 0x00571150, 0x00571108);
    EQGFX_RESOLVE(0x006E542C, 0x005710dc, 0x0057108c);
    EQGFX_RESOLVE(0x006E5430, 0x00571078, 0x0057103c);
    EQGFX_RESOLVE(0x006E5434, 0x0057102c, 0x00570ff4);
    EQGFX_RESOLVE(0x006E5438, 0x00570fd4, 0x00570f90);
    EQGFX_RESOLVE(0x006E543C, 0x00570f74, 0x00570f30);
    EQGFX_RESOLVE(0x006E5440, 0x00570f0c, 0x00570ec4);
    EQGFX_RESOLVE(0x006E5444, 0x00570e98, 0x00570e44);
    EQGFX_RESOLVE(0x006E5448, 0x00570e24, 0x00570de0);
    EQGFX_RESOLVE(0x006E544C, 0x00570dd4, 0x00570da4);
    EQGFX_RESOLVE(0x006E5450, 0x00570d90, 0x00570d54);
    EQGFX_RESOLVE(0x006E5454, 0x00570d30, 0x00570ce4);
    EQGFX_RESOLVE(0x006E5458, 0x00570cc4, 0x00570c7c);
    EQGFX_RESOLVE(0x006E545C, 0x00570c6c, 0x00570c34);
    EQGFX_RESOLVE(0x006E5460, 0x00570c18, 0x00570bd4);
    EQGFX_RESOLVE(0x006E5464, 0x00570bc4, 0x00570b90);
    EQGFX_RESOLVE(0x006E5468, 0x00570b64, 0x00570b14);
    EQGFX_RESOLVE(0x006E546C, 0x00570b00, 0x00570ac8);
    EQGFX_RESOLVE(0x006E5470, 0x00570aac, 0x00570a6c);
    EQGFX_RESOLVE(0x006E5474, 0x00570a58, 0x00570a20);
    EQGFX_RESOLVE(0x006E5478, 0x00570a08, 0x005709c8);

#undef EQGFX_RESOLVE
    return 0;

missing_export:
    EQ_CDECL(void, 0x004F8220, (const char *, ...))(errorText);
    goto return_failure;

load_failed:
    lastError = GetLastError();
    EQ_CDECL(void, 0x004F8220, (const char *, ...))(
        (const char *)0x005709b0, graphicsDllName, lastError);

return_failure:
    return 0xffffffff;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004e6f95
// Registers a command string with its key code in the EQ Commands group.
// @early-stop: string-copy interleaving, size registers, and buffer-store order still differ (best ratio 0.766).
/* fake match: stage the global word so VC6 loads it before copying the command name. */
unsigned int AddVoiceCommand_guess(unsigned int keyCode, char *commandText)
{
    char commandName[12];
    unsigned int resourceName[2];
    size_t stringLength;
    unsigned int resourceNameSize;
    unsigned int param2Size;
    unsigned int commandNameSize;
    unsigned int totalSize;
    unsigned int resourceNamePrefix;
    unsigned int *buffer;
    unsigned int result;
    unsigned int outValue;
    unsigned int *callbackObject;
    unsigned int *vtable;
    typedef int (__stdcall *RegisterKeyBindingCallback_guess)(void *, int, void *, unsigned int, unsigned int *);
    struct Name12_guess
    {
        char text[12];
    };
    resourceNamePrefix = *(unsigned int *)0x0057c5c4;
    *(Name12_guess *)commandName = *(Name12_guess *)0x0057c5b8;
    resourceName[0] = resourceNamePrefix;
    resourceName[1] = *(unsigned int *)0x0057c5c8;
    stringLength = EQ_CDECL(size_t, 0x005223D0, (const char *))(commandText);
    param2Size = (stringLength + 4) & 0xfffffffc;
    stringLength = EQ_CDECL(size_t, 0x005223D0, (const char *))((char *)resourceName);
    resourceNameSize = (stringLength + 4) & 0xfffffffc;
    stringLength = EQ_CDECL(size_t, 0x005223D0, (const char *))(commandName);
    commandNameSize = (stringLength + 4) & 0xfffffffc;
    totalSize = commandNameSize + resourceNameSize + 0x24 + param2Size;
    param2Size = param2Size + 0x24;
    buffer = (unsigned int *)EQ_CDECL(void *, 0x005218C6, (unsigned int))(totalSize);
    buffer[0] = totalSize;
    buffer[2] = keyCode;
    resourceNameSize = resourceNameSize + param2Size;
    buffer[1] = 0;
    buffer[6] = 0;
    buffer[8] = 0;
    buffer[5] = resourceNameSize;
    buffer[3] = 0x24;
    buffer[4] = param2Size;
    buffer[7] = resourceNameSize + commandNameSize;
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)(buffer + 9), commandText);
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)buffer + buffer[4], (char *)resourceName);
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))((char *)buffer + buffer[5], commandName);
    callbackObject = (unsigned int *)*(unsigned int *)0x006ece48;
    outValue = 0;
    result = 0x80004005;
    if (callbackObject != 0)
    {
        vtable = *(unsigned int **)callbackObject;
        result = (unsigned int)((RegisterKeyBindingCallback_guess)vtable[9])(callbackObject, 1, buffer, totalSize, &outValue);
    }
    EQ_CDECL(void, 0x005218BB, (void *))(buffer);
    return result;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004e72e1
// Sets up voice control: registers 54 spoken game commands (Attack, Hail...) and checks the speech engine.
// @early-stop: diagnostic cdecl cleanup scheduling and a few error-path blocks still differ.
/* fake match: model the original contiguous voice stack buffers and fields so VC6 lays out the 0x7bc frame. */
int InitializeVoiceRecognition_guess(int *activateArg)
{
    struct LanguageInfo_guess
    {
        WORD language;
        char name[66];
    };
    struct VoiceLocalFrame_guess
    {
        char requestText[540];
        GUID requestId;
        char requestTail[356];
        char findOutput[540];
        char returnedId[16];
        char engineText[356];
        LanguageInfo_guess languageInfo;
        char menuName[32];
        char commandName[32];
        GUID engineId;
        int engineMatched;
    } frame;
    int *queriedIface;
    HRESULT result;
    int commandCount;
    const char *errorText;
    typedef HRESULT (WINAPI *CoCreateInstance_guess)(const GUID *, void *, DWORD, const GUID *, void **);
    typedef char *(WINAPI *LstrcpyA_guess)(char *, const char *);
    LstrcpyA_guess lstrcpyA;
    typedef int (__stdcall *VoiceRegisterFn_guess)(void *, const void *, void *, GUID, int, int);
    typedef int (__stdcall *VoiceCreateMenuFn_guess)(void *, char *, void *, int, void **);
    typedef int (__stdcall *VoiceActivateFn_guess)(void *, int *, int);
    typedef int (__stdcall *VoiceQueryInterfaceFn_guess)(void *, const GUID *, void **);
    typedef int (__stdcall *VoiceGetEngineInfoFn_guess)(void *, GUID *);
    typedef int (__stdcall *VoiceFindEngineFn_guess)(void *, void *, int, void *);
    typedef void (__stdcall *VoiceReleaseFn_guess)(void *);

    result = EQ_GLOBAL(CoCreateInstance_guess, 0x00534374)
        ((const GUID *)0x00534d40, 0, 4, (const GUID *)0x00534cf0,
         (void **)&EQ_GLOBAL(void *, 0x006ECE44));
    if (result < 0)
    {
        EQ_CDECL(void, 0x004A3D01, (const char *, ...))
            ((const char *)0x0057cb84, result);
        EQ_CDECL(void, 0x004F8220, (const char *, ...))
            ((const char *)0x0057cb84, result);
        return result;
    }

    result = ((VoiceRegisterFn_guess)(*(void ***)EQ_GLOBAL(void *, 0x006ECE44))[3])
        (EQ_GLOBAL(void *, 0x006ECE44), (const void *)0x0058fba4, (void *)0x006ece30,
         EQ_GLOBAL(GUID, 0x00534C70), 3, 0);
    if (result >= 0)
    {

    lstrcpyA = EQ_GLOBAL(LstrcpyA_guess, 0x0053419C);
    lstrcpyA(frame.menuName, (const char *)0x0056248c);
    lstrcpyA(frame.commandName, (const char *)0x0057cb7c);
    frame.languageInfo.language = 9;
    lstrcpyA(frame.languageInfo.name, (const char *)0x0057cb70);
    result = ((VoiceCreateMenuFn_guess)(*(void ***)EQ_GLOBAL(void *, 0x006ECE44))[5])
        (EQ_GLOBAL(void *, 0x006ECE44), frame.menuName, &frame.languageInfo, 1,
         (void **)0x006ece48);
    if (result >= 0)
    {
        commandCount = 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xcf, (char *)0x0057cb64) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(200, (char *)0x0057cb5c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x69, (char *)0x0057cb50) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(1, (char *)0x0057cb44) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(8, (char *)0x0057cb3c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(9, (char *)0x0055aa0c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xb, (char *)0x0055a9d8) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xc, (char *)0x00565a34) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xd, (char *)0x0057cb34) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xe, (char *)0x0057cb30) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x14, (char *)0x0057cb28) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x16, (char *)0x0057cb1c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x16, (char *)0x0057cb10) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x17, (char *)0x0057cb04) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x18, (char *)0x0057caf4) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x19, (char *)0x0057cae8) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x1a, (char *)0x0057cadc) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x1b, (char *)0x0057cad0) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x1c, (char *)0x0057cac0) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x1d, (char *)0x0057cab4) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x1f, (char *)0x0057caac) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x20, (char *)0x0057caa4) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x21, (char *)0x0057ca98) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x22, (char *)0x0057ca8c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x23, (char *)0x0057ca80) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x24, (char *)0x0057ca78) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x213, (char *)0x0057ca68) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x214, (char *)0x0057ca58) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x215, (char *)0x0057ca48) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x216, (char *)0x0057ca38) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x217, (char *)0x0057ca28) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x218, (char *)0x0057ca18) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x3b, (char *)0x0057ca10) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xc9, (char *)0x0057ca08) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x4f, (char *)0x0057c9fc) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x50, (char *)0x0057c9ec) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xd3, (char *)0x0057c9dc) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x7b, (char *)0x0057c9d0) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c9c8) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c9bc) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0056f434) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c9b4) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0055a9b8) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c9a8) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c998) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c988) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c97c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c96c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c95c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0xca, (char *)0x0057c950) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x7c, (char *)0x0057c94c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x7d, (char *)0x0057c944) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x7e, (char *)0x0055d66c) >= 0;
        commandCount += EQ_CDECL(int, 0x004E6F95, (unsigned int, char *))(0x7f, (char *)0x0057c93c) >= 0;
        if (commandCount == 0x36)
        {
            EQ_CDECL(void, 0x004F8220, (const char *, ...))
                ((const char *)0x0057c914);
        }
        else
        {
            EQ_CDECL(void, 0x004F8220, (const char *, ...))
                ((const char *)0x0057c8dc, commandCount);
        }
        if (commandCount > 0)
        {
            result = ((VoiceActivateFn_guess)(*(void ***)EQ_GLOBAL(void *, 0x006ECE48))[3])
                (EQ_GLOBAL(void *, 0x006ECE48), activateArg, 0);
            if (result >= 0)
            {
                EQ_CDECL(void, 0x004A3D01, (const char *))
                    ((const char *)0x0057c8c0);
                EQ_CDECL(void, 0x004F8220, (const char *))
                    ((const char *)0x0057c8c0);
            }
            else
            {
                EQ_CDECL(void, 0x004A3D01, (const char *, ...))
                    ((const char *)0x0057c890, result);
                EQ_CDECL(void, 0x004F8220, (const char *, ...))
                    ((const char *)0x0057c890, result);
            }
        }
    }
    else
    {
        EQ_CDECL(void, 0x004A3D01, (const char *, ...))
            ((const char *)0x0057c864, result);
        EQ_CDECL(void, 0x004F8220, (const char *, ...))
            ((const char *)0x0057c864, result);
    }

    queriedIface = 0;
    result = ((VoiceQueryInterfaceFn_guess)(*(void ***)EQ_GLOBAL(void *, 0x006ECE44))[0])
        (EQ_GLOBAL(void *, 0x006ECE44), (const GUID *)0x00534d10,
         (void **)&queriedIface);
    if (result >= 0)
    {

    EQ_CDECL(void *, 0x00521960, (void *, int, size_t))(&frame.engineId, 0, 0x10);
    frame.engineMatched = 0;
    result = ((VoiceGetEngineInfoFn_guess)(*(void ***)queriedIface)[0x11])
        (queriedIface, &frame.engineId);
    if (result >= 0)
    {
        activateArg = 0;
        result = EQ_GLOBAL(CoCreateInstance_guess, 0x00534374)
            ((const GUID *)0x00534880, 0, 7, (const GUID *)0x00534940, (void **)&activateArg);
        if (result >= 0)
        {
            frame.requestId = frame.engineId;
            result = ((VoiceFindEngineFn_guess)(*(void ***)activateArg)[3])
                (activateArg, frame.requestText, 0, frame.findOutput);
            if (result >= 0)
            {
                if (EQ_CDECL(int, 0x00524310, (const void *, const void *, size_t))
                    (frame.returnedId, &frame.engineId, 0x10) == 0)
                {
                    EQ_CDECL(void, 0x004A3D01, (const char *, ...))
                        ((const char *)0x0057c848, frame.engineText);
                    EQ_CDECL(void, 0x004F8220, (const char *, ...))
                        ((const char *)0x0057c848, frame.engineText);
                    frame.engineMatched = 1;
                }
            }
            else
            {
                EQ_CDECL(void, 0x004A3D01, (const char *, ...))
                    ((const char *)0x0057c81c, result);
                EQ_CDECL(void, 0x004F8220, (const char *, ...))
                    ((const char *)0x0057c81c, result);
            }
        }
        else
        {
            EQ_CDECL(void, 0x004A3D01, (const char *, ...))
                ((const char *)0x0057c7e8, result);
            EQ_CDECL(void, 0x004F8220, (const char *, ...))
                ((const char *)0x0057c7e8, result);
        }
        ((VoiceReleaseFn_guess)(*(void ***)activateArg)[2])(activateArg);
    }

    if (frame.engineMatched == 0)
    {
        EQ_CDECL(void, 0x004A3D01, (const char *))((const char *)0x0057c7cc);
        EQ_CDECL(void, 0x004F8220, (const char *))((const char *)0x0057c7cc);
    }

    ((VoiceReleaseFn_guess)(*(void ***)queriedIface)[2])(queriedIface);
    return result;
    }
    errorText = (const char *)0x0057c79c;
    }
    else
    {
        errorText = (const char *)0x0057c76c;
    }

    EQ_CDECL(void, 0x004A3D01, (const char *, ...))(errorText, result);
    EQ_CDECL(void, 0x004F8220, (const char *, ...))(errorText, result);
    return result;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004E8251
// __thiscall. Finds the three chunk markers in the pack file and loads the wave header into a new WaveFileData.
char WaveFile_guess::LoadWaveFileData_guess(FfSharedRef_guess source)
{
    char found = 0;
    int count;
    int remaining;
    unsigned char *cursor;
    int skip;
    int magic;
    int headerSize;
    int start;
    int dataLength;
    WaveFileData_guess *raw;
    WaveFileData_guess *wave;
    DWORD idx;
    unsigned char buffer[WAVE_SEARCH_CHUNK];
    WaveFileHeader_guess header;

    EQ_METHOD(0, void, 0x004E8661, (WaveFile_guess *), this)();
    WavePackReader_guess pack;
    WavePackLoader_guess loader;
    EQ_METHOD(0, WavePackLoader_guess *, 0x004F8D50, (WavePackLoader_guess *), &loader)();
    if (EQ_METHOD(2, char, 0x004F99D0, (WavePackLoader_guess *, FfSharedRef_guess, WavePackReader_guess *),
            &loader)(source, &pack) == 0)
        return found;

    if (EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)(&magic, 4) == 4
        && magic == EQ_GLOBAL(int, 0x0057D018))
    {
        count = EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)
            (buffer, WAVE_SEARCH_CHUNK);
        cursor = buffer;
        skip = 4;
        while (count > 0 && !found)
        {
            for (remaining = count; remaining >= 3 && !found; remaining--)
            {
                if (EQ_GLOBAL(int, 0x005586B0) == *(int *)cursor)
                {
                    skip += 4;
                    found = 1;
                    cursor += 4;
                    count = remaining - 5;
                }
                else
                {
                    skip++;
                    cursor++;
                }
            }
            if (!found)
            {
                buffer[0] = buffer[count - 3];
                buffer[1] = buffer[count - 2];
                buffer[2] = buffer[count - 1];
                count = EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)
                    (buffer + 3, WAVE_SEARCH_CHUNK - 3);
                cursor = buffer;
            }
        }
        if (found)
        {
            found = 0;
            if (count == 0)
            {
                count = EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)
                    (buffer, WAVE_SEARCH_CHUNK);
                cursor = buffer;
            }
            while (count > 0 && !found)
            {
                for (remaining = count; remaining >= 3 && !found; remaining--)
                {
                    if (EQ_GLOBAL(int, 0x0057D010) == *(int *)cursor)
                    {
                        skip += 4;
                        found = 1;
                        cursor += 4;
                        count = remaining - 5;
                    }
                    else
                    {
                        skip++;
                        cursor++;
                    }
                }
                if (!found)
                {
                    buffer[0] = buffer[count - 3];
                    buffer[1] = buffer[count - 2];
                    buffer[2] = buffer[count - 1];
                    count = EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)
                        (buffer + 3, WAVE_SEARCH_CHUNK - 3);
                    cursor = buffer;
                }
            }
            if (found)
            {
                EQ_METHOD(0, void, 0x004F99B0, (WavePackReader_guess *), &pack)();
                EQ_METHOD(1, int, 0x004F9860, (WavePackReader_guess *, int), &pack)(skip);
                if (EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)
                        (&headerSize, 4) == 4
                    && headerSize + 2 == sizeof(WaveFileHeader_guess)
                    && EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)
                        (&header, 0x10) == 0x10)
                {
                    header.terminator_guess = 0;
                    found = 0;
                    start = EQ_METHOD(0, int, 0x004F99A0, (WavePackReader_guess *), &pack)();
                    count = EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)
                        (buffer, WAVE_SEARCH_CHUNK);
                    cursor = buffer;
                    while (count > 0 && !found)
                    {
                        for (remaining = count; remaining >= 3 && !found; remaining--)
                        {
                            if (EQ_GLOBAL(int, 0x00565A64) == *(int *)cursor)
                            {
                                found = 1;
                                start += 4;
                                cursor += 4;
                                count = remaining - 5;
                            }
                            else
                            {
                                start++;
                                cursor++;
                            }
                        }
                        if (!found)
                        {
                            buffer[0] = buffer[count - 3];
                            buffer[1] = buffer[count - 2];
                            buffer[2] = buffer[count - 1];
                            count = EQ_METHOD(2, int, 0x004F9730,
                                (WavePackReader_guess *, void *, unsigned int), &pack)
                                (buffer + 3, WAVE_SEARCH_CHUNK - 3);
                            cursor = buffer;
                        }
                    }
                    if (found)
                    {
                        EQ_METHOD(0, void, 0x004F99B0, (WavePackReader_guess *), &pack)();
                        EQ_METHOD(1, int, 0x004F9860, (WavePackReader_guess *, int), &pack)(start);
                        if (EQ_METHOD(2, int, 0x004F9730, (WavePackReader_guess *, void *, unsigned int), &pack)
                                (&dataLength, 4) == 4)
                        {
                            raw = (WaveFileData_guess *)EQ_CDECL(void *, 0x005218C6, (unsigned int))
                                (sizeof(WaveFileData_guess));
                            wave = 0;
                            if (raw != 0)
                            {
                                raw->pack_guess = 0;
                                raw->dataLength_guess = 0;
                                raw->dataStart_guess = 0;
                                raw->field20_guess = 0;
                                wave = raw;
                            }
                            idx = EQ_GLOBAL(DWORD, 0x006F035C);
                            if (idx < 0x40)
                            {
                                EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].file_guess = (LPCSTR)0x0057cfec;
                                EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].line_guess = 0xff;
                                EQ_GLOBAL(DWORD, 0x006F035C) = idx + 1;
                            }
                            if (wave == 0)
                            {
                                EQ_CDECL(void, 0x004F8310, (bool, LPCSTR, LPCSTR))
                                    (0, (LPCSTR)0x00570968, (LPCSTR)0x0057cfd8);
                            }
                            data_guess = wave;
                            wave->header_guess = header;
                            EQ_METHOD(1, void *, 0x004F9560, (void **, WavePackReader_guess *),
                                &data_guess->pack_guess)(&pack);
                            data_guess->dataLength_guess = dataLength;
                            data_guess->dataStart_guess =
                                EQ_METHOD(0, int, 0x004F99A0, (WavePackReader_guess *), &pack)();
                            found = 1;
                            data_guess->field20_guess = 0;
                            return found;
                        }
                    }
                }
            }
        }
    }

    EQ_METHOD(0, void, 0x004E8661, (WaveFile_guess *), this)();
    return 0;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004e8647
// __thiscall copy constructor.
FfSharedRef_guess::FfSharedRef_guess(const FfSharedRef_guess &src)
{
    shared_guess = src.shared_guess;
    if (shared_guess != 0)
    {
        shared_guess->refs_guess++;
    }
    extra_guess = src.extra_guess;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004e8693
// __thiscall; tail-jumps to the release thunk (0x004F9550) with this + 0x14.
void FfHasOwner_guess::ReleaseOwner_guess()
{
    EQ_METHOD(0, void, 0x004F9550, (char *), (char *)this + 0x14)();
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004ede7f
// Sends a descriptor ID to its indexed entry and the descriptor to a related object; purpose unclear.
int FFCreate2Lookup_guess::ProcessIndexedDescriptor_guess(unsigned int *descriptor)
{
    int index = (int)(*descriptor >> 0x10);
    void *entry;
    int result;

    if (index >= 0 && index < FF_CREATE2_ENTRY_COUNT &&
        (entry = entry_by_index[index]) != 0)
    {
        EQ_METHOD(1, void, 0x004EC445, (void *, unsigned short), entry)
            ((unsigned short)*descriptor);
        EQ_METHOD(1, void, 0x004EE5E4, (void *, unsigned int *), collection_resources)
            (descriptor);
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004edec0
// Runs one indexed entry's parameterless operation, or runs it for every entry; purpose unclear.
void FFCreate2Lookup_guess::InvokeSelectedEntryMethods_guess(int index)
{
    if (index < FF_CREATE2_ENTRY_COUNT && index >= -1)
    {
        if (index == -1)
        {
            void **entry = entry_by_index;
            int remaining = FF_CREATE2_ENTRY_COUNT;

            do
            {
                if (*entry != 0)
                    EQ_METHOD(0, void, 0x004EC4B5, (void *), *entry)();
                entry++;
                remaining--;
            } while (remaining != 0);
        }
        else
        {
            EQ_METHOD(0, void, 0x004EC4B5, (void *), entry_by_index[index])();
        }
    }
    return;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004edefc
// Updates an indexed entry from a descriptor and caller context; the game effect is unclear.
int FFCreate2Lookup_guess::InvokeIndexedEntryMethodWithDescriptor_guess(unsigned int *descriptor, int context)
{
    int index = *descriptor >> 0x10;
    void *entry;
    int result;

    if (index >= 0 && index < FF_CREATE2_ENTRY_COUNT &&
        (entry = entry_by_index[index]) != 0)
    {
        unsigned int resourceId = (unsigned short)*descriptor;
        EQ_METHOD(3, void, 0x004EC77D, (void *, unsigned int, int, int), entry)
            (resourceId, context, 0);
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}
