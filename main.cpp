#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int openImage(char *path)
{

    Mat image;
    image = imread(path);
    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    waitKey(0);

    return 0;
}

int capturingFromCamera()
{
    printf("about to open cam\n");

    VideoCapture cap(0);

    // Check whether the camera opened properly or not
    if (!cap.isOpened())
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    while (1)
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

        // Displaying the frame
        imshow("Frame", frame);

        //stop if a key is pressed
        if (cvWaitKey(30) >= 0)
        {
            printf("stream manually stopped \n");
            break;
        }
    }

    // releasing the video capture object
    cap.release();

    // Closing up
    destroyAllWindows();
}

int main()
{

    capturingFromCamera();
    return 0;
}