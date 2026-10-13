#include <iostream>
 
using namespace std; // std::cout std::cin std::endl - \n
 
void increment1(int x) {
    x++;
}
 
void increment2(int* x) {
    (*x)++;
}
 
void increment3(int &x) {
    x++;
}
 
int* createValue() {
    int value = 13; // local to the function's stack frame
    return &value; // returns an address
    // -> the result is that the address we receive
    // points to memory that has been deallocated
    // when the function ended and value was out of scope
    // !!! dangling pointer
}
 
int main()
{
    // on stack, main's function stack frame
    int x = 10; // int = 4B -> 0A 00 00 00
    int* px1 = &x;
    // in heap -> variables created in heap exist until we call delete
    // they are in our ownership to manage memory wise
    int* px2 = new int;
    px2 = &x;
 
    // the two primary issues when we mismanage memory
    // 1. memory leak
    // it's memory space that WE allocate and then we never deallocate
    // this usually happens when we LOSE the reference to the allocated memory
    //int* px3;
    //for (int i = 0; i < 100; i++) {
    //    px3 = new int; // allocate 4B
    //    // process px3
    //    delete px3; // deallocate 4B
    //}
 
    // 2.dangling pointer
    // it's a reference to deallocated memory space
    //int* a1;
    //int* a2;
    //a1 = new int;
    //*a1 = 10; // where a1 points to in memory -> put value 10
    //a2 = a1; // a2 points to the same address as a1
    //delete a1;
    // -> now a2 is a dangling pointer
    // what we should have done to have the same values:
    // a2 = new int;
    // *a2 = *a1; // value where a2 points to becomes values where a1 points to
 
    // ---
    int b = 1;
    std::cout << "pre-increment b = " << b << std::endl;
    increment1(b);
    std::cout << "post-increment b = " << b << std::endl;
 
    std::cout << "pre-increment b = " << b << std::endl;
    increment2(&b);
    std::cout << "post-increment b = " << b << std::endl;
 
    std::cout << "pre-increment b = " << b << std::endl;
    increment3(b);
    std::cout << "post-increment b = " << b << std::endl;
 
    return 0;
}
 
