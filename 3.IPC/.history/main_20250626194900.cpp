#include <iostream>
#include <pthread.h>
using namespace std;

void* print_hello(void *arg)
{
    int a = (int)arg;
    cout<<"Hello world for thread "<<a<<endl;
}

int main()
{
    cout<<"Hello World"<<endl;
}