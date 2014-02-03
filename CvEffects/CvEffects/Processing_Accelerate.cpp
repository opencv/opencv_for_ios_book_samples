/*****************************************************************************
 *   Processing_Accelerate.cpp
 ******************************************************************************
 *   by Kirill Kornyakov and Alexander Shishkov, 13th May 2013
 ****************************************************************************** *
 *   Copyright Packt Publishing 2013.
 *   http://bit.ly/OpenCV_for_iOS_book
 *****************************************************************************/

#include <Accelerate/Accelerate.h>
#include <opencv2/core/core.hpp>

using namespace cv;

int cvtColor_Accelerate(const Mat& src, Mat& dst,
                        Mat buff1, Mat buff2)
{
    vImagePixelCount rows = static_cast<vImagePixelCount>(src.rows);
    vImagePixelCount cols = static_cast<vImagePixelCount>(src.cols);
    
    vImage_Buffer _src = { src.data, rows, cols, src.step };
    vImage_Buffer _dst = { dst.data, rows, cols, dst.step };
    vImage_Buffer _buff1 = { buff1.data, rows, cols, buff1.step };
    vImage_Buffer _buff2 = { buff2.data, rows, cols, buff2.step };
    
    const int16_t matrix[4 * 4] = {  77, 0, 0, 0,
                                    150, 0, 0, 0,
                                     29, 0, 0, 0,
                                      0, 0, 0, 0 };
    int32_t divisor = 256;
    
    vImage_Error err;
    err = vImageMatrixMultiply_ARGB8888(&_src, &_buff1,
                                        matrix, divisor,
                                        NULL, NULL, 0 );
    
    err = vImageConvert_ARGB8888toPlanar8(&_buff1, &_dst,
                                          &_buff2, &_buff2,
                                          &_buff2, 0);
    return err;
}

int equalizeHist_Accelerate(const Mat& src, Mat& dst)
{
    vImagePixelCount rows = static_cast<vImagePixelCount>(src.rows);
    vImagePixelCount cols = static_cast<vImagePixelCount>(src.cols);
    
    vImage_Buffer _src = { src.data, rows, cols, src.step };
    vImage_Buffer _dst = { dst.data, rows, cols, dst.step };
    
    vImage_Error err;
    err = vImageEqualization_Planar8( &_src, &_dst, 0 );
    
    return err;
}
