// EQSwitch.cpp - EQSwitch (doors, levers, platforms) methods.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQSwitch.cpp (from Mac debug map)

#include "trilogy/eqswitch.h"
#include "trilogy/eq_character.h"
#include <stdio.h>
#include <string.h>

// Debug output and message sending by name, like the CRT calls: a direct call has its arguments popped
// right after it, as in the client; EQ_CDECL's constant function pointer lets VC6 merge neighbouring pops.
#undef send_message_guess
// EXTERNAL: TRILOGY 0x004E1C45 send_message_guess
void __cdecl send_message_guess(void** connection, u32 opcode, void* data, u32 size, int reliable);

// eqswitch.h has no field at 0x04 yet: the list is doubly linked, 0x04 is the previous switch.
#define EQSWITCH_PREV_GUESS(sw)  (*(EQSwitch**)((char*)(sw) + 4))

// The sound call takes its manager in ECX and file/id arguments on the stack.
#define EQSWITCH_PLAY_SOUND_GUESS(soundFile, soundId) \
    EQ_METHOD(2, int, 0x004E37F4, (void*, const char*, int), EQ_GLOBAL(void*, 0x006E4DD8))(soundFile, soundId)

// FUNCTION: TRILOGY 0x004bd55f
// Builds a door/switch from a server record, a copy or a placed object, with its model actor and type sounds.
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (from Mac debug map)
// @early-stop: switch dispatch order, copied-byte loop registers, and wrapper EDX setup still differ.
/* fake match: placementHeight and the offset-0x40 reread reproduce VC6's x87 order and copy sequence. */
EQSwitch* __fastcall EQSwitch_Constructor_guess(
    EQSwitch* self, int edxDummy, EQSwitch* sourceSwitch, void* placementActor,
    const unsigned char* record, const char* switchName)
{
    char actorDef[80];
    unsigned char* selfBytes;
    unsigned char* initBytes;
    const EQSwitch* sourceBytes;
    unsigned int value;
    float placementHeight;
    unsigned char id;
    unsigned char type;
    int count;
    void* actor;
    typedef void (__cdecl *SetActorUserData_t)(void*, void*);
    typedef void (__cdecl *SetActorInvisible_t)(void*, int);

    selfBytes = (unsigned char*)self;
    if (g_switchList_guess == 0)
    {
        g_switchList_guess = self;
        *(unsigned int*)(selfBytes + 8) = 0;
        *(unsigned int*)(selfBytes + 4) = 0;
    }
    else
    {
        g_switchList_guess->prev_guess = self;
        *(EQSwitch**)(selfBytes + 8) = g_switchList_guess;
        *(unsigned int*)(selfBytes + 4) = 0;
        g_switchList_guess = self;
    }

    *(short*)(selfBytes + 0x5E) = -1;
    *(unsigned int*)(selfBytes + 0x80) = 0;

    if (record != 0)
    {
        id = *(record + 0x28);
        *(unsigned char*)(selfBytes + 0x0C) = id;
        if ((int)(unsigned int)id >= (int)EQ_GLOBAL(unsigned int, 0x006B65A4))
            EQ_GLOBAL(unsigned int, 0x006B65A4) = (unsigned int)id + 1;
        strcpy((char*)selfBytes + 0x0D, (const char*)record);
        type = *(record + 0x29);
        *(unsigned char*)(selfBytes + 0x1C) = type;
        *(unsigned char*)(selfBytes + 0x1D) = *(record + 0x2A);
        *(unsigned char*)(selfBytes + 0x48) = *(record + 0x2B);
        *(unsigned int*)(selfBytes + 0x20) = *(unsigned int*)(record + 0x10);
        *(unsigned int*)(selfBytes + 0x24) = *(unsigned int*)(record + 0x14);
        *(unsigned int*)(selfBytes + 0x28) = *(unsigned int*)(record + 0x18);
        *(unsigned int*)(selfBytes + 0x2C) = *(unsigned int*)(record + 0x1C);
        *(unsigned int*)(selfBytes + 0x30) = *(unsigned int*)(record + 0x20);
        *(unsigned short*)(selfBytes + 0x62) = *(unsigned short*)(record + 0x2C);
        *(unsigned int*)(selfBytes + 0x34) = *(unsigned int*)(record + 0x10);
        *(unsigned int*)(selfBytes + 0x38) = *(unsigned int*)(record + 0x14);
        *(unsigned int*)(selfBytes + 0x3C) = *(unsigned int*)(record + 0x18);
        *(unsigned int*)(selfBytes + 0x40) = *(unsigned int*)(record + 0x1C);
        value = *(unsigned int*)(record + 0x20);
        *(unsigned int*)(selfBytes + 0x44) = value;
        if (*(unsigned char*)(selfBytes + 0x1C) != '5')
        {
            EQ_METHOD(1, void, 0x004BDCE3, (EQSwitch*, unsigned char), self)(*(unsigned char*)(selfBytes + 0x1D));
        }
        sprintf(actorDef, EQ_ARRAY(char, 0x0055DCC0), (char*)selfBytes + 0x0D);
        if (*(unsigned char*)(selfBytes + 0x1C) == '5')
            *(unsigned int*)(selfBytes + 0x80) = 0;
        else
        {
            actor = EQ_METHOD(9, void*, 0x004112DF, (void*, const char*, float, float, float, float, float, float, int, int), EQ_GLOBAL(void*, 0x006E4DE0))(actorDef,
                 *(float*)(selfBytes + 0x34), *(float*)(selfBytes + 0x38),
                 *(float*)(selfBytes + 0x3C), *(float*)(selfBytes + 0x40),
                 *(float*)(selfBytes + 0x44), 0.0f, 1, 0);
            *(void**)(selfBytes + 0x80) = actor;
        }

        if (EQ_GLOBAL(void*, 0x006E4DD8) != 0)
        {
            type = *(unsigned char*)(selfBytes + 0x1C);
            switch (type)
            {
            case 1:
            case 6:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DDC), 0xB0);
                if (EQ_GLOBAL(void*, 0x006E4DD8) != 0)
                    EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DD8), 0xAF);
                break;
            case 2:
            case 7:
            case 0x8C:
            case 0x91:
            case 0x92:
            case 0x96:
            case 0x97:
            case 0x98:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DE8), 0xB3);
                if (EQ_GLOBAL(void*, 0x006E4DD8) != 0)
                    EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DE4), 0xB2);
                break;
            case 0x0A:
            case 0x0B:
            case 0x0C:
            case 0x0F:
            case 0x10:
            case 0x11:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x19:
            case 0x1A:
            case 0x1B:
            case 0x4E:
            case 0x4F:
            case 0x50:
            case 0x51:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DFC), 0xB8);
                if (EQ_GLOBAL(void*, 0x006E4DD8) != 0)
                    EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DF8), 0xB7);
                break;
            case 0x1E:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DD0), 0xAD);
                if (EQ_GLOBAL(void*, 0x006E4DD8) != 0)
                    EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DD4), 0xAE);
                break;
            case 0x3B:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577E00), 0xB9);
                break;
            case 0x3C: case 0x3D: case 0x3E:
            case 0x41: case 0x42: case 0x43:
            case 0x46: case 0x47: case 0x48:
            case 0x4B: case 0x4C: case 0x4D:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DF0), 0xB5);
                if (EQ_GLOBAL(void*, 0x006E4DD8) != 0)
                    EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DF4), 0xB6);
                break;
            case 0x23:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577E10), 0xBD);
                break;
            case 0x28:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DEC), 0xB4);
                break;
            case 0x6E:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577DCC), 0xAC);
                break;
            case 0x78:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577E08), 0xBB);
                break;
            case 0x7D:
                EQSWITCH_PLAY_SOUND_GUESS(EQ_GLOBAL(const char*, 0x00577E0C), 0xBC);
                break;
            }
        }
    }
    else if (sourceSwitch != 0)
    {
        sourceBytes = sourceSwitch;
        id = sourceBytes->id_guess;
        *(unsigned char*)(selfBytes + 0x0C) = id;
        if ((int)(unsigned int)id >= (int)EQ_GLOBAL(unsigned int, 0x006B65A4))
            EQ_GLOBAL(unsigned int, 0x006B65A4) = (unsigned int)id + 1;
        strcpy((char*)selfBytes + 0x0D, (const char*)((const unsigned char *)sourceBytes + 0x0D));
        *(unsigned char*)(selfBytes + 0x1C) = *((const unsigned char *)sourceBytes + 0x1C);
        *(unsigned char*)(selfBytes + 0x1D) = *((const unsigned char *)sourceBytes + 0x1D);
        *(unsigned int*)(selfBytes + 0x34) = sourceBytes->pos.y_bits_guess;
        *(unsigned int*)(selfBytes + 0x38) = sourceBytes->pos.x_bits_guess;
        *(unsigned int*)(selfBytes + 0x3C) = sourceBytes->pos.z_bits_guess;
        *(unsigned int*)(selfBytes + 0x40) = sourceBytes->pos.heading_bits_guess;
        *(unsigned int*)(selfBytes + 0x44) = sourceBytes->pos.auxiliary_bits_guess;
        *(unsigned int*)(selfBytes + 0x20) = sourceBytes->default_pos.y_bits_guess;
        *(unsigned int*)(selfBytes + 0x24) = sourceBytes->default_pos.x_bits_guess;
        *(unsigned int*)(selfBytes + 0x28) = sourceBytes->default_pos.z_bits_guess;
        *(unsigned int*)(selfBytes + 0x2C) = sourceBytes->default_pos.heading_bits_guess;
        *(unsigned int*)(selfBytes + 0x30) = sourceBytes->default_pos.auxiliary_bits_guess;
        *(unsigned char*)(selfBytes + 0x48) = *((const unsigned char *)sourceBytes + 0x48);
        *(unsigned char*)(selfBytes + 0x49) = *((const unsigned char *)sourceBytes + 0x49);
        *(unsigned char*)(selfBytes + 0x4A) = *((const unsigned char *)sourceBytes + 0x4A);
        *(unsigned char*)(selfBytes + 0x4B) = *((const unsigned char *)sourceBytes + 0x4B);
        *(unsigned char*)(selfBytes + 0x4C) = *((const unsigned char *)sourceBytes + 0x4C);
        if (*(unsigned char*)(selfBytes + 0x4C) != 0)
        {
            EQ_CDECL(int, 0x005229E8, (const char*, ...))
                (EQ_ARRAY(char, 0x00568C20),
                 (double)*(float*)(selfBytes + 0x20),
                 (double)*(float*)(selfBytes + 0x24),
                 (double)*(float*)(selfBytes + 0x28),
                 (int)*(unsigned char*)(selfBytes + 0x4C));
        }
        count = 5;
        {
            unsigned char* dest = selfBytes + 0x4D;
            const unsigned char* soundData = (const unsigned char*)sourceSwitch + 0x57;
            do
            {
                dest[0] = ((const unsigned char*)sourceBytes)[dest - selfBytes];
                dest[5] = soundData[-5];
                dest[10] = *soundData;
                dest[0x17] = soundData[0x0D];
                soundData++;
                dest++;
                count--;
            } while (count != 0);
        }
        *(unsigned char*)(selfBytes + 0x5C) = *((const unsigned char *)sourceBytes + 0x5C);
        *(unsigned short*)(selfBytes + 0x5E) = sourceBytes->keyItem_guess;
        *(unsigned short*)(selfBytes + 0x60) = *(unsigned short*)((const unsigned char *)sourceBytes + 0x60);
        *(unsigned short*)(selfBytes + 0x62) = *(unsigned short*)((const unsigned char *)sourceBytes + 0x62);
        strcpy((char*)selfBytes + 0x69, (const char*)((const unsigned char *)sourceBytes + 0x69));
        sprintf(actorDef, EQ_ARRAY(char, 0x0055DCC0), (char*)selfBytes + 0x0D);
        if (*(unsigned char*)(selfBytes + 0x1C) == '5')
            *(unsigned int*)(selfBytes + 0x80) = 0;
        else
        {
            actor = EQ_METHOD(9, void*, 0x004112DF, (void*, const char*, float, float, float, float, float, float, int, int), EQ_GLOBAL(void*, 0x006E4DE0))(actorDef,
                 *(float*)(selfBytes + 0x34), *(float*)(selfBytes + 0x38),
                 *(float*)(selfBytes + 0x3C), *(float*)(selfBytes + 0x40),
                 *(float*)(selfBytes + 0x44), 0.0f, 1, 0);
            *(void**)(selfBytes + 0x80) = actor;
        }
    }
    else
    {
        *(unsigned char*)(selfBytes + 0x0C) =
            (unsigned char)EQ_GLOBAL(unsigned int, 0x006B65A4);
        EQ_GLOBAL(unsigned int, 0x006B65A4)++;
        EQ_CDECL(void, 0x004A0841, (char*, const char*, int))
            ((char*)selfBytes + 0x0D, switchName, 0x0F);
        *(unsigned char*)(selfBytes + 0x1C) = 0;
        *(unsigned char*)(selfBytes + 0x1D) = 0;
        *(unsigned char*)(selfBytes + 0x48) = 0;
        *(unsigned int*)(selfBytes + 0x34) = *(unsigned int*)((char*)placementActor + 0x34);
        *(unsigned int*)(selfBytes + 0x38) = *(unsigned int*)((char*)placementActor + 0x38);
        placementHeight =
            *(float*)(*(int*)((char*)placementActor + 0x70) + 0x48);
        *(float*)(selfBytes + 0x3C) =
            placementHeight + EQ_GLOBAL(float, 0x005344A0);
        value = *(unsigned int*)((char*)placementActor + 0x40);
        *(unsigned int*)(selfBytes + 0x40) = value;
        *(unsigned int*)(selfBytes + 0x44) = *(unsigned int*)((char*)placementActor + 0x44);
        *(unsigned int*)(selfBytes + 0x20) = *(unsigned int*)(selfBytes + 0x34);
        *(unsigned int*)(selfBytes + 0x24) = *(unsigned int*)(selfBytes + 0x38);
        *(unsigned int*)(selfBytes + 0x28) = *(unsigned int*)(selfBytes + 0x3C);
        *(unsigned int*)(selfBytes + 0x2C) = *(unsigned int*)(selfBytes + 0x40);
        *(unsigned int*)(selfBytes + 0x30) = *(unsigned int*)(selfBytes + 0x44);
        *(unsigned char*)(selfBytes + 0x49) = 1;
        *(unsigned char*)(selfBytes + 0x4A) = 0;
        *(unsigned char*)(selfBytes + 0x4B) = 0;
        *(unsigned char*)(selfBytes + 0x4C) = 0;
        count = 5;
        initBytes = selfBytes + 0x52;
        do
        {
            initBytes[-5] = 0xFF;
            *initBytes = 0;
            initBytes[5] = 0;
            initBytes[0x12] = 0xFF;
            initBytes++;
            count--;
        } while (count != 0);
        *(short*)(selfBytes + 0x5E) = -1;
        *(short*)(selfBytes + 0x60) = -1;
        *(short*)(selfBytes + 0x62) = -1;
        *(unsigned char*)(selfBytes + 0x5C) = 0;
        if (*(unsigned char*)(selfBytes + 0x1C) == '8')
            *(unsigned short*)(selfBytes + 0x62) = 0xFFFE;

        strcpy((char*)selfBytes + 0x69, EQ_ARRAY(char, 0x0058FBA4));
        sprintf(actorDef, EQ_ARRAY(char, 0x0055DCC0), (char*)selfBytes + 0x0D);
        if (*(unsigned char*)(selfBytes + 0x1C) == '5')
            *(unsigned int*)(selfBytes + 0x80) = 0;
        else
        {
            actor = EQ_METHOD(9, void*, 0x004112DF, (void*, const char*, float, float, float, float, float, float, int, int), EQ_GLOBAL(void*, 0x006E4DE0))(actorDef,
                 *(float*)(selfBytes + 0x34), *(float*)(selfBytes + 0x38),
                 *(float*)(selfBytes + 0x3C), *(float*)(selfBytes + 0x40),
                 *(float*)(selfBytes + 0x44), 0.0f, 0, 0);
            *(void**)(selfBytes + 0x80) = actor;
        }
        if (*(void**)(selfBytes + 0x80) == 0)
            return self;
    }

    *(float*)(selfBytes + 0x88) = 0.0f;
    if (*(short*)(selfBytes + 0x5E) == 0)
        *(short*)(selfBytes + 0x5E) = -1;
    *(unsigned char*)selfBytes = 5;
    if (*(void**)(selfBytes + 0x80) != 0)
        EQ_GLOBAL(SetActorUserData_t, 0x006E538C)(*(void**)(selfBytes + 0x80), self);
    if (*(void**)(selfBytes + 0x80) != 0 &&
        (*(unsigned char*)(selfBytes + 0x1C) == '6' ||
         (*(unsigned char*)(selfBytes + 0x1C) == '9' &&
          *(float*)(selfBytes + 0x30) == EQ_GLOBAL(float, 0x005344A4))))
        EQ_GLOBAL(SetActorInvisible_t, 0x006E5378)(*(void**)(selfBytes + 0x80), 1);
    value = EQ_CDECL(unsigned int, 0x004A461E, (void))();
    *(unsigned int*)(selfBytes + 0x84) = value;
    return self;
}

