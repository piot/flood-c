/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/text_in_stream.h>

void fldTextInStreamInit(FldTextInStream* self, FldInStream* inStream)
{
    self->inStream = inStream;
    self->line = 0;
    self->column = 0;
}

void fldTextInStreamDebug(FldTextInStream* self, const char* description)
{
    char debug[9];

    size_t run = 8;
    size_t left = self->inStream->size - self->inStream->pos;

    if (run > left) {
        run = left;
    }

    for (size_t i = 0; i < run; ++i) {
        debug[i] = *(self->inStream->p + i);
    }
    debug[run] = 0;

    CLOG_VERBOSE("%s:%d:%d '%s'", description, self->line + 1, self->column + 1, debug)
}

void fldTextInStreamTell(FldTextInStream* self, FldTextInStreamState* state)
{
    state->column = self->column;
    state->line = self->line;
    state->pos = self->inStream->pos;
    state->p = self->inStream->p;
}

int fldTextInStreamSeek(FldTextInStream* self, const FldTextInStreamState* state)
{
    self->column = state->column;
    self->line = state->line;
    self->inStream->pos = state->pos;
    self->inStream->p = state->p;

    return 0;
}

int fldTextInStreamReadCh(FldTextInStream* self, char* ch)
{
    uint8_t octet;

    int errorCode = fldInStreamReadUInt8(self->inStream, &octet);
    if (errorCode < 0) {
        return errorCode;
    }

    if (octet == 10) {
        self->line++;
        self->column = 0;
    } else {
        self->column++;
    }

    *ch = octet;

    return 0;
}

static int detectLineLength(const uint8_t* p, size_t maxCount)
{
    for (size_t i = 0; i < maxCount; ++i) {
        p--;
        if (*p == 10) {
            return i;
        }
    }

    return -1;
}

int fldTextInStreamUnreadCh(FldTextInStream* self)
{
    if (self->inStream->pos <= 0) {
        return -1;
    }

    if (*self->inStream->p == 10) {
        self->line--;
        self->column = detectLineLength(self->inStream->p, self->inStream->pos);
        if (self->column < 0) {
            return -1;
        }
    } else {
        self->column--;
    }

    self->inStream->pos--;
    self->inStream->p--;

    return 0;
}

const char* fldTextInStreamPositionString(FldTextInStream* self)
{
    static char temp[64];
    tc_snprintf(temp, 64, ":%d:%d:", self->line + 1, self->column + 1);

    return temp;
}
