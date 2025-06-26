#include <iostream>
#include <pthread.h>
using namespace std;

void print_hello(void *arg)
{
    print("Hello world for thread %d")
}

int main()
{
    cout<<"Hello World"<<endl;
}