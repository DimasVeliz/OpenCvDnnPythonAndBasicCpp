#include "DarknetUtility.hpp"

DarknetUtility::DarknetUtility(/* args */)
{

    loadClasses();
    generateRandomColors();
}

DarknetUtility::~DarknetUtility()
{
}

/*helper functions*/

void DarknetUtility::loadNetWork()
{
    std::string pathToCfgtFile = this->configFilePath;
    std::string pathToWeightFile = this->weightsFilePath;

    auto net = cv::dnn::readNetFromDarknet(pathToCfgtFile, pathToWeightFile);
    this->net = net;
    this->isConfigured = true;
}

void DarknetUtility::loadClasses()
{
    std::vector<std::string> classes;
    std::ifstream file("Hola.txt");
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

int DarknetUtility::capturingFromCamera(bool doDetection)
{
    printf("about to open cam\n");

    VideoCapture cap("/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/obj/videos/dimasPateandoLong.MP4");
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
            printf("empty frame");
            break;
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

std::string DarknetUtility::sayHi()
{
    return "Hola";
}