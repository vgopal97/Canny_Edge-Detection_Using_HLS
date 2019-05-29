#include "Canny_Edge_Detection_headers.h"
#include "Canny_Edge_Detection_config.h"

int main(int argc, char** argv)	{
	if (argc != 2)	{
		fprintf(stderr,"Invalid Number of Arguments!\nUsage:\n");
		fprintf(stderr,"<Executable Name> <input image path> \n");
		return -1;
	}

	cv::Mat in_img,in_conv_img,out_img,out_img_x,out_img_y;
	in_img = cv::imread(argv[1],0); 


	if (in_img.data == NULL)	{
		fprintf(stderr,"Cannot open image at %s\n",argv[1]);
		return 0;
	}


	in_img.convertTo(in_conv_img,CV_8U);

	out_img_x.create(in_conv_img.rows,in_conv_img.cols,CV_8U);
	out_img_y.create(in_conv_img.rows,in_conv_img.cols,CV_8U);
	out_img.create(in_conv_img.rows,in_conv_img.cols,CV_8U);

	unsigned int r=in_img.rows,c=in_img.cols;

	  static IMAGE imgInput(in_img.rows,in_img.cols);
	  static IMAGE imgOutput_x(in_img.rows,in_img.cols);
	  static IMAGE imgOutput_y(in_img.rows,in_img.cols);
	  static IMAGE imgOutput(in_img.rows,in_img.cols);

	imgInput.copyTo(in_img.data);


	AXI_STREAM INP_STREAM,OUP_STREAM_X,OUP_STREAM_Y,OUP_STREAM;

	xf::xfMat2AXIvideo(imgInput,INP_STREAM);

	xf::imwrite("in_img.jpg",imgInput);

	sobel_edge(INP_STREAM,OUP_STREAM_X,OUP_STREAM_Y,OUP_STREAM,r,c);

	xf::AXIvideo2xfMat(OUP_STREAM_X,imgOutput_x);
	xf::AXIvideo2xfMat(OUP_STREAM_Y,imgOutput_y);
	xf::AXIvideo2xfMat(OUP_STREAM,imgOutput);

	xf::imwrite("out_img_x.jpg", imgOutput_x);
	xf::imwrite("out_img_y.jpg", imgOutput_y);
	xf::imwrite("out_img.jpg", imgOutput);

	return 0;

}
