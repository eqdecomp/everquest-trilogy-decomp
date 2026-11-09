// login.cpp - login and error dialog behavior.

#include "trilogy/common.h"
#include "trilogy/login.h"
#include "trilogy/eqaddr.h"

typedef int (__stdcall *tDialogBoxParamA)(void*, const char*, void*, void*, long);
typedef void* (__stdcall *tGetDlgItem)(void*, int);
typedef int (__stdcall *tSetWindowTextA)(void*, const char*);
typedef int (__stdcall *tEndDialog)(void*, int);

// FUNCTION: TRILOGY 0x004059d2
// Opens a modal notification using the supplied title and message.
void showErrorDialog_guess(void* instance, void* parent, u32 title, u32 body)
{
    EQ_GLOBAL(u32, 0x0058FBAC) = title;
    EQ_GLOBAL(u32, 0x0058FBA8) = body;
    EQ_GLOBAL(tDialogBoxParamA, 0x00534260)(instance, (const char*)0x6C,
        parent, (void*)0x004059FC, 0);
}

// FUNCTION: TRILOGY 0x004059fc
// Sets the notification text and dismisses it when OK is selected.
// @early-stop: first title still CMP/push-mem-after-Get, no push/jmp
/* fake match: volatile title spills to keep 2 regs; duplicated body calls give push/jmp tail */
int DialogFunc_guess(void* dialog, int message, short wParam)
{
    const char* volatile title;
    const char* body;
    void* item;
    tGetDlgItem getDlgItem;
    tSetWindowTextA setWindowText;

    if (message == 0x110)
    {
        title = (const char*)EQ_GLOBAL(u32, 0x0058FBAC);
        if (title == 0)
            title = (const char*)0x0053C3FC;
        getDlgItem = EQ_GLOBAL(tGetDlgItem, 0x00534254);
        item = getDlgItem(dialog, 0x3EC);
        setWindowText = EQ_GLOBAL(tSetWindowTextA, 0x00534258);
        setWindowText(item, title);

        body = (const char*)EQ_GLOBAL(u32, 0x0058FBA8);
        if (body != 0)
            setWindowText(getDlgItem(dialog, 0x3EB), body);
        else
            setWindowText(getDlgItem(dialog, 0x3EB), (const char*)0x0053C3E8);
        return 1;
    }
    if (message == 0x111 && wParam == 1)
    {
        EQ_GLOBAL(tEndDialog, 0x0053425C)(dialog, 1);
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004e013c
// Authenticates the player's account with the world server.
// ORIGINAL FILE: EverQuest/login.cpp (from Mac debug map)
// cdecl. Login loop. While DAT_006b63c0 is set it connects, sends the login (0x1858) or the 0x20fd form, waits
// for the reply and acts on its opcode. While it is clear it shows the login dialog and checks the password.
// Returns the connection when a reply finishes the login; otherwise returns 0 with *g_everquest set to 1 or 2.
void* WorldAuthenticate(void* parentWindow, void* appInstance)
{
    typedef int (__stdcall *tMessageBoxA_guess)(void*, const char*, const char*, u32);
    struct LoginPacket_guess
    {
        char text[0xba];        /* 0x00 ConcatenateStringsWithSeparators_guess output */
        u8 zero;                /* 0xba */
        u32 flags;              /* 0xbc DAT_006b7110 */
        u16 tail;               /* 0xc0 0x9e */
        u8 unused[2];           /* 0xc2 */
    } packet;
    struct LoginRequest_guess
    {
        char account[32];       /* 0x00 */
        char password[32];      /* 0x20 */
        char second[292];       /* 0x40 */
    } request;
    char title_text[128];
    char detail_text[128];
    void* connection;
    int done;
    int result;
    int msg;
    int data;
    u16 opcode;
    tMessageBoxA_guess messageBox;
    const char* caption;

    caption = (const char*)0x0056248c;
    messageBox = EQ_GLOBAL(tMessageBoxA_guess, 0x00534230);
    connection = 0;
    done = 0;
    for (;;)
    {
        if (EQ_GLOBAL(u32, 0x006B63C0) != 0)
        {
            result = EQ_CDECL(int, 0x004E055C, (void**, char*, u16))(&connection, EQ_ARRAY(char, 0x006B62BC), EQ_GLOBAL(u16, 0x006B63BC));
            if (result != 0)
            {
                EQ_CDECL(void, 0x004A3D01, (const char*))((const char*)0x005760f4);
                if (EQ_GLOBAL(u8, 0x006EE758) != 0)
                    goto abortLogin;
                goto clearLoginFlag;
            }
            if (EQ_CDECL(size_t, 0x005223D0, (const char*))(EQ_ARRAY(char, 0x006B633C)) < 0x19)
            {
                if (EQ_CDECL(size_t, 0x005223D0, (const char*))(EQ_ARRAY(char, 0x006B623C)) < 4)
                {
shortPassword:
                    if (EQ_GLOBAL(u8, 0x006EE758) == 0)
                        messageBox(0, (const char*)0x005760c4, caption, 0x40000);
                    EQ_GLOBAL(u32, 0x006B63C0) = 0;
                    goto checkDone;
                }
                EQ_CDECL(void, 0x004A0A03, (char*, const char*, const char*, const char*))(packet.text, EQ_ARRAY(char, 0x006B61BC), EQ_ARRAY(char, 0x006B623C), EQ_ARRAY(char, 0x0058FBA4));
                packet.zero = 0;
                packet.flags = EQ_GLOBAL(u8, 0x006B7110);
                packet.tail = 0x9e;
                EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))(&connection, 0x1858, &packet, 0xc4, 1);
            }
            else
            {
                if (EQ_CDECL(size_t, 0x005223D0, (const char*))(EQ_ARRAY(char, 0x006B623C)) < 4)
                    goto shortPassword;
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(request.account, EQ_ARRAY(char, 0x006B61BC));
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(request.password, EQ_ARRAY(char, 0x006B623C));
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(request.second, EQ_ARRAY(char, 0x006B633C));
                EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))(&connection, 0x20fd, &request, 0x162, 1);
            }
            msg = EQ_CDECL(int, 0x004E04D1, (void))();
            if (msg == 0)
            {
                EQ_CDECL(void, 0x004E0601, (void*))(connection);
                connection = 0;
                if (EQ_GLOBAL(u8, 0x006EE758) == 0)
                {
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(title_text, (const char*)0x005760ac);
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(detail_text, (const char*)0x00576030);
                    EQ_CDECL(void, 0x004059D2, (void*, void*, char*, char*))(appInstance, parentWindow, title_text, detail_text);
                }
                if (parentWindow == 0)
                    goto abortLogin;
                goto clearLoginFlag;
            }
            data = EQ_CDECL(int, 0x004F4C90, (int))(msg);
            opcode = *(u16*)EQ_CDECL(int, 0x004F4C90, (int))(msg);
            if (opcode == 0x1005)
            {
                EQ_CDECL(void, 0x004E0601, (void*))(connection);
                connection = 0;
                if (EQ_GLOBAL(u8, 0x006EE758) == 0)
                    messageBox(0, (const char*)0x00576014, caption, 0x40000);
                EQ_GLOBAL(u32, 0x006B63C0) = 0;
            }
            else if (opcode == 0x1007)
            {
                done = 1;
            }
            else if (opcode == 0x200f)
            {
                EQ_CDECL(void, 0x00491BF0, (int))(data + 2);
            }
            else if (opcode == 0x21e5)
            {
                EQ_CDECL(void, 0x004A055E, (int, void*))(data + 2, connection);
                done = 1;
            }
            else if (opcode == 0x21ed)
            {
                EQ_CDECL(void, 0x004E0601, (void*))(connection);
                *(u32*)EQ_GLOBAL(void*, 0x006EFE2C) = 2;
                return 0;
            }
            EQ_CDECL(void, 0x004F4AC0, (int))(msg);
            goto checkDone;
clearLoginFlag:
            EQ_GLOBAL(u32, 0x006B63C0) = 0;
        }
        else
        {
            EQ_GLOBAL(u32, 0x006B63C0) = 1;
            if (EQ_GLOBAL(u8, 0x006EE758) != 0 ||
                EQ_GLOBAL(tDialogBoxParamA, 0x00534260)(appInstance, (const char*)0x68, parentWindow, (void*)0x004e011f, 0) == 0)
                goto abortLogin;
            EQ_GLOBAL(u32, 0x006B6494) = 0;
            if (EQ_CDECL(size_t, 0x005223D0, (const char*))(EQ_ARRAY(char, 0x006B623C)) < 4)
            {
                messageBox(0, (const char*)0x005760c4, caption, 0x40000);
                goto clearLoginFlag;
            }
        }
