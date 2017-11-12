#include <nan.h>

#include "webp/encode.h"
#include "webp/decode.h"

class Util
{
  public:
    static void formatWebPConfig(WebPConfig *config, v8::Local<v8::Object> obj);
};