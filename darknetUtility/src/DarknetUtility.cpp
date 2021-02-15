#include "DarknetUtility.hpp"

DarknetUtility::DarknetUtility(/* args */)
{

    std::vector<std::string> classes = loadClasses();
    std::vector<cv::Scalar> colors = generateRandomColors();
}

DarknetUtility::~DarknetUtility()
{
}


std::vector<std::string> DarknetUtility::loadClasses(){
    std::vector<std::string> classes;
    std::ifstream file("Hola.txt");
    std::string str;
    while (std::getline(file, str))
    {
        classes.push_back(str);
    }

    return classes;
}

std::vector<cv::Scalar> DarknetUtility::generateRandomColors(){

    int amount =80;
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

    return colors;
}

std::string DarknetUtility::sayHi(){
    return "Hola";
}