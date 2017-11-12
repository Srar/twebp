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

void FreeMemory(char *data, void *hint);

class EncodeWorker : public Nan::AsyncWorker
{
  private:
  public:
    WebPImageReader reader;
    WebPMemoryWriter *writer;
    WebPConfig *config;
    WebPPicture *webp;
    uint8_t *buffer;
    size_t bufferSize;
    Nan::Callback *callback;

    int8_t errorCode;

    EncodeWorker(
        WebPImageReader reader,
        WebPConfig *config,
        WebPPicture *webp,
        uint8_t *buffer,
        size_t bufferSize,
        Nan::Callback *callback) : AsyncWorker(callback)
    {
        this->webp = webp;
        this->config = config;
        this->buffer = buffer;
        this->reader = reader;
        this->callback = callback;
        this->bufferSize = bufferSize;
    };

    ~EncodeWorker()
    {
        WebPPictureFree(this->webp);
        delete this->webp;
        delete this->config;
        if (this->writer->mem != NULL)
        {
            delete this->writer->mem;
        }
        delete this->writer;
    }

    void Execute()
    {
        this->reader(this->buffer, this->bufferSize, this->webp, 1, NULL);

        this->writer = new WebPMemoryWriter();
        WebPMemoryWriterInit(writer);
        this->webp->writer = WebPMemoryWrite;
        this->webp->custom_ptr = writer;

        WebPEncode(this->config, this->webp);
    }

    void HandleOKCallback()
    {
        if (this->webp->error_code == VP8_ENC_OK)
        {
            v8::Local<v8::Object> webpData = Nan::CopyBuffer((char *)this->writer->mem, this->writer->size).ToLocalChecked();
            v8::Local<v8::Value> argv[] = {Nan::Null(), webpData};
            this->callback->Call(2, argv);
        }
        else
        {
            std::string errorMsg = (std::string("Encoding image error: ").append(WebPEncodingErrorMessage[this->webp->error_code]));
            CallbackError(this->callback, errorMsg.c_str());
        }
    }
};

NAN_METHOD(N_WebPEncodeAsync)
{
    if (info.Length() > 3 || info.Length() <= 1)
    {
        Nan::ThrowError("Wrong number of arguments");
        return;
    }

    v8::Local<v8::Object> imageBuffer = v8::Local<v8::Object>::Cast(info[0]);
    size_t imageBufferLen = node::Buffer::Length(imageBuffer);
    uint8_t *imageData = (uint8_t *)node::Buffer::Data(imageBuffer);

    WebPConfig *config = new WebPConfig();
    config->quality = 75;

    Nan::Callback *callback = NULL;
    if (info.Length() == 2)
    {
        callback = new Nan::Callback(info[1].As<v8::Function>());
    }
    else
    {
        Util::formatWebPConfig(config, v8::Local<v8::Object>::Cast(info[1]));
        callback = new Nan::Callback(info[2].As<v8::Function>());
    }

    if (!WebPConfigPreset(config, WEBP_PRESET_PHOTO, config->quality))
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
        CallbackError(callback, "Unsupported format of image.");
    }

    WebPInputFileSize size;
    if (!GetImageSize(imageType, imageData, &size))
    {
        CallbackError(callback, "Failed to get size of image.");
    }

    pic->width = size.width;
    pic->height = size.height;

    if (!WebPPictureAlloc(pic))
    {
        CallbackError(callback, "Encoding image error: failed malloc memory.");
    }

    WebPImageReader reader = WebPGetImageReader(imageType);
    AsyncQueueWorker(new EncodeWorker(reader, config, pic, imageData, imageBufferLen, callback));
}