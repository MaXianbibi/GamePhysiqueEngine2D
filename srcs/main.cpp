#include "Include/Include.hpp"

int main(void)
{  
    Vec2<VAR_TYPE> a(100, 2);

    Vec2 b = a.Normalize();

    std::cout << b.Magnitude() << std::endl;

    std::cout << a.Magnitude() << std::endl;


    return (0);
}