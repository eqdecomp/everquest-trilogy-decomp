// QUIRK: vc6q-byte-temp-width
// KEYWORDS: byte, temporary, widening, zeroing, regalloc, /O2
// VARIANTS: BYTE WIDE
// CFLAGS: /O2
// EXPECT: decode differs BYTE WIDE
// EXPECT@BYTE: decode has mov al, byte ptr \[edx\]\nmov cl, al\nand ecx, 0x80
// EXPECT@BYTE: decode lacks xor eax, eax
// EXPECT@WIDE: decode has xor eax, eax\nmov al, byte ptr \[edx\]\nmov ecx, eax\nand ecx, 0x80
void decode(const unsigned char *p, unsigned char *out)
{
#ifdef BYTE
    unsigned char c = *p;
#else
    unsigned int c = *p;
#endif
    int mask = c & 0x80;
    if (mask) {
        if (mask != 0x80) return;
        *out = ((p[1] ^ c) & 0x3f) ^ c;
    } else {
        *out = c;
    }
}
