#include "trilogy/eqaddr.h"

typedef struct SoundTreeNode_guess
{
    struct SoundTreeNode_guess *left;
    struct SoundTreeNode_guess *parent;
    struct SoundTreeNode_guess *right;
    int key;
    unsigned char unknown0x10[0x18];
    int color;
} SoundTreeNode_guess;

typedef struct SoundTree_guess
{
    unsigned int unknown0x00;
    SoundTreeNode_guess *header;
    unsigned int unknown0x08;
    int count;
} SoundTree_guess;

#define g_soundTreeNil EQ_GLOBAL(SoundTreeNode_guess *, 0x100AADDC)

#define SOUND_TREE_RED 0
#define SOUND_TREE_BLACK 1

// FUNCTION: EQGFX 0x10019E96
// __thiscall, callee pops 8. Unlinks node from the red-black tree, rebalances it, frees it and sets *result = node.
void __fastcall SoundTreeErase_guess_Erase_guess(SoundTree_guess *tree, int unused_edx, SoundTreeNode_guess **result, SoundTreeNode_guess *node)
{
    SoundTreeNode_guess *nodeToErase;
    SoundTreeNode_guess *leftChild;
    SoundTreeNode_guess *replacement;
    SoundTreeNode_guess *unlinked;
    SoundTreeNode_guess *successor;
    SoundTreeNode_guess *sibling;
    SoundTreeNode_guess *extreme;
    SoundTreeNode_guess *probe;
    int color;

    nodeToErase = node;
    EQ_THISCALL(void, 0x1001A407, (SoundTreeNode_guess **, int))(&node, 0);
    leftChild = nodeToErase->left;
    unlinked = nodeToErase;
    if (leftChild == g_soundTreeNil)
    {
        replacement = nodeToErase->right;
    }
    else
    {
        replacement = leftChild;
        if ((successor = nodeToErase->right) != g_soundTreeNil)
        {
            while (successor->left != g_soundTreeNil)
                successor = successor->left;
            replacement = successor->right;
            unlinked = successor;
            if (successor == nodeToErase)
                goto transplant;
            leftChild->parent = successor;
            successor->left = nodeToErase->left;
            if (successor == nodeToErase->right)
            {
                replacement->parent = successor;
            }
            else
            {
                replacement->parent = successor->parent;
                successor->parent->left = replacement;
                successor->right = nodeToErase->right;
                nodeToErase->right->parent = successor;
            }
            if (tree->header->parent == nodeToErase)
                tree->header->parent = successor;
            else if (nodeToErase->parent->left == nodeToErase)
                nodeToErase->parent->left = successor;
            else
                nodeToErase->parent->right = successor;
            unlinked = nodeToErase;
            successor->parent = nodeToErase->parent;
            color = nodeToErase->color;
            nodeToErase->color = successor->color;
            successor->color = color;
            goto check_color;
        }
    }
transplant:
    replacement->parent = unlinked->parent;
    if (tree->header->parent == nodeToErase)
        tree->header->parent = replacement;
    else if (nodeToErase->parent->left == nodeToErase)
        nodeToErase->parent->left = replacement;
    else
        nodeToErase->parent->right = replacement;
    if (tree->header->left == nodeToErase)
    {
        if (nodeToErase->right == g_soundTreeNil)
            tree->header->left = nodeToErase->parent;
        else
        {
            extreme = replacement;
            for (probe = replacement->left; probe != g_soundTreeNil; probe = probe->left)
                extreme = probe;
            tree->header->left = extreme;
        }
    }
    if (tree->header->right == nodeToErase)
    {
        if (nodeToErase->left == g_soundTreeNil)
            tree->header->right = nodeToErase->parent;
        else
        {
            extreme = replacement;
            for (probe = replacement->right; probe != g_soundTreeNil; probe = probe->right)
                extreme = probe;
            tree->header->right = extreme;
        }
    }
check_color:
    if (unlinked->color != SOUND_TREE_BLACK)
        goto free_node;
rebalance:
    if (replacement == tree->header->parent || replacement->color != SOUND_TREE_BLACK)
        goto mark_black;
    if (replacement == replacement->parent->left)
    {
        sibling = replacement->parent->right;
        if (sibling->color == SOUND_TREE_RED)
        {
            sibling->color = SOUND_TREE_BLACK;
            replacement->parent->color = SOUND_TREE_RED;
            EQ_THISCALL(void, 0x1001A341, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, replacement->parent);
            sibling = replacement->parent->right;
        }
        if (sibling->left->color != SOUND_TREE_BLACK || sibling->right->color != SOUND_TREE_BLACK)
        {
            if (sibling->right->color == SOUND_TREE_BLACK)
            {
                sibling->left->color = SOUND_TREE_BLACK;
                sibling->color = SOUND_TREE_RED;
                EQ_THISCALL(void, 0x1001A385, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, sibling);
                sibling = replacement->parent->right;
            }
            sibling->color = replacement->parent->color;
            replacement->parent->color = SOUND_TREE_BLACK;
            sibling->right->color = SOUND_TREE_BLACK;
            EQ_THISCALL(void, 0x1001A341, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, replacement->parent);
            goto mark_black;
        }
    }
    else
    {
        sibling = replacement->parent->left;
        if (sibling->color == SOUND_TREE_RED)
        {
            sibling->color = SOUND_TREE_BLACK;
            replacement->parent->color = SOUND_TREE_RED;
            EQ_THISCALL(void, 0x1001A385, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, replacement->parent);
            sibling = replacement->parent->left;
        }
        if (sibling->right->color != SOUND_TREE_BLACK || sibling->left->color != SOUND_TREE_BLACK)
        {
            if (sibling->left->color == SOUND_TREE_BLACK)
            {
                sibling->right->color = SOUND_TREE_BLACK;
                sibling->color = SOUND_TREE_RED;
                EQ_THISCALL(void, 0x1001A341, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, sibling);
                sibling = replacement->parent->left;
            }
            sibling->color = replacement->parent->color;
            replacement->parent->color = SOUND_TREE_BLACK;
            sibling->left->color = SOUND_TREE_BLACK;
            EQ_THISCALL(void, 0x1001A385, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, replacement->parent);
            goto mark_black;
        }
    }
    sibling->color = SOUND_TREE_RED;
    replacement = replacement->parent;
    goto rebalance;
mark_black:
    replacement->color = SOUND_TREE_BLACK;
free_node:
    EQ_CDECL(void, 0x10050CBB, (void *))(unlinked);
    tree->count--;
    *result = node;
}

