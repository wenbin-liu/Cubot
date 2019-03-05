#include <cube.hpp>
#include <iostream>
#include "search.h"

 const cv::Vec<float,3> Cube::X(1,0,0);
 const cv::Vec<float,3> Cube::Y(0,1,0);
 const cv::Vec<float,3> Cube::Z(0,0,1);

Cube::Cube():
     rotMat(cv::Mat::eye(3,3,CV_32F))
{

}

cv::Vec<float,3> Cube::calTurning(cv::Vec<float,3> &src)
{
    cv::Vec<float,3> dst = X;
    cv::Vec<float,3> rotAxs = {0,0,0};
    
    if(src.dot(X)==1.)
    {
        rotAxs[0] =  3;
        return rotAxs;
    }
    else if(src.dot(Y)==1.)
    {
        rotAxs[1] =  3;
        return rotAxs;
    }
       

    rotAxs = src.cross(dst);
   
    if(rotAxs == cv::Matx<float,3,1>::zeros())
    {
        if(src.dot(dst) == -1.0)   
        {
            //rotAxs 必为除去dst外的另一条轴
            rotAxs[0] = 2*dst[1];
            rotAxs[1] = 2* dst[0];
        }
    }
    
    //判断旋转轴是否为Z轴
    else if(rotAxs.dot(Z)!=0.0)
    {
      
        rotAxs = 2*X;
    }

    cv::Mat turnMat = cv::Mat::zeros(3,3,CV_32F);
    cv::Mat tempMat = cv::Mat::zeros(3,3,CV_32F);

    if(rotAxs.dot(X)!=0.)
    {
        turnMat.at<float>(0,0) = 1.;
        turnMat.at<float>(2,1)  = 1.;
        turnMat.at<float>(1,2)  = -1.;
        int turns = (int(rotAxs.dot(X))+4)%4;
        for(;turns>0;turns--)
            {
               tempMat = turnMat*rotMat;
               rotMat = tempMat;
            }
        
    }
        else
    {
        turnMat.at<float>(2,0) =-1;
        turnMat.at<float>(1,1)  = 1;
        turnMat.at<float>(0,2)  = 1;
        int turns = (int(rotAxs.dot(Y))+4)%4;
        for(;turns>0;turns--)
            {
            tempMat = turnMat*rotMat;
            rotMat = tempMat;
            }
        
    }
    return rotAxs;

}
 string Cube::U(int deg)
{
    cv::Vec<float,3>src;
    rotMat.col(2).copyTo(src);
    cv::Vec<float,3> rotAxs = calTurning( src );
    return axs2Char(rotAxs,deg);
     
}

 string Cube::D(int deg)
{
    cv::Vec<float,3>src ;
    rotMat.col(2).copyTo(src);
    src = -src;
    cv::Vec<float,3> rotAxs = calTurning( src );
    return axs2Char(rotAxs,deg);
     
}
 string Cube::L(int deg)
{
    cv::Vec<float,3>src  ;
    rotMat.col(1).copyTo(src);
    src = -src;
    cv::Vec<float,3> rotAxs = calTurning( src );
    return axs2Char(rotAxs,deg);
     
}
 string Cube::R(int deg)
{
    cv::Vec<float,3>src  ;
    rotMat.col(1).copyTo(src);
    src = src;
    cv::Vec<float,3> rotAxs = calTurning( src );
    return axs2Char(rotAxs,deg);
     
}
 string Cube::F(int deg)
{
    cv::Vec<float,3>src  ;
    rotMat.col(0).copyTo(src);
    src = src;
    cv::Vec<float,3> rotAxs = calTurning( src );
    return axs2Char(rotAxs,deg);
     
}
 string Cube::B(int deg)
{
    cv::Vec<float,3>src  ;
    rotMat.col(0).copyTo(src);
    src = -src;
    cv::Vec<float,3> rotAxs = calTurning( src );
    return axs2Char(rotAxs,deg);
}

