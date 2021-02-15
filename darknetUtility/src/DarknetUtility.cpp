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

    int amount = this->classes.size();
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

std::vector<std::string> DarknetUtility::getOutputLayersName()
{

    std::vector<std::string> outPutLayersN;
    auto layerNames = net.getLayerNames();
    auto v = net.getUnconnectedOutLayers();
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        outPutLayersN.push_back(layerNames[it[0] - 1]);
    }

    return outPutLayersN;
}


void DarknetUtility::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame)
{
    rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0));
    std::string label = format("%.2f", conf);

    if (!classes.empty())
    {
        CV_Assert(classId < (int)classes.size());
        label = classes[classId] + ": " + label;
    }
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = max(top, labelSize.height);
    rectangle(frame, Point(left, top - labelSize.height),
              Point(left + labelSize.width, top + baseLine), Scalar::all(255), FILLED);
    putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar());
}

void DarknetUtility::doPostProcessing(Mat &frame, const std::vector<Mat> &outs)
{
    float confThreshold = 0.7;
    float nmsThreshold = 0.8;

    static std::vector<int> outLayers = net.getUnconnectedOutLayers();
    static std::string outLayerType = net.getLayer(outLayers[0])->type;
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<Rect> boxes;
    if (outLayerType == "DetectionOutput")
    {
        // Network produces output blob with a shape 1x1xNx7 where N is a number of
        // detections and an every detection is a vector of values
        // [batchId, classId, confidence, left, top, right, bottom]
        CV_Assert(outs.size() > 0);
        for (size_t k = 0; k < outs.size(); k++)
        {
            float *data = (float *)outs[k].data;
            for (size_t i = 0; i < outs[k].total(); i += 7) //loop sets "i" to each batchId Index
            {
                float confidence = data[i + 2];
                if (confidence > confThreshold)
                {
                    int left = (int)data[i + 3];
                    int top = (int)data[i + 4];
                    int right = (int)data[i + 5];
                    int bottom = (int)data[i + 6];
                    int width = right - left + 1;
                    int height = bottom - top + 1;
                    if (width <= 2 || height <= 2)
                    {
                        left = (int)(data[i + 3] * frame.cols);
                        top = (int)(data[i + 4] * frame.rows);
                        right = (int)(data[i + 5] * frame.cols);
                        bottom = (int)(data[i + 6] * frame.rows);
                        width = right - left + 1;
                        height = bottom - top + 1;
                    }
                    classIds.push_back((int)(data[i + 1]) - 1); // Skip 0th background class id.
                    boxes.push_back(Rect(left, top, width, height));
                    confidences.push_back(confidence);
                }
            }
        }
    }
    else if (outLayerType == "Region")
    {
        for (size_t i = 0; i < outs.size(); ++i)
        {
            // Network produces output blob with a shape NxC where N is a number of
            // detected objects and C is a number of classes + 4 where the first 4
            // numbers are [center_x, center_y, width, height]
            float *data = (float *)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
            {
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                Point classIdPoint;
                double confidence;
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confThreshold)
                {
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;
                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)confidence);
                    boxes.push_back(Rect(left, top, width, height));
                }
            }
        }
    }
    else
        CV_Error(Error::StsNotImplemented, "Unknown output layer type: " + outLayerType);
    // NMS is used inside Region layer only on DNN_BACKEND_OPENCV for another backends we need NMS in sample
    // or NMS is required if number of outputs > 1
    if (outLayers.size() > 1 || (outLayerType == "Region"))
    {
        std::map<int, std::vector<size_t>> class2indices;
        for (size_t i = 0; i < classIds.size(); i++)
        {
            if (confidences[i] >= confThreshold)
            {
                class2indices[classIds[i]].push_back(i);
            }
        }
        std::vector<Rect> nmsBoxes;
        std::vector<float> nmsConfidences;
        std::vector<int> nmsClassIds;
        for (std::map<int, std::vector<size_t>>::iterator it = class2indices.begin(); it != class2indices.end(); ++it)
        {
            std::vector<Rect> localBoxes;
            std::vector<float> localConfidences;
            std::vector<size_t> classIndices = it->second;
            for (size_t i = 0; i < classIndices.size(); i++)
            {
                localBoxes.push_back(boxes[classIndices[i]]);
                localConfidences.push_back(confidences[classIndices[i]]);
            }
            std::vector<int> nmsIndices;
            cv::dnn::NMSBoxes(localBoxes, localConfidences, confThreshold, nmsThreshold, nmsIndices);
            for (size_t i = 0; i < nmsIndices.size(); i++)
            {
                size_t idx = nmsIndices[i];
                nmsBoxes.push_back(localBoxes[idx]);
                nmsConfidences.push_back(localConfidences[idx]);
                nmsClassIds.push_back(it->first);
            }
        }
        boxes = nmsBoxes;
        classIds = nmsClassIds;
        confidences = nmsConfidences;
    }
    for (size_t idx = 0; idx < boxes.size(); ++idx)
    {
        Rect box = boxes[idx];
        drawPred(classIds[idx], confidences[idx], box.x, box.y,box.x + box.width, box.y + box.height, frame);
    }
}




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
            imshow("Frame with no processing", frame);
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
            std::cout <<"stream manually stopped"<< std::endl;
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