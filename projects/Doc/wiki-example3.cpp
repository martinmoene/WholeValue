#include "whole_value.h"
#include <iomanip>
#include <iostream>

//WV_DEFINE_QUANTITY_TYPE(Quantity, double)
WV_DEFINE_ARITHMETIC_TYPE(Quantity, double)
WV_DEFINE_TYPE(Acceleration, Quantity)
WV_DEFINE_TYPE(Speed, Quantity)

//WV_ADD_STREAM_INSERTION_OPERATOR(Quantity)
// or
WV_ADD_STREAM_INSERTION_OPERATOR_IOMANIP(Quantity, std::fixed << std::setprecision(2) )

int main()
{
    Acceleration acc(9.8);
    Speed spd(330);
    std::cout << "acc:" << acc << ", spd:" << spd << std::endl;
}

// g++ -Wall -I../../include/ -I%BOOST_INCLUDE% -o wiki-example3 wiki-example3.cpp && wiki-example3
// cl -nologo -W3 -EHsc -I../../include/ -I%BOOST_INCLUDE% wiki-example3.cpp && wiki-example3
