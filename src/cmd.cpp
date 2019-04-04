//#include <cv.h>
#include "cube.hpp"
#include <iostream>
#include "search.h"
#include <string>
#include "uart.h"
#include <unistd.h>
#include <cstdlib>
#include "opencv2/opencv.hpp"
#include <pthread.h>
#include "clr_rec.h"
#include <map>
#include "control.hpp"

using namespace std;

cv::VideoCapture cam(1);
int main(int argc, char **argv)
{
    if(argc != 2)
        {
            cerr<<"Wrong Parameters !\n";
            exit(1);
        }

    int serialFd = serialInit("/dev/rfcomm0", 115200);
    if (serialFd < 0)
    {
        exit(1);
    }
    if (testSerial(serialFd) != 1)
    {
        exit(1);
    }

    char buffer[128];

        char tmp[6] = {'\0'};
        memcpy(tmp, argv[1], 5);
        strcpy(buffer, argv[1]);

        if (strcmp(tmp, "SOLVE") == 0)
            buffer[5] = strlen(buffer) - 6;
        write(serialFd, buffer, strlen(buffer));
        close(serialFd);

        return 0;
}