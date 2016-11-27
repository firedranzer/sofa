#include <iostream>
#include <cstdio>
#include <vector>
#include "test1.h"


using namespace sofa ;

int my_printf(){
    printf("La bonne blague...") ;
    return 0 ;
}



int main()
{
    // Invalid declaration, we should always initialize
    MyClass* cl2INVALID = new MyClass() ;
}
