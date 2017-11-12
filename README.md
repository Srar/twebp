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

### N_WebPEncode(buffer, options)

Translate to webp format from Jpeg or Png 

* `buffer`: image buffer of Jpeg or Png.
* `options`: see blow

### N_WebPEncodeAsync(buffer, callback)

### N_WebPEncodeAsync(buffer, options, callback)

Translate to webp format from Jpeg or Png, 

This method uses more memory than sync method but doesn't block loop event.

- `buffer`: image buffer of Jpeg or Png.
- `options`: see blow


### Options:

```c++
  int lossless;           // Lossless encoding (0=lossy(default), 1=lossless).
  float quality;          // between 0 (smallest file) and 100 (biggest)
                          // default value is 75.
  int method;             // quality/speed trade-off (0=fast, 6=slower-better)

  // Parameters related to lossy compression only:
  int target_size;        // if non-zero, set the desired target size in bytes.
                          // Takes precedence over the 'compression' parameter.
  float target_PSNR;      // if non-zero, specifies the minimal distortion to
                          // try to achieve. Takes precedence over target_size.
  int segments;           // maximum number of segments to use, in [1..4]
  int sns_strength;       // Spatial Noise Shaping. 0=off, 100=maximum.
  int filter_strength;    // range: [0 = off .. 100 = strongest]
  int filter_sharpness;   // range: [0 = off .. 7 = least sharp]
  int filter_type;        // filtering type: 0 = simple, 1 = strong (only used
                          // if filter_strength > 0 or autofilter > 0)
  int autofilter;         // Auto adjust filter's strength [0 = off, 1 = on]
  int alpha_compression;  // Algorithm for encoding the alpha plane (0 = none,
                          // 1 = compressed with WebP lossless). Default is 1.
  int alpha_filtering;    // Predictive filtering method for alpha plane.
                          //  0: none, 1: fast, 2: best. Default if 1.
  int alpha_quality;      // Between 0 (smallest size) and 100 (lossless).
                          // Default is 100.
  int pass;               // number of entropy-analysis passes (in [1..10]).

  int show_compressed;    // if true, export the compressed picture back.
                          // In-loop filtering is not applied.
  int preprocessing;      // preprocessing filter:
                          // 0=none, 1=segment-smooth, 2=pseudo-random dithering
  int partitions;         // log2(number of token partitions) in [0..3]. Default
                          // is set to 0 for easier progressive decoding.
  int partition_limit;    // quality degradation allowed to fit the 512k limit
                          // on prediction modes coding (0: no degradation,
                          // 100: maximum possible degradation).
  int emulate_jpeg_size;  // If true, compression parameters will be remapped
                          // to better match the expected output size from
                          // JPEG compression. Generally, the output size will
                          // be similar but the degradation will be lower.
  int thread_level;       // If non-zero, try and use multi-threaded encoding.
  int low_memory;         // If set, reduce memory usage (but increase CPU use).

  int near_lossless;      // Near lossless encoding [0 = max loss .. 100 = off
                          // (default)].
  int exact;              // if non-zero, preserve the exact RGB values under
                          // transparent area. Otherwise, discard this invisible
                          // RGB information for better compression. The default
                          // value is 0.

  int use_delta_palette;  // reserved for future lossless feature
  int use_sharp_yuv;      // if needed, use sharp (and slow) RGB->YUV conversion
```

