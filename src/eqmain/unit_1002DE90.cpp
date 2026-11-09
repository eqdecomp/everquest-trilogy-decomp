// unit_1002DE90.cpp - eqmain.dll, the address run 0x1002DE90..0x1002E170 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x1002DE90
// __cdecl, one stack arg (a pointer to a record whose dword at +0x8 is returned). The record's layout is unknown.
int __cdecl GetRecordBase_guess(int *ptr)
{
    return *(int *)((char *)ptr + 8);
}

// FUNCTION: EQMAIN 0x1002DEA0
// Initializes client bookkeeping records used by queues and buffers.
// __cdecl, no return value, four stack args (a record pointer, then three dwords). Stores arg 2 at +0x8, arg 3 at
// +0xC and arg 4 at +0x14, and clears the dwords at +0x0, +0x4 and +0x10. The record's layout is unknown.
void __cdecl InitRecord_guess(int *ptr, int value, int value2, int value3)
{
    *ptr = 0;
    ptr[1] = 0;
    ptr[2] = value;
    ptr[3] = value2;
    ptr[5] = value3;
    ptr[4] = 0;
    return;
}

// FUNCTION: EQMAIN 0x1002DED0
// Adds a client bookkeeping entry to the front of its queue.
// __cdecl, no return value, two stack args: a record pointer and an offset from the record's base at +0x8 (0 is a null
// node, and the write to its +0x4 faults). Links the node at base + offset in at the head: its dword at +0x0 takes the
// old head, its +0x4 is cleared and the record's dword at +0x0 becomes the node. An empty tail (+0x4 is 0) takes the
// node; otherwise the old head's +0x4 becomes the node. The count at +0x10 is incremented. Layout inferred from this and
// the neighbouring functions: head +0x0, tail +0x4, base +0x8, count +0x10.
void __cdecl record_add_head_guess(int *list, int nodeOffset)
{
    int oldHead;
    int *insertedNode;

    if (nodeOffset == 0)
    {
        insertedNode = (int *)0x0;
    }
    else
    {
        insertedNode = (int *)(list[2] + nodeOffset);
    }
    oldHead = *list;
    insertedNode[1] = 0;
    *insertedNode = oldHead;
    *list = (int)insertedNode;
    if (list[1] == 0)
    {
        list[1] = (int)insertedNode;
        list[4] = list[4] + 1;
        return;
    }
    *(int **)(*insertedNode + 4) = insertedNode;
    list[4] = list[4] + 1;
    return;
}

// FUNCTION: EQMAIN 0x1002DF20
// Adds a client bookkeeping entry to the back of its queue.
// __cdecl, two stack args: a record pointer and an offset from the record's base at +0x8 (0 is a null node). Appends
// the node at base + offset to the record's tail: its dword at +0x0 is cleared, its +0x4 takes the old tail, and the
// old tail's dword at +0x0 becomes the node. An empty record (head at +0x0 is 0) takes the node as its head. The count
// at +0x10 is incremented. Layout inferred from this and the neighbouring functions: head +0x0, tail +0x4, base +0x8.
void __cdecl record_add_tail_guess(int *record, int offset)
{
    int *node;

    if (offset == 0)
    {
        node = 0;
    }
    else
    {
        node = (int *)(record[2] + offset);
    }
    node[0] = 0;
    node[1] = record[1];
    record[1] = (int)node;
    if (record[0] == 0)
    {
        record[0] = (int)node;
        record[4]++;
        return;
    }
    ((int *)node[1])[0] = (int)node;
    record[4]++;
    return;
}

