# hooks

`decomp/eqd_crt.c` holds the client's version of the compiler helper `__ftol`, transcribed from the client
because the fixtures depend on its exact behaviour. Every test DLL links it.

The rest of this project's client hook tooling is not published. Documents that mention `hooks/`, `eqlaunch`
or `eqdecomp.dll` describe that private setup.