// FUNCTION: EQGFX 0x1001A1B6
// Adds an entry to an ordered tree; its game-specific purpose is unclear.
SoundTreeNode_guess **__fastcall SoundTreeInsert_guess_Insert_guess(
    SoundTree_guess *tree,
    int unused_edx,
    SoundTreeNode_guess **result,
    int relation,
    SoundTreeNode_guess *parent,
    void *value)
{
    SoundTreeNode_guess *node;
    SoundTreeNode_guess *p;
    SoundTreeNode_guess *parentNode;
    SoundTreeNode_guess *uncle;
    SoundTreeNode_guess *grandparent;
    SoundTreeNode_guess *header;

    node = (SoundTreeNode_guess *)EQ_CDECL(void *, 0x10068C72, (unsigned int))(0x2C);
    node->color = 0;
    node->parent = parent;
    node->left = g_soundTreeNil;
    node->right = g_soundTreeNil;
    EQ_CDECL(void, 0x1001A43D, (void *, void *))(&node->key, value);
    tree->count++;
    if ((parent == tree->header) || (relation != (int)g_soundTreeNil) ||
        (EQ_THISCALL(char, 0x10019D71, (void *, int, void *))(value, 0, &parent->key) != 0))
    {
        parent->left = node;
        header = tree->header;
        if (parent != header)
        {
            if (parent == header->left)
                header->left = node;
        }
        else
        {
            header->parent = node;
            header = tree->header;
            header->right = node;
        }
    }
    else
    {
        parent->right = node;
        header = tree->header;
        if (parent == header->right)
            header->right = node;
    }

    p = node;
    if (p != tree->header->parent)
    {
        do
        {
            parentNode = p->parent;
            if (parentNode->color != 0)
                break;
            grandparent = parentNode->parent;
            uncle = grandparent->left;
            if (parentNode == uncle)
            {
                uncle = grandparent->right;
                if (uncle->color == 0)
                {
                    parentNode->color = 1;
                    uncle->color = 1;
                    grandparent->color = 0;
                    p = grandparent;
                }
                else
                {
                    if (p == parentNode->right)
                    {
                        EQ_THISCALL(void, 0x1001A341, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, parentNode);
                        p = parentNode;
                    }
                    p->parent->color = 1;
                    p->parent->parent->color = 0;
                    EQ_THISCALL(void, 0x1001A385, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, p->parent->parent);
                }
            }
            else
            {
                if (uncle->color == 0)
                {
                    parentNode->color = 1;
                    uncle->color = 1;
                    grandparent->color = 0;
                    p = grandparent;
                }
                else
                {
                    if (p == parentNode->left)
                    {
                        EQ_THISCALL(void, 0x1001A385, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, parentNode);
                        p = parentNode;
                    }
                    p->parent->color = 1;
                    p->parent->parent->color = 0;
                    EQ_THISCALL(void, 0x1001A341, (SoundTree_guess *, int, SoundTreeNode_guess *))(tree, 0, p->parent->parent);
                }
            }
        } while (p != tree->header->parent);
    }

    tree->header->parent->color = 1;
    *result = node;
    return result;

}

