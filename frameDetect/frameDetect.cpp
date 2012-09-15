/*
 * frameDetect.cpp
 *
 *  Created on: Sept 15, 2012
 *      Author: Maxime Morin
 *
 * The program detects a framework and localize objects into this one.	
 * 
 */
 
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>

#define PI 3.14159265

using namespace cv;
using namespace std;

/*Functions prototype*/
namespace
{
	void help(char** av);
	int getImage(VideoCapture& capture, Mat& target);
	int displayAndWait(Mat& image);
	int wrapImage(Mat& image);
}

int main(int ac, char** av)
{
	Mat image;
 
	if (ac != 2)
	{
		help(av);
		return 1;
	}
	std::string arg = av[1];

	VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
	if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
    capture.open(atoi(arg.c_str()));
	if (!capture.isOpened())
	{
		cerr << "Failed to open a video device or video file!\n" << endl;
		help(av);
		return 1;
	}
  
	
	getImage(capture, image);
	
	/*Image processing.*/
	
	wrapImage(image);
	displayAndWait(image);
	imwrite("wrapcalib.jpg", image);
	
	return 0;
}
namespace
{	
		/*Help function*/
	void help(char** av)
	{
	  cout << "\nThis program detects a framework and localize objects into this one.\n"
		"Usage:\n./" << av[0] << " <video device number> or <filename>\n" 
		  << "\tThis is a test program for static robot vision\n"
		  << endl;
	}
	
	
	int getImage(VideoCapture& capture, Mat& target)
	{
		/*This function record a frame from the capture input.
		 * The frame is recorded in the current directory and 
		 * also loaded into the "target" argument.*/
		 
		std::vector<DataMatrixCode> codes;
		int n = 0;
		char filename[200];		
		Mat frame;
		
		/* Find the number of the image. */
		sprintf(filename, "hubert%.3d.jpg", n);
		while(fopen(filename,"r") != NULL)
		{
			n++;
			sprintf(filename, "hubert%.3d.jpg", n);
		}
	  
	  
		for (;;)
		{
			usleep(1000000); /* Wait for webcam auto exposure setting. */
			capture >> frame;
			if (frame.empty())
			continue;
			cv::Mat gray;
			cv::cvtColor(frame,gray,CV_RGB2GRAY);
			findDataMatrix(gray, codes);
			drawDataMatrixCodes(codes, frame);
			sprintf(filename, "hubert%.3d.jpg", n++);
			imwrite(filename, frame);
			cout <<  filename << endl;
			target = frame;
			return 0;
		}
		return 0;
	}
	
	int displayAndWait(Mat& image)
	{
		/* This is a debug function which displays an image 
		 * and wait for a key pressed to continue */
		 
		string window_name = "Test output";
		namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window
		imshow(window_name, image);
		cout << "Program paused : Press a key to continue..." << endl;
		char key = (char) waitKey(-1); //wait infinity for a key pressed
		return 0;
	}
	
	int wrapImage(Mat& image)
	{
		/*This function modify the perspective of the image
		 * in order to give a to view of the framework.
		 * The code comes from an example available at :
		 * http://em6.clubs.resel.fr/dokuwiki/doku.php?id=opencv */
		 		 
		int alpha_=63., beta_=90., gamma_=89.;
		int f_ = 500, dist_ = 463;
		bool video = false;
	 
		Mat source = image;
		Mat destination;
 
        // OpenCV trackbars gives integer values between 0
        // and maxValue (las parameter of createTrackbar)
        // We have to adapt this parameters
        
        double f, dist;
        double alpha, beta, gamma;
        alpha = ((double)alpha_ - 90.)*PI/180;
        beta = ((double)beta_ - 90.)*PI/180;
        gamma = ((double)gamma_ - 90.)*PI/180;
        f = (double) f_;
        dist = (double) dist_;
 
        Size taille = source.size();
        double w = (double)taille.width, h = (double)taille.height;
 
        // This matix project the image (2D) into the space (3D)
        // and center it
        Mat A1 = (Mat_<double>(4,3) <<
            1, 0, -w/2,
            0, 1, -h/2,
            0, 0,    0,
            0, 0,    1);
        // We define 3 rotations matrix (one per axis)
        Mat RX = (Mat_<double>(4, 4) <<
            1,          0,           0, 0,
            0, cos(alpha), -sin(alpha), 0,
            0, sin(alpha),  cos(alpha), 0,
            0,          0,           0, 1);
        Mat RY = (Mat_<double>(4, 4) <<
            cos(beta), 0, -sin(beta), 0,
                    0, 1,          0, 0,
            sin(beta), 0,  cos(beta), 0,
                    0, 0,          0, 1);
        Mat RZ = (Mat_<double>(4, 4) <<
            cos(gamma), -sin(gamma), 0, 0,
            sin(gamma),  cos(gamma), 0, 0,
            0,          0,           1, 0,
            0,          0,           0, 1);
        // Final rotation matrix
        Mat R = RX * RY * RZ;
        // Translation matrix : the plan is placed a a distance "dist"
        // from the video source
        Mat T = (Mat_<double>(4, 4) <<
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, dist,
            0, 0, 0, 1);
        // Video source matrix : project image in 2D and center it
        Mat A2 = (Mat_<double>(3,4) <<
            f, 0, w/2, 0,
            0, f, h/2, 0,
            0, 0,   1, 0);
        // The transformation matrix is the product of all the previous
        // matrix
        Mat transfo = A2 * (T * (R * A1));
 
        // Apply transform
        warpPerspective(source, destination, transfo, taille, INTER_CUBIC | WARP_INVERSE_MAP);
        // Write result
        image = destination;
		return 0;
	}
}
