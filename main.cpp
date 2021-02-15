#include <iostream>
#include <DarknetUtility.hpp>

int main()
{
    std::string cfgFile = "/home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/cfg/yolov3-tiny.cfg";
    std::string weightFile = "/home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/weights/yolov3-tiny.weights";
    std::string labels = "//home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/data/coco.names";
    std::string imagePath = "/home/dimas/Desktop/OpenCvDnnPythonAndBasicCpp/obj/horses.jpg";

    DarknetUtility utility(cfgFile,weightFile,labels);
    
    /*TEST*/
    /*std::cout<<utility.sayHi() <<std::endl;*/
    /*utility.capturingFromCamera(0,false);*/
    utility.doImageProcessing(imagePath);

    return 0;
}