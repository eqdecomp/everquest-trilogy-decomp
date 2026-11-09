/* msg_outgoing.cpp - outgoing RDP messages: the header fields written in front of the payload.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/msg_outgoing.c (from Mac debug map)
 *
 * C-compatible code (the rdp library is plain C).
 */
#include <windows.h>
#include <stddef.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

/* 0x005003B0: the import thunk for WSOCK32 htons. */
#define rdp_htons_guess        EQ_STDCALL(u16, 0x005003B0, (u16 n))

/* An outgoing message, partly known (no header layout yet: offsets from 0x004F7EB0 only). The flag bits
 * at 0x19 pick which optional header fields are serialised big-endian into hdr[] at 0x30. */
/* RdpMsgOutgoing_guess: the actor layout is RdpMsgOutgoing_guess (generated: trilogy/layout/rdp_message.h); the old local view's members map by offset. */
#include "trilogy/layout/rdp_message.h"
C_ASSERT(offsetof(RdpMsgOutgoing_guess, hdr) == 0x30);

// FUNCTION: TRILOGY 0x004f7eb0
// @early-stop: 33 instruction differences remain in call setup, memory operand ordering, and block scheduling.
// ORIGINAL FILE: Network/rdp/msg_outgoing.c (from Mac debug map)
// __cdecl. Resets the message and serialises its optional header fields (chosen by flags) into hdr[].
void msg_outgoing_init(volatile RdpMsgOutgoing_guess *msg)
{
    int length;
    int increment;
    int zero;
    u16 value;

    msg->self = (void *)msg;
    msg->unk_0x0C = 0;
    /* fake match: reuse the cleared field as zero in EBX and derive the +2 step for EDI. */
    zero = msg->unk_0x0C;
    increment = zero + 2;
    msg->hdr_len = zero;
    msg->sendAttempts_u16_guess = (u16)zero;
    if (msg->flagBytes_guess.flags & 2) {
        value = rdp_htons_guess(msg->field1A_u16_guess);
        *(volatile u16 *)(msg->hdr + msg->hdr_len) = value;
        length = msg->hdr_len + increment;
        msg->hdr_len = length;
    }
    if (msg->flagBytes_guess.flags & 8) {
        value = rdp_htons_guess(msg->field1C);
        *(volatile u16 *)(msg->hdr + msg->hdr_len) = value;
        length = msg->hdr_len + increment;
        msg->hdr_len = length;
        value = rdp_htons_guess(msg->field1E);
        *(volatile u16 *)(msg->hdr + msg->hdr_len) = value;
        length = msg->hdr_len + increment;
        msg->hdr_len = length;
        value = rdp_htons_guess(msg->field20);
        *(volatile u16 *)(msg->hdr + msg->hdr_len) = value;
        length = msg->hdr_len + increment;
        msg->hdr_len = length;
    }
    if (msg->flagBytes_guess.flags & 0x10) {
        msg->hdr[msg->hdr_len] = msg->byte22_u8_guess;
        length = msg->hdr_len + 1;
        msg->hdr_len = length;
        if (msg->flagBytes_guess.flags & 2) {
            msg->hdr[length] = msg->byte23;
            length = msg->hdr_len + 1;
            msg->hdr_len = length;
        }
    }
    msg->unk_0x2C = zero;
}
