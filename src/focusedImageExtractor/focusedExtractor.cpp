#include <stdio.h>
#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

ofstream logfile("log.txt",ofstream::app|ofstream::out);
VideoCapture cap;


Mat frame,canvas,regionInterest;
int interestX = 0,interestY = 0;

double interestThreshold =  0.993;

void saveImage(){
	double actualFrame = cap.get(CV_CAP_PROP_POS_MSEC);
	logfile << actualFrame << endl;
	stringstream filename;
	filename << actualFrame << ".png";
	cv::imwrite(filename.str(),frame);
}

void changeFrameRelative(double frames){

    double actualFrame = cap.get(CV_CAP_PROP_POS_MSEC);
    cout << actualFrame << endl;

    cap.set(CV_CAP_PROP_POS_MSEC,frames+actualFrame);

}

double findSimilarity(){
	Mat result;
    Mat regionInterestrio = Mat (regionInterest,Rect(0,0,regionInterest.cols-1,regionInterest.rows-1));
	Mat frameInterest = Mat(frame,Rect (interestX,interestY,regionInterest.cols,regionInterest.rows));
	matchTemplate(frameInterest, regionInterestrio, result, CV_TM_CCOEFF_NORMED);
	imshow("frame interest",frameInterest);
	imshow("result",result);
	imshow("regionInterest",regionInterest);
	waitKey(1);
	double maxVal;
	minMaxIdx(result, 0, &maxVal, 0, 0);

	return maxVal;
}

void automaticallyExtractBestFrames(){
	double curPos;
	curPos = cap.get(CV_CAP_PROP_POS_MSEC);
	cap.set(CV_CAP_PROP_POS_MSEC,0);
    cv::Mat result;
    int i = 0;
    while (bool bSuccess = cap.read(frame)){
    	double sim = findSimilarity();
    	cout << "looking at image: " << i++ << " " << sim << endl;
    	if ( sim > interestThreshold){
    		saveImage();
    	}
    }

	cap.set(CV_CAP_PROP_POS_MSEC,curPos);
	cap >> frame;
}

void onMouse (int evt, int x, int y, int flags, void* param)
{
    if (evt == CV_EVENT_LBUTTONDOWN)
    {
        int sizex = frame.cols-x-1;
        int sizey = frame.rows-y-1;
        if (sizex > 100)
            sizex = 100;
        if (sizey > 100)
            sizey = 100;
        interestX = x;
        interestY = y;
        cv::Mat roi(frame,Rect(x,y,sizex,sizey));
        regionInterest = roi.clone();
        logfile << x << " " << y << " " << cap.get(CV_CAP_PROP_POS_MSEC) << endl;

    }
}



int main( int argc, char** argv )
{

    enum ShowInterest{show,hide} showInterest=hide;

    std::cout<< " opening: " << argv[1] << std::endl;
    cap.open(argv[1]); // open the default camera
    if(!cap.isOpened()) { // check if we succeeded{
        std::cout<< " can't open file or device " << std::endl;
        return -1;
    }

    namedWindow("canvas",1);
    setMouseCallback("canvas", onMouse);


    enum playState {play,stopped} curState;
    curState = playState::stopped;
    int c = 10;
    int c2 = 100;
    interestX = 0;
    interestY = 0;

    cap>>frame;


    {
        cv::Mat roi(frame,Rect(100,100,100,100));
        regionInterest = roi.clone();
    }

    bool load_frame = true;

    for(;;)
    {
        if ( load_frame ){
            cap >> frame; // get a new frame from camera
            cout << "sim is:"<< findSimilarity() << endl;

        }
        canvas = frame.clone();

        if (showInterest == show)
        {
            cv::Mat roi(canvas,Rect(interestX,interestY,regionInterest.cols,regionInterest.rows));
            regionInterest.copyTo(roi);
        }
        imshow("canvas", canvas);
        if (curState == play){
            char key = waitKey(30) ;
            cout << key << endl;
            if (key == 27)
                break;
            if(key == 'p'){
                curState = stopped;
            }

            //else curState = stopped;
        }
        else{
            load_frame = true;

            char key = waitKey(0);
            if(key == 'p'){
                curState = play;
            }
            if (key == 27)
                break;
            cout << key << endl;
            if (key == '6'){
                changeFrameRelative(33);
            }
            if (key == '5'){
                changeFrameRelative(-33);
            }
            if (key == '7'){
                changeFrameRelative(33*c);
            }
            if (key == '4'){
                changeFrameRelative(-33*c);
            }
            if (key == '8'){
                changeFrameRelative(33*c2);
            }
            if (key == '3'){
                changeFrameRelative(-33*c2);
            }
            switch (key){
            case 's':
                if(showInterest == show)
                    showInterest = hide;
                else
                    showInterest = show;
                load_frame = false;
                break;
            case 'e':
            	saveImage ();
            	load_frame = false;
            	break;
            case 'a':
            	automaticallyExtractBestFrames();

            	break;
            }
        }
    }


  return 0;
}

