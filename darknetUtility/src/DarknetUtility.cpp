#include "DarknetUtility.hpp"

DarknetUtility::DarknetUtility(std::string configPath,std::string weightsPath, std::string labelsPath)
{
    this->configFilePath=configPath;
    this->weightsFilePath=weightsPath;
    this->labelsFilePath= labelsPath;
    loadNetWork();
    loadClasses();
    generateRandomColors();
}

DarknetUtility::~DarknetUtility()
{
}

/*helper functions*/

void DarknetUtility::loadNetWork()
{
    

    auto net = cv::dnn::readNetFromDarknet(this->configFilePath, this->weightsFilePath);
    this->net = net;
    this->isConfigured = true;
}

void DarknetUtility::loadClasses()
{
    std::string fileGiven= this->labelsFilePath;
    std::vector<std::string> classes;
    std::ifstream file(fileGiven);
    std::string str;
    while (std::getline(file, str))
    {
        classes.push_back(str);
    }

    this->classes = classes;
}

void DarknetUtility::generateRandomColors()
{

    int amount = 80;
    std::vector<cv::Scalar> colors;
    for (int i = 0; i < amount; i++)
    {
        int r, g, b;
        r = rand() % 255;
        g = rand() % 255;
        b = rand() % 255;

        cv::Scalar tmp(r, g, b);
        colors.push_back(tmp);
    }

    this->colors = colors;
}

/*External Interface*/
int DarknetUtility::doImageProcessing(std::string imagePath)
{

    if (!this->isConfigured)
    {
        return -1;
    }

    cv::Mat image = cv::imread(imagePath);
    return doInternalProcessing(image);
}

int DarknetUtility::doVideoProcessing(cv::VideoCapture cap, bool doDetection)
{
    // Check whether the camera opened properly or not
    if (!cap.isOpened())
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    while (true)
    {
        Mat frame;
        // Capturing frames continuously
        cap.read(frame);
        // stop if no frames are captured
        if (frame.empty())
        {
            std::cout << "Empty Frame" << std::endl;
            return -1;
        }

        if (!doDetection)
        {
            // Displaying the frame
            imshow("Frame", frame);
        }
        else
        {
            doInternalProcessing(frame);
        }

        //stop if a key is pressed
        if (waitKey(1) >= 0)
        {
            printf("stream manually stopped \n");
            break;
        }
    }

    // releasing the video capture object
    cap.release();

    // Closing up
    destroyAllWindows();
    return 0;
}

int DarknetUtility::capturingFromCamera(int camNumber, bool doDetection)
{
    cv::VideoCapture cap(camNumber);
    if (!cap.isOpened())
    {
        return -1;
    }

    return doVideoProcessing(cap, doDetection);
}

int DarknetUtility::capturingFromCamera(std::string streamerOrVideoFile, bool doDetection)
{
    cv::VideoCapture cap(streamerOrVideoFile);
    if (!cap.isOpened())
    {
        return -1;
    }

    return doVideoProcessing(cap, doDetection);
}

std::string DarknetUtility::sayHi()
{
    return "Hola";
}