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
        cout<<"train:t  continue:c  onlytest:o \n";
        cin>>choice;
        if(choice=='t')
            trainModel(cam);
        else if(choice=='o')
            { 
                getPic_rec(clr);
                return 0;
            }
        else if(choice=='c')
           break;

        else break;
        
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
                //Y轴方向反了  临时解决办法
        // char solCstrAdj[128]={'0'};
        // int j = 0;
        // for(const char *ptr = solCstr;*ptr!='\0';ptr++)
        // {
        //     if(*ptr == 'y' && *(ptr+1) == '\'')
        //         {
        //             solCstrAdj[j++] = 'y';
        //             ptr++;
        //         }
        //     else if(*ptr == 'y' && *(ptr+1) != 'y')
        //         {
        //             solCstrAdj[j++] = 'y';
        //             solCstrAdj[j++] = '\'';
        //         }
        //     else if(*ptr == 'Y' && *(ptr+1) == '\'')
        //     {
        //         solCstrAdj[j++] = 'Y';
        //         ptr++;
        //     }
        //     else if(*ptr == 'Y'&& *(ptr+1) != 'Y')
        //         {
        //             solCstrAdj[j++] = 'Y';
        //             solCstrAdj[j++] = '\'';
        //         }
        //     else
        //     {
        //         if((*ptr == 'Y'&& *(ptr+1) == 'Y')||(*ptr == 'y'&& *(ptr+1) == 'y'))
        //         {
        //             solCstrAdj[j++] = *(ptr++);
        //             solCstrAdj[j++] = *(ptr);
        //         }
        //         else
        //             solCstrAdj[j++]=*ptr;
        //     }
            

        // }

        //
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