typedef int (__cdecl *sprintf_guess_t)(char *buffer, const char *format, ...);

// FUNCTION: TRILOGY 0x004f73c0
// Translates network socket errors into diagnostic text.
// ORIGINAL FILE: Network/rdp/ustrerror.c (from Mac debug map)
char *net_strerror(unsigned int socketError)
{
    char *message = (char *)0x006EFED8;
    switch (socketError)
    {
    case 0x2736:
        ((sprintf_guess_t)0x005224FB)((char *)0x006EFED8, (const char *)0x0057F69C, socketError);
        return (char *)0x006EFED8;
    case 0x2747:
        ((sprintf_guess_t)0x005224FB)((char *)0x006EFED8, (const char *)0x0057F69C, socketError);
        return (char *)0x006EFED8;
    case 0x2714: message = (char *)0x0057F970; break;
    case 0x2719: message = (char *)0x0057F964; return message;
    case 0x271d: message = (char *)0x0057F958; return message;
    case 0x271e: message = (char *)0x0057F94c; return message;
    case 0x2726: message = (char *)0x0057F940; return message;
    case 0x2728: message = (char *)0x0057F934; return message;
    case 0x2733: message = (char *)0x0057F924; return message;
    case 0x2734: message = (char *)0x0057F914; return message;
    case 0x2735: message = (char *)0x0057F908; return message;
    case 0x2737: message = (char *)0x0057F8F8; return message;
    case 0x2738: message = (char *)0x0057F8EC; return message;
    case 0x2739: message = (char *)0x0057F8DC; return message;
    case 0x273a: message = (char *)0x0057F8CC; return message;
    case 0x273b: message = (char *)0x0057F8B8; return message;
    case 0x273c: message = (char *)0x0057F8A4; return message;
    case 0x273d: message = (char *)0x0057F894; return message;
    case 0x273e: message = (char *)0x0057F884; return message;
    case 0x273f: message = (char *)0x0057F874; return message;
    case 0x2740: message = (char *)0x0057F864; return message;
    case 0x2741: message = (char *)0x0057F850; return message;
    case 0x2742: message = (char *)0x0057F844; return message;
    case 0x2743: message = (char *)0x0057F834; return message;
    case 0x2744: message = (char *)0x0057F824; return message;
    case 0x2745: message = (char *)0x0057F814; return message;
    case 0x2746: message = (char *)0x0057F804; return message;
    case 0x2748: message = (char *)0x0057F7F8; return message;
    case 0x2749: message = (char *)0x0057F7EC; return message;
    case 0x274a: message = (char *)0x0057F7DC; return message;
    case 0x274b: message = (char *)0x0057F7CC; return message;
    case 0x274c: message = (char *)0x0057F7BC; return message;
    case 0x274d: message = (char *)0x0057F7AC; return message;
    case 0x274e: message = (char *)0x0057F7A0; return message;
    case 0x274f: message = (char *)0x0057F790; return message;
    case 0x2750: message = (char *)0x0057F780; return message;
    case 0x2751: message = (char *)0x0057F770; return message;
    case 0x2752: message = (char *)0x0057F760; return message;
    case 0x2753: message = (char *)0x0057F754; return message;
    case 0x2754: message = (char *)0x0057F748; return message;
    case 0x2755: message = (char *)0x0057F73C; return message;
    case 0x2756: message = (char *)0x0057F730; return message;
    case 0x2757: message = (char *)0x0057F724; return message;
    case 0x276b: message = (char *)0x0057F714; return message;
    case 0x276c: message = (char *)0x0057F700; return message;
    case 0x276d: message = (char *)0x0057F6EC; return message;
    case 0x2775: message = (char *)0x0057F6E0; return message;
    case 0x2afd:
        ((sprintf_guess_t)0x005224FB)((char *)0x006EFED8, (const char *)0x0057F69C, socketError);
        return (char *)0x006EFED8;
    case 0x2afe:
        ((sprintf_guess_t)0x005224FB)((char *)0x006EFED8, (const char *)0x0057F69C, socketError);
        return (char *)0x006EFED8;
    case 0x2aff:
        ((sprintf_guess_t)0x005224FB)((char *)0x006EFED8, (const char *)0x0057F69C, socketError);
        return (char *)0x006EFED8;
    case 0x2b00:
        ((sprintf_guess_t)0x005224FB)((char *)0x006EFED8, (const char *)0x0057F69C, socketError);
        return (char *)0x006EFED8;
    case 0x2af9: message = (char *)0x0057F6CC; return message;
    case 0x2afa: message = (char *)0x0057F6BC; return message;
    case 0x2afb: message = (char *)0x0057F6AC; return message;
    case 0x2afc: message = (char *)0x0057F6A0; return message;
    default:
        ((sprintf_guess_t)0x005224FB)((char *)0x006EFED8, (const char *)0x0057F69C, socketError);
        break;
    }
    return message;
}