#undef EQSWITCH_PLAY_SOUND_GUESS

// FUNCTION: TRILOGY 0x004bdc26
// Unregisters a switch and cleans up its associated world actor.
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (from Mac debug map)
// The destructor (eqswitch.h declares none, so a __fastcall free function with `this` in ECX).
// Unlinks the switch from the global list, then hands its actor back to the display (CDisplay at 0x006E4DE0).
void __fastcall EQSwitch_Destructor_guess(EQSwitch* self)
{
    if (self->prev_guess != 0 && self->next != 0)
    {
        self->prev_guess->next = self->next;
        self->next->prev_guess = self->prev_guess;
    }
    else if (self->prev_guess == 0 && self->next != 0)
    {
        self->next->prev_guess = 0;
        g_switchList_guess = self->next;
    }
    else if (self->next == 0 && self->prev_guess != 0)
    {
        self->prev_guess->next = 0;
    }
    else
    {
        self->next = 0;
        self->prev_guess = 0;
        g_switchList_guess = 0;
    }

    if (self->actor != 0)
    {
        if (self->type == 0x35)
            EQ_METHOD(1, void, 0x0040827A, (void*, void**), EQ_GLOBAL(void*, 0x006E4DE0))(&self->actor);
        else
            EQ_METHOD(1, void, 0x00411715, (void*, void*), EQ_GLOBAL(void*, 0x006E4DE0))(self->actor);
    }
}

