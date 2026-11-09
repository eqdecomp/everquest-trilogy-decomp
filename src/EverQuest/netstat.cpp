#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

typedef void (__cdecl *t3dOverlayLock_guess_t)(int surface, int* pixels, unsigned int* pitch);
typedef void (__cdecl *t3dOverlayUnlock_guess_t)(int surface);

// FUNCTION: TRILOGY 0x004e06d1
// Renders the packet history bar for 16-bit color mode.
// @early-stop: 0.7475; offset frame, palette registers, and pixel-loop scheduling still differ.
void DrawPacketLossBar16_guess(int surfaceHandle, unsigned short x, unsigned short y,
                  unsigned short tickOffset, unsigned __int64 packetMask,
                  int direction, int colorId)
{
    unsigned short dimColor;
    unsigned int green;
    int greenMid;
    unsigned short midColor;
    unsigned int tmp;
    unsigned short* dst;
    unsigned char bStack0000000b;
    unsigned char bStack0000000f;
    struct LockResult {
        int pixels;
        unsigned int pitch;
    } lockResult;
    unsigned char bitArray[68];
    int rowStart;
    unsigned int pitch;

    if (surfaceHandle != 0) {
        bitArray[0x40] = 0;
        if (0 < direction) {
            tmp = 0;
            do {
                bitArray[tmp] = (packetMask & 0x8000000000000000ui64) != 0;
                packetMask = packetMask << 1;
                tmp = tmp + 1;
            } while (tmp < 0x40);
        }
        else {
            tmp = 0;
            do {
                bitArray[tmp] = (unsigned char)(packetMask & 1);
                packetMask = packetMask >> 1;
                tmp = tmp + 1;
            } while (tmp < 0x40);
        }
        if (0 < direction) {
            tickOffset = 7 - tickOffset;
        }
        green = 0;
        tmp = 0;
        switch (colorId) {
        case 15:
            tmp = 0xff;
            green = 0xff;
            break;
        case 13:
            tmp = 0xff;
            break;
        case 2:
            green = 0xff;
            break;
        }
        if (*(int*)(*(unsigned int*)0x006e50a0 + 0x7a1c) != 0) {
            colorId = (((tmp & 0xfff8) << 6 | green & 0x1f8) << 2);
            midColor = (unsigned short)((tmp * 3 & 0x3e0) << 6) | (short)green * 3 & 0x7e0U;
            dimColor = (unsigned short)(((tmp & 0xfff0) << 6 | green & 0x3f0) << 1);
        }
        else {
            greenMid = (green * 3 & 0x1f0) << 1;
            colorId = (int)(unsigned short)(((tmp & 0xfff8 | 0xff00) << 4 | green & 0x7c) << 3);
            midColor = (unsigned short)(((tmp * 3 << 5) & 0xfc00) | greenMid | 0x8000);
            dimColor = (unsigned short)(((unsigned int)((unsigned short)tmp & 0xfff0 | 0xfe00) << 4 | green & 0xf8) << 2);
        }
        EQ_GLOBAL(t3dOverlayLock_guess_t, 0x006E5348)(
            surfaceHandle, &lockResult.pixels, &lockResult.pitch);
        pitch = lockResult.pitch >> 1;
        rowStart = lockResult.pixels
            + (y * pitch + (unsigned int)x) * 2;
        tickOffset = tickOffset & 0xffff;
        if (tickOffset < 0x40) {
            dst = (unsigned short*)(rowStart + tickOffset * 2);
            tmp = 0x47 - tickOffset >> 3;
            do {
                *dst = dimColor;
                dst = dst + 8;
                tmp = tmp - 1;
            } while (tmp != 0);
        }
        bStack0000000f = 0;
        dst = (unsigned short*)(rowStart + pitch * 2);
        tickOffset = 0;
        do {
            bStack0000000b = bitArray[tickOffset + 1];
            if (bitArray[0] != 0) {
                if (bStack0000000f != 0 && bStack0000000b != 0) {
                    dst[pitch] = (unsigned short)colorId;
                    *dst = midColor;
                    dst[pitch * 2] = midColor;
                }
                else {
                    if (bStack0000000f == 0 && bStack0000000b == 0)
                        dst[pitch] = dimColor;
                    else
                        dst[pitch] = midColor;
                    *dst = dimColor;
                    dst[pitch * 2] = dimColor;
                }
            }
            dst = dst + 1;
            bStack0000000f = bitArray[0];
            packetMask = packetMask << 1;
            tickOffset = tickOffset + 1;
            bitArray[0] = bStack0000000b;
        } while (tickOffset < 0x40);
        EQ_GLOBAL(t3dOverlayUnlock_guess_t, 0x006E5350)(surfaceHandle);
    }
}

