/*****************************************************************************
 *   Processing_NEON.cpp
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ****************************************************************************** *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#include "Processing.hpp"

#if defined(__ARM_NEON__)
  #include <arm_neon.h>
#endif

#define USE_NEON true
#define USE_FIXED_POINT false

using namespace cv;

void alphaBlendC1_NEON(const Mat& src, Mat& dst, const Mat& alpha)
{
    CV_Assert(src.type() == dst.type() == alpha.type() == CV_8UC1 &&
              src.isContinuous() && dst.isContinuous() &&
              alpha.isContinuous() &&
              (src.cols % 8 == 0) &&
              (src.cols == dst.cols) && (src.cols == alpha.cols));
    
#if !defined(__ARM_NEON__) || !USE_NEON
    alphaBlendC1(src, dst, alpha);
#else
    uchar* pSrc = src.data;
    uchar* pDst = dst.data;
    uchar* pAlpha = alpha.data;
    for(int i=0; i < src.total(); i+=8, pSrc+=8, pDst+=8, pAlpha+=8)
    {
        //load
        uint8x8_t vsrc = vld1_u8(pSrc);
        uint8x8_t vdst = vld1_u8(pDst);
        uint8x8_t valpha = vld1_u8(pAlpha);
        uint8x8_t v255 = vdup_n_u8(255);
        
        // source
        uint16x8_t mult1 = vmull_u8(vsrc, valpha);
        
        // destination
        uint8x8_t tmp = vsub_u8(v255, valpha);
        uint16x8_t mult2 = vmull_u8(tmp, vdst);
        
        //add them
        uint16x8_t sum = vaddq_u16(mult1, mult2);
        
        //take upper bytes, emulation /255
        uint8x8_t out = vshrn_n_u16(sum, 8);
        
        // store to the memory
        vst1_u8(pDst, out);
    }
#endif
}

void multiply_NEON(Mat& src, float multiplier)
{
    CV_Assert(src.type() == CV_8UC1 && src.isContinuous() &&
              (src.cols % 8 == 0));
    
#if !defined(__ARM_NEON__) || !USE_NEON
    src *= multiplier;
#elif USE_FIXED_POINT
    uchar fpMult = uchar((multiplier * 128.f) + 0.5f);
    uchar* ptr = src.data;
    for(int i = 0; i < src.total(); i+=8, ptr+=8)
    {
        uint8x8_t vsrc = vld1_u8(ptr);
        uint8x8_t vmult = vdup_n_u8(fpMult);
        uint16x8_t product = vmull_u8(vsrc, vmult);
        uint8x8_t out = vqshrn_n_u16(product, 7);
        vst1_u8(ptr, out);
    }
#else
    uchar* ptr = src.data;
    for(int i = 0; i < src.total(); i+=8, ptr+=8)
    {
        float32x4_t vmult1 = vdupq_n_f32(multiplier);
        float32x4_t vmult2 = vdupq_n_f32(multiplier);
        
        uint8x8_t in = vld1_u8(ptr); //load
        
        //convert to 16bit
        uint16x8_t in16bit = vmovl_u8(in);
        
        // split vector
        uint16x4_t in16bit1 = vget_high_u16(in16bit);
        uint16x4_t in16bit2 = vget_low_u16(in16bit);
        
        // convert to float
        uint32x4_t in32bit1 = vmovl_u16(in16bit1);
        uint32x4_t in32bit2 = vmovl_u16(in16bit2);
        float32x4_t inFlt1 = vcvtq_f32_u32(in32bit1);
        float32x4_t inFlt2 = vcvtq_f32_u32(in32bit2);
        
        // multiplication
        float32x4_t outFlt1 = vmulq_f32(vmult1, inFlt1);
        float32x4_t outFlt2 = vmulq_f32(vmult2, inFlt2);
        
        // convert from float
        uint32x4_t out32bit1 = vcvtq_u32_f32(outFlt1);
        uint32x4_t out32bit2 = vcvtq_u32_f32(outFlt2);
        uint16x4_t out16bit1 = vmovn_u32(out32bit1);
        uint16x4_t out16bit2 = vmovn_u32(out32bit2);
        
        // combine back
        uint16x8_t out16bit = vcombine_u16(out16bit2, out16bit1);
        
        // convert to 8bit
        uint8x8_t out8bit = vqmovn_u16(out16bit);
        
        // store to the memory
        vst1_u8(ptr, out8bit);
    }
#endif
}
