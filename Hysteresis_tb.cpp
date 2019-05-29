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

	out_img.create(in_conv_img.rows,in_conv_img.cols,CV_8U);
	
	static xf::Mat<INTYPE, HEIGHT,WIDTH,NPC1> imgInput(in_img.rows,in_img.cols);
	static xf::Mat<OUTTYPE,HEIGHT,WIDTH,NPC1> imgOutput(in_img.rows,in_img.cols);

	AXI_STREAM IN_STREAM,OUT_STREAM;
	unsigned int r=imgInput.rows,c=imgInput.cols;

	imgInput.copyTo(in_img.data);


	xf::xfMat2AXIvideo(imgInput,IN_STREAM);

	xf::imwrite("in_img.jpg",imgInput);

	hysteresis(IN_STREAM,OUT_STREAM,r,c);
	
	xf::AXIvideo2xfMat(OUT_STREAM,imgOutput);

	xf::imwrite("out_img.jpg", imgOutput);

	return 0;

}
