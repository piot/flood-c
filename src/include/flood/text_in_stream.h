/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef FLD_TEXT_IN_STREAM_H
#define FLD_TEXT_IN_STREAM_H

struct FldInStream;

typedef struct FldTextInStream {
    struct FldInStream* inStream;
    int line;
    int column;
} FldTextInStream;

typedef struct FldTextInStreamState {
    int line;
    int column;
    const uint8_t* p;
    size_t pos;
} FldTextInStreamState;

void fldTextInStreamInit(FldTextInStream* self, FldInStream* inStream);
void fldTextInStreamDebug(FldTextInStream* self, const char* description);
void fldTextInStreamTell(FldTextInStream* self, FldTextInStreamState* state);
int fldTextInStreamSeek(FldTextInStream* self, const FldTextInStreamState* state);
int fldTextInStreamReadCh(FldTextInStream* self, char* ch);
int fldTextInStreamUnreadCh(FldTextInStream* self);

const char* fldTextInStreamPositionString(FldTextInStream* self);

#endif