string Cube::axs2Char(cv::Vec<float,3> &rotAxs,int deg)
{
    string cmdStr;
    if(fabs(rotAxs.dot(X) - 3.0)<1e-5)
    {
        if(deg == 1)
            cmdStr = "x";
        else if(deg == -1)
            cmdStr = "x'";
        else if(deg == 2)
            cmdStr = "xx";
    }

    else if(fabs(rotAxs.dot(X) - 2.0)<1e-5)
    {
        cmdStr = "XX";
        if(deg == 1)
            cmdStr = cmdStr + "y";
        else if(deg == -1)
            cmdStr = cmdStr + "y'";
        else if(deg == 2)
            cmdStr = cmdStr + "yy";
    }
    else if(fabs(rotAxs.dot(X) - 1.0)<1e-5)
    {
        cmdStr = "X'";
        if(deg == 1)
            cmdStr = cmdStr + "y";
        else if(deg == -1)
            cmdStr = cmdStr + "y'";
        else if(deg == 2)
            cmdStr = cmdStr + "yy";
    }
        else if(fabs(rotAxs.dot(X) + 1.0)<1e-5)
    {
        cmdStr = "X";
        if(deg == 1)
            cmdStr = cmdStr + "y";
        else if(deg == -1)
            cmdStr = cmdStr + "y'";
        else if(deg == 2)
            cmdStr = cmdStr + "yy";
    }

    
    //Y轴
    else  if(fabs(rotAxs.dot(Y) - 3.0)<1e-5)
    {
        if(deg == 1)
            cmdStr = "y";
        else if(deg == -1)
            cmdStr = "y'";
        else if(deg == 2)
            cmdStr = "yy";
    }

    else if(fabs(rotAxs.dot(Y) - 2.0)<1e-5)
    {
        cmdStr = "YY";
        if(deg == 1)
            cmdStr = cmdStr + "x";
        else if(deg == -1)
            cmdStr = cmdStr + "x'";
        else if(deg == 2)
            cmdStr = cmdStr + "xx";
    }
    else if(fabs(rotAxs.dot(Y) - 1.0)<1e-5)
    {
        cmdStr = "Y'";
        if(deg == 1)
            cmdStr = cmdStr + "x";
        else if(deg == -1)
            cmdStr = cmdStr + "x'";
        else if(deg == 2)
            cmdStr = cmdStr + "xx";
    }
        else if(fabs(rotAxs.dot(Y) + 1.0)<1e-5)
    {
        cmdStr = "Y";
        if(deg == 1)
            cmdStr = cmdStr + "x";
        else if(deg == -1)
            cmdStr = cmdStr + "x'";
        else if(deg == 2)
            cmdStr = cmdStr + "xx";
    }
    return cmdStr;
    
}

std::string Cube::convertKociembaStr(char *str)
{
    std::string cmdStr;
    for(int i=0;str[i]!='\0';i++)
    {
        if(str[i] == 'U')
        {
            if(str[i+1]==' ')
                cmdStr += U(1);
            else if(str[i+1] == '\'')
                cmdStr += U(-1);
            else if(str[i+1] == '2')
                cmdStr += U(2);     
        }
        else if(str[i] == 'D')
        {
            if(str[i+1]==' ')
                cmdStr += D(1);
            else if(str[i+1] == '\'')
                cmdStr += D(-1);
            else if(str[i+1] == '2')
                cmdStr += D(2);     
        }

        else if(str[i] == 'R')
        {
            if(str[i+1]==' ')
                cmdStr += R(1);
            else if(str[i+1] == '\'')
                cmdStr += R(-1);
            else if(str[i+1] == '2')
                cmdStr += R(2);     
        }

        else if(str[i] == 'L')
        {
            if(str[i+1]==' ')
                cmdStr += L(1);
            else if(str[i+1] == '\'')
                cmdStr += L(-1);
            else if(str[i+1] == '2')
                cmdStr += L(2);     
        }

        else if(str[i] == 'F')
        {
            if(str[i+1]==' ')
                cmdStr += F(1);
            else if(str[i+1] == '\'')
                cmdStr += F(-1);
            else if(str[i+1] == '2')
                cmdStr += F(2);     
        }

        else if(str[i] == 'B')
        {
            if(str[i+1]==' ')
                cmdStr += B(1);
            else if(str[i+1] == '\'')
                cmdStr += B(-1);
            else if(str[i+1] == '2')
                cmdStr += B(2);     
        }
        printMat(rotMat);
        cout<<endl;
    }
    return cmdStr;
}

std::string Cube::solve(char *facelets)
{
     std::string cmdStr;
        char *sol = solution(
            facelets,
            24,
            1000,
            0,
            "cache"
        );
       
        if(sol == NULL)
        return cmdStr;
        else if(sol[0]=='E')
        return std::string(sol);
    cout<<sol<<endl;
    cmdStr = convertKociembaStr(sol);
    free(sol);
    return cmdStr;
}
void Cube::printMat(cv::Mat const &mat)
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            cout<<mat.at<float>(i,j);
        }
        cout<<endl;
    }
}