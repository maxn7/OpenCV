/*
 * starter_video.cpp
 *
 *  Created on: Nov 23, 2010
 *      Author: Ethan Rublee
 *
 * A starter sample for using opencv, get a video stream and display the images
 * easy as CV_PI right?
 *  
 *   Modified on: Sept 14, 2012
 *      By: Maxime Morin
 * 
 * Automatically take a picture from the device and return the path.	
 * 
 */
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>

using namespace cv;
using namespace std;

//hide the local functions in an anon namespace
namespace
{
void help(char** av)
{
  cout << "\nThis program justs gets you started reading images from video\n"
    "Usage:\n./" << av[0] << " <video device number>\n" << "q,Q,esc -- quit\n"
      << "space   -- save frame\n\n"
      << "\tThis is a starter sample, to get you up and going in a copy pasta fashion\n"
      << "\tThe program captures frames from a camera connected to your computer.\n"
      << "\tTo find the video device number, try ls /dev/video* \n"
      << "\tYou may also pass a video file, like my_vide.avi instead of a device number"
      << endl;
}

int process(VideoCapture& capture)
{
  std::vector<DataMatrixCode> codes;
  int n = 0;
  char filename[200];
  

  
  /*Find the number of the image*/
  sprintf(filename, "hubert%.3d.jpg", n);
  while(fopen(filename,"r") != NULL) {
	n++;
	sprintf(filename, "hubert%.3d.jpg", n);
  }
  
  
  Mat frame;
  for (;;)
  {
	usleep(1000000);
    capture >> frame;
    if (frame.empty())
      continue;
    cv::Mat gray;
    cv::cvtColor(frame,gray,CV_RGB2GRAY);
    findDataMatrix(gray, codes);
    drawDataMatrixCodes(codes, frame);
    //char key = (char) waitKey(5); //delay N millis, usually long enough to display and capture input
    
    sprintf(filename, "hubert%.3d.jpg", n++);
    imwrite(filename, frame);
    cout <<  filename << endl;
    return 0;
    
  }
  return 0;
}

}

int main(int ac, char** av)
{

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
  return process(capture);
}
