#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

class DarknetUtilility
{
private:
    std::string configFilePath;
    std::string weightsFilePath;
    std::vector<std::string> colors;
    std::vector<std::string> classes;

    /*helper functions*/
    std::vector<std::string> loadClasses();
    std::vector<cv::Scalar> generateRandomColors();

public:
    DarknetUtilility(/* args */);
    ~DarknetUtilility();
    
};