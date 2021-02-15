#include <iostream>
#include <DarknetUtility.hpp>

int main()
{
    DarknetUtility utility;
    std::string resp= utility.sayHi();
    std::cout<<resp <<std::endl;
    return 0;
}