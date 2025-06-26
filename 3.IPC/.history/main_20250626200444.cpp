#include <iostream>
#include <pthread.h>
using namespace std;

pthread_mutex_t mutex;

void* print_hello(void *arg)
{
    int* a;
    a = (int *)arg;
    mutex
    cout<<"Hello world for thread "<<*a<<endl;
    return NULL;
}

int main()
{
    pthread_t t1,t2;
    int a=1,b=2;
    pthread_create(&t1,NULL,print_hello,(void*)&a);
    pthread_create(&t2,NULL,print_hello,(void*)&b);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}