// FUNCTION: TRILOGY 0x004e08ff
// Renders the packet history bar for 32-bit color mode.
// ORIGINAL FILE: EverQuest/netstat.cpp (from Mac debug map)
// @early-stop: color register allocation and the post-lock pixel loop still differ after eight iterations without a higher match.
void __cdecl DrawPacketLossBar(int surfaceHandle, unsigned short x, unsigned short y,
                                     unsigned int tickOffset, unsigned __int64 packetMask,
                                     int direction, unsigned int colorId)
{
    unsigned int dimColor;
    unsigned int stride;
    unsigned int tickCount;
    unsigned int tmp;
    unsigned int* dst;
    unsigned char bStack0000000b;
    unsigned char bStack0000000f;
    unsigned char bitArray[68];
    unsigned int brightColor;
    int rowStart;

    if (surfaceHandle != 0) {
        bitArray[0x40] = 0;
        if (direction > 0) {
            tmp = 0;
            do {
                bitArray[tmp] = (packetMask & 0x8000000000000000ui64) != 0;
                packetMask = packetMask << 1;
                tmp = tmp + 1;
            } while (tmp < 0x40);
        }
        else {
            tmp = 0;
            do {
                bitArray[tmp] = (unsigned char)(packetMask & 1);
                packetMask = packetMask >> 1;
                tmp = tmp + 1;
            } while (tmp < 0x40);
        }
        if (0 < direction) {
            tickOffset = 7 - tickOffset;
        }
        stride = 0;
        tmp = 0;
        switch (colorId) {
        case 2:
            stride = 0xff;
            break;
        case 0xd:
            tmp = 0xff;
            break;
        case 0xf:
            tmp = 0xff;
            stride = 0xff;
            break;
        default:
            break;
        }
        brightColor = ((tmp | 0xffffff00) << 8 | stride) << 8;
        dimColor = ((tmp & 0xfffffffe | 0xfffffe00) << 8 | stride & 0xfffffffe) << 7;
        tmp = tmp * 0xc000 & 0xffff0000 | stride * 0xc0 & 0xffffff00 | 0xff000000;
        EQ_GLOBAL(t3dOverlayLock_guess_t, 0x006E5348)(surfaceHandle, &rowStart, &colorId);
        stride = colorId >> 2;
        rowStart = rowStart + (y * stride + (unsigned int)x) * 4;
        tickOffset = tickOffset & 0xffff;
        if (tickOffset < 0x40) {
            dst = (unsigned int*)(rowStart + tickOffset * 4);
            tickCount = 0x47 - tickOffset >> 3;
            do {
                *dst = dimColor;
                dst = dst + 8;
                tickCount = tickCount - 1;
            } while (tickCount != 0);
        }
        bStack0000000f = 0;
        dst = (unsigned int*)(rowStart + stride * 4);
        tickOffset = 0;
        do {
            bStack0000000b = bitArray[tickOffset + 1];
            if (bitArray[0] != 0) {
                if (bStack0000000f != 0 && bStack0000000b != 0) {
                    dst[stride] = brightColor;
                    *dst = tmp;
                    dst[stride * 2] = tmp;
                }
                else {
                    if (bStack0000000f == 0 && bStack0000000b == 0)
                        dst[stride] = dimColor;
                    else
                        dst[stride] = tmp;
                    *dst = dimColor;
                    dst[stride * 2] = dimColor;
                }
            }
            dst = dst + 1;
            bStack0000000f = bitArray[0];
            packetMask = packetMask << 1;
            tickOffset = tickOffset + 1;
            bitArray[0] = bStack0000000b;
        } while (tickOffset < 0x40);
        EQ_GLOBAL(t3dOverlayUnlock_guess_t, 0x006E5350)(surfaceHandle);
    }
    return;
}

