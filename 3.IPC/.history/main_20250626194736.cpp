#include <iostream>
#include <pthread.h>
using namespace std;

void print_hello(void *arg)
{
    print("Hello world")
}

int main()
{
    cout<<"Hello World"<<endl;
}