#include <cv.h>
#include "cube.hpp"
#include <iostream>
#include "search.h"
#include <string>

using namespace std;

int main(int argc,char **argv)
{
    Cube cube;
  if (argc > 1) {
        char patternized[64];
        char* facelets = argv[1];
        if (argc > 2) {
            patternize(facelets, argv[2], patternized);
            facelets = patternized;
        }
        for(int i=0;facelets[i]!='\0';i++)
        {
            if(facelets[i] == 'g')
                facelets[i] = 'U';
            else if(facelets[i] == 'y')
                facelets[i] = 'F';
            else if(facelets[i] == 'r')
                facelets[i] = 'R';
            else if(facelets[i] == 'w')
                facelets[i] = 'B';
            else if(facelets[i] == 'o')
                facelets[i] = 'L';
            else if(facelets[i] == 'b')
                facelets[i] = 'D';
        }

        cout<<facelets<<endl;

        string sol = cube.solve(facelets);
        if (sol == "") {
            cout<<"Unsolvable cube!"<<endl;
            return 2;
        }
        cout<<sol<<endl;
        
        return 0;
    } else {
        return 1;
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