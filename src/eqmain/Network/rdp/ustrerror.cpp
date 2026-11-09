// eqmain: the same original object as trilogy's src/Network/rdp/ustrerror.cpp (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.

#include <stdio.h>

// FUNCTION: EQMAIN 0x10030660
// Cloned from trilogy net_strerror_guess 0x004F73C0 (the same original code: BinDiff sibling, names/siblings/).
char *net_strerror_guess(unsigned int socketError)
{
    char *message = (char *)0x1013C9F0;
    switch (socketError)
    {
    case 0x2714: message = (char *)0x10049BC0; break;
    case 0x2719: message = (char *)0x10049BB4; break;
    case 0x271d: message = (char *)0x10049BA8; break;
    case 0x271e: message = (char *)0x10049B9C; break;
    case 0x2726: message = (char *)0x10049B90; break;
    case 0x2728: message = (char *)0x10049B84; break;
    case 0x2733: message = (char *)0x10049B74; break;
    case 0x2734: message = (char *)0x10049B64; break;
    case 0x2735: message = (char *)0x10049B58; break;
    case 0x2737: message = (char *)0x10049B48; break;
    case 0x2738: message = (char *)0x10049B3C; break;
    case 0x2739: message = (char *)0x10049B2C; break;
    case 0x273a: message = (char *)0x10049B1C; break;
    case 0x273b: message = (char *)0x10049B08; break;
    case 0x273c: message = (char *)0x10049AF4; break;
    case 0x273d: message = (char *)0x10049AE4; break;
    case 0x273e: message = (char *)0x10049AD4; break;
    case 0x273f: message = (char *)0x10049AC4; break;
    case 0x2740: message = (char *)0x10049AB4; break;
    case 0x2741: message = (char *)0x10049AA0; break;
    case 0x2742: message = (char *)0x10049A94; break;
    case 0x2743: message = (char *)0x10049A84; break;
    case 0x2744: message = (char *)0x10049A74; break;
    case 0x2745: message = (char *)0x10049A64; break;
    case 0x2746: message = (char *)0x10049A54; break;
    case 0x2748: message = (char *)0x10049A48; break;
    case 0x2749: message = (char *)0x10049A3C; break;
    case 0x274a: message = (char *)0x10049A2C; break;
    case 0x274b: message = (char *)0x10049A1C; break;
    case 0x274c: message = (char *)0x10049A0C; break;
    case 0x274d: message = (char *)0x100499FC; break;
    case 0x274e: message = (char *)0x100499F0; break;
    case 0x274f: message = (char *)0x100499E0; break;
    case 0x2750: message = (char *)0x100499D0; break;
    case 0x2751: message = (char *)0x100499C0; break;
    case 0x2752: message = (char *)0x100499B0; break;
    case 0x2753: message = (char *)0x100499A4; break;
    case 0x2754: message = (char *)0x10049998; break;
    case 0x2755: message = (char *)0x1004998C; break;
    case 0x2756: message = (char *)0x10049980; break;
    case 0x2757: message = (char *)0x10049974; break;
    case 0x276b: message = (char *)0x10049964; break;
    case 0x276c: message = (char *)0x10049950; break;
    case 0x276d: message = (char *)0x1004993C; break;
    case 0x2775: message = (char *)0x10049930; break;
    case 0x2af9: message = (char *)0x1004991C; break;
    case 0x2afa: message = (char *)0x1004990C; break;
    case 0x2afb: message = (char *)0x100498FC; break;
    case 0x2afc: message = (char *)0x100498F0; break;
    default:
        sprintf((char *)0x1013C9F0, (const char *)0x100498EC, socketError);
        break;
    }
    return message;
}
