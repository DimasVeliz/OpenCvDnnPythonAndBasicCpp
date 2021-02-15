#include <iostream>
#include <DarknetUtility.hpp>

int main()
{
    std::string cfgFile = "/home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/cfg/yolov3-tiny.cfg";
    std::string weightFile = "/home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/weights/yolov3-tiny.weights";
    std::string labels = "//home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/data/coco.names";
    std::string imagePath = "/home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/obj/horses.jpg";
    std::string videoPath = "//home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/obj/family_crossing.mp4";

    DarknetUtility utility(cfgFile,weightFile,labels);
    
    /*TEST*/
    /*std::cout<<utility.sayHi() <<std::endl;*/
    /*utility.capturingFromCamera(0,false);*/
    /*utility.doImageProcessing(imagePath);*/
    /*utility.capturingFromCamera(0,true);*/
    utility.capturingFromCamera(videoPath,true);

    return 0;
}