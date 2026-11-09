// unit_10009B10.cpp - eqmain.dll, the address run 0x10009B10..0x10009FB0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).
// Third-party code: this file reconstructs, from the binary, functions of zlib 1.1.2 (Copyright (C) 1995-1998
// Jean-loup Gailly and Mark Adler) that eqmain.dll links. It is ALTERED: written from the compiled code, not
// the original zlib source (names and layouts follow zlib's). See THIRD_PARTY.md.

#include "trilogy/eqaddr.h"

#ifdef EQD_MATCH
extern char eqd_va_0x1004195C;
extern char eqd_va_0x1004175C;
extern char eqd_va_0x1004185C;
#endif

// zlib 1.1.x's z_stream, as far as deflate_fast reads it.
/* z_stream: the actor layout is z_stream (generated: eqmain/layout/z_stream.h); the old local view's members map by offset. */
#include "eqmain/layout/z_stream.h"

// zlib's ct_data: one 4-byte node of a Huffman tree.
/* CtData_guess: the layout is ct_data (generated: eqmain/layout/deflate_state.h); the old local view's members map by offset. */
#include "eqmain/layout/deflate_state.h"

// zlib 1.1.x's deflate_state (the queue unit): the full layout is deflate_state (generated:
// eqmain/layout/deflate_state.h, included above); the old local view's members keep their names.
// deflate_fast reads max_lazy_match as max_insert_length (zlib's #define, below).

#define max_insert_length max_lazy_match

#define MIN_MATCH       3
#define MAX_MATCH       258
#define MIN_LOOKAHEAD   0x106
#define MAX_DIST(s)     ((s)->w_size - MIN_LOOKAHEAD)
#define NIL             0
#define TOO_FAR         0x1000
#define LITERALS        256
#define Z_NO_FLUSH      0
#define Z_FINISH        4
#define Z_HUFFMAN_ONLY  2

#define NEED_MORE       0
#define BLOCK_DONE      1
#define FINISH_STARTED  2
#define FINISH_DONE     3

#define g_length_code_guess     EQ_ARRAY(unsigned char, 0x1004195C)
#define g_dist_code_guess       EQ_ARRAY(unsigned char, 0x1004175C)
#define g_distCodeHigh_guess    EQ_ARRAY(unsigned char, 0x1004185C)

#define fill_window_guess       EQ_CDECL(void, 0x10009960, (deflate_state *))
#define longest_match_guess     EQ_CDECL(unsigned int, 0x10009E40, (deflate_state *, unsigned int))
#define tr_flush_block_guess    EQ_CDECL(unsigned int, 0x1000CD90, (deflate_state *, unsigned char *, unsigned int, int))
#define inflate_flush_guess     EQ_CDECL(void, 0x10009780, (z_stream *))

