#include "Canny_Edge_Detection_headers.h"
#include "Canny_Edge_Detection_config.h"

int main()	{
	cv::Mat sobel_img,out_img,sobel_img_x,sobel_img_y;
	sobel_img_x = cv::imread("C:/Users/vbandhak/Desktop/Canny_Edge_Detection/sobeled_data/lane_sobeled_x.jpg",0);
	sobel_img_y = cv::imread("C:/Users/vbandhak/Desktop/Canny_Edge_Detection/sobeled_data/lane_sobeled_y.jpg",0);
	sobel_img =   cv::imread("C:/Users/vbandhak/Desktop/Canny_Edge_Detection/sobeled_data/lane_sobeled.jpg",0);

	static IMAGE sobel_x(sobel_img.rows,sobel_img.cols);
	static IMAGE sobel_y(sobel_img.rows,sobel_img.cols);
	static IMAGE sobel(sobel_img.rows,sobel_img.cols);
	static IMAGE imgOutput(sobel_img.rows,sobel_img.cols);

	sobel_x.copyTo(sobel_img_x.data);
	sobel_y.copyTo(sobel_img_y.data);
	sobel.copyTo(sobel_img.data);

	AXI_STREAM IN_STREAM_X,IN_STREAM_Y,IN_SOBEL_STREAM,OUT_STREAM;

	xf::xfMat2AXIvideo(sobel_x,IN_STREAM_X);
	xf::xfMat2AXIvideo(sobel_y,IN_STREAM_Y);
	xf::xfMat2AXIvideo(sobel,IN_SOBEL_STREAM);

	unsigned int r=sobel.rows,c=sobel.cols;

	non_maximum_suppression(IN_STREAM_X,IN_STREAM_Y,IN_SOBEL_STREAM,OUT_STREAM,r,c);

	xf::AXIvideo2xfMat(OUT_STREAM,imgOutput);
	xf::imwrite("out_img.jpg", imgOutput);

	return 0;

}
