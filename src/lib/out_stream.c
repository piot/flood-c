/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/out_stream.h>
#include <stdio.h>
#include <tiny-libc/tiny_libc.h>

void fldOutStreamInit(FldOutStream* self, uint8_t* octets, size_t count)
{
    self->octets = octets;
    self->p = octets;
    self->pos = 0;
    self->size = count;
}

void fldOutStreamRewind(FldOutStream* self)
{
    self->pos = 0;
    self->p = self->octets;
}

int fldOutStreamWriteUInt8(FldOutStream* self, uint8_t t)
{
    if (self->pos + 1 > self->size) {
        CLOG_SOFT_ERROR("fldWriteUint8: too far %zu %zu", self->pos, self->size)
        return -1;
    }

    *self->p++ = t;
    self->pos++;
    return 0;
}

int fldOutStreamWriteInt8(FldOutStream* self, int8_t t)
{
    if (self->pos + 1 > self->size) {
        CLOG_SOFT_ERROR("fldWriteInt8: too far %zu %zu", self->pos, self->size)
        return -1;
    }

    *self->p++ = t;
    self->pos++;
    return 0;
}

int fldOutStreamWriteUInt16(FldOutStream* self, uint16_t v)
{
    if (self->pos + 2 > self->size) {
        CLOG_SOFT_ERROR("fldWriteUint16: too far %zu %zu", self->pos, self->size)
        return -2;
    }

    *self->p++ = (v >> 8) & 0xff;
    *self->p++ = v & 0xff;

    self->pos += 2;

    return 0;
}

int fldOutStreamWriteUInt32(FldOutStream* self, uint32_t v)
{
    if (self->pos + 4 > self->size) {
        CLOG_SOFT_ERROR("fldWriteUInt32: too far %zu %zu", self->pos, self->size)
        return -2;
    }

    *self->p++ = (v >> 24);
    *self->p++ = (v >> 16) & 0xff;
    *self->p++ = (v >> 8) & 0xff;
    *self->p++ = v & 0xff;

    self->pos += 4;

    return 0;
}

int fldOutStreamWriteInt32(FldOutStream* self, int32_t v)
{
    if (self->pos + 4 > self->size) {
        CLOG_SOFT_ERROR("fldWriteInt32: too far %zu %zu", self->pos, self->size)
        return -2;
    }

    *self->p++ = (v >> 24);
    *self->p++ = (v >> 16) & 0xff;
    *self->p++ = (v >> 8) & 0xff;
    *self->p++ = v & 0xff;

    self->pos += 4;

    return 0;
}

int fldOutStreamWriteUInt64(FldOutStream* self, uint64_t v)
{
    int errorCode = fldOutStreamWriteUInt32(self, v >> 32);
    if (errorCode < 0) {
        return errorCode;
    }
    errorCode = fldOutStreamWriteUInt32(self, v & 0xffffffff);

    return errorCode;
}

int fldOutStreamWritevf(FldOutStream* self, const char* fmt, va_list pl)
{
    int countLeft = self->size - self->pos - 1;
    if (countLeft < 1) {
        return -1;
    }

    int countWritten = vsnprintf((char*)self->p, countLeft, fmt, pl);
    if (countWritten <= 0) {
        return -2;
    }

    if (countWritten == countLeft) {
        return -3;
    }

    self->p += countWritten;
    self->pos += countWritten;

    if (self->pos >= self->size) {
        CLOG_SOFT_ERROR("fldOutStreamWritevf: too far %zu %zu", self->pos, self->size)
    }

    return 0;
}

int fldOutStreamWrites(FldOutStream* self, const char* s)
{
    return fldOutStreamWriteOctets(self, (const uint8_t*)s, tc_strlen(s));
}

int fldOutStreamWritef(FldOutStream* self, const char* fmt, ...)
{
    va_list pl;

    va_start(pl, fmt);
    int ret = fldOutStreamWritevf(self, fmt, pl);
    va_end(pl);
    return ret;
}

int fldOutStreamWriteOctets(FldOutStream* self, const uint8_t* t, size_t count)
{
    if (self->pos + count > self->size) {
        CLOG_SOFT_ERROR("fldWriteOctets: too far %zu %zu %zu", self->pos, count, self->size)
        return -1;
    }

    tc_memcpy_octets(self->p, t, count);
    self->p += count;
    self->pos += count;

    return 0;
}
