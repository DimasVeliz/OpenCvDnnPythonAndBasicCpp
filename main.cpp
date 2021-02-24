#include <iostream>
#include <DarknetUtility.hpp>

int main()
{
    std::string cfgFile = "/home/dimas/Documents/OpenCvDnnPythonAndBasicCpp/cfg/yolov3-tiny.cfg";
    std::string weightFile = "/home/dimas/Documents/OpenCvDnnPythonAndBasicCpp/weights/yolov3-tiny.weights";
    std::string labels = "/home/dimas/Documents/OpenCvDnnPythonAndBasicCpp/data/coco.names";
    std::string imagePath = "/home/dimas/Documents/OpenCvDnnPythonAndBasicCpp/obj/horses.jpg";
    std::string videoPath = "/home/dimas/Documents/OpenCvDnnPythonAndBasicCpp/obj/family_crossing.mp4";


    std::string cfgFileYV4 = "/home/dimas/Documents/OpenCvDnnPythonAndBasicCpp/cfg/yolov4-tiny.cfg";
    std::string weightFileYV4 = "/home/dimas/Documents/OpenCvDnnPythonAndBasicCpp/weights/yolov4-tiny.weights";
    

    //DarknetUtility utility(cfgFile,weightFile,labels);
    
    /*TEST*/
    /*std::cout<<utility.sayHi() <<std::endl;*/
    /*utility.capturingFromCamera(0,false);*/
    /*utility.doImageProcessing(imagePath);*/
    //utility.capturingFromCamera(0,true);
    /*utility.capturingFromCamera(videoPath,true);*/

    DarknetUtility utility(cfgFileYV4,weightFileYV4,labels);
    //utility.doImageProcessing(imagePath);

   ///utility.capturingFromCamera(videoPath,true);
    utility.capturingFromCamera(0,true);



    return 0;
}