#include <iostream>
#include <chrono>
#include <pthread.h>
#include <random>
using namespace std;

#define MAX_PHASE1_TIME 20
#define MAX_PHASE2_TIME 20
#define SLEEP_MULTIPLIER 1000

pthread_mutex_t mutex;
auto start_time = chrono::high_resolution_clock::now();

long long get_time() {
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    long long elapsed_time_ms = duration.count();
    return elapsed_time_ms;
  }

int get_random_number() {
    std::random_device rd;
    std::mt19937 generator(rd());
  
    double lambda = 10000.234;
    std::poisson_distribution<int> poissonDist(lambda);
    return poissonDist(generator);
  }

enum operation_phase { DOCUMENT_RECREATION, LOGBOOK_ENTRY};

class Operative{
    int id;
    int phase1_time;
    int phase2_time;    
    operation_phase phase;

public:
    Operative (int id) {
        this->id = id;
        phase1_time = get_random_number() % MAX_PHASE1_TIME + 1;
        phase2_time = get_random_number() % MAX_PHASE2_TIME + 1;
        phase = DOCUMENT_RECREATION;
    }

};

vector <Operative> operatives;
int typewriting_stations[4];

void initialize() {
    for (int i=0;i<4;i++){

    }
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