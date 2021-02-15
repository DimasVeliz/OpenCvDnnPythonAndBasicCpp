#include <iostream>
#include <DarknetUtility.hpp>

int main()
{
    std::string cfgFile = "/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/cfg/yolov3-tiny.cfg";
    std::string weightFile = "/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/weights/yolov3-tiny.weights";
    std::string labels = "/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/data/coco.names";
    std::string imagePath = "/home/dimas/Documents/TensorFlowScripts/OpenCVDnnCppModule/obj/img/horses.jpg";

    DarknetUtility utility(cfgFile,weightFile,labels);
    
    /*TEST*/
    /*std::cout<<utility.sayHi() <<std::endl;*/
    /*utility.capturingFromCamera(0,false);*/

    
    return 0;
}