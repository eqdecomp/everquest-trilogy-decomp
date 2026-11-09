// unit_1000F230.cpp - eqmain.dll, the address run 0x1000F230..0x1000F2FF (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include <string.h>
#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x1000f230
// Feeds buffered data to front-end asset loading.
// __thiscall (this in ECX; the EDX parameter is the dummy), 2 stack args (copiedBytes = dst, dstCursor = count), callee pops
// 8 bytes. Copies min(count, end - pos) bytes from the buffered stream at *self to dst and returns how many were copied;
// 0 when pos >= end. Stream fields as offsets from *self: +8 = a 0x2000-byte block, +0x2008 = end, +0x200c = pos,
// +0x2010 = index into the block. Each chunk is copied from the index up to the block's end; when the block is used up
// and bytes are still wanted, the stream's vtable slot 0 (refill) runs and the index goes back to 0. As in the
// original, the result and the write cursor live in the two argument slots (copiedBytes and dstCursor are reused).
int __fastcall buffered_read_guess(char *holder, int edx_unused, char *copiedBytes, unsigned int dstCursor)
{
    unsigned int end;
    unsigned int pos;
    unsigned int avail;
    unsigned int n;
    unsigned int fill;
    unsigned int room;
    unsigned int chunk;
    char *dst;

    end = *(unsigned int *)(*(char **)holder + 0x2008);
    pos = *(unsigned int *)(*(char **)holder + 0x200c);
    if (end > pos)
    {
        avail = end - pos;
        n = dstCursor;
        if (n > avail)
            n = avail;
        dst = copiedBytes;
        copiedBytes = 0;
        dstCursor = (unsigned int)dst;
        if (n != 0)
        {
            do
            {
                fill = *(unsigned int *)(*(char **)holder + 0x2010);
                room = 0x2000 - fill;
                if (room != 0)
                {
                    chunk = room;
                    if (n < chunk)
                        chunk = n;
                    memcpy((char *)dstCursor, *(char **)holder + 8 + fill, chunk);
                    n = n - chunk;
                    copiedBytes = copiedBytes + chunk;
                    *(unsigned int *)(*(char **)holder + 0x2010) = *(unsigned int *)(*(char **)holder + 0x2010) + chunk;
                    *(unsigned int *)(*(char **)holder + 0x200c) = *(unsigned int *)(*(char **)holder + 0x200c) + chunk;
                    dstCursor = dstCursor + chunk;
                }
                if (n > 0)
                {
                    ((void (__fastcall *)(void *))(**(void ***)*(char **)holder))(*(char **)holder);
                    *(unsigned int *)(*(char **)holder + 0x2010) = 0;
                }
            } while (n != 0);
        }
        return (int)copiedBytes;
    }
    return 0;
}

// Stand-in for std::_Lockit (VC6 <yvals.h>): no data, its ctor and dtor are called by address.
struct FlagContainerLockit_guess
{
    FlagContainerLockit_guess() { EQ_METHOD(0, void, 0x1003E980, (FlagContainerLockit_guess *), this)(); }
    ~FlagContainerLockit_guess() { EQ_METHOD(0, void, 0x1003EA1C, (FlagContainerLockit_guess *), this)(); }
};

#define g_flagcontainer_nil_eqmain_guess     EQ_GLOBAL(char *, 0x1004CF14)
#define g_flagcontainer_nilrefs_eqmain_guess EQ_GLOBAL(int, 0x1004CF10)

// FUNCTION: EQMAIN 0x1000f300
// Constructs an empty MSVC6 std::_Tree (map/set, unique keys): allocator, compare, shared nil node and head.
// @early-stop: nil-node right link stores through tmp instead of reloading the global nil pointer.
// __thiscall (this in ECX; the EDX parameter is the dummy), 2 stack args (secondByteSource = compare, firstByteSource = allocator), callee pops
// 8 bytes. Copies *firstByteSource to +0 and *secondByteSource to +1, clears +8, buys a 0x18-byte node (+4 = 0, +0x14 = 1). Under the lock it
// makes that node the shared nil node (0x1004cf14, left and right cleared) when there is none, else frees it; then it bumps
// the nil's count (0x1004cf10). Outside the lock it buys a second node (parent = nil, colour 0) as the head (+4), points its
// left and the head's right at itself, clears +0xc and returns this. Node: +0 left, +4 parent, +8 right, +0x14 colour.
char *__fastcall FlagContainer_initializeFlagContainer_guess(char *self, int edx_unused, const char *secondByteSource, const char *firstByteSource)
{
    struct FlagContainerNode_guess
    {
        FlagContainerNode_guess *left;
        FlagContainerNode_guess *parent;
        FlagContainerNode_guess *right;
        char value[8];
        unsigned int color;
    };
    FlagContainerNode_guess *tmp;
    FlagContainerNode_guess *node;

    *(unsigned char *)self = *firstByteSource;
    *(unsigned char *)(self + 1) = *secondByteSource;
    *(unsigned char *)(self + 8) = 0;
    tmp = (FlagContainerNode_guess *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x18);
    node = tmp;
    tmp->parent = 0;
    tmp->color = 1;
    {
        FlagContainerLockit_guess lk;

        if (g_flagcontainer_nil_eqmain_guess == 0)
        {
            g_flagcontainer_nil_eqmain_guess = (char *)tmp;
            tmp->left = 0;
            node = 0;
            tmp->right = 0;
        }
        g_flagcontainer_nilrefs_eqmain_guess = g_flagcontainer_nilrefs_eqmain_guess + 1;
    }
    if (node != 0)
    {
        EQ_CDECL(void, 0x10033800, (void *))(node);
    }
    tmp = (FlagContainerNode_guess *)g_flagcontainer_nil_eqmain_guess;
    node = (FlagContainerNode_guess *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x18);
    node->parent = tmp;
    node->color = 0;
    *(FlagContainerNode_guess **)(self + 4) = node;
    *(unsigned int *)(self + 0xc) = 0;
    node->left = node;
    (*(FlagContainerNode_guess **)(self + 4))->right = *(FlagContainerNode_guess **)(self + 4);
    return self;
}
