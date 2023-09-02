/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef FLD_OUT_STREAM_H
#define FLD_OUT_STREAM_H

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct FldOutStream {
    uint8_t* octets;
    uint8_t* p;
    size_t size;
    size_t pos;
    bool writeDebugInfo;
} FldOutStream;

void fldOutStreamInit(struct FldOutStream* self, uint8_t* octets, size_t count);
void fldOutStreamRewind(FldOutStream* self);
int fldOutStreamWriteUInt8(struct FldOutStream* self, uint8_t t);
int fldOutStreamWriteInt8(struct FldOutStream* self, int8_t t);
int fldOutStreamWriteUInt16(struct FldOutStream* self, uint16_t v);
int fldOutStreamWriteInt32(struct FldOutStream* self, int32_t t);
int fldOutStreamWriteUInt32(struct FldOutStream* self, uint32_t v);
int fldOutStreamWriteUInt64(struct FldOutStream* self, uint64_t v);

int fldOutStreamWriteOctets(struct FldOutStream* self, const uint8_t* t, size_t count);
int fldOutStreamWritef(FldOutStream* self, const char* fmt, ...);
int fldOutStreamWritevf(FldOutStream* self, const char* fmt, va_list list);
int fldOutStreamWrites(FldOutStream* self, const char* s);

void fldOutStreamWriteMarker(FldOutStream* outStream, uint8_t expectedId);

#endif
