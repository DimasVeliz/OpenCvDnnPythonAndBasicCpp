#include <iostream>
#include <DarknetUtility.hpp>

int main()
{
    std::string cfgFile = "/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/cfg/yolov3-tiny.cfg";
    std::string weightFile = "/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/weights/yolov3-tiny.weights";
    std::string labels = "/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/data/coco.names";
    std::string imagePath = "/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/obj/img/horses.jpg";

    DarknetUtility utility(cfgFile,weightFile,labels);
    std::string resp= utility.sayHi();
    std::cout<<resp <<std::endl;
    return 0;
}