// FUNCTION: TRILOGY 0x004bdca8
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (address range of the Mac-mapped EQSwitch functions; unnamed)
// The one caller passes the list head in ECX, but the walk starts from the global head, not `this`.
EQSwitch* EQSwitch::FindSwitchByActor_guess(void* actor)
{
    EQSwitch* sw;

    for (sw = g_switchList_guess; sw != 0; sw = sw->next)
    {
        if (actor == sw->actor)
            return sw;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004bdcc7
// Walks the global switch list and compares the low byte of the argument with each switch id.
EQSwitch* EQSwitch::FindSwitchById_guess(u8 id)
{
    EQSwitch* sw;

    for (sw = g_switchList_guess; sw != 0; sw = sw->next)
    {
        if (id == sw->id_guess)
            return sw;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004bdce3
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (address range of the Mac-mapped EQSwitch functions; TAKP name)
// Mode 1 -> state 2; any other mode is stored as the new state, except that state 3 becomes 0.
void EQSwitch::ResetSwitchPosition_guess(u8 mode)
{
    pos.y = default_pos.y;
    pos.x = default_pos.x;
    pos.z = default_pos.z;
    pos.heading = default_pos.heading;
    pos.auxiliary_guess = default_pos.auxiliary_guess;
    if (mode == 1)
        state = 2;
    else
        state = state == 3 ? 0 : mode;
}

// FUNCTION: TRILOGY 0x004bdd20
// Starts a switch transition when its target state changes.
void __fastcall ToggleDoorState_guess(EQSwitch *switchObj)
{
    char targetState;

    switchObj->inputStates_guess[0] = '\x01' - switchObj->requiredInputStates_guess[0];
    targetState = *(char *)((char *)switchObj + 0x48);
    if (switchObj->state != targetState) {
        if ((*(char *)((char *)switchObj + 0x1C) != '(') &&
            (*(char *)((char *)switchObj + 0x1C) != '-')) {
            if (targetState == '\0') {
                switchObj->state = 3;
                return;
            }
            if (targetState == '\x01') {
                switchObj->state = 2;
                return;
            }
            switchObj->state = targetState;
            return;
        }
        EQ_METHOD(3, void, 0x004BE44E, (EQSwitch*, int, int, int), switchObj)(0, 0, 0xFF);
    }
    return;
}

// FUNCTION: TRILOGY 0x004bdd62
// Provides type-based movement speed and acceleration for switch animation.
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (from Mac debug map)
float EQSwitch::TopSpeed_guess(float* pAccel)
{
    float speed = 0.0f;

    switch (type)
    {
    case 100:
    case 105:
        speed = 3.0f;
        if (pAccel)
            *pAccel = 0.025f;
        break;
    case 101:
    case 106:
        speed = 9.0f;
        if (pAccel)
            *pAccel = 0.075f;
        break;
    case 102:
    case 107:
        speed = 15.0f;
        if (pAccel)
            *pAccel = 0.125f;
        break;
    case 115:
    case 116:
        speed = 45.0f;
        if (pAccel)
            *pAccel = 0.375f;
        break;
    }
    return speed;
}

// FUNCTION: TRILOGY 0x004bddf1
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (address range of the Mac-mapped EQSwitch functions; TAKP name)
// In milliseconds. A param of -2 or -3 means the switch never changes back (0).
// @early-stop: same 83 instructions, but two arm bodies near the end are laid out in the other order (first diff `je @71` vs `je @75`, 13 differ); not reproduced by reordering the cases yet.
int EQSwitch::GetSwitchStateChangeTime_guess()
{
    int time;

    switch (type)
    {
    case 0:
    case 5:
        time = 10000;
        break;
    case 35:
    case 145:
    case 151:
        time = 6000;
        break;
    case 116:
        /* own copy of the 6000 arm: when the four labels share one, VC6 lays it out after the default */
        time = 6000;
        break;
    case 40:
    case 45:
        if (param_guess > 0)
            time = param_guess;
        else
            time = 20000;
        break;
    case 53:
        time = triggerMode_guess == 0;
        break;
    case 57:
    case 58:
    case 140:
        time = 2000;
        break;
    case 105:
    case 106:
    case 107:
        time = 0;
        break;
    case 115:
        time = 1;
        break;
    case 130:
        time = 800;
        break;
    default:
        time = 20000;
        break;
    case 150:
        time = 4000;
        break;
    case 120:
    case 125:
    case 135:
    case 146:
    case 152:
        time = 3000;
        break;
    }
    if (param_guess == -2 || param_guess == -3)
        time = 0;
    return time;
}

// FUNCTION: TRILOGY 0x004bdee6
// Looks up the damage associated with a switch type.
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (from Mac debug map)
// @early-stop: zero default uses mov/and instead of xor, adding one instruction and shifting branch targets.
/* fake match: the high-byte mask keeps switchType in ECX while forming the zero default in EAX. */
int EQSwitch::GetSwitchDamage()
{
    int switchType = type;
    int damage = switchType & 0xffffff00;

    if (switchType <= 0x8c)
    {
        if (switchType == 0x8c)
            return 10;

        if (switchType >= 0x73)
        {
            if (0x74 < switchType)
            {
                if (switchType == 0x78 || switchType == 0x7d)
                    return 0x1e;

                if (switchType == 0x82 || switchType == 0x87)
                    return 4;
            }
            else
                return 4;
        }
    }
    else
    {
        switchType -= 0x91;
        if (switchType == 0)
            return 10;

        --switchType;
        if (switchType == 0)
            return 0x32;

        switchType -= 4;
        if (switchType != 0)
        {
            --switchType;
            if (switchType != 0)
            {
                switch (switchType)
                {
                case 1:
                    return 0x32;
                default:
                    return damage;
                }
            }
        }
        return 10;
    }

    return damage;
}

// the old file-local view (layouts/eq_switch.json alias)

// FUNCTION: TRILOGY 0x004bdf41
// Changes a switch's state and triggers its type-specific transition sounds.
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (from Mac debug map)
void EQSwitch::ChangeState(char state, int unused)
{
    u8 switchType;
    u32 time;

    if (state == 3)
    {
        *(float*)((char*)this + 0x88) =
            EQ_METHOD(1, float, 0x004BDD62, (EQSwitch*, float*), this)(0);
    }
    else if (state == 1)
    {
        *(float*)((char*)this + 0x88) = 0.0f;
    }

    switchType = type;
    if (switchType == 0x39)
        return;
    if (switchType == 0x3a)
        return;
    if ((char)this->state == state)
        return;
    this->state = (u8)state;

    switch (switchType)
    {
    case 0:
    case 5:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0x87);
        if ((char)this->state != 3)
            break;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0x88);
        break;
    case 1:
    case 6:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb0);
        if ((char)this->state != 3)
            break;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xaf);
        break;
    case 2:
    case 7:
    case 0x8c:
    case 0x91:
    case 0x92:
    case 0x96:
    case 0x97:
    case 0x98:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb3);
        if ((char)this->state != 3)
            break;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb2);
        break;
    case 0xa:
    case 0xb:
    case 0xc:
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x4e:
    case 0x4f:
    case 0x50:
    case 0x51:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb8);
        if ((char)this->state != 3)
            break;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb7);
        break;
    case 0x23:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0x87);
        if ((char)this->state != 3)
            break;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xbd);
        break;
    case 0x1e:
        if (state == 2 || state == 3)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xad);
        if ((char)this->state != 0)
            break;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xae);
        break;
    case 0x28:
        if (state == 2 || state == 3)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb4);
        break;
    case 0x3b:
        if (state == 2 || state == 3)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb9);
        break;
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x4b:
    case 0x4c:
    case 0x4d:
        if (state == 2 || state == 3)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb5);
        if ((char)this->state != 0)
            break;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xb6);
        break;
    case 0x6e:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xac);
        break;
    case 0x73:
    case 0x74:
    case 0x82:
    case 0x87:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0x77);
        if ((char)this->state != 3)
            break;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0x77);
        break;
    case 0x78:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xbb);
        break;
    case 0x7d:
        if (state == 2)
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), EQ_GLOBAL(void*, 0x006E4DE0))(pos.y, pos.x, pos.z, 0xbc);
        break;
    }

    time = EQ_CDECL(u32, 0x004A461E, (void))();
    *(u32*)((char*)this + 0x84) = time;
}

