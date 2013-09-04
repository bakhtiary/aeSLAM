#include <unistd.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "viso_mono_flight.h"

#include <opencv2/opencv.hpp>
#include <renderVO.hpp>

using namespace cv;

using namespace std;
int main(int argc, char** argv) {

	char buf[1000];
	getcwd(buf, 1000);

	//string dir = "/home/abakhtiary/projects/aeSLAM/aeSLAM/datasets/driving/2010_03_04_drive_0033";
	string dir = "/home/abakhtiary/projects/aeSLAM/aeSLAM/datasets/flying/2.1-SantBoi,extracted(third_video)/sector2/undistorted";

	if (argc < 2) {
		cout << "current folder is" << buf << endl;
		cout << "the folder where aerial images are can be input in the commandline parameters" << endl;

//		return 1;
	}else{
		string dir = argv[1];
	}

	cout << "looking at this folder for the images to be processed:\n"<< dir << endl;


	VisualOdometryMonoFlight::parameters param1;
	{
		param1.calib.cu = 607.53659;
		param1.calib.cv = 356.09882;
		param1.calib.f = (1722.13586 + 1715.39398)/2;
	}

	VisualOdometryMonoFlight::parameters param2;
	{
		param2.calib.cu = 635.9;
		param2.calib.cv = 194.1;
		param2.calib.f = 645.2;
		param2.height = 1.6;
		param2.pitch = -0.8;
	}

	VisualOdometryMonoFlight::parameters param = param1;

	VisualOdometryMonoFlight viso(param);

	Matrix pose = Matrix::eye(4);

	cout << pose << endl << endl;

	VideoCapture cap ("/home/abakhtiary/projects/aeSLAM/aeSLAM/datasets/flying/2.1-SantBoi,extracted(third_video)/sector2/extracted.mp4");


	for (int32_t i=1; i<700; i++) {

		// input file names
		char base_name[256]; sprintf(base_name,"%06d.png",i);

		// catch image read/write errors here
		try {

			string img_file_name  = dir+string("/I1_") + base_name;
			// load left and right input image
			cout << "loading image:" << img_file_name << endl;

			Mat img;
			Mat img_bw;
			img_bw= imread(img_file_name,CV_LOAD_IMAGE_GRAYSCALE);
			//cap >> img;
			//cvtColor(img,img_bw,COLOR_RGB2GRAY );
			imshow("image",img_bw);
			if ( ((char) waitKey(10)) == 27){
				cout << "have a nice day" << endl;
				return 0;
			}


			//
			//	      // image dimensions
			int32_t width  = img_bw.cols;
			int32_t height = img_bw.rows;
			//
			//	      // convert input images to uint8_t buffer
			uint8_t* img_data  = (uint8_t*)malloc(width*height*sizeof(uint8_t));
			int32_t k=0;
			cout << "copying" << endl;
			for (int32_t v=0; v<height; v++) {
				for (int32_t u=0; u<width; u++) {
					img_data[k]  = img_bw.at<uchar>(v,u);
					k++;
				}
			}

			cout << "Processing: Frame: " << i;
			//	      // compute visual odometry
			//

			int32_t dims[] = {width,height,width};
			if (viso.process(img_data,dims)) {
			//
			//	        // on success, update current pose
			pose = pose * Matrix::inv(viso.getMotion());
			//
			//	        // output some statistics
			double num_matches = viso.getNumberOfMatches();
			double num_inliers = viso.getNumberOfInliers();
			cout << ", Matches: " << num_matches;
			cout << ", Inliers: " << 100.0*num_inliers/num_matches << " %" << ", Current pose: " << endl;
			cout << pose << endl << endl;
			//
			} else {
				cout << " ... failed!" << endl;
			}
			//
			//	      // release uint8_t buffers
			  free(img_data);
			//	      free(right_img_data);

			// catch image read errors here
		} catch (...) {
			cerr << "ERROR: Couldn't read input files!" << endl;
			return 1;
		}
	}
	return 0;
}

