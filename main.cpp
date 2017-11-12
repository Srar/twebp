#include <nan.h>
#include <string>

#include "webp/encode.h"
#include "webp/decode.h"

#include "./dec/pngdec.h"
#include "./dec/jpegdec.h"
#include "./dec/imageio_util.h"
#include "./encode_worker.hpp"

#include "./errors_msg.h"

#include "./util.hpp"

NAN_METHOD(N_WebPGetFeatures)
{
    if (info.Length() != 1)
    {
        Nan::ThrowError("Wrong number of arguments");
        return;
    }

    v8::Local<v8::Object> imageBuffer = v8::Local<v8::Object>::Cast(info[0]);
    size_t imageBufferLen = node::Buffer::Length(imageBuffer);

    WebPBitstreamFeatures *webpInfo = new WebPBitstreamFeatures();

    if (WebPGetFeatures((uint8_t *)node::Buffer::Data(imageBuffer), imageBufferLen, webpInfo) != VP8_STATUS_OK)
    {
        Nan::ThrowError("Can't get info of webp from buffer.");
        return;
    }

    v8::Local<v8::Object> returnObject = Nan::New<v8::Object>();
    Nan::Set(returnObject, Nan::New("width").ToLocalChecked(), Nan::New(webpInfo->width));
    Nan::Set(returnObject, Nan::New("height").ToLocalChecked(), Nan::New(webpInfo->height));
    Nan::Set(returnObject, Nan::New("has_alpha").ToLocalChecked(), Nan::New(webpInfo->has_alpha));
    info.GetReturnValue().Set(returnObject);

    delete webpInfo;
}

NAN_METHOD(N_WebPEncode)
{
    if (info.Length() > 2 || info.Length() <= 0)
    {
        Nan::ThrowError("Wrong number of arguments");
        return;
    }

    WebPConfig config;
    config.quality = 75;

    v8::Local<v8::Object> imageBuffer = v8::Local<v8::Object>::Cast(info[0]);
    size_t imageBufferLen = node::Buffer::Length(imageBuffer);
    uint8_t *imageData = (uint8_t *)node::Buffer::Data(imageBuffer);

    if(info.Length() == 2) {
        Util::formatWebPConfig(&config, v8::Local<v8::Object>::Cast(info[1]));
    }

    if (!WebPConfigPreset(&config, WEBP_PRESET_PHOTO, config.quality))
    {
        return;
    }

    // Setup the input data, allocating a picture of width x height dimension
    WebPPicture *pic = new WebPPicture();
    if (!WebPPictureInit(pic))
        return; // version error

    WebPInputFileFormat imageType = WebPGuessImageType(imageData, imageBufferLen);

    if (imageType == WEBP_UNSUPPORTED_FORMAT)
    {
        Nan::ThrowError("Unsupported format of image.");
        return;
    }

    WebPInputFileSize size;
    if (!GetImageSize(imageType, imageData, &size))
    {
        Nan::ThrowError("Failed to get size of image.");
        return;
    }

    pic->width = size.width;
    pic->height = size.height;

    if (!WebPPictureAlloc(pic))
    {
        Nan::ThrowError(std::string("Encoding image error: failed malloc memory.").c_str());
        return;
    }

    WebPImageReader reader = WebPGetImageReader(imageType);
    reader(imageData, imageBufferLen, pic, 1, NULL);

    WebPMemoryWriter *writer = new WebPMemoryWriter();
    pic->writer = WebPMemoryWrite;
    pic->custom_ptr = writer;
    WebPMemoryWriterInit(writer);

    if (!WebPEncode(&config, pic))
    {
        WebPPictureFree(pic);
        Nan::ThrowError((std::string("Encoding image error: ").append(WebPEncodingErrorMessage[pic->error_code])).c_str());
        return;
    }
    WebPPictureFree(pic);

    info.GetReturnValue().Set(Nan::CopyBuffer((char *)writer->mem, writer->size).ToLocalChecked());
    delete writer->mem;
    delete writer;
    delete pic;
}

// Module initialization logic
NAN_MODULE_INIT(Initialize)
{
    // Export the `Hello` function (equivalent to `export function Hello (...)` in JS)
    NAN_EXPORT(target, N_WebPGetFeatures);
    NAN_EXPORT(target, N_WebPEncode);
    NAN_EXPORT(target, N_WebPEncodeAsync);
}

// Create the module called "addon" and initialize it with `Initialize` function (created with NAN_MODULE_INIT macro)
NODE_MODULE(addon, Initialize);
