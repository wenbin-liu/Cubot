#include <cv.h>
#include <string>

using namespace std;




class Cube
{
public:
    Cube();
    cv::Mat rotMat;
    static const cv::Vec<float,3> X;
    static const cv::Vec<float,3> Y;
    static const cv::Vec<float,3> Z;


    std::string U(int deg);
    std::string D(int deg);
    std::string F(int deg);
    std::string B(int deg);
    std::string L(int deg);
    std::string R(int deg);

private:
    cv::Vec<float,3> calTurning(cv::Vec<float,3> &src);
    string axs2Char(cv::Vec<float,3> &rotAxs,int deg);
    
};


