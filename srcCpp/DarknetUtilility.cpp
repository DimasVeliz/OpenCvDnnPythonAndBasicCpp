#include <DarknetUtilility.hpp>

DarknetUtilility::DarknetUtilility(/* args */)
{

    std::vector<std::string> classes = loadClasses();
    std::vector<cv::Scalar> colors = generateRandomColors();
}

DarknetUtilility::~DarknetUtilility()
{
}


std::vector<std::string> DarknetUtilility::loadClasses(){

}

std::vector<cv::Scalar> DarknetUtilility::generateRandomColors(){
    
}