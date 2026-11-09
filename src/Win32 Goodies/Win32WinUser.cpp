// Win32WinUser.cpp - thin wrappers over Win32 window calls.
// ORIGINAL FILE: GameLib/Win32 Goodies/Win32WinUser.cp (from Mac debug map)

#include <windows.h>

#include "trilogy/eqaddr.h"

#pragma comment(lib, "user32.lib")     // SendMessageA

// Local stand-in: the owning class is unknown and no header defines the HWND at 0x287c (CDisplay is the caller).
class DisplayWnd_guess
{
public:
    char pad_guess[0x287c];
    HWND hwnd_guess;                    // 0x287c

    void SendMessageA_guess(LPARAM lparam);
};

// FUNCTION: TRILOGY 0x0040e56e
// ORIGINAL FILE: GameLib/Win32 Goodies/Win32WinUser.cp (from Mac debug map)
// Sends private message 0x4642 with wParam 0x4242 to the window at 0x287c.
void DisplayWnd_guess::SendMessageA_guess(LPARAM lparam)
{
    SendMessageA(hwnd_guess, 0x4642, 0x4242, lparam);
}

// Local stand-in: the callers are EQ_Bag and the sliders, whose headers don't expose a common base with a RECT at 4.
class RectElement_guess
{
public:
    int pad_guess;                      // 0x0
    RECT rect_guess;                    // 0x4

    void SetRect_Win32_guess(int dx, int dy);
};

// FUNCTION: TRILOGY 0x00419583
// ORIGINAL FILE: GameLib/Win32 Goodies/Win32WinUser.cp (from Mac debug map)
// Despite the name this moves the rectangle at +4 by (dx, dy).
void RectElement_guess::SetRect_Win32_guess(int dx, int dy)
{
    OffsetRect(&rect_guess, dx, dy);
}

// 0x006EE754 the main window handle (no header names it yet).
#define g_main_hwnd_guess           EQ_GLOBAL(HWND, 0x006EE754)

// FUNCTION: TRILOGY 0x004eaf2c
// ORIGINAL FILE: GameLib/Win32 Goodies/Win32WinUser.cp (from Mac debug map)
// __cdecl. Sends WM_SYSCOMMAND to the main window: SC_RESTORE (0xF120) when the flag is 0, else SC_MAXIMIZE (0xF030).
void SendMessageA_guess(char maximize)
{
    if (maximize != 0)
        SendMessageA(g_main_hwnd_guess, WM_SYSCOMMAND, 0xF030, 0);
    else
        SendMessageA(g_main_hwnd_guess, WM_SYSCOMMAND, 0xF120, 0);
}

// Local stand-in for a red-black tree node (STL-style map): left at 0x0, right at 0x8, shared nil node at 0x006EFEB4.
struct TreeNode_guess
{
    TreeNode_guess* left_guess;         // 0x0
    TreeNode_guess* parent_guess;       // 0x4
    TreeNode_guess* right_guess;        // 0x8
};

// Local stand-in for the tree owner: the header pointer is at 0x4 and the node count at 0xc.
class Tree_guess
{
public:
    int pad0_guess;                      // 0x0
    TreeNode_guess* header_guess;        // 0x4
    int pad8_guess;                      // 0x8
    int size_guess;                      // 0xc

    void EraseRange_guess(void** result, TreeNode_guess* first, TreeNode_guess* last);
    int EraseKey_guess(int param_1);
    void Erase_guess(TreeNode_guess* node);
    TreeNode_guess* Lower_bound_guess(int* key);
    TreeNode_guess* Upper_bound_guess(int* key);

    ~Tree_guess()
    {
        EQ_METHOD(0, void, 0x004EE8BE, (Tree_guess*), this)();
    }
};

#define g_tree_nil_guess            EQ_GLOBAL(TreeNode_guess*, 0x006EFEB4)

// Local stand-in for the owner whose vtable and tree member are cleaned here.
class CleanupStringResources_guess
{
public:
    char pad_guess[0x2c];
    Tree_guess resources_guess;         // 0x2c

