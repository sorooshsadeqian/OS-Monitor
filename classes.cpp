#include "classes.hpp"

Path::Path(vector <Road*> r, int i) {
    id = i;
    for (Road* x : r) {
        roads.push_back(x);
    }
}

Road::Road (string a, string b, int c) {
    entry = a;
    exit = b;
    h = c;
    monitor = new Monitor();
}

Car::Car (int x, Path* pa, int i) {
    p = x;
    path = pa;
    id = i;
}

Emission::Emission() {
    emission = 0;
    sem_init(&lock, 0, 1);
}

Monitor::Monitor() {
    sem_init(&sem, 0, 1);
}

long long int Monitor::move_car (int p, int h, milliseconds &entry, milliseconds &exit) {
    sem_wait(&sem);
    double emission = 0;
    long long int result;
    entry = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (int i = 0 ; i < pow(10,7) ; i++) {
        emission += floor(i/(pow(10,6) * h * p));
    }
    exit = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    sem_post(&sem);
    result = (long long int) emission;
    return result;
}