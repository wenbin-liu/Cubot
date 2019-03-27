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

int main(int argc, char **argv)
{
    Cube cube;
    if (argc > 1)
    {
        char patternized[64];
        char *facelets = argv[1];
        if (argc > 2)
        {

            

                patternize(facelets, argv[2], patternized);
                facelets = patternized;
            
        }
        int j=0;
        int i;
        for (i = 0; facelets[j] != '\0'; i++)
        {
            if (facelets[j] == 'g')
                facelets[i] = 'U';
            else if (facelets[j] == 'y')
                facelets[i] = 'F';
            else if (facelets[j] == 'r')
                facelets[i] = 'R';
            else if (facelets[j] == 'w')
                facelets[i] = 'B';
            else if (facelets[j] == 'o')
                facelets[i] = 'L';
            else if (facelets[j] == 'b')
                facelets[i] = 'D';
            else 
                i--;
            j++;
        }
        facelets[i] = '\0';

        cout << facelets << endl;

        string sol = cube.solve(facelets);
        if (sol == "")
        {
            cout << "Unsolvable cube!" << endl;
            return 2;
        }
        cout << sol << endl;

        return 0;
    }
    else
    {

   

        return 0;
    }



    return 0;
}