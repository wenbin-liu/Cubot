#include <iostream>
//#include<sstream>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

namespace clrRecog
{
int pic_size = 150;

const vector<bool> _skip{
    0,1,0,1,0,0,0,0
};
const vector<bool> _keep{
    0, 0, 0, 0, 0, 1, 0, 1};

//up camera perspective quad points ---  up, right, back
const vector<vector<Point2f>> quadPtSrc{
  
    vector<Point2f>{Point2f(335, 82), Point2f(345, 238), Point2f(245, 229), Point2f(235, 86)},
      vector<Point2f>{Point2f(442, 75), Point2f(451, 217), Point2f(345, 238), Point2f(335, 82)},
    
};

//destiny perspective quad points
const vector<Point2f> quadPtDst{Point2f(0, 0), Point2f(150, 0), Point2f(150, 150), Point2f(0, 150)};

}; // namespace clrRecog
void displayColor(char colorLetter[], int num_Lblock)
{
    Mat background = Mat::zeros(170 , 220* num_Lblock, CV_8UC3);
    for (int _i = 0; _i < num_Lblock; _i++)
    {

        for (int i = 0; i < 9; i++)
        {
            Rect rec1 = Rect((i % 3) * 60 + 220 * _i, (i / 3) * 60, 50, 50);
            switch (colorLetter[_i *9+i])
            {
            case 'w':
                rectangle(background, rec1, Scalar(255, 255, 255), -1, 8, 0);
                break;
            case 'y':
                rectangle(background, rec1, Scalar(0, 255, 255), -1, 8, 0);
                break;
            case 'b':
                rectangle(background, rec1, Scalar(255, 0, 0), -1, 8, 0);
                break;
            case 'g':
                rectangle(background, rec1, Scalar(0, 255, 0), -1, 8, 0);
                break;
            case 'r':
                rectangle(background, rec1, Scalar(0, 0, 255), -1, 8, 0);
                break;
            case 'o':
                rectangle(background, rec1, Scalar(0, 120, 255), -1, 8, 0);
                break;
            }
        }
        rectangle(background, Rect( 220 * (_i+1),0, 40, 170), Scalar(0, 0, 0), -1, 8, 0);
    }
    imshow("cubicClr", background);
}
// namespace clrRecog

void trainModel(cv::VideoCapture cam)
{
    using namespace clrRecog;
    Mat Img;

   
    //preparation for SVM
    char inputLabels[60];
    Mat svmData, svmLabels;

    int blkGap_x = 15;
    int blkGap_y = 20;
    int clrSize = 15;
    int blkSize = pic_size / 3;

    cout << "input 'r'to record,'s' to leave" << endl;
    while (1)
    {
        cam >> Img;

        char key = waitKey(30);
        if (key == 's')
            break;
        else if (key == 'r')
        {
            cout << "please input colors:(up then front)" << endl;
            cin >> inputLabels;
            cout << "color recorded" << endl;
            displayColor(inputLabels, 2);

            for (int k = 0; k < 2; k++)
            {
                Mat perImg;
                Mat transmtx = getPerspectiveTransform(quadPtSrc[k], quadPtDst);
                warpPerspective(Img, perImg, transmtx, Size(pic_size, pic_size));

                for (int i = 0; i < 9; i++)
                {

                    int x = i % 3;
                    int y = i / 3;
                    Rect extractArea = Rect(x * blkSize + blkGap_x, y * blkSize + blkGap_y, clrSize, clrSize);

                    Mat clrImg = perImg(extractArea).clone();
                    rectangle(perImg, extractArea, Scalar(255, 255, 255));
                    cvtColor(clrImg, clrImg, COLOR_BGR2HSV);
                    for (int j = 0; j < clrSize * clrSize; j++)
                    {
                        float x[2];
                        x[0] = clrImg.at<Vec3b>(j)[0] / 179.0f;
                        x[1] = clrImg.at<Vec3b>(j)[1] / 255.0f;

                        svmData.push_back(Mat(1, 2, CV_32FC1, x));
                        svmLabels.push_back((int)inputLabels[k * 9 + i]);
                    }
                }
                imshow("perspective", perImg);
                waitKey(0);
            }
        }
        imshow("orignal", Img);
    }
    //start training
    cout << "start training svm\n";
    svmLabels.convertTo(svmLabels, CV_32SC1);

    Ptr<ml::SVM> model = ml::SVM::create();
    model->setType(ml::SVM::C_SVC);
    model->setKernel(ml::SVM::CHI2);
    model->setGamma(5);
    model->setC(10);
    model->setCoef0(0);
    model->setNu(0);
    model->setP(0);
    model->setTermCriteria(TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 80000, 0.00005));

    auto tData = ml::TrainData::create(svmData, ml::ROW_SAMPLE, svmLabels);

    model->train(tData);
    cout << "training finished\n";
    model->save("../colorParams.xml");
}