// FUNCTION: TRILOGY 0x004be347
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (from Mac debug map)
// eqswitch.h lacks the key item (0x5e), lock level (0x4c) and lock param (0x62) fields: raw offsets.
bool EQSwitch::SwitchWasOpened(short keyItem, short skill, void* player)
{
    short damage;
    short lockParam;
    int target;
    const char* message;

    damage = (short)EQ_METHOD(0, int, 0x004BDEE6, (EQSwitch*), this)();
    if (damage > 0)
    {
        if ((short)*(u8*)((char*)this + 0x4c) < skill)
            return true;
        if (player == 0)
            return false;
        if (*(char*)((char*)player + 0x98) == 1)
            return true;
        target = *(int*)((char*)player + 0x74);
        if (target == 0)
            return false;
        message = "You failed to disarm the trap.";
    }
    else
    {
        if (*(short*)((char*)this + 0x5e) == -1 || state == 1)
            return true;
        lockParam = *(short*)((char*)this + 0x62);
        if (lockParam < -1)
        {
            if (keyItem != *(short*)((char*)this + 0x5e))
                return false;
            if (lockParam == skill)
                return true;
            return false;
        }
        if (keyItem == *(short*)((char*)this + 0x5e))
            return true;
        if (skill < 1 || skill == 0xfe)
        {
            if (player == 0)
                return false;
            target = *(int*)((char*)player + 0x74);
            if (target == 0)
                return false;
            message = "It's locked and you're not holding the key.";
        }
        else
        {
            if ((short)*(u8*)((char*)this + 0x4c) < skill)
                return true;
            if (player == 0)
                return false;
            target = *(int*)((char*)player + 0x74);
            if (target == 0)
                return false;
            if (*(u8*)((char*)this + 0x4c) > 0xd1)
                message = "This lock cannot be picked.";
            else
                message = "You are not sufficiently skilled to pick this lock.";
        }
    }
    EQ_CDECL(void, 0x004A07E9, (int, const char*, int))(target, message, 0xd);
    return false;
}

