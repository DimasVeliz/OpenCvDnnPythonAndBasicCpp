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
    std::string labelsFilePath;
    std::vector<cv::Scalar> colors;
    std::vector<std::string> classes;
    cv::dnn::dnn4_v20190902::Net net;
    bool isConfigured;


    /*helper functions*/
    void loadClasses();
    void generateRandomColors();
    void loadNetWork();


    /*Internal Functionality around OpenCv*/
    std::vector<std::string> getOutputLayersName();
    void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame);
    void doPostProcessing(Mat &frame, const std::vector<Mat> &outs);
    
    cv::Mat doInternalProcessing(cv::Mat image);
    int doVideoProcessing(cv::VideoCapture cap, bool doDetection);

    

public:
    DarknetUtility(std::string configPath,std::string weightsPath,std::string labelsPath);
    ~DarknetUtility();

    
    /*External Interface*/    
    int doImageProcessing(std::string imagePath);
    int capturingFromCamera(int camNumber, bool doDetection);
    int capturingFromCamera(std::string streamerOrVideoFile, bool doDetection);



    std::string sayHi();
    
};