// FUNCTION: EQMAIN 0x10009b10
// Compresses input data for output by the client.
// __cdecl. zlib 1.1.x's deflate_fast: hashes each string, takes a match from longest_match when one is near
// enough, tallies literals and matches inline, flushes full blocks. Returns need_more/block_done/finish_*.
int __cdecl DeflateFast_guess(deflate_state *s, int flush)
{
    unsigned int hash_head;
    int bflush;

    hash_head = 0;
    for (;;)
    {
        if (s->lookahead < MIN_LOOKAHEAD)
        {
            fill_window_guess(s);
            if (s->lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH)
            {
                return NEED_MORE;
            }
            if (s->lookahead == 0)
                break;
        }
        if (s->lookahead >= MIN_MATCH)
        {
            s->ins_h = ((s->ins_h << s->hash_shift) ^ s->window[s->strstart + (MIN_MATCH - 1)]) & s->hash_mask;
            hash_head = s->prev[s->strstart & s->w_mask] = s->head[s->ins_h];
            s->head[s->ins_h] = (unsigned short)s->strstart;
        }
        if (hash_head != 0 && s->strstart - hash_head <= s->w_size - MIN_LOOKAHEAD)
        {
            if (s->strategy != Z_HUFFMAN_ONLY)
            {
                s->match_length = longest_match_guess(s, hash_head);
            }
        }
        if (s->match_length >= MIN_MATCH)
        {
            unsigned char len = (unsigned char)(s->match_length - MIN_MATCH);
            unsigned short dist = (unsigned short)(s->strstart - s->match_start);
            s->d_buf[s->last_lit] = dist;
            s->l_buf[s->last_lit++] = len;
            dist--;
            s->dyn_ltree[g_length_code_guess[len] + LITERALS + 1].freq++;
            s->dyn_dtree[dist < 256 ? g_dist_code_guess[dist] : g_distCodeHigh_guess[dist >> 7]].freq++;
            bflush = (s->last_lit == s->lit_bufsize - 1);

            s->lookahead -= s->match_length;
            if (s->match_length <= s->max_insert_length && s->lookahead >= MIN_MATCH)
            {
                s->match_length--;
                do
                {
                    s->strstart++;
                    s->ins_h = ((s->ins_h << s->hash_shift) ^ s->window[s->strstart + (MIN_MATCH - 1)]) & s->hash_mask;
                    hash_head = s->prev[s->strstart & s->w_mask] = s->head[s->ins_h];
                    s->head[s->ins_h] = (unsigned short)s->strstart;
                } while (--s->match_length != 0);
                s->strstart++;
            }
            else
            {
                s->strstart += s->match_length;
                s->match_length = 0;
                s->ins_h = s->window[s->strstart];
                s->ins_h = ((s->ins_h << s->hash_shift) ^ s->window[s->strstart + 1]) & s->hash_mask;
            }
        }
        else
        {
            unsigned char cc = s->window[s->strstart];
            s->d_buf[s->last_lit] = 0;
            s->l_buf[s->last_lit++] = cc;
            s->dyn_ltree[cc].freq++;
            bflush = (s->last_lit == s->lit_bufsize - 1);
            s->lookahead--;
            s->strstart++;
        }
        if (bflush)
        {
            tr_flush_block_guess(s, s->block_start >= 0L ? &s->window[(unsigned int)s->block_start] : 0,
                                 (unsigned int)((long)s->strstart - s->block_start), 0);
            s->block_start = s->strstart;
            inflate_flush_guess(s->strm);
            if (s->strm->avail_out == 0)
                return NEED_MORE;
        }
    }
    tr_flush_block_guess(s, s->block_start >= 0L ? &s->window[(unsigned int)s->block_start] : 0,
                         (unsigned int)((long)s->strstart - s->block_start), flush == Z_FINISH);
    s->block_start = s->strstart;
    inflate_flush_guess(s->strm);
    if (s->strm->avail_out == 0)
        return flush == Z_FINISH ? FINISH_STARTED : NEED_MORE;
    return flush == Z_FINISH ? FINISH_DONE : BLOCK_DONE;
}

// FUNCTION: EQMAIN 0x10009e40
// __cdecl. zlib 1.1.x's longest_match (deflate.c): walks the hash chain from cur_match and returns the best length.
unsigned int __cdecl LongestMatch_guess(deflate_state *s, unsigned int cur_match)
{
    unsigned int chain_length = s->max_chain_length;
    unsigned char *scan = s->window + s->strstart;
    unsigned char *match;
    int len;
    int best_len = s->prev_length;
    int nice_match = s->nice_match;
    unsigned int limit = s->strstart > (unsigned int)MAX_DIST(s) ?
        s->strstart - (unsigned int)MAX_DIST(s) : NIL;
    unsigned short *prev = s->prev;
    unsigned int wmask = s->w_mask;
    unsigned char *strend = s->window + s->strstart + MAX_MATCH;
    unsigned char scan_end1 = scan[best_len - 1];
    unsigned char scan_end = scan[best_len];

    if (s->prev_length >= s->good_match) {
        chain_length >>= 2;
    }
    if ((unsigned int)nice_match > s->lookahead) nice_match = s->lookahead;

    do {
        match = s->window + cur_match;
        if (match[best_len] != scan_end ||
            match[best_len - 1] != scan_end1 ||
            *match != *scan ||
            *++match != scan[1]) continue;

        scan += 2, match++;
        do {
        } while (*++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 scan < strend);

        len = MAX_MATCH - (int)(strend - scan);
        scan = strend - MAX_MATCH;

        if (len > best_len) {
            s->match_start = cur_match;
            best_len = len;
            if (len >= nice_match) break;
            scan_end1 = scan[best_len - 1];
            scan_end = scan[best_len];
        }
    } while ((cur_match = prev[cur_match & wmask]) > limit
             && --chain_length != 0);

    if ((unsigned int)best_len <= s->lookahead) return (unsigned int)best_len;
    return s->lookahead;
}

