/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef FLD_IN_STREAM_H
#define FLD_IN_STREAM_H

#include <stdint.h>
#include <stdlib.h>

typedef struct FldInStream {
    const uint8_t* octets;
    const uint8_t* p;
    size_t size;
    size_t pos;
} FldInStream;

void fldInStreamInit(struct FldInStream* self, const uint8_t* octets, size_t count);
int fldInStreamReadUInt8(struct FldInStream* self, uint8_t* t);
int fldInStreamReadInt8(struct FldInStream* self, int8_t* t);
int fldInStreamReadUInt16(struct FldInStream* self, uint16_t* t);
int fldInStreamReadInt32(struct FldInStream* self, int32_t* t);
int fldInStreamReadUInt32(struct FldInStream* self, uint32_t* t);
int fldInStreamReadUInt64(struct FldInStream* self, uint64_t* t);
int fldInStreamReadOctets(struct FldInStream* self, uint8_t* t, size_t count);

int fldInStreamExUnread(struct FldInStream* self);

#endif
