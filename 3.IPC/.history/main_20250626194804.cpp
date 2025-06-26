#include <iostream>
#include <pthread.h>
using namespace std;

void print_hello(void *arg)
{
    int a = (int *)arg;
    
}

int main()
{
    cout<<"Hello World"<<endl;
}