#ifndef _CANNY_EDGE_DETECTION_CONFIG_H_
#define _CANNY_EDGE_DETECTION_CONFIG_H_

#include "hls_stream.h"
#include "ap_int.h"
#include "hls_math.h"
#include "common/xf_common.h"
#include "common/xf_utility.h"
#include "Canny_Edge_Detection_config_params.h"
#include "common/xf_utility.h"
#include "common/xf_infra.h"


/* set the height and width */
#define 	HEIGHT 		1080
#define 	WIDTH  		1920


#define INTYPE  XF_8UC1
#define OUTTYPE XF_8UC1
#define TYPE    XF_8UC1
#define NPC1    XF_NPPC1


typedef hls::stream<ap_axiu<8, 1, 1, 1> > AXI_STREAM;
typedef xf::Mat<TYPE, HEIGHT, WIDTH, NPC1> IMAGE;


void sobel_edge(AXI_STREAM& INPUT_STREAM , AXI_STREAM& OUTPUT_STREAM_X , AXI_STREAM& OUTPUT_STREAM_Y , AXI_STREAM& OUTPUT_STREAM ,unsigned int rows , unsigned int columns);

void gaussian_blur(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM , unsigned int r, unsigned int c);

void non_maximum_suppression(AXI_STREAM& INPUT_STREAM_X,AXI_STREAM& INPUT_STREAM_Y,AXI_STREAM& INPUT_SOBEL_STREAM,AXI_STREAM& OUTPUT_STREAM,unsigned int r,unsigned int c);

void double_threshold(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM,unsigned int r,unsigned int c);

void hysteresis(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM,unsigned int r,unsigned int c);

void canny_edge(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM,unsigned int r,unsigned int c);


#endif  // end of _XF_CUSTOM_CONV_CONFIG_H_
