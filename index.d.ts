// Type definitions for webp 1.0.0
// Project: twebp
// Definitions by: Srar

/// <reference types="node" />

interface WebPEncodeOptions {
    // Lossless encoding (0=lossy(default), 1=lossless).
    lossless?: 0 | 1,
    // Between 0 (smallest file) and 100 (biggest). Default value is 75.
    quality?: number,
    // quality/speed trade-off (0=fast, 6=slower-better)
    method?: 0 | 1 | 2 | 3 | 4 | 5 | 6,
    // if non-zero, set the desired target size in bytes. Takes precedence over the 'compression' parameter.
    target_size?: number,
    // if non-zero, specifies the minimal distortion to try to achieve. Takes precedence over target_size.
    target_PSNR?: number,
    // maximum number of segments to use, in [1..4]
    segments?: 1 | 2 | 3 | 4,
    // Spatial Noise Shaping. 0=off, 100=maximum.
    sns_strength?: number,
    // range: [0 = off .. 100 = strongest]
    filter_strength?: number,
    // range: [0 = off .. 7 = least sharp]
    filter_sharpness?: 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7,
    // filtering type: 0 = simple, 1 = strong (only used if filter_strength > 0 or autofilter > 0)
    filter_type?: number,
    // Auto adjust filter's strength [0 = off, 1 = on]
    autofilter?: 0 | 1,
    // Algorithm for encoding the alpha plane (0 = none, 1 = compressed with WebP lossless). Default is 1.
    alpha_compression?: 0 | 1,
    // Predictive filtering method for alpha plane. 0: none, 1: fast, 2: best. Default if 1.
    alpha_filtering?: 0 | 1 | 2,
    // Between 0 (smallest size) and 100 (lossless). Default is 100.
    alpha_quality?: number,
    // number of entropy-analysis passes (in [1..10]).
    pass?: 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10,
    // if true, export the compressed picture back. In-loop filtering is not applied.
    show_compressed?: 0 | 1,
    // preprocessing filter: 0=none, 1=segment-smooth, 2=pseudo-random dithering
    preprocessing?: 0 | 1 | 2,
    // log2(number of token partitions) in [0..3]. Default is set to 0 for easier progressive decoding.
    partitions?: 0 | 1 | 2 | 3,
    // quality degradation allowed to fit the 512k limit on prediction modes coding (0: no degradation, 100: maximum possible degradation).
    partition_limit?: number,
    // If true, compression parameters will be remapped to better match the expected output size from 
    // JPEG compression. Generally, the output size will be similar but the degradation will be lower.
    emulate_jpeg_size?: number,
    // If non-zero, try and use multi-threaded encoding.
    thread_level?: number,
    // If set, reduce memory usage (but increase CPU use).
    low_memory?: number,
    // Near lossless encoding [0 = max loss .. 100 = off(default)].
    near_lossless?: number,
    // if non-zero, preserve the exact RGB values under transparent area. Otherwise, discard this invisible RGB information for better compression. The default value is 0.
    exact?: number,
    // reserved for future lossless feature
    use_delta_palette?: number,
    // if needed, use sharp (and slow) RGB->YUV conversion
    use_sharp_yuv?: number,
}

export function N_WebPGetFeatures(buf: Buffer): {
    width: number;
    height: number,
    has_alpha: 0 | 1
};

export function N_WebPEncode(buf: Buffer): Buffer;
export function N_WebPEncode(buf: Buffer, options: WebPEncodeOptions): Buffer;

export function N_WebPEncodeAsync(buf: Buffer, cb: Function);
export function N_WebPEncodeAsync(buf: Buffer, options: WebPEncodeOptions, cb: Function);