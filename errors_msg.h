
#ifndef ERRORS_MSG_H_
#define ERRORS_MSG_H_

#include <nan.h>
#include <string>
#include "webp/encode.h"
#include "webp/decode.h"


#define CallbackError(callback, errmsg)                 \
    v8::Local<v8::Value> argv[] = {Nan::Error(errmsg)}; \
    callback->Call(1, argv);                            \
    return;

static const char *const WebPEncodingErrorMessage[VP8_ENC_ERROR_LAST] = {
    "OK",
    "OUT_OF_MEMORY: Out of memory allocating objects",
    "BITSTREAM_OUT_OF_MEMORY: Out of memory re-allocating byte buffer",
    "NULL_PARAMETER: NULL parameter passed to function",
    "INVALID_CONFIGURATION: configuration is invalid",
    "BAD_DIMENSION: Bad picture dimension. Maximum width and height "
    "allowed is 16383 pixels.",
    "PARTITION0_OVERFLOW: Partition #0 is too big to fit 512k.\n"
    "To reduce the size of this partition, try using less segments "
    "with the -segments option, and eventually reduce the number of "
    "header bits using -partition_limit. More details are available "
    "in the manual (`man cwebp`)",
    "PARTITION_OVERFLOW: Partition is too big to fit 16M",
    "BAD_WRITE: Picture writer returned an I/O error",
    "FILE_TOO_BIG: File would be too big to fit in 4G",
    "USER_ABORT: encoding abort requested by user"};

#endif