/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <tiny-libc/tiny_libc.h>

void fldInStreamInit(FldInStream* self, const uint8_t* octets, size_t count)
{
    self->octets = octets;
    self->p = octets;
    self->pos = 0;
    self->size = count;
}

int fldInStreamReadUInt8(FldInStream* self, uint8_t* t)
{
    self->pos++;
    if (self->pos > self->size) {
        CLOG_SOFT_ERROR("fldReadUint8: read too far! %zu %zu", self->pos, self->size);
        *t = 0xff;
        return -1;
    }

    *t = *self->p++;
    return 0;
}

int fldInStreamReadInt8(FldInStream* self, int8_t* t)
{
    self->pos++;
    if (self->pos > self->size) {
        CLOG_SOFT_ERROR("fldReadInt8: read too far! %zu %zu", self->pos, self->size)
        *t = 0xff;
        return -1;
    }

    *t = *self->p++;

    return 0;
}

int fldInStreamReadUInt16(struct FldInStream* self, uint16_t* t)
{
    self->pos += 2;
    if (self->pos > self->size) {
        CLOG_SOFT_ERROR("fldReadUint16: read too far! %zu %zu", self->pos, self->size)
        return -2;
    }

    const uint8_t* p = self->p;
    uint32_t v = *p++ << 8;
    v |= *p++;

    self->p = p;

    *t = v;

    return 0;
}

int fldInStreamReadInt32(struct FldInStream* self, int32_t* t)
{
    self->pos += 4;
    if (self->pos > self->size) {
        CLOG_SOFT_ERROR("fldReadInt32: read too far! %zu %zu", self->pos, self->size)

        return -2;
    }

    const uint8_t* p = self->p;
    int32_t v = *p++ << 24;
    v |= *p++ << 16;
    v |= *p++ << 8;
    v |= *p++;

    self->p = p;

    *t = v;

    return 0;
}

int fldInStreamReadUInt32(struct FldInStream* self, uint32_t* t)
{
    self->pos += 4;
    if (self->pos > self->size) {
        CLOG_SOFT_ERROR("fldReadUInt32: read too far! %zu %zu", self->pos, self->size)

        return -2;
    }

    const uint8_t* p = self->p;
    uint32_t v = *p++ << 24;
    v |= *p++ << 16;
    v |= *p++ << 8;
    v |= *p++;

    self->p = p;

    *t = v;

    return 0;
}

int fldInStreamReadUInt64(struct FldInStream* self, uint64_t* out)
{
    uint32_t hi, low;
    int errorCode = fldInStreamReadUInt32(self, &hi);
    if (errorCode < 0) {
        return errorCode;
    }

    errorCode = fldInStreamReadUInt32(self, &low);
    if (errorCode < 0) {
        return errorCode;
    }

    uint64_t result = hi;
    result = hi;
    result <<= 32;
    result |= low;

    *out = result;

    return 0;
}

int fldInStreamReadOctets(FldInStream* self, uint8_t* t, size_t count)
{
    self->pos += count;
    if (self->pos > self->size) {
        CLOG_SOFT_ERROR("fldInStreamReadOctets: read too far! %zu (%zu) %zu", self->pos, count, self->size)
        *t = 0xff;
        return -1;
    }

    tc_memcpy_octets(t, self->p, count);
    self->p += count;

    return 0;
}
