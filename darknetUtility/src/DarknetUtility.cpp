#include "DarknetUtility.hpp"

DarknetUtility::DarknetUtility(std::string configPath, std::string weightsPath, std::string labelsPath)
{
    this->configFilePath = configPath;
    this->weightsFilePath = weightsPath;
    this->labelsFilePath = labelsPath;
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
    std::string fileGiven = this->labelsFilePath;
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

/*Internal Functionality around OpenCv*/

cv::Mat DarknetUtility::doInternalProcessing(cv::Mat image)
{

    if (image.empty())
    {
        std::cout << "The image didnt load properly" << std::endl;
        return image;
    }
    else
    {
        //working with the dnn functionalities and adding anotations to the image
        double scale = 0.00392;
        bool cropDecision = false;
        bool swapRBDecision = true;

        //works
        auto blob = cv::dnn::blobFromImage(image, scale, Size(416, 416), Scalar(0, 0, 0), swapRBDecision, cropDecision);
        //works
        net.setInput(blob);

        //works
        auto outputLayersName = getOutputLayersName();

        //works
        //printVectorAux(outputLayersName);

        std::vector<cv::Mat> outs;
        net.forward(outs, outputLayersName);
        //testing the result size
        //std::cout<<outs.size()<<std::endl;

        std::vector<int> class_id;
        std::vector<double> confidences;
        std::vector<cv::Rect2d> boxes;

        doPostProcessing(image, outs);

        /*The image with the annotations is returned*/
        return image;
    }
}

/*External Interface*/
int DarknetUtility::doImageProcessing(std::string imagePath)
{

    if (!this->isConfigured)
    {
        return -1;
    }

    cv::Mat image = cv::imread(imagePath);
    cv::Mat annotatedPicture = doInternalProcessing(image);
    if (annotatedPicture.empty())
    {
        std::cout << "The image didnt load properly" << std::endl;
        return -1;
    }
    cv::imshow("Predictions", annotatedPicture);
    cv::waitKey(30);
    cv::destroyAllWindows();
    return 0;
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
            cv::Mat annotatedPicture = doInternalProcessing(frame);
            if (annotatedPicture.empty())
            {
                std::cout << "The processing failed" << std::endl;
                return -1;
            }
            cv::imshow("Predictions", annotatedPicture);
        }

        //stop if a key is pressed
        if (cv::waitKey(1) >= 0)
        {
            printf("stream manually stopped \n");
            break;
        }
    }

    // releasing the video capture object
    cap.release();

    // Closing up
    cv::destroyAllWindows();
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