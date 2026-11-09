#include "trilogy/eqaddr.h"

typedef struct SoundTreeNode_guess
{
    struct SoundTreeNode_guess *left;
    struct SoundTreeNode_guess *parent;
    struct SoundTreeNode_guess *right;
    int key;
} SoundTreeNode_guess;

typedef struct SoundTree_guess
{
    unsigned int unknown_00;
    SoundTreeNode_guess *header;
} SoundTree_guess;

#define EQGFX_SOUND_TREE_LOWER_BOUND EQ_FASTCALL(SoundTreeNode_guess *, 0x1001A30D, (SoundTree_guess *, int, void *))
#define EQGFX_COMPARE_SOUND_TREE_KEY EQ_THISCALL(char, 0x10019D71, (void *, int, void *))

// FUNCTION: EQGFX 0x10019e55
// Selects the lower-bound node on a zero comparison; otherwise the output is the tree header.
void __fastcall FindTreeNodeByKey_guess(SoundTree_guess *tree, int unused_edx, int *result, int key)
{
    int boundValue;
    int *selectedValue;

    boundValue = (int)EQGFX_SOUND_TREE_LOWER_BOUND(tree, unused_edx, (void *)key);
    if (boundValue != (int)tree->header)
    {
        if (EQGFX_COMPARE_SOUND_TREE_KEY((void *)key, 0, &((SoundTreeNode_guess *)boundValue)->key) == 0)
        {
            selectedValue = &boundValue;
            goto copy_result;
        }
    }
    key = (int)tree->header;
    selectedValue = &key;

copy_result:
    *result = *selectedValue;
}
