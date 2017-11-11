// Copyright 2016 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
//  Utility functions used by the image decoders.
//

#include "./imageio_util.h"

#if defined(_WIN32)
#include <fcntl.h> // for _O_BINARY
#include <io.h>    // for _setmode()
#endif
#include <stdlib.h>
#include <string.h>

#include "webp/types.h"

#include "./pngdec.h"
#include "./jpegdec.h"


/* From https://github.com/webmproject/libwebp/blob/master/imageio/image_dec.c#L14 */
static WEBP_INLINE uint32_t GetBE32(const uint8_t buf[])
{
    return ((uint32_t)buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}

static WEBP_INLINE uint16_t GetBE16(const uint8_t buf[])
{
    return (((uint8_t)buf[0] << 8) | ((uint8_t)buf[1]));
}

int GetImageSize(WebPInputFileFormat format, const uint8_t *const data, WebPInputFileSize *image_size)
{
    switch ((int)format)
    {
    case WEBP_PNG_FORMAT:
        image_size->width = GetBE32(data + 16);
        image_size->height = GetBE32(data + 20);
        break;
    case WEBP_JPEG_FORMAT:
        image_size->width = GetBE16(data + 3);
        image_size->height = GetBE16(data + 5);
        break;
    default:
        return 0;
    }
    return 1;
}

WebPImageReader WebPGetImageReader(WebPInputFileFormat format)
{
    switch (format)
    {
    case WEBP_PNG_FORMAT:
        return (WebPImageReader)ReadPNG;
    case WEBP_JPEG_FORMAT:
        return (WebPImageReader)ReadJPEG;
    // case WEBP_TIFF_FORMAT:
    //     return ReadTIFF;
    // case WEBP_WEBP_FORMAT:
    //     return ReadWebP;
    // case WEBP_PNM_FORMAT:
    //     return ReadPNM;
    default:
        return NULL;
    }
}

/* From https://github.com/webmproject/libwebp/blob/master/imageio/image_dec.c#L18 */
WebPInputFileFormat WebPGuessImageType(const uint8_t *const data, size_t data_size)
{
    WebPInputFileFormat format = WEBP_UNSUPPORTED_FORMAT;
    if (data != NULL && data_size >= 12)
    {
        const uint32_t magic1 = GetBE32(data + 0);
        const uint32_t magic2 = GetBE32(data + 8);
        if (magic1 == 0x89504E47U)
        {
            format = WEBP_PNG_FORMAT;
        }
        else if (magic1 >= 0xFFD8FF00U && magic1 <= 0xFFD8FFFFU)
        {
            format = WEBP_JPEG_FORMAT;
        }
        else if (magic1 == 0x49492A00 || magic1 == 0x4D4D002A)
        {
            format = WEBP_TIFF_FORMAT;
        }
        else if (magic1 == 0x52494646 && magic2 == 0x57454250)
        {
            format = WEBP_WEBP_FORMAT;
        }
        else if (((magic1 >> 24) & 0xff) == 'P')
        {
            const int type = (magic1 >> 16) & 0xff;
            // we only support 'P5 -> P7' for now.
            if (type >= '5' && type <= '7')
                format = WEBP_PNM_FORMAT;
        }
    }
    return format;
}

// -----------------------------------------------------------------------------
// File I/O

FILE *ImgIoUtilSetBinaryMode(FILE *file)
{
#if defined(_WIN32)
    if (_setmode(_fileno(file), _O_BINARY) == -1)
    {
        fprintf(stderr, "Failed to reopen file in O_BINARY mode.\n");
        return NULL;
    }
#endif
    return file;
}

int ImgIoUtilReadFromStdin(const uint8_t **data, size_t *data_size)
{
    static const size_t kBlockSize = 16384; // default initial size
    size_t max_size = 0;
    size_t size = 0;
    uint8_t *input = NULL;

    if (data == NULL || data_size == NULL)
        return 0;
    *data = NULL;
    *data_size = 0;

    if (!ImgIoUtilSetBinaryMode(stdin))
        return 0;

    while (!feof(stdin))
    {
        // We double the buffer size each time and read as much as possible.
        const size_t extra_size = (max_size == 0) ? kBlockSize : max_size;
        void *const new_data = realloc(input, max_size + extra_size);
        if (new_data == NULL)
            goto Error;
        input = (uint8_t *)new_data;
        max_size += extra_size;
        size += fread(input + size, 1, extra_size, stdin);
        if (size < max_size)
            break;
    }
    if (ferror(stdin))
        goto Error;
    *data = input;
    *data_size = size;
    return 1;

Error:
    free(input);
    fprintf(stderr, "Could not read from stdin\n");
    return 0;
}

int ImgIoUtilReadFile(const char *const file_name,
                      const uint8_t **data, size_t *data_size)
{
    int ok;
    void *file_data;
    size_t file_size;
    FILE *in;
    const int from_stdin = (file_name == NULL) || !strcmp(file_name, "-");

    if (from_stdin)
        return ImgIoUtilReadFromStdin(data, data_size);

    if (data == NULL || data_size == NULL)
        return 0;
    *data = NULL;
    *data_size = 0;

    in = fopen(file_name, "rb");
    if (in == NULL)
    {
        fprintf(stderr, "cannot open input file '%s'\n", file_name);
        return 0;
    }
    fseek(in, 0, SEEK_END);
    file_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    file_data = malloc(file_size);
    if (file_data == NULL)
        return 0;
    ok = (fread(file_data, file_size, 1, in) == 1);
    fclose(in);

    if (!ok)
    {
        fprintf(stderr, "Could not read %d bytes of data from file %s\n",
                (int)file_size, file_name);
        free(file_data);
        return 0;
    }
    *data = (uint8_t *)file_data;
    *data_size = file_size;
    return 1;
}

int ImgIoUtilWriteFile(const char *const file_name,
                       const uint8_t *data, size_t data_size)
{
    int ok;
    FILE *out;
    const int to_stdout = (file_name == NULL) || !strcmp(file_name, "-");

    if (data == NULL)
    {
        return 0;
    }
    out = to_stdout ? ImgIoUtilSetBinaryMode(stdout) : fopen(file_name, "wb");
    if (out == NULL)
    {
        fprintf(stderr, "Error! Cannot open output file '%s'\n", file_name);
        return 0;
    }
    ok = (fwrite(data, data_size, 1, out) == 1);
    if (out != stdout)
        fclose(out);
    return ok;
}

// -----------------------------------------------------------------------------

void ImgIoUtilCopyPlane(const uint8_t *src, int src_stride,
                        uint8_t *dst, int dst_stride, int width, int height)
{
    while (height-- > 0)
    {
        memcpy(dst, src, width * sizeof(*dst));
        src += src_stride;
        dst += dst_stride;
    }
}

// -----------------------------------------------------------------------------

int ImgIoUtilCheckSizeArgumentsOverflow(uint64_t nmemb, size_t size)
{
    const uint64_t total_size = nmemb * size;
    int ok = (total_size == (size_t)total_size);
#if defined(WEBP_MAX_IMAGE_SIZE)
    ok = ok && (total_size <= (uint64_t)WEBP_MAX_IMAGE_SIZE);
#endif
    return ok;
}

// -----------------------------------------------------------------------------