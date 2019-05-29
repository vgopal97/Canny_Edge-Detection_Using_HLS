#include "Canny_Edge_Detection_config.h"

void hysteresis(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM,unsigned int r,unsigned int c) {


#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE axis port=OUTPUT_STREAM

#pragma HLS INTERFACE ap_stable port=r
#pragma HLS INTERFACE ap_stable port=c
/*
#pragma HLS STREAM variable=INPUT_STREAM depth=1080*1920
#pragma HLS STREAM variable=OUTPUT_STREAM depth=1080*1920
*/

	static IMAGE _dth(r,c);
	static IMAGE _hss(r,c);

	xf::AXIvideo2xfMat(INPUT_STREAM,_dth);
	
	int a=_dth.rows,b=_dth.cols,x,y;

	int weak=90,strong=255;

	l1:for(x=1;x<a-1;x++) {
#pragma HLS loop_tripcount min=1 max=1080
#pragma HLS pipeline
		l2:for(y=1;y<b-1;y++) {
#pragma HLS loop_tripcount min=1 max=1920
			if(_dth.data[b*(x)+y]==weak) {
				if ((_dth.data[b*(x+1) + (y-1)] == strong) || (_dth.data[b*(x+1) + y] == strong) || (_dth.data[b*(x+1) + (y+1)] == strong) || (_dth.data[b*(x) + (y-1)] == strong) || (_dth.data[b*(x) + (y+1)] == strong) || (_dth.data[b*(x-1) + (y-1)] == strong) || (_dth.data[b*(x-1) + y] == strong) || (_dth.data[b*(x-1) + (y+1)] == strong)) {

					_hss.data[b*x + y]=strong;
				}

				else {

					_hss.data[b*x + y]=0;
				}
			}

			else {
				_hss.data[b*x+y]=_dth.data[b*x+y];
			}
		}

	}

	xf:: xfMat2AXIvideo(_hss,OUTPUT_STREAM);

}