checkDone:
        if (done != 0)
            return connection;
    }
abortLogin:
    *(u32*)EQ_GLOBAL(void*, 0x006EFE2C) = 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004e041f
// Reads the player's name from the server reply into the supplied buffer.
// ORIGINAL FILE: EverQuest/login.cpp (from Mac debug map)
// @early-stop: Timeout epilogue and cdecl argument cleanup still differ after eight non-improving iterations.
int GetPlayerName(char* playerName)
{
    int msg;
    int success;
    typedef void (__stdcall *tSleep_guess)(unsigned long);

    success = 0;
    msg = EQ_CDECL(int, 0x004E04D1, (void))();
    if (msg == 0)
    {
        int retryCount;

        retryCount = 0;
        for (;;)
        {
            msg = EQ_CDECL(int, 0x004E04D1, (void))();
            if (msg != 0)
                break;
            EQ_GLOBAL(tSleep_guess, 0x00534198)(1);
            retryCount++;
            if (retryCount >= 6)
            {
                EQ_CDECL(void, 0x004C1703, (const char*, ...))((const char*)0x00576138);
                return success;
            }
        }
    }
    if (*EQ_CDECL(unsigned short*, 0x004F4C90, (int))(msg) != 0x8001)
    {
        EQ_CDECL(void, 0x004C1703, (const char*, ...))((const char*)0x00576118,
            *(unsigned short*)EQ_CDECL(void*, 0x004F4C90, (int))(msg));
    }
    else
    {
        char* messageText;

        success = 1;
        messageText = (char*)EQ_CDECL(void*, 0x004F4C90, (int))(msg);
        messageText++;
        messageText++;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(playerName, messageText);
    }
    EQ_CDECL(void, 0x004F4AC0, (int))(msg);
    return success;
}