// FUNCTION: TRILOGY 0x004be42c
// The damage gate tests AX; the short argument passes when it is below 1 or exactly 0x00FE.
char EQSwitch::GetSwitchDamage2_guess(s16 value)
{
    int damage = GetSwitchDamage();
    if ((s16)damage != 0)
    {
        damage = 1;
        if (value < damage)
            return (char)damage;
        if (value == 0x00FE)
            return (char)damage;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004be44e
// Uses a door/switch: range check, then sends 0x208D, or if 0x006B6494 is set toggles it and linked switches.
// ORIGINAL FILE: EverQuest/EQSwitch.cpp (from Mac debug map)
// @early-stop: loop id byte spills to [ebp+8] (orig push eax); mov ecx,esi not hoisted above the 0x4A test; 20.0f reads a rdata copy
void EQSwitch::UseSwitch(int playerID, short packetArg, short value)
{
    u8 switchType;
    char result;
    short switchDamage;
    int player;
    int index;
    int foundSwitch;
    float distance;
    char message[12];

    switchType = *(u8*)((char*)this + 0x1c);
    if (switchType == 0x36)
        return;
    if (switchType == 0x37)
        return;
    if (switchType == 0x38)
        return;
    if (((switchType < 0x69) || (0x6b < switchType)) &&
        ((switchDamage = (short)EQ_METHOD(0, int, 0x004BDEE6, (void*), this)()),
         switchDamage == 0))
    {
        if (*(char*)((char*)this + 0x1d) == '\x02')
            return;
        if (*(char*)((char*)this + 0x1d) == '\x03')
            return;
    }
    if (*(char*)((char*)this + 0x1d) == '\x01' &&
        *(short*)((char*)this + 0x62) == -2)
        return;

    if (value > 0x00fe)
        value = 0x00fe;
    player = 0;
    if ((u32)playerID > 0)
    {
        player = (int)EQ_METHOD(1, void*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(playerID);
    }
    if (player != 0)
    {
        result = (char)EQ_METHOD(1, int, 0x004BE42C, (void*, short), this)(value);
        if (result != '\0')
            return;
        distance = EQ_METHOD(7, float, 0x004129F3, (void*, float, float, float, float, float, float, float), EQ_GLOBAL(void*, 0x006E4DE0))(
             *(float*)((char*)player + 0x34),
             *(float*)((char*)player + 0x38),
             *(float*)((char*)player + 0x38),
             *(float*)((char*)this + 0x34),
             *(float*)((char*)this + 0x38),
             *(float*)((char*)this + 0x3c),
             0.0f);
        if (distance > *(float*)((char*)player + 0xd8) + 20.0f)
        {
            EQ_CDECL(int, 0x005229E8, (const char*, ...))
                (EQ_ARRAY(const char, 0x00568D08), (double)distance,
                 (double)*(float*)((char*)player + 0x34),
                 (double)*(float*)((char*)player + 0x38),
                 (double)*(float*)((char*)player + 0x3c),
                 (double)*(float*)((char*)this + 0x34),
                 (double)*(float*)((char*)this + 0x38),
                 (double)*(float*)((char*)this + 0x3c));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
                 EQ_ARRAY(const char, 0x00568CE4));
            return;
        }
    }
    if (EQ_GLOBAL(char, 0x006B63F8) == '\0')
    {
        if (DAT_006b6494 == 0)
        {
            message[0] = *(char*)((char*)this + 0x0c);
            *(short*)(message + 4) = packetArg;
            *(int*)(message + 8) = playerID;
            *(short*)(message + 2) = value;
            send_message_guess(EQ_ARRAY(void*, 0x006B3194), 0x208d, message, 0x0c, 1);
            return;
        }
    }
    else if (DAT_006b6494 == 0)
    {
        return;
    }

    if (*(char*)((char*)this + 0x4a) == '\0')
    {
        result = (char)EQ_METHOD(3, int, 0x004BE347, (void*, short, short, int), this)(packetArg, value, player);
        if (result != '\0')
        {
            switchDamage = (short)EQ_METHOD(0, int, 0x004BDEE6, (void*), this)();
            if (switchDamage != 0)
            {
                EQ_METHOD(2, void, 0x004BDF41, (void*, char, int), this)(4, player);
            }
            else
            {
                if (*(char*)((char*)this + 0x1d) == '\0')
                    EQ_METHOD(2, void, 0x004BDF41, (void*, char, int), this)(2, player);
                else
                    EQ_METHOD(2, void, 0x004BDF41, (void*, char, int), this)(3, player);
            }
        }
    }
    else
    {
        result = (char)EQ_METHOD(3, int, 0x004BE347, (void*, short, short, int), this)(packetArg, value, player);
        if (result != '\0')
        {
            if (*(char*)((char*)this + 0x49) != '\0')
            {
                *(char*)((char*)this + 0x52) =
                    *(char*)((char*)this + 0x52) == '\0';
            }
            else
            {
                if ((u32)playerID > 0)
                    return;
            }
            if ((((*(char*)((char*)this + 0x52) == *(char*)((char*)this + 0x57)) &&
                  (*(char*)((char*)this + 0x53) == *(char*)((char*)this + 0x58))) &&
                 ((*(char*)((char*)this + 0x54) == *(char*)((char*)this + 0x59) &&
                   ((*(char*)((char*)this + 0x55) == *(char*)((char*)this + 0x5a) &&
                     (*(char*)((char*)this + 0x56) == *(char*)((char*)this + 0x5b))))))) ||
                (*(char*)((char*)this + 0x1d) == '\x01'))
            {
                switchDamage = (short)EQ_METHOD(0, int, 0x004BDEE6, (void*), this)();
                if (switchDamage != 0)
                {
                    EQ_METHOD(2, void, 0x004BDF41, (void*, char, int), this)(4, player);
                }
                else
                {
                    if (*(char*)((char*)this + 0x1d) == '\0')
                        EQ_METHOD(2, void, 0x004BDF41, (void*, char, int), this)(2, player);
                    else
                        EQ_METHOD(2, void, 0x004BDF41, (void*, char, int), this)(3, player);
                }
                index = 0;
                do
                {
                    char linkedId = *(char*)((char*)this + 0x64 + index);
                    if (linkedId != -1 && linkedId != *(char*)((char*)this + 0x0c) &&
                        (foundSwitch = (int)EQ_METHOD(1, void*, 0x004BDCC7, (void*, u8), this)((u8)linkedId)) != 0)
                    {
                        char slot = *(char*)((char*)this + 0x4d + index);
                        if (slot >= 0 && slot <= 4)
                        {
                            if (*(char*)((char*)foundSwitch + 0x49) == '\0' || slot != '\0')
                            {
                                *(char*)((char*)foundSwitch + 0x52 + slot) =
                                    *(char*)((char*)foundSwitch + 0x52 + slot) == '\0';
                            }
                            ((EQSwitch*)foundSwitch)->UseSwitch(0, 0, 0x00ff);
                        }
                    }
                    index = index + 1;
                } while (index < 5);
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004dcee4
// Mouse click: finds the door/switch under the cursor and uses it with the slot-0 item number and a skill rank.
int __fastcall ClickedSwitch(void* unused_this, int unused_edx, short screenX, short screenY)
{
    char itemType;
    EQ_Item* pEVar2;
    EQSwitch* pSwitch;
    short switchDamage;
    short temp;
    u16 value;
    struct SwitchMessage_guess
    {
        u32 switch_id;      /* 0x00 */
        short item_nr;      /* 0x04 */
        u16 skill;          /* 0x06 */
    } message;

    value = 0;
    pSwitch = EQ_METHOD(1, EQSwitch*, 0x004BDCA8, (void*, void*), EQ_GLOBAL(void*, 0x006E4D78))(
         EQ_METHOD(3, void*, 0x0041125D, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DE0))(screenX, screenY, 0));
    if (pSwitch != 0)
    {
        temp = -1;
        switchDamage = (short)EQ_METHOD(0, int, 0x004BDEE6, (EQSwitch*), pSwitch)();
        if (switchDamage != 0)
        {
            value = EQ_METHOD(1, u8, 0x0041A3E9, (EQ_Character*, int), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(0x11);
        }
        else
        {
            pEVar2 = EQ_GLOBAL(EQ_Character*, 0x006E4DA8)->inventory_items[0];
            if (pEVar2 != 0)
            {
                if (pEVar2->type == 0)
                {
                    itemType = pEVar2[1].name[0x16];
                    switch (itemType)
                    {
                    case 0x0c:
                        temp = pEVar2->item_nr;
                        value = EQ_METHOD(1, u8, 0x0041A3E9, (EQ_Character*, int), EQ_GLOBAL(EQ_Character*, 0x006E4DA8))(0x23);
                        break;
                    case 0x21:
                        temp = pEVar2->item_nr;
                        value = 0;
                        break;
                    default:
                        if (itemType == 0x27)
                        {
                            temp = pEVar2->item_nr;
                            value = *(u16*)(pEVar2[1].lore + 0x11);
                            if (pSwitch->type == 0x38)
                            {
                                message.switch_id = pSwitch->id_guess;
                                message.item_nr = temp;
                                message.skill = value;
                                send_message_guess(EQ_ARRAY(void*, 0x006B3194), 0x210f, &message, 8, 1);
                            }
                        }
                        else
                        {
                            temp = pEVar2->item_nr;
                            value = 0;
                        }
                        break;
                    }
                }
                else
                {
                    temp = pEVar2->item_nr;
                    value = 0;
                }
            }
        }
        EQ_METHOD(3, void, 0x004BE44E, (EQSwitch*, int, short, short), pSwitch)(EQ_GLOBAL(EQ_Character*, 0x006E4DA8)->player->spawn_id, temp, value);
    }
    return (int)pSwitch;
}
