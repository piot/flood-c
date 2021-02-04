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

#endif