// FUNCTION: TRILOGY 0x004e0ac2
// __cdecl. Population count of a 64-bit value (low byte of the count).
char __cdecl LODWORD_countSetBits_guess(unsigned __int64 packetMask)
{
    char count;

    count = 0;
    while (packetMask != 0) {
        count = count + (char)((unsigned char)packetMask & 1);
        packetMask = packetMask >> 1;
    }
    return count;
}

typedef void (__cdecl *t3dOverlayClear_guess_t)(int surface, int color);
typedef unsigned int (__stdcall *timeGetTime_guess_t)(void);   // 0x00534278 is the IAT slot of WINMM timeGetTime

// FUNCTION: TRILOGY 0x004e0af0
// Shows players connection responsiveness and recent packet delivery quality.
// ORIGINAL FILE: EverQuest/netstat.cpp (from Mac debug map)
// __cdecl. Draws the net status line: the ping text, two packet-loss percentages with their bit bars.
// conn is the connection handed to connection_get_perf_stats (0x004F23E0); secondPacketMask is the 64-bit second
// mask. Bug kept: the final HD text reuses the last bar colour while the GDI text uses 2.
/* fake match: lossColor holds the bar colour, baseY keeps y; displayLossPercent is stored inside the compares and elapsedMs takes the age first. */
void __cdecl DrawNetStatus(int hdc, int x, int lossColor, unsigned short xOffset,
                                 unsigned short yOffset, int conn, unsigned short barIndex,
                                 unsigned __int64 secondPacketMask, int ping)
{
    char text[80];
    unsigned int stats[10];
    double displayLossPercent;
    unsigned int now;
    float firstLossPercent;
    float secondLossPercent;
    unsigned int packetIndex;
    unsigned __int64 firstPacketMask;
    unsigned int elapsedMs;
    unsigned char bits;
    int color;
    int y;
    int baseY;

    if (hdc != 0 && (EQ_GLOBAL(int, 0x006E5480) != 1 || EQ_GLOBAL(int, 0x006E4E68) != 0)) {
        now = EQ_GLOBAL(timeGetTime_guess_t, 0x00534278)();
        EQ_CDECL(void, 0x004F23E0, (int, unsigned int*))(conn, stats);
        elapsedMs = now - stats[0];
        packetIndex = stats[4];
        firstPacketMask = *(unsigned __int64*)&stats[2];
        if (elapsedMs > stats[7] + 500) {
            elapsedMs = elapsedMs - stats[7];
            firstPacketMask = firstPacketMask << (elapsedMs / 500);
            packetIndex = elapsedMs / 500 + stats[4];
        }
        bits = EQ_CDECL(char, 0x004E0AC2, (unsigned __int64))(firstPacketMask);
        firstLossPercent = (1.0f - (float)bits * 0.015625f) * 100.0f;
        bits = EQ_CDECL(char, 0x004E0AC2, (unsigned __int64))(secondPacketMask);
        secondLossPercent = (1.0f - (float)bits * 0.015625f) * 100.0f;
        EQ_GLOBAL(t3dOverlayClear_guess_t, 0x006E5338)(hdc, 0xff00ff);
        if (stats[7] < 600)
            color = 2;
        else
            color = (stats[7] < 900) ? 0xf : 0xd;
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(text, EQ_ARRAY(char, 0x005765AC), stats[7]);
        baseY = lossColor;
        y = (short)baseY;
        if (EQ_GLOBAL(int, 0x006E5480) == 1) {
            EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, (short)x + xOffset, yOffset + y,
                 color, EQ_GLOBAL(int, 0x006EFDFC), 1);
        } else {
            EQ_CDECL(short, 0x0040BE28, (int, char*, int, int, int, int))
                (hdc, text, (short)x, y, (short)color, EQ_GLOBAL(int, 0x006EFDFC));
        }
        if ((displayLossPercent = (double)firstLossPercent) < 35.0) {
            lossColor = 2;
        } else {
            lossColor = 0xf;
            if ((displayLossPercent = (double)firstLossPercent) >= 45.0)
                lossColor = 0xd;
        }
        if (*(int*)(EQ_GLOBAL(int, 0x006E50A0) + 0x7a18) == 0x20 && EQ_GLOBAL(int, 0x006E5480) == 0) {
            EQ_CDECL(void, 0x004E08FF, (int, int, int, int, unsigned __int64, int, int))
                (hdc, x, baseY + 0xe, (unsigned short)packetIndex % 8, firstPacketMask, 1, (short)lossColor);
        } else {
            EQ_CDECL(void, 0x004E06D1, (int, int, int, int, unsigned int, unsigned int, int, int))
                (hdc, x, baseY + 0xe, (unsigned short)packetIndex % 8, (unsigned int)firstPacketMask,
                 (unsigned int)(firstPacketMask >> 32), 1, (short)lossColor);
        }
        x += 0x20;
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(text, EQ_ARRAY(char, 0x005765A4), displayLossPercent);
        if (EQ_GLOBAL(int, 0x006E5480) == 1) {
            EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, (short)x + xOffset, yOffset + y,
                 lossColor, EQ_GLOBAL(int, 0x006EFDFC), 1);
        } else {
            EQ_CDECL(short, 0x0040BE28, (int, char*, int, int, int, int))
                (hdc, text, (short)x, y, (short)lossColor, EQ_GLOBAL(int, 0x006EFDFC));
        }
        x += 0x20;
        if (now - stats[0] < 15000) {
            if (secondLossPercent < 35.0) {
                lossColor = 2;
            } else {
                lossColor = 0xf;
                if (secondLossPercent >= 45.0)
                    lossColor = 0xd;
            }
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (text, EQ_ARRAY(char, 0x005765A4), (double)secondLossPercent);
            if (EQ_GLOBAL(int, 0x006E5480) == 1) {
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, (short)x + xOffset, yOffset + y,
                     lossColor, EQ_GLOBAL(int, 0x006EFDFC), 1);
            } else {
                EQ_CDECL(short, 0x0040BE28, (int, char*, int, int, int, int))
                    (hdc, text, (short)x, y, (short)lossColor, EQ_GLOBAL(int, 0x006EFDFC));
            }
            if (*(int*)(EQ_GLOBAL(int, 0x006E50A0) + 0x7a18) == 0x20 && EQ_GLOBAL(int, 0x006E5480) == 0) {
                EQ_CDECL(void, 0x004E08FF, (int, int, int, int, unsigned __int64, int, int))
                    (hdc, x, baseY + 0xe, barIndex % 8, secondPacketMask, -1, (short)lossColor);
            } else {
                EQ_CDECL(void, 0x004E06D1, (int, int, int, int, unsigned int, unsigned int, int, int))
                    (hdc, x, baseY + 0xe, barIndex % 8, (unsigned int)secondPacketMask,
                     (unsigned int)(secondPacketMask >> 32), -1, (short)lossColor);
            }
            x += 0x20;
        }
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))(text, EQ_ARRAY(char, 0x005765AC), ping);
        if (EQ_GLOBAL(int, 0x006E5480) == 1) {
            EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text, (short)x + xOffset, yOffset + y,
                 lossColor, EQ_GLOBAL(int, 0x006EFDFC), 1);
        } else {
            EQ_CDECL(short, 0x0040BE28, (int, char*, int, int, int, int))
                (hdc, text, (short)x, y, 2, EQ_GLOBAL(int, 0x006EFDFC));
        }
    }
}
