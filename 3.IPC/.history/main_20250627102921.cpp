#include <iostream>
#include <chrono>
#include <pthread.h>
#include <random>
#include <vector>
#include <unistd.h>
#include <semaphore.h>
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
    sem_t unit_lock;

public:
    Unit(int id,Operative *leader) {
        this->id = id;
        current_phase1_done = 0;
        this->leader = leader;
        sem_init(&unit_lock,0,0);
    }

    int getID() {
        return id;
    }

    int getPhase1Done() {
        return current_phase1_done;
    }

    void incPhase1Done() {
        current_phase1_done++;
    }

    Operative* getLeader() {
        return leader;
    }

    void lock_unit() {
        sem_wait(&unit_lock);
    }

    void unlock_unit() {
        sem_post(&unit_lock);
    }

};

int N,M;
vector <Operative*> operatives;
vector <Unit*> units;
pthread_mutex_t stations_lock[4];
pthread_mutex_t output_lock,intelligence_hub_lock;
pthread_mutex_t rw_mutex,rw_critical;
int rc,operations_completed;
pthread_t staff1,staff2;

void initialize() {
    for (int i=0;i<4;i++){
        pthread_mutex_init(&stations_lock[i],NULL);
    }
    int unit_num = 0;
    for (int i=0;i<N;i++){
        Operative* operative = new Operative(i+1);
        operatives.push_back(operative);
        if(operative->getID()%M == 0){
            unit_num++;
            Unit *unit = new Unit(unit_num,operative);
            units.push_back(unit);
        }
    }
    pthread_mutex_init(&output_lock,NULL);
    pthread_mutex_init(&intelligence_hub_lock,NULL);
    pthread_mutex_init(&rw_mutex,NULL);
    pthread_mutex_init(&rw_critical,NULL);
    start_time = chrono::high_resolution_clock::now();
    rc = 0;
    operations_completed = 0;
}

void* staff_read(void* arg)
{
    int* staff = (int *)arg;
    int current_operation;
    pthread_mutex_lock(&rw_mutex);
    rc++;
    if(rc == 1) pthread_mutex_lock(&rw_critical);
    pthread_mutex_unlock(&rw_mutex);
    current_operation = operations_completed;
    pthread_mutex_lock(&output_lock);
    cout<<"Intelligence Staff "<<*staff<<" began reviewing logbook at time "<<to_string(get_time())<<". Operations completed = "<<current_operation<<endl;
    pthread_mutex_unlock(&output_lock);
    pthread_mutex_lock(&rw_mutex);
    rc--;
    if (rc == 0) pthread_mutex_unlock(&rw_critical);
    pthread_mutex_unlock(&rw_mutex);
    return NULL;
}

void* staff(void* arg)
{
    int* staff = (int *)arg;
    int current_operation;
    while(true) {
        pthread_mutex_lock(&rw_mutex);
        rc++;
        if(rc == 1) pthread_mutex_lock(&rw_critical);
        pthread_mutex_unlock(&rw_mutex);
        current_operation = operations_completed;
        pthread_mutex_lock(&output_lock);
        cout<<"Intelligence Staff "<<*staff<<" began reviewing logbook at time "<<to_string(get_time())<<". Operations completed = "<<current_operation<<endl;
        pthread_mutex_unlock(&output_lock);
        pthread_mutex_lock(&rw_mutex);
        rc--;
        if (rc == 0) pthread_mutex_unlock(&rw_critical);
        pthread_mutex_unlock(&rw_mutex);
        int total_operations = ceil(N/static_cast<float> (M));
        if(current_operation == total_operations) break;
        usleep(2000);
    }
    return NULL;
}

void logbook_entry(Unit* unit)
{
    Operative* leader = unit->getLeader();
    usleep(leader->getPhase2Time() * SLEEP_MULTIPLIER);
    pthread_mutex_lock(&rw_critical);
    pthread_mutex_lock(&output_lock);
    operations_completed++;
    cout<<"Unit "<<unit->getID()<<" has completed intelligence distribution at time "<<to_string(get_time())<<endl;
    pthread_mutex_unlock(&output_lock);
    pthread_mutex_unlock(&rw_critical);
}

void* document_recreation(void *arg)
{
    Operative* operative = (Operative *)arg;
    int station = operative->getID() % 4 + 1;
    int unit_id = ceil(operative->getID()/static_cast<float> (M));
    Unit *unit = units[unit_id - 1];

    pthread_mutex_lock(&output_lock);
    cout<<"Operative "<<operative->getID()<<" has arrived at typewriting station at time "<<to_string(get_time())<<endl;
    pthread_mutex_unlock(&output_lock);

    pthread_mutex_lock(&stations_lock[station-1]);
    usleep(operative->getPhase1Time() * SLEEP_MULTIPLIER);
    pthread_mutex_unlock(&stations_lock[station-1]);

    int a=2;
    pthread_mutex_lock(&output_lock);
    cout<<"Operative "<<operative->getID()<<" has completed document recreation at time "<<to_string(get_time())<<endl;
    pthread_create(&staff2,NULL,staff_read,&a);
    pthread_mutex_unlock(&output_lock);

    unit->incPhase1Done();
    if(unit->getPhase1Done() == M) {
        unit->unlock_unit();
    }
    if(operative->getID() == unit->getLeader()->getID()) {
        unit->lock_unit();
        pthread_mutex_lock(&output_lock);
        cout<<"Unit "<<unit->getID()<<" has completed document recreation phase at time "<<to_string(get_time())<<endl;
        pthread_create(&staff2,NULL,staff_read,&a)
        pthread_mutex_unlock(&output_lock);
        pthread_mutex_lock(&intelligence_hub_lock);
        logbook_entry(unit);
        pthread_mutex_unlock(&intelligence_hub_lock);
    }

    
    return NULL;
}

int main()
{
    cin>>N>>M;
    pthread_t phase1_threads[N];
    initialize ();
    int a = 1;
    pthread_create(&staff1,NULL,staff,&a);
    
    int operative_count = N;
    bool started[N]= {false};

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
    pthread_join(staff1,NULL);
    return 0;
}