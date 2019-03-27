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
#include"clr_rec.h"
#include <map>

using namespace std;

cv::VideoCapture cam(1);

int bSums(cv::Mat &src)
{
	
	int counter = 0;
	//迭代器访问像素点
	cv::MatIterator_<uchar> it = src.begin<uchar>();
	cv::MatIterator_<uchar> itend = src.end<uchar>();  
	for (; it!=itend; ++it)
	{
		if((*it)>0) counter+=1;//二值化后，像素点是0或者255
	}			
	return counter;
}
bool comparePic(cv::Mat &pic1,cv::Mat &pic2,int threshold)
{
    cv::Mat pic1Gray,pic2Gray,picDiff,binPic;
    cv::cvtColor(pic1, pic1Gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(pic2, pic2Gray, cv::COLOR_BGR2GRAY);

        cv::absdiff(pic2Gray, pic1Gray, picDiff);
        cv::threshold(picDiff, binPic, 30, 255, cv::THRESH_BINARY);
        // cv::findContours(binPic,contourList,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);
        int point_sum=bSums(binPic);
        return point_sum < threshold;

}



cv::Mat image[7];

void * catchPicThre(void *args)
{
    char clrLetter[60]{"\0"};
    int pic_count=0;
    int ismoved=1;
   // cv::VideoCapture cam(1);

   


    cv::Mat picNow, picLastGray(480, 640, 0),picLast;
    cv::Mat picDiff, binPic,picNowGray;
    vector<vector<cv::Point>> contourList;
    stringstream fileName;
    while (pic_count<=8)
    {
        cam >> picNow;

        cv::cvtColor(picNow, picNowGray, cv::COLOR_BGR2GRAY);

        cv::absdiff(picNowGray, picLastGray, picDiff);
        cv::threshold(picDiff, binPic, 30, 255, cv::THRESH_BINARY);
        // cv::findContours(binPic,contourList,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);
        int point_sum=bSums(binPic);
        if(point_sum<=40&&ismoved==1)
        {   
           
                ismoved = 0;
                // cv::imshow("img", picNow);
                if(pic_count<1 || !comparePic(picNow,picLast,4000))
                {
                pic_count++;
                fileName.str("");
                fileName << "image" << pic_count << ".jpg";
               
                cv::imwrite(fileName.str(), picNow);
                cv::waitKey(300);
                picNow.copyTo(picLast);
                }
            
        }
        else if(point_sum>40)
            ismoved=1;


        picNowGray.copyTo(picLastGray);
    }
    //cout<<"color squence"<<clrLetter<<endl;
}

void cameraReadCmd(int fd)
{

    //  cv::VideoCapture cam(1);
    int i = 0;
    char buffer[10] = "START";
    //FIRST IMAGE
    usleep(1000000*2);
    // cam >> image[i++] ;
    // cv::imshow("Picture",image[i-1]);
    // cv::waitKey(40);
    //cout<<"Picture "<<i-1<<endl;

    strcpy(buffer + 5, "RXL"); //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);
    // cam >> image[i++] ;
    // cout<<"Picture "<<i-1<<endl;

    // cv::imshow("Picture",image[i-1]);
    // cv::waitKey(40);

    // SECOND IMAGE
    //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);
    // cam >> image[i++] ;
    //cout<<"Picture "<<i-1<<endl;

    // cv::imshow("Picture",image[i-1]);
    // cv::waitKey(40);

    //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);
    //cam >> image[i++] ;
    //cout<<"Picture "<<i-1<<endl;

    // cv::imshow("Picture",image[i-1]);
    // cv::waitKey(40);

    // //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);

    strcpy(buffer + 5, "XYT"); //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);

    strcpy(buffer + 5, "RYL"); //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);
    // cam >> image[i++] ;
    // cout<<"Picture "<<i-1<<endl;

    // cv::imshow("Picture",image[i-1]);
    // cv::waitKey(40);

    //SECOND IMAGE
    //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);
    // cam >> image[i++] ;
    // cout<<"Picture "<<i-1<<endl;

    // cv::imshow("Picture",image[i-1]);
    // cv::waitKey(40);

    // //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);
    // cam >> image[i++] ;
    // cout<<"Picture "<<i-1<<endl;

    // cv::imshow("Picture",image[i-1]);
    // cv::waitKey(40);

    // //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);


    strcpy(buffer + 5, "XYT"); //Right X Loose
    write(fd, buffer, 8);
    usleep(1000000*2);
}
int testSerial(int serialFd)
{
    char testStr[] = "STARTECH";
    write(serialFd, testStr, strlen(testStr));
    int i = 0;
    char *recv = testStr;
    int timeCount = 0;
    cout << "Waiting for connection";
    while (i < 2 && timeCount < 1000)
    {
        i += read(serialFd, recv + i, 2 - i);
        usleep(10000);
        timeCount += 1;
        cout << (timeCount % 20 == 0 ? "." : "");
        cout.flush();
    }
    recv[2] = '\0';
    cout << endl;
    if (i == 2 && strcmp(recv, "OK") == 0)
    {
        cout << "Connection Ready" << endl;
        return 1;
    }
    else
    {
        cout << "Connection Faild" << endl;
        return 0;
    }
}
void faceRotCounterClk(char order[])
{
    char temp;
    temp = order[0];
    order[0] = order[2];
    order[2] = order[8];
    order[8] = order[6];
    order[6] = temp;
    
    temp = order[1];
    order[1] = order[5];
    order[5] = order[7];
    order[7] = order[3];
    order[3] = temp;

}
void faceRotClk(char order[])
{
    char temp;
    temp = order[0];
    order[0] = order[6];
    order[6] = order[8];
    order[8] = order[2];
    order[2] = temp;
    
    temp = order[1];
    order[1] = order[3];
    order[3] = order[7];
    order[7] = order[5];
    order[5] = temp;

}
void orderRearrange(char  order[])
{
    map<char,char>  orderMap;
    orderMap.insert(pair<char,char>(order[4+3*9],'U'));
    orderMap.insert(pair<char,char>(order[4+2*9],'R'));
    orderMap.insert(pair<char,char>(order[4+5*9],'F'));
    orderMap.insert(pair<char,char>(order[4+1*9],'D'));
    orderMap.insert(pair<char,char>(order[4+0*9],'L'));
    orderMap.insert(pair<char,char>(order[4+4*9],'B'));

    char orderTemp[54];
    for(int i=0;i<54;i++)
    {
        orderTemp[i] = orderMap[order[i]];
    }
    memcpy(order,orderTemp+3*9,9);
    memcpy(order+9,orderTemp+2*9,9);
    memcpy(order+9*2,orderTemp+5*9,9);
    memcpy(order+9*3,orderTemp+1*9,9);
    memcpy(order+9*4,orderTemp+0*9,9);
    memcpy(order+9*5,orderTemp+4*9,9);
    //R面 F面 逆时针90
    faceRotClk(order+9);
    faceRotClk(order+18);
    //L面 B面顺时针90
    faceRotCounterClk(order+9*4);
    faceRotCounterClk(order+9*5);
    //D面180
    faceRotClk(order+27);
    faceRotClk(order+27);



}
int main(int argc, char **argv)
{

   // catchPicThre();

    int serialFd = serialInit("/dev/rfcomm0", 115200);
    if (serialFd < 0)
    {
        exit(1);
    }
    if (testSerial(serialFd) != 1)
    {
        exit(1);
    }
   cv:: Mat Img;
     for(int i=0;i<10;i++)
    cam>>Img;

    
  //  cv::VideoCapture cam(1);
   char clr[60]={"\0"};
     while(1)
    {
        char choice='c';
        cout<<"train:t  continue:c  onlytest:o\n";
        cin>>choice;
        if(choice=='t')
            trainModel(cam);
        else if(choice=='o')
               { 
                   getPic_rec(clr);
                    return 0;
               }
        else
           break;
        
    }

    pthread_t id;

    pthread_create(&id,NULL,(catchPicThre),NULL);
    cameraReadCmd(serialFd);
    pthread_join(id,NULL);

    getPic_rec(clr);
    char buffer[128] = "SOLVE ";

    Cube cube;
    if (argc > 1)
    {
        char patternized[64];
        char *facelets = argv[1];
        if (argc > 2)
        {
            if (strcmp(argv[1], "-c") == 0)
            {

                strcpy(buffer + 6, argv[2]);

                buffer[5] = strlen(buffer) - 6;
                write(serialFd, buffer, buffer[5] + 6);

                // flush(serialFd);
                close(serialFd);

                return 0;
            }
            else
            {

                patternize(facelets, argv[2], patternized);
                facelets = patternized;
            }
        }
        for (int i = 0; facelets[i] != '\0'; i++)
        {
            if (facelets[i] == 'g')
                facelets[i] = 'U';
            else if (facelets[i] == 'y')
                facelets[i] = 'F';
            else if (facelets[i] == 'r')
                facelets[i] = 'R';
            else if (facelets[i] == 'w')
                facelets[i] = 'B';
            else if (facelets[i] == 'o')
                facelets[i] = 'L';
            else if (facelets[i] == 'b')
                facelets[i] = 'D';
        }

        cout << facelets << endl;

        string sol = cube.solve(facelets);
        if (sol == "")
        {
            cout << "Unsolvable cube!" << endl;
            return 2;
        }
        cout << sol << endl;
        const char *solCstr = sol.c_str();
        strcpy(buffer + 6, solCstr);

        buffer[5] = strlen(buffer) - 6;
        write(serialFd, buffer, buffer[5] + 6);

        // flush(serialFd);
        close(serialFd);

        return 0;
    }
    else
    {
        orderRearrange(clr);

        cout<<clr<<endl;

        string sol = cube.solve(clr);
        if (sol == "")
        {
            cout << "Unsolvable cube!" << endl;
            return 2;
        }
        cout << sol << endl;
        const char *solCstr = sol.c_str();
        strcpy(buffer + 6, solCstr);

        buffer[5] = strlen(buffer) - 6;
        write(serialFd, buffer, buffer[5] + 6);

        close(serialFd);

        return 0;
    }

    // Cube cube;
    // cout<<cube.U(1)<<endl;
    // printMat(cube.rotMat);
    // cout<<cube.L(1)<<endl;
    // printMat(cube.rotMat);
    // cout<<cube.B(1)<<endl;
    // printMat(cube.rotMat);




    return 0;
}