/*
 * suite_hash_table.h - the shared pieces of the client's STL hash_map (an STLport-style hashtable).
 *
 * A table is 0x14 bytes: three empty function objects (hasher, key equality, key extractor) at +0..+2,
 * the bucket vector at +4 (first, last, end of storage), and the element count at +0x10. The end of
 * storage and the count each carry the allocator as an empty base.
 *
 * The allocator is empty but has user-declared (empty) constructors and destructor, so allocator
 * temporaries and the allocator-carrying members have unwind entries (the destructor is the client's
 * folded empty function 0x004032A0). A hash_map's default constructor passes an allocator temporary to
 * the table constructor; that temporary lives until the table (including its bucket initialisation)
 * has been built, which gives the EH state 0 the constructors store after the first two vector words.
 *
 * Each table type supplies its own bucket initialisation method (it differs by value type), so the
 * table classes themselves live next to their methods; this header only holds the common members.
 *
 * C++ only (VC6).
 */
#ifndef TRILOGY_SUITE_HASH_TABLE_H
#define TRILOGY_SUITE_HASH_TABLE_H

#include "trilogy/eqaddr.h"

/* Empty function objects: one byte each in the table. */
struct SuiteHasher_guess {};
struct SuiteKeyEqual_guess {};
struct SuiteKeyExtractor_guess {};

class SuiteAllocator_guess
{
public:
    SuiteAllocator_guess() {}
    SuiteAllocator_guess(const SuiteAllocator_guess&) {}
    ~SuiteAllocator_guess() {}
};

/* A value stored together with the allocator (empty base). */
template <class T>
class SuiteAllocProxy_guess : public SuiteAllocator_guess
{
public:
    T data;

    SuiteAllocProxy_guess(const SuiteAllocator_guess& allocator, T value)
        : SuiteAllocator_guess(allocator), data(value)
    {
    }
};

/* The bucket vector: first / last / end of storage. */
class SuiteBucketVector_guess
{
public:
    void** first;
    void** last;
    SuiteAllocProxy_guess<void**> endOfStorage;

    SuiteBucketVector_guess(const SuiteAllocator_guess& allocator)
        : first(0), last(0), endOfStorage(allocator, 0)
    {
    }
    /* Returns the bucket storage to the Suite pool (pool_free_guess); the client's out-of-line copy, which the
       unwind code calls, is the tail jump at 0x00401127. */
    ~SuiteBucketVector_guess()
    {
        EQ_METHOD(0, void, 0x00401167, (void*), this)();
    }
};

#endif /* TRILOGY_SUITE_HASH_TABLE_H */