// FUNCTION: EQMAIN 0x10009fb0
// __cdecl. zlib 1.1's deflate_slow (lazy match evaluation): returns 0 need_more, 1 block_done, 2/3 finish_started/done.
int __cdecl DeflateSlow_guess(deflate_state *s, int flush)
{
    unsigned int hash_head = 0;
    int bflush;

    for (;;)
    {
        if (s->lookahead < MIN_LOOKAHEAD)
        {
            fill_window_guess(s);
            if (s->lookahead < MIN_LOOKAHEAD && flush == 0)
            {
                return 0;
            }
            if (s->lookahead == 0)
            {
                break;
            }
        }
        if (s->lookahead >= 3)
        {
            s->ins_h = ((s->ins_h << s->hash_shift) ^ s->window[s->strstart + 2]) & s->hash_mask;
            s->prev[s->strstart & s->w_mask] = hash_head = s->head[s->ins_h];
            s->head[s->ins_h] = (unsigned short)s->strstart;
        }
        s->prev_length = s->match_length, s->prev_match = s->match_start;
        s->match_length = 2;

        if (hash_head != 0 && s->prev_length < s->max_lazy_match && s->strstart - hash_head <= s->w_size - MIN_LOOKAHEAD)
        {
            if (s->strategy != 2)
            {
                s->match_length = longest_match_guess(s, hash_head);
            }
            if (s->match_length <= 5 &&
                (s->strategy == 1 || (s->match_length == 3 && s->strstart - s->match_start > TOO_FAR)))
            {
                s->match_length = 2;
            }
        }
        if (s->prev_length >= 3 && s->match_length <= s->prev_length)
        {
            unsigned int max_insert = s->strstart + s->lookahead - 3;
            {
                unsigned char len = (unsigned char)(s->prev_length - 3);
                unsigned short dist = (unsigned short)(s->strstart - 1 - s->prev_match);
                s->d_buf[s->last_lit] = dist;
                s->l_buf[s->last_lit++] = len;
                dist--;
                s->dyn_ltree[g_length_code_guess[len] + 256 + 1].freq++;
                s->dyn_dtree[dist < 256 ? g_dist_code_guess[dist] : g_dist_code_guess[256 + (dist >> 7)]].freq++;
                bflush = (s->last_lit == s->lit_bufsize - 1);
            }
            s->lookahead -= s->prev_length - 1;
            s->prev_length -= 2;
            do
            {
                if (++s->strstart <= max_insert)
                {
                    s->ins_h = ((s->ins_h << s->hash_shift) ^ s->window[s->strstart + 2]) & s->hash_mask;
                    s->prev[s->strstart & s->w_mask] = hash_head = s->head[s->ins_h];
                    s->head[s->ins_h] = (unsigned short)s->strstart;
                }
            } while (--s->prev_length != 0);
            s->match_available = 0;
            s->match_length = 2;
            s->strstart++;

            if (bflush)
            {
                tr_flush_block_guess(s, s->block_start >= 0L ? &s->window[(unsigned int)s->block_start] : 0,
                                     (unsigned int)((long)s->strstart - s->block_start), 0);
                s->block_start = s->strstart;
                inflate_flush_guess(s->strm);
                if (s->strm->avail_out == 0)
                {
                    return 0;
                }
            }
        }
        else if (s->match_available)
        {
            {
                unsigned char cc = s->window[s->strstart - 1];
                s->d_buf[s->last_lit] = 0;
                s->l_buf[s->last_lit++] = cc;
                s->dyn_ltree[cc].freq++;
                bflush = (s->last_lit == s->lit_bufsize - 1);
            }
            if (bflush)
            {
                tr_flush_block_guess(s, s->block_start >= 0L ? &s->window[(unsigned int)s->block_start] : 0,
                                     (unsigned int)((long)s->strstart - s->block_start), 0);
                s->block_start = s->strstart;
                inflate_flush_guess(s->strm);
            }
            s->strstart++;
            s->lookahead--;
            if (s->strm->avail_out == 0)
            {
                return 0;
            }
        }
        else
        {
            s->match_available = 1;
            s->strstart++;
            s->lookahead--;
        }
    }
    if (s->match_available)
    {
        unsigned char cc = s->window[s->strstart - 1];
        s->d_buf[s->last_lit] = 0;
        s->l_buf[s->last_lit++] = cc;
        s->dyn_ltree[cc].freq++;
        s->match_available = 0;
    }
    tr_flush_block_guess(s, s->block_start >= 0L ? &s->window[(unsigned int)s->block_start] : 0,
                         (unsigned int)((long)s->strstart - s->block_start), flush == 4);
    s->block_start = s->strstart;
    inflate_flush_guess(s->strm);
    if (s->strm->avail_out == 0)
    {
        return flush == 4 ? 2 : 0;
    }
    return flush == 4 ? 3 : 1;
}
