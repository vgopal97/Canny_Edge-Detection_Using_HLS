#include "Canny_Edge_Detection_config.h"

void gaussian_blur(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM , unsigned int r, unsigned int c) {


#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE axis port=OUTPUT_STREAM
#pragma HLS INTERFACE ap_stable port=r
#pragma HLS INTERFACE ap_stable port=c

/*#pragma HLS RESOURCE core=AXI_SLAVE variable=r metadata="-bus_bundle CONTROL"
#pragma HLS RESOURCE core=AXI_SLAVE variable=c metadata="-bus_bundle CONTROL"


#pragma HLS STREAM variable=INPUT_STREAM depth=1080*1920
#pragma HLS STREAM variable=OUTPUT_STREAM_X depth=1080*1920*/


	 static IMAGE _src(r,c);
	 static IMAGE _dst(r,c);

	 xf::AXIvideo2xfMat(INPUT_STREAM,_src);

	//int k[KERNEL_WIDTH*KERNEL_HEIGHT]={1,4,7,4,1,4,16,26,16,4,7,26,41,26,7,4,16,26,16,4,1,4,7,4,1};
	 ap_fixed<16,1> kernel[KERNEL_WIDTH*KERNEL_HEIGHT]={0.075,0.124,0.075,0.124,0.204,0.124,0.075,0.124,0.075};
	//ap_fixed<16,1> kernel[KERNEL_WIDTH*KERNEL_HEIGHT]={0.003765,0.015019,0.023792,0.015019,0.003765,0.015019,0.059912,0.094907,0.059912,0.015019,0.023792,0.094907,0.150342,0.094907,0.023792,0.015019,0.059912,0.094907,0.059912,0.015019,0.003765,0.015019,0.023792,0.015019,0.003765};
	int x,y,j,i;
	int sum=0;
	unsigned int b=_src.cols,a=_src.rows;


#pragma HLS pipeline


	l1:for(x=1;x<a-1;x++){
#pragma HLS loop_tripcount min=1 max=1080
		l2:for(y=1;y<b-1;y++){
#pragma HLS loop_tripcount min=1 max=1920
	    	sum=0;
	        l3:for(i=-1;i<=1;i++){
	        	l4:for(j=-1;j<=1;j++){
	            	sum=sum+kernel[(i+1)*(KERNEL_WIDTH)+j+1]*_src.data[b*(x-i)+(y-j)];
					}
	            }
	        _dst.data[b*x+y]=sum;
	        }
	    }


	    xf:: xfMat2AXIvideo(_dst,OUTPUT_STREAM);

	}
