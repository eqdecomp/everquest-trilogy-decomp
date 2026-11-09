#include "trilogy/eqaddr.h"

// Tree node as this function reads it: left, parent, right, then the stored value.
struct TreeNode_guess
{
    TreeNode_guess *left;
    TreeNode_guess *parent;
    TreeNode_guess *right;
    union
    {
        char value[0x20];
        struct
        {
            char value_prefix[0x1c];
            int value_tail;
        } value_words;
    };
};

struct TreeIteratorValue_guess
{
    TreeNode_guess *node;

    TreeIteratorValue_guess(TreeNode_guess *value) : node(value) {}
};

struct TreeResult_guess
{
    TreeNode_guess *node;
    char inserted;
};

struct Tree_guess
{
    int unknown_0;
    TreeNode_guess *header;
    char insertion_mode;
    int size;

    void t3dInsertTreeValue_guess(TreeResult_guess *result, void *key);
    void EraseRange_guess(void **result, TreeNode_guess *first, TreeNode_guess *last);
    void Erase_guess(TreeNode_guess *node);
};

#define tree_nil_node EQ_GLOBAL(TreeNode_guess *, 0x100AADDC)
#define tree_count EQ_GLOBAL(int, 0x100AADE0)
#define TREE_NODE_BYTES 0x2c

// Tree key as this function reads it: two float triples, compared from the second one.
struct TreeKey_guess
{
    float low[3];
    float high[3];

    char t3dIsAnyKeyComponentLess_guess(TreeKey_guess *other);
};

class CleanupStringResources_guess
{
public:
    CleanupStringResources_guess *cleanupObject_guess(unsigned char flags);
};

