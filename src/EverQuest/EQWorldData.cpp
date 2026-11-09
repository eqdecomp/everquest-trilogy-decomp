// EQWorldData.cpp - EQWorldData

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

struct GetFullZoneName_guess
{
    u8 hour;                    // 0x00
    u8 minute;                  // 0x01
    u8 day;                     // 0x02
    u8 month;                   // 0x03
    short year;                 // 0x04
    u8 baseHour;                // 0x06
    u8 baseMinute;              // 0x07
    u8 baseDay;                 // 0x08
    u8 baseMonth;               // 0x09
    short baseYear;             // 0x0a
    int lastAdvance;            // 0x0c
    int baseTime;               // 0x10

    void AdvanceTime(int param_2);
    void CurrentGameTime(char* param_2);
};

// ORIGINAL FILE: EverQuest/EQWorldData.cpp (from Mac debug map)
// FUNCTION: TRILOGY 0x004be74b GetFullZoneName_guess::AdvanceTime
// Advance the game clock and calendar from the base date at twenty times real time.
// @early-stop: elapsedYears stays in memory (the original keeps it in EDI), so the divisor constants and the minute loop differ.
/* fake match: remainders written as x -= (x / c) * c; VC6 folds x % c into a second div here, the original reuses the quotient. */
void GetFullZoneName_guess::AdvanceTime(int currentTime)
{
    u8 value;
    short currentYear;
    u32 elapsedYears;
    u32 elapsedMonths;
    u32 elapsedHours;
    u32 remainingSeconds;
    u32 elapsedDays;

    if ((u32)(currentTime - lastAdvance) >= 3000)
    {
        remainingSeconds = EQ_CDECL(int, 0x005213E9, (int *))(0) * 0x14 - baseTime * 0x14;
        elapsedYears = remainingSeconds / 0x1baf800;
        remainingSeconds -= elapsedYears * 0x1baf800;
        elapsedMonths = remainingSeconds / 0x24ea00;
        remainingSeconds -= elapsedMonths * 0x24ea00;
        elapsedDays = remainingSeconds / 0x15180;
        remainingSeconds -= elapsedDays * 0x15180;
        elapsedHours = remainingSeconds / 0xe10;
        value = (char)((remainingSeconds - elapsedHours * 0xe10) / 0x3c) + baseMinute;
        minute = value;
        while (0x3b < value)
        {
            value = value - 0x3c;
            elapsedHours = elapsedHours + 1;
            minute = value;
        }
        value = (baseHour + (char)elapsedHours) - 1;
        hour = value;
        while (0x17 < value)
        {
            hour = hour - 0x18;
            elapsedDays = elapsedDays + 1;
            value = hour;
        }
        hour = hour + 1;
        value = (baseDay + (char)elapsedDays) - 1;
        day = value;
        while (0x1b < value)
        {
            elapsedMonths = elapsedMonths + 1;
            day = day - 0x1c;
            value = day;
        }
        day = day + 1;
        value = baseMonth + (char)elapsedMonths;
        month = value;
        while (0xc < value)
        {
            month = month - 0xc;
            elapsedYears = elapsedYears + 1;
            value = month;
        }
        currentYear = baseYear + (short)elapsedYears;
        year = currentYear;
        if (9999 < currentYear)
        {
            year = 0;
        }
        lastAdvance = currentTime;
    }
    return;
}

// FUNCTION: TRILOGY 0x004be864
// Format the game time and date with AM/PM, weekday, day ordinal, month and year for display.
// ORIGINAL FILE: EverQuest/EQWorldData.cpp (from Mac debug map)
void GetFullZoneName_guess::CurrentGameTime(char* dst)
{
    u8 value;
    struct
    {
        char weekday[60];
        char month[60];
        char ordinal[12];
        char time[12];
    } localStrings;

    value = hour;
    if (value == 0xc)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
            localStrings.time, (char*)0x00568f4c, (u32)0xc);
    }
    else if (value == 0x18)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
            localStrings.time, (char*)0x00568f44, (u32)0xc);
    }
    else if (0xc < value)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
            localStrings.time, (char*)0x00568f4c, (u32)value - 0xc);
    }
    else
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
            localStrings.time, (char*)0x00568f44, (u32)value);
    }

    value = day;
    switch (value)
    {
    case 1:
    case 0x15:
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
            localStrings.ordinal, (char*)0x00568f24, (int)value);
        break;
    case 2:
    case 0x16:
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
            localStrings.ordinal, (char*)0x00568f2c, (int)value);
        break;
    case 3:
    case 0x17:
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
            localStrings.ordinal, (char*)0x00568f34, (int)value);
        break;
    default:
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
            localStrings.ordinal, (char*)0x00568f3c, (int)value);
        break;
    }

    value = day;
    if (value > 0x15)
    {
        value = value - 0x15;
    }
    else if (value > 0xe)
    {
        value = value - 0xe;
    }
    else if (7 < value)
    {
        value = value - 7;
    }
    switch (value)
    {
    case 1:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.weekday, (char*)0x00568ee0);
        break;
    case 2:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.weekday, (char*)0x00568ee8);
        break;
    case 3:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.weekday, (char*)0x00568ef0);
        break;
    case 4:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.weekday, (char*)0x00568ef8);
        break;
    case 5:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.weekday, (char*)0x00568f04);
        break;
    case 6:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.weekday, (char*)0x00568f10);
        break;
    case 7:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.weekday, (char*)0x00568f18);
        break;
    default:
        break;
    }

    switch (month)
    {
    case 1:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568ed8);
        break;
    case 2:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568ecc);
        break;
    case 3:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568ec4);
        break;
    case 4:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568ebc);
        break;
    case 5:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568eb8);
        break;
    case 6:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568eb0);
        break;
    case 7:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568ea8);
        break;
    case 8:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568ea0);
        break;
    case 9:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568e94);
        break;
    case 10:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568e8c);
        break;
    case 0xb:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568e80);
        break;
    case 0xc:
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(localStrings.month, (char*)0x00568e74);
        break;
    default:
        break;
    }
    EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(
        dst,
        (char*)0x00568e40,
        localStrings.time,
        localStrings.weekday,
        localStrings.ordinal,
        localStrings.month,
        (int)year);
    return;
}
