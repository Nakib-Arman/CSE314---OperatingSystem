#include <iostream>
#include <pthread.h>
using namespace std;

void* print_hello(void *arg)
{
    int* a;
    a = (int *)arg;
    cout<<"Hello world for thread "<<*a<<endl;
    return 
}

int main()
{
    pthread_t t1,t2;
    int a=1,b=2;
    pthread_create(&t1,NULL,print_hello,(void*)&a);
    pthread_create(&t2,NULL,print_hello,(void*)&b);
    return 0;
}