// FUNCTION: EQGFX 0x10019c6a
void __fastcall clearMemoryAndResetPointers_guess(Tree_guess *tree)
{
    int lockScope;
    TreeNode_guess *header;
    TreeNode_guess *nilToFree;

    header = tree->header;
    EQ_METHOD(3, void, 0x10019DBE, (Tree_guess *, void **, TreeIteratorValue_guess, TreeIteratorValue_guess), tree)(
        (void **)&lockScope, TreeIteratorValue_guess(header->left), TreeIteratorValue_guess(header));
    EQ_CDECL(void, 0x10050CBB, (void *))(tree->header);
    tree->header = 0;
    tree->size = 0;
    nilToFree = 0;
    EQ_METHOD(0, void, 0x10075A66, (int *), &lockScope)();
    if (--tree_count == 0)
    {
        nilToFree = tree_nil_node;
        tree_nil_node = 0;
    }
    EQ_METHOD(0, void, 0x10075B02, (int *), &lockScope)();
    if (nilToFree != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(nilToFree);
}

// FUNCTION: EQGFX 0x10019cc9
// Inserts a tree entry, rejecting duplicates when configured; game-specific purpose is unclear.
void Tree_guess::t3dInsertTreeValue_guess(TreeResult_guess *result, void *key)
{
    char comparison = 1;
    TreeNode_guess *parent = header;
    TreeNode_guess *current = header->parent;

    while (current != tree_nil_node)
    {
        comparison = EQ_METHOD(1, char, 0x10019D71, (void *, void *), key)(current->value);
        parent = current;
        if (comparison == 0)
            current = current->right;
        else
            current = current->left;
    }

    if (insertion_mode == 0)
    {
        if (comparison != 0)
        {
            if (parent == header->left)
                goto insert_node;
            EQ_METHOD(0, void, 0x1001A3CB, (TreeNode_guess **), &parent)();
        }

        comparison = EQ_METHOD(1, char, 0x10019D71, (void *, void *), parent->value)(key);
        if (comparison == 0)
        {
            result->node = parent;
            result->inserted = 0;
            return;
        }
    }

insert_node:
    {
        TreeNode_guess *inserted = EQ_METHOD(4, TreeNode_guess *, 0x1001A1B6,
            (void *, void **, TreeNode_guess *, TreeNode_guess *, void *), this)(&key, current, parent, key);
        result->inserted = 1;
        result->node = inserted->left;
    }
    return;
}

// FUNCTION: EQGFX 0x10019d71
// Checks whether any bound in one tree key is below its counterpart.
char TreeKey_guess::t3dIsAnyKeyComponentLess_guess(TreeKey_guess *other)
{
    int componentLess = high[0] < other->high[0] || high[1] < other->high[1] || high[2] < other->high[2] ||
        low[0] < other->low[0] || low[1] < other->low[1] || low[2] < other->low[2];
    if (componentLess)
        return 1;
    else
        return 0;
}

// FUNCTION: EQGFX 0x10019dbe
// Removes entries in a tree range, clearing the tree when the range covers every entry.
void Tree_guess::EraseRange_guess(void **result, TreeNode_guess *first, TreeNode_guess *last)
{
    Tree_guess *localTree;
    TreeNode_guess *node;
    TreeNode_guess *next;

    if (size == 0 || first != header->left || last != header)
    {
        while (first != last)
        {
            node = first;
            EQ_METHOD(0, void, 0x1001A407, (TreeNode_guess **), &first)();
            EQ_METHOD(2, void, 0x10019E96,
                (Tree_guess *, Tree_guess **, TreeNode_guess *), this)(&localTree, node);
        }
        *result = first;
    }
    else
    {
        node = header->parent;
        next = node;
        if (node != tree_nil_node)
        {
            do
            {
                Erase_guess(next->right);
                next = next->left;
                EQ_CDECL(void, 0x10050CBB, (void *))(node);
                node = next;
            } while (next != tree_nil_node);
        }

        header->parent = tree_nil_node;
        header->left = header;
        next = header->left;
        size = 0;
        header->right = header;
        *result = next;
    }
}

// FUNCTION: EQGFX 0x1001A0F7
// __thiscall, callee pops 4. Frees the subtree under node post-order: right subtree recursively, then node itself.
void Tree_guess::Erase_guess(TreeNode_guess *node)
{
    TreeNode_guess *current = node;

    if (node != tree_nil_node)
    {
        do
        {
            Erase_guess(current->right);
            current = current->left;
            EQ_CDECL(void, 0x10050CBB, (void *))(node);
            node = current;
        } while (current != tree_nil_node);
    }
}

// FUNCTION: EQGFX 0x1001A12D
// Creates the shared tree sentinel when needed and initializes a new empty tree.
void __fastcall Initialize_guess(Tree_guess *tree)
{
    struct LocalState_guess
    {
        int lock_storage;
        TreeNode_guess *pending_node;
    } state;
    TreeNode_guess *spare_node;
    TreeNode_guess *nil_node;
    TreeNode_guess *header_node;

    spare_node = (TreeNode_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(TREE_NODE_BYTES);
    spare_node->parent = 0;
    spare_node->value_words.value_tail = 1;
    state.pending_node = spare_node;
    EQ_METHOD(0, void, 0x10075A66, (int *), &state.lock_storage)();
    if (tree_nil_node == 0)
    {
        tree_nil_node = spare_node;
        spare_node->left = 0;
        state.pending_node = 0;
        tree_nil_node->right = 0;
    }
    ++tree_count;
    EQ_METHOD(0, void, 0x10075B02, (int *), &state.lock_storage)();
    if (state.pending_node != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(state.pending_node);
    }
    nil_node = tree_nil_node;
    header_node = (TreeNode_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(TREE_NODE_BYTES);
    header_node->parent = nil_node;
    header_node->value_words.value_tail = 0;
    tree->header = header_node;
    tree->size = 0;
    header_node->left = header_node;
    tree->header->right = tree->header;
}

// FUNCTION: EQGFX 0x10021BC0
// Performs object cleanup and optionally releases its storage; the object's resource role is unclear.
CleanupStringResources_guess *CleanupStringResources_guess::cleanupObject_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x10021BDC, (CleanupStringResources_guess *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10050CBB, (void *))(this);
    return this;
}
