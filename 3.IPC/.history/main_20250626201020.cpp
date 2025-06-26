#include <iostream>
#include <pthread.h>
using namespace std;

pthread_mutex_t mutex;

class Operative{
    
}

void* document_recreation(void *arg)
{
    int* a;
    a = (int *)arg;
    pthread_mutex_lock(&mutex);
    cout<<"Hello world for thread "<<*a<<endl;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t t1,t2;
    int a=1,b=2;
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&t1,NULL,print_hello,(void*)&a);
    pthread_create(&t2,NULL,print_hello,(void*)&b);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}