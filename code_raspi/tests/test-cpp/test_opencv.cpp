#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include<iostream>
using namespace std;

int main() {
   cv::Mat myImage;//Declaring a matrix to load the frames//
   cv::namedWindow("Video Player");//Declaring the video to show the video//
   cv::VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
   if (!cap.isOpened()){ //This section prompt an error message if no video stream is found//
      cout << "No video stream detected" << endl;
      system("pause");
      return-1;
   }
   while (true){ //Taking an everlasting loop to show the video//
      cap >> myImage;
      if (myImage.empty()){ //Breaking the loop if no video frame is detected//
         break;
      }
      imshow("Video Player", myImage);//Showing the video//
      char c = (char)cv::waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
      if (c == 27){ //If 'Esc' is entered break the loop//
         break;
      }
   }
   cap.release();//Releasing the buffer memory//
   return 0;
}
