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
        phase1_time = (get_random_number() % MAX_PHASE1_TIME) + 1;
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

class Unit {
    int id;
    int current_phase1_done;
    Operative *leader;

public:
    Unit(int id,Operative *leader) {
        this->id = id;
        current_phase1_done = 0;
        this->leader = leader;
    }

};

vector <Operative*> operatives;
vector <Unit*> units;
int typewriting_stations[4];
pthread_mutex_t stations_lock[4];
pthread_mutex_t output_lock;
int N,M;

void initialize() {
    for (int i=0;i<4;i++){
        typewriting_stations[i]=i;
        pthread_mutex_init(&stations_lock[i],NULL);
    }
    int unit_num = 0;
    for (int i=0;i<N;i++){
        Operative* operative = new Operative(i+1);
        operatives.push_back(operative);
        if(operative->getID()%M == 0){
            unit_num++;
            Unit *unit = new Unit(unit_num,operative)
        }
    }
    pthread_mutex_init(&output_lock,NULL);
    start_time = chrono::high_resolution_clock::now();
}

void* document_recreation(void *arg)
{
    Operative* operative = (Operative *)arg;
    int station = operative->getID() % 4 + 1;
    pthread_mutex_lock(&output_lock);
    cout<<"Operative "<<operative->getID()<<" has arrived at typewriting station "<<station<<" at time "<<to_string(get_time())<<endl;
    pthread_mutex_unlock(&output_lock);
    pthread_mutex_lock(&stations_lock[station-1]);
    usleep(operative->getPhase1Time() * SLEEP_MULTIPLIER);
    pthread_mutex_unlock(&stations_lock[station-1]);
    pthread_mutex_lock(&output_lock);
    cout<<"Operative "<<operative->getID()<<" has completed document recreation at time "<<to_string(get_time())<<endl;
    pthread_mutex_unlock(&output_lock);
    
    return NULL;
}

int main()
{
    cin>>N;
    pthread_t phase1_threads[N];
    initialize ();
    
    int operative_count = N;
    bool started[N];

    while(operative_count){
        int random_operative = get_random_number() % N;
        if (!started[random_operative]) {
            started[random_operative] = true;
            pthread_create(&phase1_threads[random_operative],NULL, document_recreation, (void *)operatives[random_operative]);
            operative_count--;
            usleep(1000);
        }
    }
    for (int i=0;i<N;i++) {
        pthread_join(phase1_threads[i],NULL);
    }
    return 0;
}