    CleanupStringResources_guess* DestroyWindow(unsigned char flags);
    ~CleanupStringResources_guess();
};

// FUNCTION: TRILOGY 0x004ee4dd
// ORIGINAL FILE: GameLib/Win32 Goodies/Win32WinUser.cp (from Mac debug map)
CleanupStringResources_guess* CleanupStringResources_guess::DestroyWindow(unsigned char flags)
{
    EQ_METHOD(0, void, 0x004EE4F9, (CleanupStringResources_guess*), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void*))(this);
    return this;
}

// FUNCTION: TRILOGY 0x004ee4f9
// @early-stop: EH state store and vptr store order, loop cursor/nil register swap (ECX/EAX), member dtor's dropped state store and EDX dummy (inline stand-in dtor).
CleanupStringResources_guess::~CleanupStringResources_guess()
{
    int* nextNode;
    int* parentNode;
    int* currentNode;

    *(void**)this = (void*)0x00535968;
    currentNode = (int*)resources_guess.header_guess->left_guess;
    while (currentNode != (int*)resources_guess.header_guess)
    {
        if (currentNode[4] != 0)
            EQ_CDECL(void, 0x005218BB, (void*))((void*)currentNode[4]);
        currentNode[4] = 0;
        nextNode = (int*)currentNode[2];
        if (g_tree_nil_guess != (TreeNode_guess*)nextNode)
        {
            do
            {
                currentNode = nextNode;
                nextNode = (int*)*currentNode;
            } while (g_tree_nil_guess != (TreeNode_guess*)nextNode);
        }
        else
        {
            parentNode = (int*)currentNode[1];
            while (currentNode == (int*)parentNode[2])
            {
                currentNode = parentNode;
                parentNode = (int*)parentNode[1];
            }
            if ((int*)currentNode[2] != parentNode)
                currentNode = parentNode;
        }
    }
    /* fake match: VC6 builds the EH frame (and the member's state stores) only when the member is live across a call
       through a pointer variable; a constant-address call is treated as non-throwing. */
    void (__fastcall *clear_guess)(Tree_guess*) = EQ_THISCALL(void, 0x004EEAA3, (Tree_guess*));
    clear_guess(&resources_guess);
}

// FUNCTION: TRILOGY 0x004ee584
void __fastcall clearMemoryAndResetPointers_thunk_guess(Tree_guess* treePtr)
{
    EQ_METHOD(0, void, 0x004EE8BE, (Tree_guess*), treePtr)();
}

// FUNCTION: TRILOGY 0x004ee8be
// @early-stop: the arg-setup `lea eax,[ebp-4]` and the lock-scope `xor esi,esi` each land one slot
// off the original's instruction order (scheduling; the EQ_THISCALL dummy EDX changes register pressure).
/* fake match: the volatile left-node read forces the value into ECX like the original; a plain read
   pairs with `push` and folds into `push dword ptr [eax]`. */
void __fastcall clearMemoryAndResetPointers_guess(Tree_guess* tree)
{
    int nodeToDelete;
    int lockGuard;
    TreeNode_guess* first;

    first = *(TreeNode_guess * volatile *)&tree->header_guess->left_guess;
    EQ_METHOD(3, void, 0x004EE9BB, (Tree_guess*, void**, TreeNode_guess*, TreeNode_guess*), tree)((void**)&lockGuard, first, tree->header_guess);
    EQ_CDECL(void, 0x005218BB, (void*))(tree->header_guess);
    tree->header_guess = 0;
    tree->size_guess = 0;
    nodeToDelete = 0;
    EQ_METHOD(0, void, 0x00531890, (int*), &lockGuard)();
    --EQ_GLOBAL(int, 0x006EFEB8);
    if (EQ_GLOBAL(int, 0x006EFEB8) == 0)
    {
        nodeToDelete = (int)g_tree_nil_guess;
        g_tree_nil_guess = 0;
    }
    EQ_METHOD(0, void, 0x0053192C, (int*), &lockGuard)();
    if (nodeToDelete != 0)
    {
        EQ_CDECL(void, 0x005218BB, (void*))((void*)nodeToDelete);
    }
}