void readFaceColor(Mat Img, char colorLetter[],int face_num,Mat &perImg)
{
    using namespace clrRecog;

    static string modelPath = "../colorParams.xml";

    if (access(modelPath.c_str(), F_OK))
    {
        printf("color model file(s) do not exist!\n");
        exit(-1);
    }

    //load svm model and init cube
    Ptr<ml::SVM> model = ml::SVM::load(modelPath);

   // Mat Img;
   // for (int n = 0; n < 10; n++)
      //  cam >> Img;

    const char colorName[]{'w', 'r', 'b', 'o', 'g', 'y'};
    int blkGap_x = 15;
    int blkGap_y = 15;
    int clrSize = 20;
    int blkSize = pic_size / 3;
    vector<float> clrRes;

    //for (int _i = 0; _i < 2; _i++)
    
       // Mat perImg;
        Mat transmtx = getPerspectiveTransform(quadPtSrc[face_num/4], quadPtDst);
        warpPerspective(Img, perImg, transmtx, Size(pic_size, pic_size));
       // if(sign_keep)

        for (int i = 0; i < 9; i++)
        {
            if (face_num==5)
            {
                i = 3;
            }
            else if (face_num==7)
            {
                i = 5;
            }

            if (_skip[face_num] && i == 7)
                continue;
            int x = i % 3;
            int y = i / 3;
            Rect extractArea = Rect(x * blkSize + blkGap_x, y * blkSize + blkGap_y, clrSize, clrSize);

            Mat clrImg = perImg(extractArea).clone();
            rectangle(perImg, extractArea, Scalar(255, 255, 255));
            cvtColor(clrImg, clrImg, COLOR_BGR2HSV);
            for (int j = 0; j < clrSize * clrSize; j++)
            {
                float x[2];
                x[0] = clrImg.at<Vec3b>(j)[0] / 179.0f;
                x[1] = clrImg.at<Vec3b>(j)[1] / 255.0f;

                Mat predictMat(1, 2, CV_32FC1, x);
                clrRes.push_back(model->predict(predictMat));
            }

            int count[6]{0};
            for (auto lt : clrRes)
            {
                for (int k = 0; k < 6; k++)
                {
                    if (abs(colorName[k] - lt) < 0.1)
                    {
                        count[k]++;
                        break;
                    }
                }
            }
            clrRes.clear();

            int maxNum = 0, maxIdx = 0;
            for (int k = 0; k < 6; k++)
            {
                if (count[k] > maxNum)
                {
                    maxNum = count[k];
                    maxIdx = k;
                }
            }
            if (_keep[face_num])
            {
               colorLetter[0] = colorName[maxIdx];

               cout<<"colorName[maxIdx]:"<<colorName[maxIdx]<<endl;
                break;
            }
            else   {
               // cout<<"i="<<i<<" colorLetter[i] =  "<<colorName[maxIdx]<<endl;
                colorLetter[i] = colorName[maxIdx];
            }
        }
        imshow("orignal", Img);
        imshow("perspective", perImg);
        cout << colorLetter << endl;
        //waitKey(0);
        

    
    displayColor(colorLetter, 1);
    waitKey(0);
}

void getPic_rec(char clr[])
{
    Mat Img;
    Mat perImg;
    Mat background = Mat::zeros(170 , 180* 6, CV_8UC3);
    char picPath[20]={"\0"};
    for(int i=0;i<8;i++)
    {
        if(i<=4)
        sprintf(picPath,"../build/image%d.jpg",i+1) ;
        else
        sprintf(picPath,"../build/image%d.jpg",i+2) ;
        Img=cv::imread(picPath);
        cout<<"pic_num:"<<i<<endl;
        if(i==5)
        readFaceColor(Img,&clr[9+7],i,perImg);
        else if(i==7)
        readFaceColor(Img,&clr[3*9+7],i,perImg);
         else if(i==6)
         readFaceColor(Img,&clr[5*9],i,perImg);
        else 
        {
            /* code */
             readFaceColor(Img,&clr[i*9],i,perImg);
        }
       // Rect extractArea = Rect(i*150, 0, 150, 150);
        //rectangle rect()
       // Mat face_area=perImage(Rect(0,0,perImg.cols,perImg.rows));
       //prImg.copyTo(perImg,extractArea);
    }

    cout<<endl<<endl;
    for(int i=0;i<60;i++)
    {
       
        cout<<clr[i]<<(int)clr[i]<<" ";
        if(i!=0&&(1+i)%9==0)
        cout<<endl;
    }

    displayColor(clr,6);
    waitKey(0);
}