// FUNCTION: EQGFX 0x1001A30D
// Finds the ordered-tree entry where a key search should begin.
// __fastcall (this in ECX, one stack arg, callee pops 4). Lower bound of key: the last node visited where the comparer at 0x10019D71 returns 0, else the header.
SoundTreeNode_guess *__fastcall LowerBound_guess(SoundTree_guess *tree, int unused_edx, void *key)
{
    SoundTreeNode_guess *bound;
    SoundTreeNode_guess *node;

    bound = tree->header;
    node = bound->parent;
    while (node != g_soundTreeNil)
    {
        if (EQ_THISCALL(char, 0x10019D71, (void *, int, void *))(&node->key, 0, key) != 0)
            node = node->right;
        else
        {
            bound = node;
            node = node->left;
        }
    }
    return bound;
}

// FUNCTION: EQGFX 0x1001A341
// Keeps the ordered index efficient when it leans right.
void __fastcall SoundTree_guess_RotateLeft_guess(SoundTree_guess *tree, int unused_edx, SoundTreeNode_guess *node)
{
    SoundTreeNode_guess *pivot = node->right;
    node->right = pivot->left;
    if (pivot->left != g_soundTreeNil)
        pivot->left->parent = node;
    pivot->parent = node->parent;
    if (node == tree->header->parent)
        tree->header->parent = pivot;
    else
    {
        SoundTreeNode_guess *parent = node->parent;
        if (node == parent->left)
            parent->left = pivot;
        else
            parent->right = pivot;
    }
    pivot->left = node;
    node->parent = pivot;
}

// FUNCTION: EQGFX 0x1001A385
// Keeps the ordered index efficient when it leans left.
void __fastcall SoundTree_guess_RotateRight_guess(SoundTree_guess *tree, int unused_edx, SoundTreeNode_guess *node)
{
    SoundTreeNode_guess *pivot = node->left;
    node->left = pivot->right;
    if (pivot->right != g_soundTreeNil)
        pivot->right->parent = node;
    pivot->parent = node->parent;
    if (node == tree->header->parent)
        tree->header->parent = pivot;
    else
    {
        SoundTreeNode_guess *parent = node->parent;
        if (node == parent->right)
            parent->right = pivot;
        else
            parent->left = pivot;
    }
    pivot->right = node;
    node->parent = pivot;
}

// FUNCTION: EQGFX 0x1001A3CB
// Moves the ordered-tree cursor to the preceding entry.
void __fastcall IteratorDecrement_guess(SoundTreeNode_guess **iter)
{
    SoundTreeNode_guess *node;
    if ((*iter)->color == 0 && (*iter)->parent->parent == *iter)
    {
        *iter = (*iter)->right;
    }
    else
    {
        if ((*iter)->left != g_soundTreeNil)
        {
            node = (*iter)->left;
            while (node->right != g_soundTreeNil)
                node = node->right;
            *iter = node;
        }
        else
        {
            do
            {
                node = (*iter)->parent;
                if (*iter != node->left)
                    break;
                *iter = node;
            } while (1);
            *iter = node;
        }
    }
}

// FUNCTION: EQGFX 0x1001A407
// Advances *iter to the in-order successor of its node (the tree's iterator increment).
void __fastcall IteratorIncrement_guess(SoundTreeNode_guess **iter)
{
    SoundTreeNode_guess *nil = g_soundTreeNil;
    SoundTreeNode_guess *node = (*iter)->right;

    if (node != nil)
    {
        while (node->left != nil)
        {
            node = node->left;
        }
    }
    else
    {
        for (node = (*iter)->parent; *(SoundTreeNode_guess * volatile *)iter == node->right; node = node->parent)
        {
            *iter = node;
        }
        if ((*iter)->right == node)
        {
            return;
        }
    }
    *iter = node;
    return;
}
