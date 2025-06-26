#include <iostream>
#include <chrono>
#include <pthread.h>
#include <random>
#include <vector>
#include <unistd.h>
using namespace std;

#define MAX_PHASE1_TIME 20
#define MAX_PHASE2_TIME 20
#define SLEEP_MULTIPLIER 1000

pthread_mutex_t phase1_mutex;
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

    int getID() {
        return id;
    }

    int getPhase1Time() {
        return phase1_time;
    }

    int getPhase2Time() {
        return phase2_time;
    }

    operation_phase getPhase() {
        return phase;
    }

};

vector <Operative*> operatives;
int typewriting_stations[4];
int N;
int M;

void initialize() {
    for (int i=0;i<4;i++){
        typewriting_stations[i]=i;
    }
    for (int i=0;i<N;i++){
        operatives.push_back(new Operative(i+1));
    }
    pthread_mutex_init(&phase1_mutex,NULL);
    start_time = chrono::high_resolution_clock::now();
}

void* document_recreation(void *arg)
{
    Operative* operative = (Operative *)arg;
    pthread_mutex_lock(&phase1_mutex);
    cout<<"Operative "<<operative->getID()<<" has arrived at typewriting station at time "<<to_string(get_time());
    pthread_mutex_unlock(&phase1_mutex);
    usleep(operative->getPhase1Time()*SLEEP_MULTIPLIER);
    pthread_mutex_lock(&phase1_mutex);
    cout<<"Operative "<<operative->getID()<<" has completed document recreation at time "<<to_string(get_time());
    pthread_mutex_unlock(&phase1_mutex);
    return NULL;
}

int main()
{
    pthread_t t1,t2;
    int a=1,b=2;
    pthread_mutex_init(&phase1_mutex,NULL);
    pthread_create(&t1,NULL,document_recreation,(void*)&a);
    pthread_create(&t2,NULL,print_hello,(void*)&b);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}