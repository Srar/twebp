#include <nan.h>

#include "webp/encode.h"
#include "webp/decode.h"

#include "./util.hpp"

void Util::formatWebPConfig(WebPConfig *config, v8::Local<v8::Object> obj)
{
    if (!(obj->Get(Nan::New<v8::String>("lossless").ToLocalChecked())->IsUndefined()))
        config->lossless = obj->Get(Nan::New<v8::String>("lossless").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("quality").ToLocalChecked())->IsUndefined()))
        config->quality = (float)obj->Get(Nan::New<v8::String>("quality").ToLocalChecked())->NumberValue();

    if (!(obj->Get(Nan::New<v8::String>("method").ToLocalChecked())->IsUndefined()))
        config->method = obj->Get(Nan::New<v8::String>("method").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("target_size").ToLocalChecked())->IsUndefined()))
        config->target_size = obj->Get(Nan::New<v8::String>("target_size").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("target_PSNR").ToLocalChecked())->IsUndefined()))
        config->target_PSNR = (float)obj->Get(Nan::New<v8::String>("target_PSNR").ToLocalChecked())->NumberValue();

    if (!(obj->Get(Nan::New<v8::String>("segments").ToLocalChecked())->IsUndefined()))
        config->segments = obj->Get(Nan::New<v8::String>("segments").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("sns_strength").ToLocalChecked())->IsUndefined()))
        config->sns_strength = obj->Get(Nan::New<v8::String>("sns_strength").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("filter_strength").ToLocalChecked())->IsUndefined()))
        config->filter_strength = obj->Get(Nan::New<v8::String>("filter_strength").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("filter_sharpness").ToLocalChecked())->IsUndefined()))
        config->filter_sharpness = obj->Get(Nan::New<v8::String>("filter_sharpness").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("filter_type").ToLocalChecked())->IsUndefined()))
        config->filter_type = obj->Get(Nan::New<v8::String>("filter_type").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("autofilter").ToLocalChecked())->IsUndefined()))
        config->autofilter = obj->Get(Nan::New<v8::String>("autofilter").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("alpha_compression").ToLocalChecked())->IsUndefined()))
        config->alpha_compression = obj->Get(Nan::New<v8::String>("alpha_compression").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("alpha_filtering").ToLocalChecked())->IsUndefined()))
        config->alpha_filtering = obj->Get(Nan::New<v8::String>("alpha_filtering").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("alpha_quality").ToLocalChecked())->IsUndefined()))
        config->alpha_quality = obj->Get(Nan::New<v8::String>("alpha_quality").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("pass").ToLocalChecked())->IsUndefined()))
        config->pass = obj->Get(Nan::New<v8::String>("pass").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("show_compressed").ToLocalChecked())->IsUndefined()))
        config->show_compressed = obj->Get(Nan::New<v8::String>("show_compressed").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("preprocessing").ToLocalChecked())->IsUndefined()))
        config->preprocessing = obj->Get(Nan::New<v8::String>("preprocessing").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("partitions").ToLocalChecked())->IsUndefined()))
        config->partitions = obj->Get(Nan::New<v8::String>("partitions").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("partition_limit").ToLocalChecked())->IsUndefined()))
        config->partition_limit = obj->Get(Nan::New<v8::String>("partition_limit").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("emulate_jpeg_size").ToLocalChecked())->IsUndefined()))
        config->emulate_jpeg_size = obj->Get(Nan::New<v8::String>("emulate_jpeg_size").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("thread_level").ToLocalChecked())->IsUndefined()))
        config->thread_level = obj->Get(Nan::New<v8::String>("thread_level").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("low_memory").ToLocalChecked())->IsUndefined()))
        config->low_memory = obj->Get(Nan::New<v8::String>("low_memory").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("near_lossless").ToLocalChecked())->IsUndefined()))
        config->near_lossless = obj->Get(Nan::New<v8::String>("near_lossless").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("exact").ToLocalChecked())->IsUndefined()))
        config->exact = obj->Get(Nan::New<v8::String>("exact").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("use_delta_palette").ToLocalChecked())->IsUndefined()))
        config->use_delta_palette = obj->Get(Nan::New<v8::String>("use_delta_palette").ToLocalChecked())->Uint32Value();

    if (!(obj->Get(Nan::New<v8::String>("use_sharp_yuv").ToLocalChecked())->IsUndefined()))
        config->use_sharp_yuv = obj->Get(Nan::New<v8::String>("use_sharp_yuv").ToLocalChecked())->Uint32Value();
}
