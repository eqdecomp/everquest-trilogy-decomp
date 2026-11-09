# Third-party code and notices

This repository reconstructs, from the compiled client, source code for some libraries that the original
EverQuest Trilogy binaries link statically. That code was **written from the binary, not copied from the
libraries' source**. It is **altered**: names, layouts and structure follow the libraries where they could be
recognised, and the text is largely ours, though some lines necessarily match the original source. The files
concerned say so in a header comment.

## zlib 1.1.2

- Where: `src/eqmain/unit_10008D60.cpp` (inflate/deflate/trees/adler32 as linked into eqmain.dll),
  `src/eqmain/unit_10009B10.cpp` (deflate_fast, longest_match, deflate_slow), the layouts
  `include/eqmain/layout/{z_stream,deflate_state,inflate_blocks_state}.h`, and the z_stream declaration in
  `include/trilogy/eqhandlemsg.h`. The CRC-32 routines in `src/Network/rdp/cypher.c` and
  `src/eqmain/Network/rdp/cypher.cpp` use zlib's CRC-32 algorithm (the table is read from the user's
  binary, not stored here).
- Version: all three binaries carry `inflate 1.1.2` / `deflate 1.1.2` copyright strings (an older comment
  that said 1.1.3 was wrong and has been corrected).
- **Altered source:** this is not the original zlib source, and must not be misrepresented as such.

zlib's notice:

```
 Copyright (C) 1995-1998 Jean-loup Gailly and Mark Adler

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Jean-loup Gailly        Mark Adler
  jloup@gzip.org          madler@alumni.caltech.edu
```

## libdes 3.24 (Eric Young)

- Where: `src/eqmain/unit_100312C0.cpp` (des_set_key-style key schedule, DES CBC checksum and the front
  end's DES helpers) and `src/eqmain/EverQuest/EQ_CreateBS.cpp` (the DES block function). eqmain.dll
  carries the string `libdes v 3.24 - 20-Apr-1996 - eay`. The DES S-boxes and key tables are read from the
  user's binary by address, not stored here.
- **Altered source:** reconstructed from the binary, not the original libdes source.
- Licence: Eric Young's SSLeay-style licence (a BSD-style licence with an attribution clause). The text
  below is the licence header Eric Young's DES code carried in the mincom.oz.au era, as found in the libdes
  copy that the KAME/NetBSD kernels imported (its source also carries the `libdes v 3.24 - 20-Apr-1996`
  version string). Its terms: keep the copyright notice, credit Eric Young as the author of the parts used
  (this file and the README do), and the licence of this code cannot be changed.

```
Copyright (C) 1995-1998 Eric Young (eay@mincom.oz.au)
All rights reserved.

This file is part of an SSL implementation written
by Eric Young (eay@mincom.oz.au).
The implementation was written so as to conform with Netscapes SSL
specification.  This library and applications are
FREE FOR COMMERCIAL AND NON-COMMERCIAL USE
as long as the following conditions are aheared to.

Copyright remains Eric Young's, and as such any Copyright notices in
the code are not to be removed.  If this code is used in a product,
Eric Young should be given attribution as the author of the parts used.
This can be in the form of a textual message at program startup or
in documentation (online or textual) provided with the package.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   "This product includes software developed by Eric Young (eay@mincom.oz.au)"

THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

The licence and distribution terms for any publically available version or
derivative of this code cannot be changed.  i.e. this code cannot simply be
copied and put under another distribution licence
[including the GNU Public Licence.]
```

This product includes software developed by Eric Young (eay@mincom.oz.au): the DES routines reconstructed
in the files listed above. Those files stay under this licence, not the repository's MIT licence.

## Other library code referenced, not reproduced

- **Microsoft Visual C++ 6 runtime (CRT) and STL:** not included. `hooks/decomp/eqd_crt.c` is a
  transcription of the client's own compiler helper `__ftol`. Some game structures follow the shape of VC6's STL red-black
  tree (`src/ffcreate2/ff/packfs.cpp`, `src/eqgfx/ffcreate2/ff/packfs.cpp`,
  `src/eqgfx/EverQuest/SoundManager.c`); that code is reconstructed from the binary.
- **libxml2:** eqgame.exe links it. Only function names appear (in `names/`); no libxml2 code is here.
- **DirectX SDK and D3DX:** not included and not needed for the Unicorn tests. `eqgfx_dx7.dll` links
  Microsoft's D3DX statically; that part of the image is left out of this repository. DirectX types in the source are our
  own partial declarations; interface IDs and names are interface facts.
- **Miles Sound System (mss32.dll), Smacker:** only import names and call sites. No headers or code.
- **eqgfx_dx7.dll** carries `Copyright 1994 Computer Presentations, Inc.` and `Copyright 1995-1997
  PyroTechnix, Inc.` strings: the renderer's original authors. The renderer reconstruction is part of the
  EverQuest client code (see README.md, "Disclaimer").

## Reference material (facts only, no code)

`reference/MANIFEST.md` lists the community projects used as references (EQEmu, ShowEQ, MacroQuest,
EQClassic, TAKP) and their licences. Facts such as names and packet layouts were taken from them; their
code was not copied.