// FUNCTION: TRILOGY 0x004ee9bb
// @early-stop: condition/load order, branch scheduling, wrapper EDX setup, and loop cleanup/result-store order still differ.
void Tree_guess::EraseRange_guess(void** result, TreeNode_guess* first, TreeNode_guess* last)
{
    Tree_guess* nodeResult;
    TreeNode_guess* node;
    TreeNode_guess* next;

    if (size_guess == 0 || first != header_guess->left_guess || last != header_guess)
    {
        while (first != last)
        {
            node = first;
            EQ_METHOD(0, void, 0x004EEF62, (TreeNode_guess**), &first)();
            EQ_METHOD(2, void, 0x004EEAF1, (Tree_guess*, Tree_guess**, TreeNode_guess*), this)(&nodeResult, node);
        }
        *result = first;
    }
    else
    {
        node = header_guess->parent_guess;
        if (node != g_tree_nil_guess)
        {
            next = node;
            do
            {
                Erase_guess(next->right_guess);
                next = next->left_guess;
                EQ_CDECL(void, 0x005218BB, (void*))(node);
                node = next;
            } while (next != g_tree_nil_guess);
        }

        header_guess->parent_guess = g_tree_nil_guess;
        size_guess = 0;
        header_guess->left_guess = header_guess;
        header_guess->right_guess = header_guess;
        *result = header_guess->left_guess;
    }
}

// FUNCTION: TRILOGY 0x004eea52
int Tree_guess::EraseKey_guess(int param_1)
{
    int last;
    int first;
    int iterator;

    last = EQ_METHOD(1, int, 0x004EF08B, (Tree_guess*, int), this)(param_1);
    first = EQ_METHOD(1, int, 0x004EEF98, (Tree_guess*, int), this)(param_1);
    param_1 = 0;
    iterator = first;
    while (iterator != last)
    {
        param_1 = param_1 + 1;
        EQ_METHOD(0, void, 0x004EEF62, (int*), &iterator)();
    }
    EQ_METHOD(3, void, 0x004EE9BB, (Tree_guess*, int*, int, int), this)(&iterator, first, last);
    return param_1;
}

// FUNCTION: TRILOGY 0x004eeaa3
// @early-stop: `push dword ptr [eax]` where the original pushes the same value from EDX (the folded
// call's EDX dummy owns the register, so VC6 reloads memory instead).
/* fake match: the volatile left-node load holds the value in EDX, the slot the folded call's dummy
   uses, so the dummy lands where the original's unscheduled EDX does. */
void __fastcall ClearTree_guess(Tree_guess* treePtr)
{
    TreeNode_guess* first = *(TreeNode_guess * volatile *)&treePtr->header_guess->left_guess;
    void* result;

    EQ_THISCALL(void, 0x004EE9BB, (Tree_guess*, int, void**, TreeNode_guess*, TreeNode_guess*))
        (treePtr, (int)first, &result, treePtr->header_guess->left_guess, treePtr->header_guess);
}

// FUNCTION: TRILOGY 0x004eed52
// __thiscall. Frees the subtree under node post-order: the right subtree recursively, then the node itself
// with the client's cdecl operator delete (0x005218BB), then continues with the left child. Stops at the nil node.
/* fake match: A traversal alias keeps the original parameter and left-walk pointer in EDI/ESI. */
void Tree_guess::Erase_guess(TreeNode_guess* node)
{
    TreeNode_guess* current = node;

    if (node != g_tree_nil_guess)
    {
        do
        {
            Erase_guess(current->right_guess);
            current = current->left_guess;
            EQ_CDECL(void, 0x005218BB, (void*))(node);
            node = current;
        } while (current != g_tree_nil_guess);
    }
}

// Local stand-in for std::_Lockit: the 4-byte guard the tree constructor passes by address to the lock calls.
class Lockit_guess
{
public:
    int pad_guess;                      // 0x0
};

