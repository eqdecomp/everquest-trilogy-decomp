#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

// FUNCTION: TRILOGY 0x004e04d1
// Waits for a usable message from a peer connection.
// ORIGINAL FILE: EverQuest/net.cpp (from Mac debug map)
// @early-stop: empty-receive path still has an extra test and shares the message return epilogue.
int wait_message(void)
{
    int msg;
    int value;
    int *ptr;
    unsigned int count;
    int flag;

    for (;;)
    {
        while (true)
        {
            msg = EQ_CDECL(int, 0x004F1600, (void*, int))
                (EQ_GLOBAL(void*, 0x006ECCA8), 10000);
            if (msg == 0)
                break;
            value = EQ_CDECL(int, 0x004F4CB0, (int))(msg);
            if (value != 0)
                break;
            EQ_CDECL(void, 0x004F4AC0, (int))(msg);
        }
        if (msg == 0)
            break;
        ptr = EQ_CDECL(int*, 0x004F23C0, (int))(value);
        if (*ptr == 0)
        {
            EQ_CDECL(void, 0x004F21D0, (int, int, int, int))(value, 0, 0, 0);
            EQ_CDECL(void, 0x004F4AC0, (int))(msg);
            msg = 0;
        }
        if (msg != 0)
            break;
    }

    if (msg != 0)
    {
        count = EQ_CDECL(unsigned int, 0x004F4C80, (int))(msg);
        if (((count < 2) ||
             (flag = EQ_CDECL(int, 0x004F4CC0, (int))(msg), flag != 0)) ||
            (value = EQ_CDECL(int, 0x004F23B0, (int))(value), value == 0))
        {
            EQ_CDECL(void, 0x004F4AC0, (int))(msg);
            msg = 0;
        }
    }
    return msg;
}

// FUNCTION: TRILOGY 0x004e055c
// Starts the network service and connects to a remote server.
int InitializeConnection_guess(unsigned int *connectionOut, int text, int value)
{
    int result;
    int *ptr;

    if (EQ_GLOBAL(void*, 0x006ECCA8) != 0)
    {
        EQ_CDECL(void, 0x004F1160, (void*, int))
            (EQ_GLOBAL(void*, 0x006ECCA8), 0);
        EQ_GLOBAL(void*, 0x006ECCA8) = 0;
    }
    result = EQ_CDECL(int, 0x004F0C70, (void**, int, int, int))
        (&EQ_GLOBAL(void*, 0x006ECCA8), 0, 1, 0x40000002);
    if ((result == 0) &&
        (result = EQ_CDECL(int, 0x004F1460, (void*, void*, int, int, int))
            (EQ_GLOBAL(void*, 0x006ECCA8), connectionOut, text, value, 1), result == 0))
    {
        ptr = (int *)EQ_CDECL(void*, 0x004F23C0, (int))(*connectionOut);
        *ptr = 0x200;
        EQ_CDECL(void, 0x004F46A0, (int, int))(*connectionOut, 0x1400);
        EQ_CDECL(void, 0x004F23A0, (int, int))(*connectionOut, 0x40000);
    }
    else
    {
        if (EQ_GLOBAL(void*, 0x006ECCA8) != 0)
        {
            EQ_CDECL(void, 0x004F1160, (void*, int))
                (EQ_GLOBAL(void*, 0x006ECCA8), 0);
            EQ_GLOBAL(void*, 0x006ECCA8) = 0;
        }
    }
    return result;
}

// FUNCTION: TRILOGY 0x004e0601
// Closes the peer connection and stops the network service.
// ORIGINAL FILE: EverQuest/net.cpp (from Mac debug map)
void net_shutdown(int connection)
{
    if (connection != 0)
    {
        EQ_CDECL(void, 0x004F15A0, (int, int, int))(connection, 10000, 0);
    }
    EQ_CDECL(void, 0x004F1160, (void*, int))
        (EQ_GLOBAL(void*, 0x006ECCA8), 0);
    EQ_GLOBAL(void*, 0x006ECCA8) = 0;
}

// FUNCTION: TRILOGY 0x004E0632
// Waits for the world server to disconnect, then closes any remaining connection.
// ORIGINAL FILE: EverQuest/net.cpp (from Mac debug map)
void wait_for_disconnect(void)
{
    int start_time;
    int connected;
    void* message;
    int has_fin;
    void* world_connection;

    start_time = EQ_CDECL(int, 0x004A461E, (void))();
    while (true)
    {
        world_connection = EQ_GLOBAL(void*, 0x006B3194);
        connected = world_connection != 0
            ? EQ_CDECL(int, 0x004F23B0, (void*))(world_connection)
            : 0;
        if (connected == 0)
            break;

        message = EQ_CDECL(void*, 0x004F1600, (void*, int))
            (EQ_GLOBAL(void*, 0x006ECCA8), 10000);
        if (message != 0)
        {
            has_fin = EQ_CDECL(int, 0x004F4CC0, (void*))(message);
            if ((has_fin != 0) && (EQ_GLOBAL(void*, 0x006B3194) != 0))
            {
                world_connection = EQ_GLOBAL(void*, 0x006B3194);
                EQ_CDECL(void, 0x004E0601, (int))((int)world_connection);
                EQ_GLOBAL(void*, 0x006B3194) = 0;
            }
            EQ_CDECL(void, 0x004F4AC0, (void*))(message);
        }

        connected = EQ_CDECL(int, 0x004A461E, (void))() - start_time;
        if (120000 < (unsigned int)connected)
        {
            EQ_CDECL(void, 0x004C1703, (const char*))((const char*)0x0057656C);
            break;
        }
        ((void (__stdcall *)(unsigned long))EQ_GLOBAL(void*, 0x00534198))(1);
    }

    world_connection = EQ_GLOBAL(void*, 0x006B3194);
    if (world_connection != 0)
    {
        EQ_CDECL(void, 0x004E0601, (int))((int)world_connection);
        EQ_GLOBAL(void*, 0x006B3194) = 0;
    }
}