// FUNCTION: EQMAIN 0x1002DF70
// Orders a client bookkeeping entry by the supplied comparison rule.
// @early-stop: final head-link temporary uses ECX instead of the original EDX
// __cdecl, two stack args: a record pointer and an offset from the record's base at +0x8 (0 is a null node). Inserts the
// node at base + offset into the record's sorted list (head +0x0, tail +0x4, base +0x8, a second base +0xC, count +0x10,
// compare callback +0x14). Compare keys are node addresses plus the +0xC - +0x8 delta; compare(existing, new) > 0 means
// the existing node sorts after the new one. The node goes before the head when compare(head, new) > 0, else after the
// tail when compare(tail, new) <= 0, else before the first node after the head that compares greater. The count at +0x10
// is incremented. Layout inferred from this and the neighbouring functions.
void __cdecl record_insert_sorted_guess(int *record, int offset)
{
    unsigned int *node;
    unsigned int *cur;
    int head;
    int *head_node;
    int head_key;
    int tail_key;
    int cur_key;
    int key;
    int cmp;

    head = record[0];
    if (head != 0)
    {
        if (offset == 0)
        {
            key = 0;
        }
        else
        {
            key = record[3] + offset;
        }
        if (record[0] == 0)
        {
            head_key = 0;
        }
        else
        {
            head_key = (record[3] - record[2]) + head;
        }
        cmp = ((int (__cdecl *)(int, int))record[5])(head_key, key);
        if (cmp <= 0)
        {
            if (offset == 0)
            {
                key = 0;
            }
            else
            {
                key = record[3] + offset;
            }
            if (record[1] == 0)
            {
                tail_key = 0;
            }
            else
            {
                tail_key = (record[3] - record[2]) + record[1];
            }
            cmp = ((int (__cdecl *)(int, int))record[5])(tail_key, key);
            if (cmp <= 0)
            {
                if (offset == 0)
                {
                    node = 0;
                }
                else
                {
                    node = (unsigned int *)(record[2] + offset);
                }
                node[0] = 0;
                node[1] = record[1];
                record[1] = (int)node;
                if (record[0] == 0)
                {
                    record[0] = (int)node;
                    record[4] = record[4] + 1;
                    return;
                }
                *(unsigned int **)node[1] = node;
                record[4] = record[4] + 1;
                return;
            }
            cur = *(unsigned int **)record[0];
            if (offset == 0)
            {
                node = 0;
            }
            else
            {
                node = (unsigned int *)(record[2] + offset);
            }
            for (;;)
            {
                if (offset == 0)
                {
                    key = 0;
                }
                else
                {
                    key = record[3] + offset;
                }
                if (cur == 0)
                {
                    cur_key = 0;
                }
                else
                {
                    cur_key = (record[3] - record[2]) + (int)cur;
                }
                cmp = ((int (__cdecl *)(int, int))record[5])(cur_key, key);
                if (cmp > 0)
                {
                    break;
                }
                cur = *(unsigned int **)cur;
            }
            node[0] = (int)cur;
            node[1] = cur[1];
            cur[1] = (int)node;
            *(unsigned int **)node[1] = node;
            record[4] = record[4] + 1;
            return;
        }
    }
    if (offset == 0)
    {
        head_node = 0;
    }
    else
    {
        head_node = (int *)(record[2] + offset);
    }
    head = record[0];
    head_node[1] = 0;
    head_node[0] = head;
    record[0] = (int)head_node;
    if (record[1] == 0)
    {
        record[1] = (int)head_node;
        record[4] = record[4] + 1;
        return;
    }
    *(int **)(*head_node + 4) = head_node;
    record[4] = record[4] + 1;
    return;
}

// FUNCTION: EQMAIN 0x1002E0C0
// Finds a client bookkeeping entry matching a supplied value.
// @early-stop: 16 normalized differences remain, mainly the status register and redundant flag tests
// __cdecl, two stack args (a record pointer, then a dword passed to the callback). Walks the list from the head
// node (dword +0x0): each node's offset is its address minus the base at +0x8, and the callback at +0x14 gets
// (dword +0xC + offset, arg 2), or 0 for offset 0. A callback result of 0 returns that offset; a negative result
// walks on; a positive one, or the end of the list, returns 0. The record's layout is unknown.
int __cdecl FindRecordNode_guess(int *list, int addr)
{
    int *node;
    int offset;
    int status;

    node = (int *)*list;
    status = -1;
    if (node == 0)
    {
        return 0;
    }
    while (node != 0 && status < 0)
    {
        if (node == 0)
        {
            offset = 0;
        }
        else
        {
            offset = (int)node - list[2];
        }
        if (offset == 0)
        {
            status = 0;
        }
        else
        {
            status = list[3] + offset;
        }
        status = ((int (__cdecl *)(int, int))list[5])(status, addr);
        if (status == 0)
        {
            return offset;
        }
        node = (int *)*node;
    }
    return 0;
}

