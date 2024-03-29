/*
 * RIFF codec tags
 * copyright (c) 2000 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * internal header for RIFF based (de)muxers
 * do NOT include this in end user applications
 */

#ifndef AVFORMAT_RIFF_H
#define AVFORMAT_RIFF_H

#include "../libavcodec/avcodec.h"
#include "avio.h"
#include "internal.h"
#include "metadata.h"

extern const AVMetadataConv ff_riff_info_conv[];
extern const char ff_riff_tags[][5];

int64_t ff_start_tag(AVIOContext *pb, const char *tag);
void ff_end_tag(AVIOContext *pb, int64_t start);

/**
 * Read BITMAPINFOHEADER structure and set AVStream codec width, height and
 * bits_per_encoded_sample fields. Does not read extradata.
 * @return codec tag
 */
int ff_get_bmp_header(AVIOContext *pb, AVStream *st);

void ff_put_bmp_header(AVIOContext *pb, AVCodecContext *enc, const AVCodecTag *tags, int for_asf);
int ff_put_wav_header(AVIOContext *pb, AVCodecContext *enc);
enum CodecID ff_wav_codec_get_id(unsigned int tag, int bps);
int ff_get_wav_header(AVIOContext *pb, AVCodecContext *codec, int size);

extern const AVCodecTag ff_codec_bmp_tags[];
extern const AVCodecTag ff_codec_wav_tags[];

unsigned int ff_codec_get_tag(const AVCodecTag *tags, enum CodecID id);
enum CodecID ff_codec_get_id(const AVCodecTag *tags, unsigned int tag);
void ff_parse_specific_params(AVCodecContext *stream, int *au_rate, int *au_ssize, int *au_scale);

typedef uint8_t ff_asf_guid[16];

int ff_read_riff_info(AVFormatContext *s, int64_t size);

#define FF_PRI_GUID \
    "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
#define FF_ARG_GUID(g) \
    g[0],g[1],g[2],g[3],g[4],g[5],g[6],g[7],g[8],g[9],g[10],g[11],g[12],g[13],g[14],g[15]

static av_always_inline int ff_guidcmp(const void *g1, const void *g2)
{
    return memcmp(g1, g2, sizeof(ff_asf_guid));
}

void ff_get_guid(AVIOContext *s, ff_asf_guid *g);

typedef struct {
    enum CodecID id;
    ff_asf_guid guid;
} AVCodecGuid;

enum CodecID ff_codec_guid_get_id(const AVCodecGuid *guids, ff_asf_guid guid);

extern const AVCodecGuid ff_codec_wav_guids[];

#define FF_MEDIASUBTYPE_BASE_GUID \
    0x00,0x00,0x10,0x00,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71

#endif /* AVFORMAT_RIFF_H */
