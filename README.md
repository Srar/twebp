# twebp
native module that libwebp for nodejs without memory leaking.

## Installation

### CentOS7

```
yum install libpng-devel libjpeg-turbo-devel libwebp-devel -y
```

### macOS
```
brew install webp libpng jpeg
```

## API Reference

### N_WebPGetFeatures(buffer) 
Get outline from buffer of webp.

* `buffer`: image buffer of webp.

The method returns an object similar to:

````json
{
  "width": 100,
  "height": 200,
  "has_alpha": 0
}
````

### N_WebPEncode(buffer)

Translate to webp format from Jpeg or Png 

* `buffer`: image buffer of Jpeg or Png.

### N_WebPEncodeAsync(buffer, callback)

Translate to webp format from Jpeg or Png, 

This method uses more memory than sync method but doesn't block loop event.

- `buffer`: image buffer of Jpeg or Png.

