
#include "functions.h"
#include <stdio.h>

int add5(int x)
{
    x += 5;
    return x;
}

void testC()
{
    int x = 2;
    add5(x);
}

int main()
{

    puts("Example of calling asm from C");

    printf("func1(): %d\n", func1());
    func2();
    testC(); // just a test how gcc uses frame pointer
    printf("func3(): %d\n", func3(10));
    printf("func3(): %d\n", func4(10, 7));

    return 0;
}
