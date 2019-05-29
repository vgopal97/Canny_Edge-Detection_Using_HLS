#include "Canny_Edge_Detection_config.h"

void double_threshold(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM,unsigned int r,unsigned int c)  {


#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE axis port=OUTPUT_STREAM


#pragma HLS INTERFACE ap_stable port=r
#pragma HLS INTERFACE ap_stable port=c

/*#pragma HLS RESOURCE core=AXI_SLAVE variable=r metadata="-bus_bundle CONTROL"
#pragma HLS RESOURCE core=AXI_SLAVE variable=c metadata="-bus_bundle CONTROL"

#pragma HLS STREAM variable=INPUT_STREAM depth=1080*1920
#pragma HLS STREAM variable=OUTPUT_STREAM depth=1080*1920*/

	static IMAGE _nms(r,c);
	static IMAGE _dth(r,c);

	xf::AXIvideo2xfMat(INPUT_STREAM,_nms);	



	int max_value=255;

	int high_thresh=max_value*0.3;

	int low_thresh=max_value*0.1;

	int strong=255,weak=90,a=_nms.rows,b=_nms.cols;

	int x,y;

	l1:for(x=1;x<a-1;x++) {
#pragma HLS loop_tripcount min=1 max=1080
#pragma HLS pipeline
		l2:for(y=1;y<b-1;y++) {
#pragma HLS loop_tripcount min=1 max=1920
			if(_nms.data[b*x + y]>=high_thresh) {

				_dth.data[b*x + y]=strong;

			}

			if((_nms.data[b*x + y] <= high_thresh) && (_nms.data[b*x + y] >= low_thresh)) {

				_dth.data[b*x + y]=weak;

			}


		}


	}
	xf:: xfMat2AXIvideo(_dth,OUTPUT_STREAM);

}
