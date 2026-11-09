// unit_1002DD60.cpp - eqmain.dll, the address run 0x1002DE10.. (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <string.h>
#include "trilogy/eqaddr.h"

// The message being reassembled, as eqmain.dll lays it out: RdpMsgArrival_guess (include/trilogy/rdp.h) with
// its 8-byte link gone, so every field sits 8 bytes lower. Fragment i's payload starts at 0x34 + i * 0x200.
typedef struct EqmainMsgArrival_guess
{
    unsigned char unknown00[8];     // 0x00
    void *sender;                   // 0x08  the connection; set by fragment 0
    unsigned short flags;           // 0x0C  copied from fragment 0
    unsigned short seq;             // 0x0E  copied from fragment 0
    unsigned short first_frag_seq;  // 0x10  frag_seq of fragment 0
    unsigned char unknown12[2];     // 0x12
    unsigned short frag_count;      // 0x14
    unsigned char channel;          // 0x16
    unsigned char channel_seq;      // 0x17
    unsigned short frags_received;  // 0x18
    unsigned char unknown1A[2];     // 0x1A
    unsigned int size;              // 0x1C  set when the last fragment arrives
    unsigned char unknown20[0x14];  // 0x20
    unsigned char data[1];          // 0x34  fragment slots of 0x200 bytes; the extent is unknown
} EqmainMsgArrival_guess;

// The parsed fragment header: the fields this function reads (RdpPacketInfo_guess in include/trilogy/rdp.h).
typedef struct EqmainPacketInfo_guess
{
    unsigned short flags;           // 0x00
    unsigned short seq;             // 0x02
    unsigned char unknown04[4];     // 0x04
    unsigned short frag_seq;        // 0x08  sequence number of this fragment
    unsigned char unknown0A[2];     // 0x0A
    unsigned short frag_index;      // 0x0C  0-based fragment number within the message
    unsigned short frag_count;      // 0x0E
    unsigned char channel;          // 0x10
    unsigned char channel_seq;      // 0x11
    unsigned char unknown12[2];     // 0x12
    unsigned short data_len;        // 0x14  payload bytes of this fragment
} EqmainPacketInfo_guess;

// FUNCTION: EQMAIN 0x1002dd60
// Assembles a received network message from its packet fragments.
// __cdecl. Adds one fragment: fragment 0 also sets the message's header fields and sender. Copies the payload to
// its slot, sets the size when the last fragment (by index) arrives, and returns whether all fragments are in
// (frag_count == frags_received). Ghidra types the result bool; EAX holds 0 or 1. No bounds checks.
int __cdecl AddMessageFragment_guess(EqmainMsgArrival_guess *msg, void *sender, EqmainPacketInfo_guess *info,
                                     const void *data)
{
    if (info->frag_index == 0)
    {
        msg->flags = info->flags;
        msg->seq = info->seq;
        msg->first_frag_seq = info->frag_seq;
        msg->frag_count = info->frag_count;
        msg->channel = info->channel;
        msg->channel_seq = info->channel_seq;
        msg->sender = sender;
    }
    msg->frags_received++;
    memcpy(msg->data + info->frag_index * 0x200, data, info->data_len);
    if (info->frag_index + 1 == info->frag_count)
        msg->size = info->frag_index * 0x200 + info->data_len;
    return info->frag_count == msg->frags_received;
}

// FUNCTION: EQMAIN 0x1002de10
// Initializes an unidentified front-end record; its game purpose is unclear.
// __cdecl. Zeroes 13 dwords (0x34 bytes) at the record, then stores value at +0x8 (the third dword).
// The record's layout is unknown.
void __cdecl ClearRecordAndSetDword_guess(unsigned int* record, unsigned int value)
{
    memset(record, 0, 0x34);
    record[2] = value;
}
