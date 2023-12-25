#include <iostream>

void PrintHello() {
    std::cout << "Hello World!\n";
}

int Add(int i1, int i2) {
    return i1 + i2;
}

void PrintInteger(int i) {
    std::cout << i << "\n";
}

int main()
{
    PrintHello();
    int i = Add(10, 5);
    PrintInteger(i);
}
