#include <nan.h>
#include <string>

#include "webp/encode.h"
#include "webp/decode.h"

#include "./dec/pngdec.h"
#include "./dec/jpegdec.h"
#include "./dec/imageio_util.h"

#include "./encode_worker.h"

#include "./errors_msg.h"

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
    uint8_t *imageBuffer = (uint8_t *)node::Buffer::Data(info[0]->ToObject());
    size_t imageBufferLen = (size_t)info[1]->Uint32Value();
    Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());

    WebPConfig *config = new WebPConfig();
    if (!WebPConfigPreset(config, WEBP_PRESET_PHOTO, 90))
    {
        return;
    }

    // Setup the input data, allocating a picture of width x height dimension
    WebPPicture *pic = new WebPPicture();
    if (!WebPPictureInit(pic))
        return; // version error

    WebPInputFileFormat imageType = WebPGuessImageType(imageBuffer, imageBufferLen);

    if (imageType == WEBP_UNSUPPORTED_FORMAT)
    {
        CallbackError(callback, "Unsupported format of image.");
    }

    WebPInputFileSize size;
    if (!GetImageSize(imageType, imageBuffer, &size))
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
    AsyncQueueWorker(new EncodeWorker(reader, config, pic, imageBuffer, imageBufferLen, callback));
}