// CFLAGS: /O2
#include "trilogy/eqaddr.h"

struct OpenPackNameKey_guess;

struct FfResourceOwner_guess
{
    int *resource_guess;

    void releaseResource_guess(void);
    bool openPackedFile_guess(OpenPackNameKey_guess archiveKey, OpenPackNameKey_guess fileKey);
};

#define closeFileHandle_guess(object) EQ_METHOD(0, void, 0x10052A20, (FfResourceOwner_guess *), (object))

// A reference-counted object: slot 0 of its vtable is the deleting destructor, the count is at 0x4.
struct OpenPackCounted_guess
{
    virtual void destroy_guess(int flags);
    int refCount;                           // 0x4
};

// The archive's file name string: text at 0x8, length at 0xc.
struct OpenPackNameString_guess : OpenPackCounted_guess
{
    char *text;                             // 0x8
    unsigned int length;                    // 0xc
};

// A node of an archive's file map (keyed by the file name hash).
struct OpenPackFileNode_guess
{
    OpenPackFileNode_guess *left;           // 0x0
    OpenPackFileNode_guess *parent;         // 0x4
    OpenPackFileNode_guess *right;          // 0x8
    unsigned int hash;                      // 0xc
    int unknown10;                          // 0x10
    int offset;                             // 0x14
    int size;                               // 0x18
};

struct OpenPackFileIterator_guess
{
    OpenPackFileNode_guess *node;           // 0x0
};

struct OpenPackFileMap_guess
{
    int unknown0;                           // 0x0
    OpenPackFileNode_guess *head;           // 0x4
};

struct OpenPackFileMapHolder_guess : OpenPackCounted_guess
{
    OpenPackFileMap_guess *map;             // 0x8
};

struct OpenPackArchive_guess
{
    OpenPackNameString_guess *name;         // 0x0
    int unknown4;                           // 0x4
    OpenPackFileMapHolder_guess *files;     // 0x8
};

struct OpenPackArchiveHolder_guess : OpenPackCounted_guess
{
    OpenPackArchive_guess *archive;         // 0x8
};

// A shared reference to an archive; copied inline, released when it goes away.
struct OpenPackArchiveRef_guess
{
    OpenPackArchiveHolder_guess *object;    // 0x0

    OpenPackArchiveRef_guess()
    {
        object = 0;
    }
    OpenPackArchiveRef_guess(const OpenPackArchiveRef_guess &other)
    {
        object = other.object;
        if (object != 0)
            object->refCount++;
    }
    ~OpenPackArchiveRef_guess()
    {
        if (object != 0 && --object->refCount == 0 && object != 0)
            object->destroy_guess(1);
    }
};

// A shared reference to a name; constructed (0x10021938) and copied (0x10053de0) out of line.
struct OpenPackNameRef_guess
{
    OpenPackCounted_guess *object;          // 0x0

    OpenPackNameRef_guess(const char *text)
    {
        EQ_METHOD(1, OpenPackNameRef_guess *, 0x10021938, (OpenPackNameRef_guess *, const char *), this)(text);
    }
    OpenPackNameRef_guess(const OpenPackNameRef_guess &other)
    {
        EQ_METHOD(1, OpenPackNameRef_guess *, 0x10053DE0, (OpenPackNameRef_guess *, const OpenPackNameRef_guess *),
                  this)(&other);
    }
    ~OpenPackNameRef_guess()
    {
        if (object != 0 && --object->refCount == 0 && object != 0)
            object->destroy_guess(1);
    }
};

// A name and its hash (0x100526c0 fills the hash in on first use).
struct OpenPackNameKey_guess
{
    OpenPackNameRef_guess name;             // 0x0
    unsigned int hash;                      // 0x4
};

struct OpenPackArchiveNode_guess;

struct OpenPackArchiveIterator_guess
{
    OpenPackArchiveNode_guess *node;        // 0x0
};

// The value of an archive map node: the hash, then the archive.
struct OpenPackArchiveEntry_guess
{
    unsigned int hash;                      // 0x0
    OpenPackArchiveRef_guess archive;       // 0x4
};

struct OpenPackArchiveMap_guess
{
    int unknown0;                           // 0x0
    OpenPackArchiveNode_guess *head;        // 0x4
};

// The read-ahead file of one packed entry (0x2024 bytes); vtable slot 0 is readFileData (0x10051e80).
struct OpenPackFileData_guess
{
    unsigned int vtable;                    // 0x0
    int fd;                                 // 0x4
    char buffer[0x2000];                    // 0x8
    int size;                               // 0x2008
    int position;                           // 0x200c
    int bufferPosition;                     // 0x2010
    int offset;                             // 0x2014
    OpenPackNameRef_guess name;             // 0x2018
    unsigned int hash;                      // 0x201c
    int unknown2020;                        // 0x2020

    OpenPackFileData_guess(int fileOffset)
        : fd(-1), size(0), position(0), bufferPosition(0), offset(fileOffset), name(0), unknown2020(0)
    {
        vtable = 0x10076408;
    }
    static void *operator new(unsigned int bytes)
    {
        return EQ_CDECL(void *, 0x10068C72, (unsigned int))(bytes);
    }
    static void operator delete(void *memory)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(memory);
    }
};

// Reads the first block of a packed file through vtable slot 0; returns the byte count.
struct OpenPackFileReader_guess
{
    virtual int readFileData_guess(void);
};

struct OpenPackLogEntry_guess
{
    const char *file;                       // 0x0
    int line;                               // 0x4
};

