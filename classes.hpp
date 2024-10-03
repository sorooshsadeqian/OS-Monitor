#ifndef __CLASSES__
#define __CLASSES__

#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <semaphore.h>
#include <cmath>

using namespace std;
using namespace std::chrono;

class Monitor {
public:
    Monitor();
    sem_t sem;
    long long int move_car (int p, int h, milliseconds &entry, milliseconds &exit);
};

class Road {
public:
    Road(string a, string b, int c);
    string entry;
    string exit;
    int h;
    Monitor* monitor;
};

class Path {
public:
    int id;
    Path(vector <Road*> r, int i);
    vector <Road*> roads;
};

class Car {
public:
    int id;
    int p;
    Car(int x, Path* pa, int i);
    Path* path;
    vector <long long int> emission;
    vector <long long int> total_emissions;
    vector <milliseconds> entry_times;
    vector <milliseconds> exit_times;
};

class Emission {
public:
    Emission();
    sem_t lock;
    long long int emission;
};

#endif