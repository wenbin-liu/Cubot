#ifndef __CONTROL_H
#define __CONTROL_H
#include "opencv2/opencv.hpp"

int bSums(cv::Mat &src);

bool comparePic(cv::Mat &pic1,cv::Mat &pic2,int threshold);

void * catchPicThre(void *args);
     
void cameraReadCmd(int fd);

int testSerial(int serialFd);
void faceRotCounterClk(char order[]);
void faceRotClk(char order[]);
void orderRearrange(char  order[]);

#endif