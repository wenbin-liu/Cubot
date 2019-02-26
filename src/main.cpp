#include <cv.h>
#include "cube.hpp"
#include <iostream>

using namespace std;
void printMat(cv::Mat const &mat)
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
int main(int argc,char **argv)
{
    Cube cube;
    cout<<cube.U(1)<<endl;
    printMat(cube.rotMat);
    cout<<cube.L(1)<<endl;
    printMat(cube.rotMat);
    cout<<cube.B(1)<<endl;
    printMat(cube.rotMat);

    return 0;
}