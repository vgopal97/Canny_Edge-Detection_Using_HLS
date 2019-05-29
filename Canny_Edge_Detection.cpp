#include "Canny_Edge_Detection_config.h"

void canny_edge(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM,unsigned int r,unsigned int c) {

	#pragma HLS INTERFACE axis port=INPUT_STREAM
	#pragma HLS INTERFACE axis port=OUTPUT_STREAM

	#pragma HLS INTERFACE ap_stable port=r
	#pragma HLS INTERFACE ap_stable port=c
	
	AXI_STREAM GAUSSIAN_STREAM,SOBELX_STREAM,SOBELY_STREAM,SOBEL_STREAM,NMS_STREAM,DT_STREAM,H_STREAM;
	
#pragma HLS STREAM variable=GAUSSIAN_STREAM depth=1080*1920
#pragma HLS STREAM variable=SOBELX_STREAM depth=1080*1920
#pragma HLS STREAM variable=SOBELY_STREAM depth=1080*1920
#pragma HLS STREAM variable=SOBEL_STREAM depth=1080*1920
#pragma HLS STREAM variable=NMS_STREAM depth=1080*1920
#pragma HLS STREAM variable=DT_STREAM depth=1080*1920
#pragma HLS STREAM variable=H_STREAM depth=1080*1920
#pragma HLS dataflow

	gaussian_blur(INPUT_STREAM,GAUSSIAN_STREAM,r,c);

	sobel_edge(GAUSSIAN_STREAM,SOBELX_STREAM,SOBELY_STREAM,SOBEL_STREAM,r,c);

	non_maximum_suppression(SOBELX_STREAM,SOBELY_STREAM,SOBEL_STREAM,NMS_STREAM,r,c);

	double_threshold(NMS_STREAM,DT_STREAM,r,c);

	hysteresis(DT_STREAM,OUTPUT_STREAM,r,c);


}
