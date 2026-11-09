// Member declarations for this original eqgfx address unit. Layouts precede this include.
class ListOwnerMethods10059A86_guess : public ListOwner_guess {
public:
    void freeListNodes_guess();
    void destroyListContents_guess();
    void insertSortedListNode_guess(ListNode_guess *node);
    void clearListOwner_guess();
    void freeOwnedListBlocks_guess();
    void destroyAndFreeOwnedListBlocks_guess();
};

class BufferSetMethods10059A86_guess : public BufferSet_guess {
public:
    void destroyOwnedMembers_guess();
};

class MappedFileMethods10059A86_guess : public MappedFile_guess {
public:
    int thunk_FUN_10061942();
};

