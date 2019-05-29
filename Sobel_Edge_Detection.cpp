#include "Canny_Edge_Detection_config.h"

void sobel_edge(AXI_STREAM& INPUT_STREAM , AXI_STREAM& OUTPUT_STREAM_X , AXI_STREAM& OUTPUT_STREAM_Y ,AXI_STREAM& OUTPUT_STREAM, unsigned int r ,unsigned  int c) {


#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE axis port=OUTPUT_STREAM_X
#pragma HLS INTERFACE axis port=OUTPUT_STREAM_Y
#pragma HLS INTERFACE axis port=OUTPUT_STREAM

#pragma HLS INTERFACE ap_stable port=r
#pragma HLS INTERFACE ap_stable port=c


/*#pragma HLS RESOURCE core=AXI_SLAVE variable=r metadata="-bus_bundle CONTROL"
#pragma HLS RESOURCE core=AXI_SLAVE variable=c metadata="-bus_bundle CONTROL"


#pragma HLS STREAM variable=INPUT_STREAM depth=1080*1920
#pragma HLS STREAM variable=OUTPUT_STREAM_X depth=1080*1920
#pragma HLS STREAM variable=OUTPUT_STREAM_Y depth=1080*1920
#pragma HLS STREAM variable=OUTPUT_STREAM depth=1080*1920*/


	 static IMAGE _src(r,c);
	 static IMAGE _dstx(r,c);
	 static IMAGE _dsty(r,c);
	 static IMAGE _dst(r,c);



	xf::AXIvideo2xfMat(INPUT_STREAM,_src);

	int i,j,x,y;
	int sum_x,sum_y;
	int gdx[FILTER_WIDTH*FILTER_HEIGHT]={-1,0,1,-2,0,2,-1,0,1};
	int gdy[FILTER_WIDTH*FILTER_HEIGHT]={-1,-2,-1,0,0,0,1,2,1};

	l1:for(x=1;x<r-1;x++){
#pragma HLS pipeline
		#pragma HLS loop_tripcount min=1 max=1080
		l2:for(y=1;y<c-1;y++){
			#pragma HLS loop_tripcount min=1 max=1920
			sum_x=0;
			sum_y=0;
			conv1:for(i=-1;i<=1;i++){
			        conv2:for(j=-1;j<=1;j++){
			         		sum_x=sum_x+gdx[(i+1)*(FILTER_WIDTH)+j+1]*_src.data[c*(x-i)+(y-j)];
			         		sum_y=sum_y+gdy[(i+1)*(FILTER_WIDTH)+j+1]*_src.data[c*(x-i)+(y-j)];
			 				}
			             }
			_dsty.data[c*x + y]=sum_y;
		    _dstx.data[c*x + y]=sum_x;
		    _dst.data[c*x + y]=sqrt(float(sum_x*sum_x+sum_y*sum_y));
	     }

	}
#pragma HLS dataflow
	xf:: xfMat2AXIvideo(_dstx,OUTPUT_STREAM_X);
	xf:: xfMat2AXIvideo(_dsty,OUTPUT_STREAM_Y);
	xf:: xfMat2AXIvideo(_dst,OUTPUT_STREAM);

}
