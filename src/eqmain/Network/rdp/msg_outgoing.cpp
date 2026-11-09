/* msg_outgoing.cpp - outgoing RDP messages (eqmain.dll): the header fields written in front of the payload.
 *
 * C-compatible code (the rdp library is plain C).
 */
#include <stddef.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

/* 0x100322AE: the client's import thunk for WSOCK32 htons (jmp [IAT]). */
#define rdp_htons_guess EQ_STDCALL(u16, 0x100322AE, (u16 n))

/* An outgoing message as eqmain.dll lays it out. Every field this function touches sits 8 bytes lower than
 * in the trilogy sibling (src/Network/rdp/msg_outgoing.cpp); there is no self pointer here. The flag bits at
 * 0x11 pick which optional header fields are serialised big-endian into hdr[] at 0x28. File-local until
 * eqmain/rdp.h has the struct. */
/* RdpMsgOutgoing_eqmain_guess: the actor layout is RdpMsgOutgoing_eqmain_guess (generated: eqmain/layout/rdp_message.h); the old local view's members map by offset. */
#include "eqmain/layout/rdp_message.h"
C_ASSERT(offsetof(RdpMsgOutgoing_eqmain_guess, hdr) == 0x28);

// FUNCTION: EQMAIN 0x10030f30
// Prepares an outgoing RDP message for transmission.
// @early-stop: the constant 2 is held in EBX (its flag tests need a byte register), where the original keeps it in EDI and the zero in EBX; block 3 tests flags after the hdr_len store.
// __cdecl. Resets the message and serialises its optional header fields (chosen by flags) into hdr[].
// hdr_len is re-read from the message after each store through hdr (a char/short store may alias it), as the original does.
void msg_outgoing_init_guess(RdpMsgOutgoing_eqmain_guess *msg)
{
    u16 value;
    msg->hdr_len = 0;
    msg->sendCount_u16_guess = 0;
    if (msg->flagBytes_guess.flags & 2) {
        value = rdp_htons_guess(msg->field12_u16_guess);
        *(u16 *)(msg->hdr + msg->hdr_len) = value;
        msg->hdr_len = msg->hdr_len + 2;
    }
    if (msg->flagBytes_guess.flags & 8) {
        value = rdp_htons_guess(msg->field14);
        *(u16 *)(msg->hdr + msg->hdr_len) = value;
        msg->hdr_len = msg->hdr_len + 2;
        value = rdp_htons_guess(msg->field16_u16_guess);
        *(u16 *)(msg->hdr + msg->hdr_len) = value;
        msg->hdr_len = msg->hdr_len + 2;
        value = rdp_htons_guess(msg->field18);
        *(u16 *)(msg->hdr + msg->hdr_len) = value;
        msg->hdr_len = msg->hdr_len + 2;
    }
    if (msg->flagBytes_guess.flags & 0x10) {
        *(msg->hdr + msg->hdr_len) = msg->byte1A_u8_guess;
        msg->hdr_len = msg->hdr_len + 1;
        if (msg->flagBytes_guess.flags & 2) {
            *(msg->hdr + msg->hdr_len) = msg->byte1B;
            msg->hdr_len = msg->hdr_len + 1;
        }
    }
    msg->unknown24 = 0;
}