// FUNCTION: TRILOGY 0x004eed88
// @early-stop: ESI/EDI allocation is reversed because this models `this` as a standalone fastcall parameter.
// Tree constructor body (this in ECX, nothing popped; __fastcall here, as clearMemoryAndResetPointers_guess is).
// Under a _Lockit, the first tree makes the shared nil node (0x006EFEB4) from a fresh node and clears its
// left/right; later trees free that spare. Then the tree count (0x006EFEB8) goes up, and this tree gets a
// header node at +4: parent = nil, left = right = itself, and the node count at +0xc is 0.
// The original keeps an EBP frame (push ebp; mov ebp,esp); the lock object sits at [ebp-8], so this one does too.
/* fake match: Keep a register copy for node setup while the stack state tracks cleanup beside the lock. */
#pragma optimize("y", off)
void __fastcall Tree_init_guess(Tree_guess* tree)
{
    struct LocalState_guess
    {
        Lockit_guess lock_guess;
        TreeNode_guess* pending_guess;
    } state_guess;
    TreeNode_guess* spare;
    TreeNode_guess* nil_node;
    TreeNode_guess* header_node;

    spare = (TreeNode_guess*)EQ_CDECL(void*, 0x005218C6, (unsigned int))(0x18);
    spare->parent_guess = 0;
    *(int*)((char*)spare + 0x14) = 1;
    state_guess.pending_guess = spare;
    EQ_METHOD(0, void, 0x00531890, (Lockit_guess*), &state_guess.lock_guess)();
    if (g_tree_nil_guess == 0)
    {
        g_tree_nil_guess = spare;
        spare->left_guess = 0;
        state_guess.pending_guess = 0;
        g_tree_nil_guess->right_guess = 0;
    }
    ++EQ_GLOBAL(int, 0x006EFEB8);
    EQ_METHOD(0, void, 0x0053192C, (Lockit_guess*), &state_guess.lock_guess)();
    if (state_guess.pending_guess != 0)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(state_guess.pending_guess);
    }
    nil_node = g_tree_nil_guess;
    header_node = (TreeNode_guess*)EQ_CDECL(void*, 0x005218C6, (unsigned int))(0x18);
    header_node->parent_guess = nil_node;
    *(int*)((char*)header_node + 0x14) = 0;
    tree->header_guess = header_node;
    tree->size_guess = 0;
    header_node->left_guess = header_node;
    tree->header_guess->right_guess = tree->header_guess;
}
#pragma optimize("y", on)

// FUNCTION: TRILOGY 0x004EEF98
/* fake match: The volatile alias sequences the sentinel read after the initial node load. */
TreeNode_guess* Tree_guess::Lower_bound_guess(int* key)
{
    TreeNode_guess* result = *(TreeNode_guess**)((char*)this + 4);
    TreeNode_guess* node;

    node = *(TreeNode_guess**)((char*)result + 4);
    TreeNode_guess* volatile& nil_slot = g_tree_nil_guess;
    TreeNode_guess* nil = nil_slot;
    if (node != nil)
    {
        do
        {
            if (*(int*)((char*)node + 0xC) < *key)
                node = node->right_guess;
            else
            {
                result = node;
                node = node->left_guess;
            }
        } while (node != nil);
    }

    return result;
}

// FUNCTION: TRILOGY 0x004EF08B
/* fake match: The volatile reference keeps the nil load after the root-pointer load. */
TreeNode_guess* Tree_guess::Upper_bound_guess(int* key)
{
    TreeNode_guess* node = header_guess->parent_guess;
    TreeNode_guess* result = header_guess;
    TreeNode_guess* volatile& nil_slot = g_tree_nil_guess;
    TreeNode_guess* nil = nil_slot;

    while (node != nil)
    {
        if (*key < *(int*)((char*)node + 0xC))
        {
            result = node;
            node = node->left_guess;
        }
        else
        {
            node = node->right_guess;
        }
    }
    return result;
}
