#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

class DarknetUtility
{
private:
    /*fields*/
    std::string configFilePath;
    std::string weightsFilePath;
    std::vector<std::string> colors;
    std::vector<std::string> classes;

    /*helper functions*/
    std::vector<std::string> loadClasses();
    std::vector<cv::Scalar> generateRandomColors();


    /*Internal Functionality around OpenCv*/
    cv::dnn::dnn4_v20190902::Net loadNetWork();
    std::vector<std::string> getOutputLayersName();
    void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame);
    void doPostProcessing(Mat &frame, const std::vector<Mat> &outs, cv::dnn::Net &net);
    
    /*External Interface*/    
    int doInternalProcessing(cv::Mat imagePassed);
    

public:
    DarknetUtility(/* args */);
    ~DarknetUtility();

    
    int doImageProcessing(std::string imagePath);
    int capturingFromCamera(bool doDetection);


    std::string sayHi();
    
};