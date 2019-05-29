#include "Canny_Edge_Detection_headers.h"
#include "Canny_Edge_Detection_config.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr,"Invalid Number of Arguments!\nUsage:\n");
		fprintf(stderr,"<Executable Name> <input image path> \n");
		return -1;
	}

	cv::Mat in_img, in_gray, in_conv_img,out_img;


	in_img = cv::imread(argv[1],0); 


	if (in_img.data == NULL)
	{
		fprintf(stderr,"Cannot open image at %s\n",argv[1]);
		return 0;
	}


	/*  convert to specific types  */
	in_img.convertTo(in_conv_img,CV_32F);			//Size conversion


		/////////////////    OpenCV reference   /////////////////

	out_img.create(in_conv_img.rows,in_conv_img.cols,CV_8U);



	static IMAGE imgInput(in_img.rows,in_img.cols);
	static IMAGE imgOutput(in_img.rows,in_img.cols);



	imgInput.copyTo(in_img.data);

	AXI_STREAM IN_STREAM,OUT_STREAM;
	unsigned int r=imgInput.rows,c=imgInput.cols;
	xf::imwrite("in_img.jpg", imgInput);

	xf::xfMat2AXIvideo(imgInput,IN_STREAM);

	#if __SDSCC__
	perf_counter hw_ctr;
	hw_ctr.start();
	#endif
	gaussian_blur(IN_STREAM,OUT_STREAM,r,c);
	#if __SDSCC__
	hw_ctr.stop();
	uint64_t hw_cycles = hw_ctr.avg_cpu_cycles();
	#endif
	//out_img.data = (unsigned char *)imgOutput.copyFrom();

	xf::AXIvideo2xfMat(OUT_STREAM,imgOutput);
	xf::imwrite("out_img.jpg", imgOutput);

	return 0;

}
