#include "Canny_Edge_Detection_config.h"

void non_maximum_suppression(AXI_STREAM& INPUT_STREAM_X,AXI_STREAM& INPUT_STREAM_Y,AXI_STREAM& INPUT_SOBEL_STREAM,AXI_STREAM& OUTPUT_STREAM,unsigned int r,unsigned int c) {
#pragma HLS INTERFACE axis port=INPUT_STREAM_X
#pragma HLS INTERFACE axis port=INPUT_STREAM_Y
#pragma HLS INTERFACE axis port=INPUT_SOBEL_STREAM
#pragma HLS INTERFACE axis port=OUTPUT_STREAM

#pragma HLS INTERFACE ap_stable port=r
#pragma HLS INTERFACE ap_stable port=c

/*#pragma HLS RESOURCE core=AXI_SLAVE variable=r metadata="-bus_bundle CONTROL"
#pragma HLS RESOURCE core=AXI_SLAVE variable=c metadata="-bus_bundle CONTROL"


#pragma HLS STREAM variable=INPUT_STREAM_X depth=1080*1920
#pragma HLS STREAM variable=INPUT_STREAM_Y depth=1080*1920
#pragma HLS STREAM variable=INPUT_SOBEL_STREAM depth=1080*1920
#pragma HLS STREAM variable=OUTPUT_STREAM depth=1080*1920*/


static IMAGE _gx(r,c);
static IMAGE _gy(r,c);
static IMAGE _sobel(r,c);
static IMAGE _nms(r,c);

#pragma HLS dataflow
xf::AXIvideo2xfMat(INPUT_STREAM_X,_gx);
xf::AXIvideo2xfMat(INPUT_STREAM_Y,_gy);
xf::AXIvideo2xfMat(INPUT_SOBEL_STREAM,_sobel);


	int x,y;
	int a=_sobel.rows,b=_sobel.cols,q,p;
	ap_fixed<16,8> angle=0,PI=3.14159265359;


	l1:for(x=1;x<a-1;x++){
		#pragma HLS loop_tripcount min=1 max=1080
#pragma HLS pipeline
		l2:for(y=1;y<b-1;y++){
			#pragma HLS loop_tripcount min=1 max=1920
			angle = (_gx.data[b*x+y] == 0) ? ap_fixed<16,8>(PI/2) : ap_fixed<16,8>(atanf(_gy.data[b*x+y]/_gx.data[b*x+y]));

			//angle= (angle<0) ? (PI+angle) : angle;

			q=255;

			p=255;

			if((angle>=0 && angle<=PI/8) || (7*PI/8<=angle && angle<=PI)) {

				q=_sobel.data[b*x + (y+1)];

				p=_sobel.data[b*x + (y-1)];

			}


			else if((angle>=PI/8) && (angle<=3*PI/8)) {

				q=_sobel.data[b*(x+1) + (y-1)];

				p=_sobel.data[b*(x-1) + (y+1)];

			}

			else if((angle>=3*PI/8) && (angle<=5*PI/8)) {

				q=_sobel.data[b*(x+1) + y];

				p=_sobel.data[b*(x-1) + y];

			}

			else if((angle>=5*PI/8) && (angle<=7*PI/8)) {

				q=_sobel.data[b*(x-1) + (y-1)];

				p=_sobel.data[b*(x+1) + (y+1)];

			}



			if((_sobel.data[b*x+y]>=q) && (_sobel.data[b*x+y]>=p)) {

				_nms.data[b*x+y]=_sobel.data[b*x+y];

			}


			else {

				_nms.data[b*x+y]=0;

			
			}
	
		}


	}


	  xf:: xfMat2AXIvideo(_nms,OUTPUT_STREAM);

}
