/*****************************************************************************
 * mc-c.c: aarch64 motion compensation
 *****************************************************************************
 * Copyright (C) 2009-2025 x264 project
 *
 * Authors: David Conrad <lessen42@gmail.com>
 *          Janne Grunau <janne-x264@jannau.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at licensing@x264.com.
 *****************************************************************************/

#include "common/common.h"
#include "mc.h"

#define x264_prefetch_ref_aarch64 x264_template(prefetch_ref_aarch64)
void x264_prefetch_ref_aarch64( pixel *, intptr_t, int );
#define x264_prefetch_fenc_420_aarch64 x264_template(prefetch_fenc_420_aarch64)
void x264_prefetch_fenc_420_aarch64( pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_prefetch_fenc_422_aarch64 x264_template(prefetch_fenc_422_aarch64)
void x264_prefetch_fenc_422_aarch64( pixel *, intptr_t, pixel *, intptr_t, int );

#define x264_memcpy_aligned_neon x264_template(memcpy_aligned_neon)
void *x264_memcpy_aligned_neon( void *dst, const void *src, size_t n );
#define x264_memzero_aligned_neon x264_template(memzero_aligned_neon)
void x264_memzero_aligned_neon( void *dst, size_t n );

#define x264_pixel_avg_16x16_neon x264_template(pixel_avg_16x16_neon)
void x264_pixel_avg_16x16_neon( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_16x8_neon x264_template(pixel_avg_16x8_neon)
void x264_pixel_avg_16x8_neon ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_8x16_neon x264_template(pixel_avg_8x16_neon)
void x264_pixel_avg_8x16_neon ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_8x8_neon x264_template(pixel_avg_8x8_neon)
void x264_pixel_avg_8x8_neon  ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_8x4_neon x264_template(pixel_avg_8x4_neon)
void x264_pixel_avg_8x4_neon  ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_4x16_neon x264_template(pixel_avg_4x16_neon)
void x264_pixel_avg_4x16_neon ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_4x8_neon x264_template(pixel_avg_4x8_neon)
void x264_pixel_avg_4x8_neon  ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_4x4_neon x264_template(pixel_avg_4x4_neon)
void x264_pixel_avg_4x4_neon  ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_4x2_neon x264_template(pixel_avg_4x2_neon)
void x264_pixel_avg_4x2_neon  ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );

#define x264_pixel_avg_4x16_sve x264_template(pixel_avg_4x16_sve)
void x264_pixel_avg_4x16_sve ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_4x8_sve x264_template(pixel_avg_4x8_sve)
void x264_pixel_avg_4x8_sve  ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_4x4_sve x264_template(pixel_avg_4x4_sve)
void x264_pixel_avg_4x4_sve  ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_pixel_avg_4x2_sve x264_template(pixel_avg_4x2_sve)
void x264_pixel_avg_4x2_sve  ( pixel *, intptr_t, pixel *, intptr_t, pixel *, intptr_t, int );

#define x264_pixel_avg2_w4_neon x264_template(pixel_avg2_w4_neon)
void x264_pixel_avg2_w4_neon ( pixel *, intptr_t, pixel *, intptr_t, pixel *, int );
#define x264_pixel_avg2_w8_neon x264_template(pixel_avg2_w8_neon)
void x264_pixel_avg2_w8_neon ( pixel *, intptr_t, pixel *, intptr_t, pixel *, int );
#define x264_pixel_avg2_w16_neon x264_template(pixel_avg2_w16_neon)
void x264_pixel_avg2_w16_neon( pixel *, intptr_t, pixel *, intptr_t, pixel *, int );
#define x264_pixel_avg2_w20_neon x264_template(pixel_avg2_w20_neon)
void x264_pixel_avg2_w20_neon( pixel *, intptr_t, pixel *, intptr_t, pixel *, int );

#define x264_plane_copy_core_neon x264_template(plane_copy_core_neon)
void x264_plane_copy_core_neon( pixel *dst, intptr_t i_dst,
                                pixel *src, intptr_t i_src, int w, int h );
#define x264_plane_copy_swap_core_neon x264_template(plane_copy_swap_core_neon)
void x264_plane_copy_swap_core_neon( pixel *dst, intptr_t i_dst,
                                     pixel *src, intptr_t i_src, int w, int h );
#define x264_plane_copy_deinterleave_neon x264_template(plane_copy_deinterleave_neon)
void x264_plane_copy_deinterleave_neon(  pixel *dstu, intptr_t i_dstu,
                                         pixel *dstv, intptr_t i_dstv,
                                         pixel *src,  intptr_t i_src, int w, int h );
#define x264_plane_copy_deinterleave_rgb_neon x264_template(plane_copy_deinterleave_rgb_neon)
void x264_plane_copy_deinterleave_rgb_neon( pixel *dsta, intptr_t i_dsta,
                                            pixel *dstb, intptr_t i_dstb,
                                            pixel *dstc, intptr_t i_dstc,
                                            pixel *src,  intptr_t i_src, int pw, int w, int h );
#define x264_plane_copy_interleave_core_neon x264_template(plane_copy_interleave_core_neon)
void x264_plane_copy_interleave_core_neon( pixel *dst,  intptr_t i_dst,
                                           pixel *srcu, intptr_t i_srcu,
                                           pixel *srcv, intptr_t i_srcv, int w, int h );

#define x264_store_interleave_chroma_neon x264_template(store_interleave_chroma_neon)
void x264_store_interleave_chroma_neon( pixel *dst, intptr_t i_dst, pixel *srcu, pixel *srcv, int height );
#define x264_load_deinterleave_chroma_fdec_neon x264_template(load_deinterleave_chroma_fdec_neon)
void x264_load_deinterleave_chroma_fdec_neon( pixel *dst, pixel *src, intptr_t i_src, int height );
#define x264_load_deinterleave_chroma_fenc_neon x264_template(load_deinterleave_chroma_fenc_neon)
void x264_load_deinterleave_chroma_fenc_neon( pixel *dst, pixel *src, intptr_t i_src, int height );

#define x264_mc_weight_w16_neon x264_template(mc_weight_w16_neon)
#define x264_mc_weight_w16_nodenom_neon x264_template(mc_weight_w16_nodenom_neon)
#define x264_mc_weight_w16_offsetadd_neon x264_template(mc_weight_w16_offsetadd_neon)
#define x264_mc_weight_w16_offsetsub_neon x264_template(mc_weight_w16_offsetsub_neon)
#define x264_mc_weight_w20_neon x264_template(mc_weight_w20_neon)
#define x264_mc_weight_w20_nodenom_neon x264_template(mc_weight_w20_nodenom_neon)
#define x264_mc_weight_w20_offsetadd_neon x264_template(mc_weight_w20_offsetadd_neon)
#define x264_mc_weight_w20_offsetsub_neon x264_template(mc_weight_w20_offsetsub_neon)
#define x264_mc_weight_w4_neon x264_template(mc_weight_w4_neon)
#define x264_mc_weight_w4_nodenom_neon x264_template(mc_weight_w4_nodenom_neon)
#define x264_mc_weight_w4_offsetadd_neon x264_template(mc_weight_w4_offsetadd_neon)
#define x264_mc_weight_w4_offsetsub_neon x264_template(mc_weight_w4_offsetsub_neon)
#define x264_mc_weight_w8_neon x264_template(mc_weight_w8_neon)
#define x264_mc_weight_w8_nodenom_neon x264_template(mc_weight_w8_nodenom_neon)
#define x264_mc_weight_w8_offsetadd_neon x264_template(mc_weight_w8_offsetadd_neon)
#define x264_mc_weight_w8_offsetsub_neon x264_template(mc_weight_w8_offsetsub_neon)
#define MC_WEIGHT(func)\
void x264_mc_weight_w20##func##_neon( pixel *, intptr_t, pixel *, intptr_t, const x264_weight_t *, int );\
void x264_mc_weight_w16##func##_neon( pixel *, intptr_t, pixel *, intptr_t, const x264_weight_t *, int );\
void x264_mc_weight_w8##func##_neon ( pixel *, intptr_t, pixel *, intptr_t, const x264_weight_t *, int );\
void x264_mc_weight_w4##func##_neon ( pixel *, intptr_t, pixel *, intptr_t, const x264_weight_t *, int );\
\
static void (* mc##func##_wtab_neon[6])( pixel *, intptr_t, pixel *, intptr_t, const x264_weight_t *, int ) =\
{\
    x264_mc_weight_w4##func##_neon,\
    x264_mc_weight_w4##func##_neon,\
    x264_mc_weight_w8##func##_neon,\
    x264_mc_weight_w16##func##_neon,\
    x264_mc_weight_w16##func##_neon,\
    x264_mc_weight_w20##func##_neon,\
};

MC_WEIGHT()
MC_WEIGHT(_nodenom)
MC_WEIGHT(_offsetadd)
MC_WEIGHT(_offsetsub)

#define x264_mc_copy_w4_neon x264_template(mc_copy_w4_neon)
void x264_mc_copy_w4_neon ( pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_mc_copy_w8_neon x264_template(mc_copy_w8_neon)
void x264_mc_copy_w8_neon ( pixel *, intptr_t, pixel *, intptr_t, int );
#define x264_mc_copy_w16_neon x264_template(mc_copy_w16_neon)
void x264_mc_copy_w16_neon( pixel *, intptr_t, pixel *, intptr_t, int );

#define x264_mc_chroma_neon x264_template(mc_chroma_neon)
void x264_mc_chroma_neon( pixel *, pixel *, intptr_t, pixel *, intptr_t, int, int, int, int );
#define x264_integral_init4h_neon x264_template(integral_init4h_neon)
void x264_integral_init4h_neon( uint16_t *, pixel *, intptr_t );
#define x264_integral_init4v_neon x264_template(integral_init4v_neon)
void x264_integral_init4v_neon( uint16_t *, uint16_t *, intptr_t );
#define x264_integral_init8h_neon x264_template(integral_init8h_neon)
void x264_integral_init8h_neon( uint16_t *, pixel *, intptr_t );
#define x264_integral_init8v_neon x264_template(integral_init8v_neon)
void x264_integral_init8v_neon( uint16_t *, intptr_t );
#define x264_frame_init_lowres_core_neon x264_template(frame_init_lowres_core_neon)
void x264_frame_init_lowres_core_neon( pixel *, pixel *, pixel *, pixel *, pixel *, intptr_t, intptr_t, int, int );

#define x264_mbtree_propagate_cost_neon x264_template(mbtree_propagate_cost_neon)
void x264_mbtree_propagate_cost_neon( int16_t *, uint16_t *, uint16_t *, uint16_t *, uint16_t *, float *, int );

#define x264_mbtree_fix8_pack_neon x264_template(mbtree_fix8_pack_neon)
void x264_mbtree_fix8_pack_neon( uint16_t *dst, float *src, int count );
#define x264_mbtree_fix8_unpack_neon x264_template(mbtree_fix8_unpack_neon)
void x264_mbtree_fix8_unpack_neon( float *dst, uint16_t *src, int count );

static void (* const pixel_avg_wtab_neon[6])( pixel *, intptr_t, pixel *, intptr_t, pixel *, int ) =
{
    NULL,
    x264_pixel_avg2_w4_neon,
    x264_pixel_avg2_w8_neon,
    x264_pixel_avg2_w16_neon,   // no slower than w12, so no point in a separate function
    x264_pixel_avg2_w16_neon,
    x264_pixel_avg2_w20_neon,
};

static void (* const mc_copy_wtab_neon[5])( pixel *, intptr_t, pixel *, intptr_t, int ) =
{
    NULL,
    x264_mc_copy_w4_neon,
    x264_mc_copy_w8_neon,
    NULL,
    x264_mc_copy_w16_neon,
};

static void weight_cache_neon( x264_t *h, x264_weight_t *w )
{
    if( w->i_scale == 1<<w->i_denom )
    {
        if( w->i_offset < 0 )
        {
            w->weightfn = mc_offsetsub_wtab_neon;
            w->cachea[0] = -w->i_offset;
        }
        else
        {
            w->weightfn = mc_offsetadd_wtab_neon;
            w->cachea[0] = w->i_offset;
        }
    }
    else if( !w->i_denom )
        w->weightfn = mc_nodenom_wtab_neon;
    else
        w->weightfn = mc_wtab_neon;
}

static void mc_luma_neon( pixel *dst,    intptr_t i_dst_stride,
                          pixel *src[4], intptr_t i_src_stride,
                          int mvx, int mvy,
                          int i_width, int i_height, const x264_weight_t *weight )
{
    int qpel_idx = ((mvy&3)<<2) + (mvx&3);
    intptr_t offset = (mvy>>2)*i_src_stride + (mvx>>2);
    pixel *src1 = src[x264_hpel_ref0[qpel_idx]] + offset;
    if( (mvy&3) == 3 )             // explicit if() to force conditional add
        src1 += i_src_stride;

    if( qpel_idx & 5 ) /* qpel interpolation needed */
    {
        pixel *src2 = src[x264_hpel_ref1[qpel_idx]] + offset + ((mvx&3) == 3);
        pixel_avg_wtab_neon[i_width>>2](
                dst, i_dst_stride, src1, i_src_stride,
                src2, i_height );
        if( weight->weightfn )
            weight->weightfn[i_width>>2]( dst, i_dst_stride, dst, i_dst_stride, weight, i_height );
    }
    else if( weight->weightfn )
        weight->weightfn[i_width>>2]( dst, i_dst_stride, src1, i_src_stride, weight, i_height );
    else
        mc_copy_wtab_neon[i_width>>2]( dst, i_dst_stride, src1, i_src_stride, i_height );
}

static pixel *get_ref_neon( pixel *dst,   intptr_t *i_dst_stride,
                              pixel *src[4], intptr_t i_src_stride,
                              int mvx, int mvy,
                              int i_width, int i_height, const x264_weight_t *weight )
{
    int qpel_idx = ((mvy&3)<<2) + (mvx&3);
    intptr_t offset = (mvy>>2)*i_src_stride + (mvx>>2);
    pixel *src1 = src[x264_hpel_ref0[qpel_idx]] + offset;
    if( (mvy&3) == 3 )             // explicit if() to force conditional add
        src1 += i_src_stride;

    if( qpel_idx & 5 ) /* qpel interpolation needed */
    {
        pixel *src2 = src[x264_hpel_ref1[qpel_idx]] + offset + ((mvx&3) == 3);
        pixel_avg_wtab_neon[i_width>>2](
                dst, *i_dst_stride, src1, i_src_stride,
                src2, i_height );
        if( weight->weightfn )
            weight->weightfn[i_width>>2]( dst, *i_dst_stride, dst, *i_dst_stride, weight, i_height );
        return dst;
    }
    else if( weight->weightfn )
    {
        weight->weightfn[i_width>>2]( dst, *i_dst_stride, src1, i_src_stride, weight, i_height );
        return dst;
    }
    else
    {
        *i_dst_stride = i_src_stride;
        return src1;
    }
}

#define x264_hpel_filter_neon x264_template(hpel_filter_neon)
void x264_hpel_filter_neon( pixel *dsth, pixel *dstv, pixel *dstc,
                            pixel *src, intptr_t stride, int width,
                            int height, int16_t *buf );


#if !HIGH_BIT_DEPTH && HAVE_I8MM
#define x264_hpel_filter_neon_i8mm x264_template(hpel_filter_neon_i8mm)
void x264_hpel_filter_neon_i8mm( pixel *dsth, pixel *dstv, pixel *dstc,
                                 pixel *src, intptr_t stride, int width,
                                 int height, int16_t *buf );
#endif // !HIGH_BIT_DEPTH && HAVE_I8MM

PLANE_COPY(16, neon)
PLANE_COPY_SWAP(16, neon)
PLANE_INTERLEAVE(neon)
PROPAGATE_LIST(neon)

void x264_mc_init_aarch64( uint32_t cpu, x264_mc_functions_t *pf )
{

    if( cpu&X264_CPU_ARMV8 )
    {
        pf->prefetch_fenc_420 = x264_prefetch_fenc_420_aarch64;
        pf->prefetch_fenc_422 = x264_prefetch_fenc_422_aarch64;
        pf->prefetch_ref      = x264_prefetch_ref_aarch64;
    }

    if( cpu&X264_CPU_NEON )
    {
        pf->mbtree_propagate_cost = x264_mbtree_propagate_cost_neon;
        pf->mbtree_propagate_list = mbtree_propagate_list_neon;
        pf->mbtree_fix8_pack      = x264_mbtree_fix8_pack_neon;
        pf->mbtree_fix8_unpack    = x264_mbtree_fix8_unpack_neon;

        pf->memcpy_aligned  = x264_memcpy_aligned_neon;
        pf->memzero_aligned = x264_memzero_aligned_neon;

        pf->avg[PIXEL_16x16] = x264_pixel_avg_16x16_neon;
        pf->avg[PIXEL_16x8]  = x264_pixel_avg_16x8_neon;
        pf->avg[PIXEL_8x16]  = x264_pixel_avg_8x16_neon;
        pf->avg[PIXEL_8x8]   = x264_pixel_avg_8x8_neon;
        pf->avg[PIXEL_8x4]   = x264_pixel_avg_8x4_neon;
        pf->avg[PIXEL_4x16]  = x264_pixel_avg_4x16_neon;
        pf->avg[PIXEL_4x8]   = x264_pixel_avg_4x8_neon;
        pf->avg[PIXEL_4x4]   = x264_pixel_avg_4x4_neon;
        pf->avg[PIXEL_4x2]   = x264_pixel_avg_4x2_neon;

        pf->copy_16x16_unaligned = x264_mc_copy_w16_neon;
        pf->copy[PIXEL_16x16]    = x264_mc_copy_w16_neon;
        pf->copy[PIXEL_8x8]      = x264_mc_copy_w8_neon;
        pf->copy[PIXEL_4x4]      = x264_mc_copy_w4_neon;

        pf->weight       = mc_wtab_neon;
        pf->offsetadd    = mc_offsetadd_wtab_neon;
        pf->offsetsub    = mc_offsetsub_wtab_neon;
        pf->weight_cache = weight_cache_neon;

        pf->mc_chroma = x264_mc_chroma_neon;
        pf->mc_luma = mc_luma_neon;
        pf->get_ref = get_ref_neon;

        pf->integral_init4h = x264_integral_init4h_neon;
        pf->integral_init8h = x264_integral_init8h_neon;
        pf->integral_init4v = x264_integral_init4v_neon;
        pf->integral_init8v = x264_integral_init8v_neon;

        pf->frame_init_lowres_core = x264_frame_init_lowres_core_neon;

        pf->load_deinterleave_chroma_fdec = x264_load_deinterleave_chroma_fdec_neon;
        pf->load_deinterleave_chroma_fenc = x264_load_deinterleave_chroma_fenc_neon;

        pf->store_interleave_chroma       = x264_store_interleave_chroma_neon;

        pf->plane_copy                  = plane_copy_neon;
        pf->plane_copy_swap             = plane_copy_swap_neon;
        pf->plane_copy_deinterleave     = x264_plane_copy_deinterleave_neon;
        pf->plane_copy_deinterleave_rgb = x264_plane_copy_deinterleave_rgb_neon;
        pf->plane_copy_interleave       = plane_copy_interleave_neon;

        pf->hpel_filter = x264_hpel_filter_neon;
    }

#if !HIGH_BIT_DEPTH
#if HAVE_SVE
    if( cpu&X264_CPU_SVE )
    {
        pf->avg[PIXEL_4x16]  = x264_pixel_avg_4x16_sve;
        pf->avg[PIXEL_4x8]   = x264_pixel_avg_4x8_sve;
        pf->avg[PIXEL_4x4]   = x264_pixel_avg_4x4_sve;
        pf->avg[PIXEL_4x2]   = x264_pixel_avg_4x2_sve;
    }
#endif

#if HAVE_I8MM
    if( cpu&X264_CPU_I8MM )
    {
        pf->hpel_filter = x264_hpel_filter_neon_i8mm;
    }
#endif // HAVE_I8MM
#endif // !HIGH_BIT_DEPTH
}