typedef void (__stdcall *OpenPackLockFunction_guess)(void *lock);
#define enterPackFsLock_guess EQ_GLOBAL(OpenPackLockFunction_guess, 0x100760B4)
#define leavePackFsLock_guess EQ_GLOBAL(OpenPackLockFunction_guess, 0x100760B8)
#define g_packfs_lock_guess EQ_ARRAY(void, 0x100FBBD0)
#define g_packfs_archives_guess EQ_GLOBAL(OpenPackArchiveMap_guess, 0x100FBBE8)
#define g_packfs_log_count_guess EQ_GLOBAL(unsigned int, 0x100FC014)
#define g_packfs_log_entries_guess EQ_ARRAY(OpenPackLogEntry_guess, 0x100FBC10)
#define PACKFS_LOG_CAPACITY 0x40
#define PACKFS_SOURCE_FILE ((const char *)0x100a3b68)
#define PACKFS_NEW_FILE_DATA_TEXT ((const char *)0x100a3bec)
#define PACKFS_ALLOCATION_ERROR ((const char *)0x10087020)
#define CRT_O_BINARY 0x8000

// FUNCTION: EQGFX 0x10052730
// The entry point forwards this resource owner to the shared file-handle cleanup routine.
void FfResourceOwner_guess::releaseResource_guess(void)
{
    closeFileHandle_guess(this)();
}

// FUNCTION: EQGFX 0x10052740
// Opens a file within a selected archive for reading.
// __thiscall, both keys by value (callee pops 16). Looks fileKey up in archive archiveKey under the pack lock and
// stores a new open OpenPackFileData_guess in *this; returns 1 when its first block read, else 0.
#pragma optimize("t", on)
bool FfResourceOwner_guess::openPackedFile_guess(OpenPackNameKey_guess archiveKey, OpenPackNameKey_guess fileKey)
{
    bool opened;

    enterPackFsLock_guess(g_packfs_lock_guess);
    EQ_METHOD(0, void, 0x10052A20, (FfResourceOwner_guess *), this)();
    {
        OpenPackArchiveRef_guess archiveRef;
        unsigned int archiveHash;
        OpenPackArchiveIterator_guess archiveIter;

        EQ_METHOD(0, void, 0x100526C0, (OpenPackNameKey_guess *), &archiveKey)();
        archiveHash = archiveKey.hash;
        EQ_METHOD(2, OpenPackArchiveIterator_guess *, 0x100534F0,
                  (OpenPackArchiveMap_guess *, OpenPackArchiveIterator_guess *, const unsigned int *),
                  &g_packfs_archives_guess)(&archiveIter, &archiveHash);
        if (g_packfs_archives_guess.head != archiveIter.node)
        {
            OpenPackFileIterator_guess fileIter;
            unsigned int fileHash;
            OpenPackArchiveEntry_guess *entry;

            entry = EQ_METHOD(0, OpenPackArchiveEntry_guess *, 0x10052DC0, (OpenPackArchiveIterator_guess *),
                              &archiveIter)();
            EQ_METHOD(1, OpenPackArchiveRef_guess *, 0x100545D0, (OpenPackArchiveRef_guess *, OpenPackArchiveRef_guess),
                      &archiveRef)(entry->archive);
            EQ_METHOD(0, void, 0x100526C0, (OpenPackNameKey_guess *), &fileKey)();
            fileHash = fileKey.hash;
            EQ_METHOD(2, OpenPackFileIterator_guess *, 0x10053650,
                      (OpenPackFileMap_guess *, OpenPackFileIterator_guess *, const unsigned int *),
                      archiveRef.object->archive->files->map)(&fileIter, &fileHash);
            if (archiveRef.object->archive->files->map->head != fileIter.node)
            {
                OpenPackFileData_guess *fileData;
                OpenPackNameString_guess *archiveName;
                const char *fileName;
                int fd;

                fileData = new OpenPackFileData_guess(fileIter.node->offset);
                if (g_packfs_log_count_guess < PACKFS_LOG_CAPACITY)
                {
                    g_packfs_log_entries_guess[g_packfs_log_count_guess].file = PACKFS_SOURCE_FILE;
                    g_packfs_log_entries_guess[g_packfs_log_count_guess].line = 0x2fa;
                    g_packfs_log_count_guess++;
                }
                if (fileData == 0)
                    EQ_CDECL(void, 0x100549A0, (int, const char *, const char *))(0, PACKFS_ALLOCATION_ERROR,
                                                                                 PACKFS_NEW_FILE_DATA_TEXT);
                archiveName = archiveRef.object->archive->name;
                if (archiveName != 0 && archiveName->length > 0)
                    fileName = archiveName->text;
                else
                    fileName = 0;
                fd = EQ_CDECL(int, 0x1006AC3A, (const char *, int))(fileName, CRT_O_BINARY);
                fileData->fd = fd;
                fileData->size = fileIter.node->size;
                EQ_CDECL(long, 0x1006AB62, (int, long, int))(fd, fileData->offset, 0);
                if (fileData != 0)
                {
                    if (((OpenPackFileReader_guess *)fileData)->readFileData_guess() > 0)
                    {
                        fileData->position = 0;
                        fileData->bufferPosition = 0;
                        EQ_METHOD(1, OpenPackNameRef_guess *, 0x10021AF3, (OpenPackNameRef_guess *, OpenPackNameRef_guess),
                                  &fileData->name)(fileKey.name);
                        opened = true;
                        fileData->hash = fileKey.hash;
                        resource_guess = (int *)fileData;
                        goto unlock;
                    }
                    if (fileData->name.object != 0)
                        EQ_METHOD(0, void, 0x1002190D, (OpenPackCounted_guess *), fileData->name.object)();
                    EQ_CDECL(void, 0x10050CBB, (void *))(fileData);
                }
            }
        }
        opened = false;
    unlock:
        leavePackFsLock_guess(g_packfs_lock_guess);
    }
    return opened;
}