// FUNCTION: EQMAIN 0x1002E120
// Gets the head entry's offset for client bookkeeping.
// __cdecl, one stack arg (a pointer to a record whose dword at +0x0 and +0x8 are read). Returns 0 when the
// dword at +0x0 is 0, else that dword minus the dword at +0x8. The record's layout is unknown.
int __cdecl GetRecordDelta_guess(int *record)
{
    if (*record == 0)
    {
        return 0;
    }
    return *record - record[2];
}

// FUNCTION: EQMAIN 0x1002E130
// Dequeues the next client bookkeeping entry for its consumer.
// __cdecl, one stack arg (a pointer to a record: dword +0x0 is the head node, +0x4 the tail, +0x8 a base, +0x10 a
// count). Unlinks the head node (its dword at +0x0 is the next node, whose +0x4 is cleared) and returns the head's
// address minus the base. Returns 0 when the list is empty; the node is not freed.
int __cdecl PopRecordDelta_guess(int *list)
{
    int *head;
    int *next;
    int delta;

    head = (int *)*list;
    if (head == (int *)0x0)
    {
        delta = 0;
    }
    else
    {
        delta = (int)head - list[2];
    }
    if (*list != 0)
    {
        next = (int *)*(int *)*list;
        *list = (int)next;
        if (next == (int *)0x0)
        {
            list[1] = (int)next;
        }
        else
        {
            next[1] = 0;
        }
        list[4] = list[4] - 1;
    }
    return delta;
}

// FUNCTION: EQMAIN 0x1002E170
// Removes a selected entry from a client bookkeeping queue.
// __cdecl, two stack args: a record pointer and a delta (0 means no node, else the node is at base + delta). The
// record's dword +0x0 is the head node, +0x4 the tail, +0x8 a base, +0x10 a count; a node's +0x0 is its next node and
// +0x4 its previous. Unlinks that node (head, tail or middle case) and counts one out; the node is not freed. Returns
// the delta unchanged (EAX is never written). Delta 0 with both the head and the tail set faults (reads [0x4]).
int __cdecl RemoveRecordDelta_guess(int *list, int nodeDelta)
{
    int *edgeNode;
    int nextNode;
    int *previousNode;
    int *targetNode;

    if (nodeDelta == 0)
    {
        targetNode = (int *)0x0;
    }
    else
    {
        targetNode = (int *)(list[2] + nodeDelta);
    }
    edgeNode = (int *)*list;
    if (edgeNode == targetNode)
    {
        if (edgeNode != (int *)0x0)
        {
            nextNode = *edgeNode;
            *list = nextNode;
            if (nextNode == 0)
            {
                list[1] = 0;
                list[4] = list[4] - 1;
                return nodeDelta;
            }
            *(int *)(nextNode + 4) = 0;
            list[4] = list[4] - 1;
            return nodeDelta;
        }
    }
    else
    {
        edgeNode = (int *)list[1];
        if (edgeNode == targetNode)
        {
            if (edgeNode != (int *)0x0)
            {
                previousNode = (int *)edgeNode[1];
                list[1] = (int)previousNode;
                if (previousNode == (int *)0x0)
                {
                    *list = 0;
                    list[4] = list[4] - 1;
                    return nodeDelta;
                }
                *previousNode = 0;
                list[4] = list[4] - 1;
                return nodeDelta;
            }
        }
        else
        {
            *(int *)targetNode[1] = *targetNode;
            *(int *)(*targetNode + 4) = targetNode[1];
            list[4] = list[4] - 1;
        }
    }
    return nodeDelta;
}
