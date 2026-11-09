#ifndef EQGFX_DEVICE_RESOURCE_SCOPE_GUESS_H
#define EQGFX_DEVICE_RESOURCE_SCOPE_GUESS_H

#include "trilogy/eqaddr.h"

// Base-resource cleanup remains active while a device releases its owned allocations.
struct DeviceResourceScope_guess {
    void *owner;
    ~DeviceResourceScope_guess() {
        *(void **)owner = (void *)0x10079710;
        EQ_METHOD(0, void, 0x10062E6C, (void *), owner)();
    }
};

#endif
