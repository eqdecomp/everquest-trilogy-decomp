/* eqd_crt.c - CRT helpers eqgame.exe links its own version of. Linked into eqdecomp.dll (hooks\eqdecomp.py)
 * and into every Unicorn test DLL (tests\unicorn\conftest.py, Harness extra_sources), so a rewrite's
 * compiler-generated helper calls run the client's code, not VC6 LIBC's.
 *
 * __ftol (the compiler's float -> integer conversion behind every `(int)f` / `(long)f` cast): the client's is
 * at 0x004EF0CF, a fast truncating version that never touches the FPU control word (fistp of the value, then
 * fixes the rounding by the sign of the difference; returns the 64-bit result in EDX:EAX, pops ST0). It is
 * not VC6 LIBC's ftol (fnstcw/fldcw/fistp), and the two disagree outside the int range. Copied instruction
 * for instruction; tests\unicorn\test_crt_defaults.py checks it against the client's bytes.
 * C, no annotation: not a decomp source (the hook build only scans src\).
 */

__declspec(naked) void _ftol(void)
{
    __asm {
        lea     ecx, [esp - 8]
        sub     esp, 0x10
        and     ecx, 0xfffffff8
        fld     st(0)
        fistp   qword ptr [ecx]
        fild    qword ptr [ecx]
        mov     edx, dword ptr [ecx + 4]
        mov     eax, dword ptr [ecx]
        test    eax, eax
        je      integer_zero
    not_zero:
        fsubp   st(1), st
        test    edx, edx
        je      positive
        fstp    dword ptr [ecx]
        mov     ecx, dword ptr [ecx]
        add     esp, 0x10
        xor     ecx, 0x80000000
        add     ecx, 0x7fffffff
        adc     eax, 0
        ret
    positive:
        fstp    dword ptr [ecx]
        mov     ecx, dword ptr [ecx]
        add     esp, 0x10
        add     ecx, 0x7fffffff
        sbb     eax, 0
        ret
    integer_zero:
        test    edx, 0x7fffffff
        jne     not_zero
        fstp    dword ptr [ecx]
        fstp    dword ptr [ecx]
        add     esp, 0x10
        ret
    }
}
