#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include "classes.hpp"

#define DATA_PATH "./car_data/"
#define INPUT_FILENAME "input.txt"

using namespace std;

void start (int car_id);
void get_inputs ();

vector <Road*> roads;
vector <Path*> paths;
vector <Car*> cars;
Emission* total_emission = new Emission();
int car_id;
int path_id;


int main () {

    system("rm -f ./car_data/*");

    get_inputs();

    thread threads[cars.size()];
    for (int i = 0 ; i < cars.size() ; i++) {
        threads[i] = thread(start, i);
    }
    for (int i = 0 ; i < cars.size() ; i++) {
        threads[i].join();
    }

}

void start (int car_id) {
    int roads_crossed = 0;
    Car* car = cars[car_id];
    while (roads_crossed != car->path->roads.size()) {
        Road* road = car->path->roads[roads_crossed];
        milliseconds entry, exit;
        long long int emission = road->monitor->move_car(car->p, road->h, entry, exit);
        car->entry_times.push_back(entry);
        car->exit_times.push_back(exit);
        car->emission.push_back(emission);
        sem_wait(&(total_emission->lock));
        total_emission->emission += emission;
        car->total_emissions.push_back(total_emission->emission);
        sem_post(&total_emission->lock);
        roads_crossed++;
    } 
    string filename = DATA_PATH + to_string(car->path->id) + "-" + to_string(car->id); 
    fstream out_file;
    out_file.open(filename, fstream::out);
    for (int i = 0 ; i < car->path->roads.size() ; i++) {
        string temp = car->path->roads[i]->entry + ", " + to_string(car->entry_times[i].count()) + ", " + car->path->roads[i]->exit + ", " + to_string(car->exit_times[i].count()) + ", " + to_string(car->emission[i]) + ", " + to_string(car->total_emissions[i]);
        out_file << temp << endl;
    }
    out_file.close();
}

void get_inputs () {

    fstream file;
    string temp;
    file.open(INPUT_FILENAME);

    while (getline(file, temp) && temp != "#") {
        vector <string> parsed_temp;
        stringstream ss(temp);
        string token;
        char delim = '-';
        while (getline(ss, token, delim)) {
            stringstream t (token);
            string trimmed_token;
            t >> trimmed_token;
            parsed_temp.push_back(trimmed_token);
        }
        Road* new_road = new Road(parsed_temp[0], parsed_temp[1], stoi(parsed_temp[2]));
        roads.push_back(new_road);
    }
    int pnum;
    while (getline(file, temp)) {
        vector <string> parsed_temp;
        stringstream ss(temp);
        string token;
        char delim = '-';
        while (getline(ss, token, delim)) {
            stringstream t (token);
            string trimmed_token;
            t >> trimmed_token;
            parsed_temp.push_back(trimmed_token);
        }
        getline(file, temp);
        int count = stoi(temp);
        vector <Road*> path_roads;
        for (int i = 1; i < parsed_temp.size() ; i++) {
            int j = i - 1;
            for (Road* r : roads) {
                if (r->entry == parsed_temp[j] && r->exit == parsed_temp[i]) {
                    path_roads.push_back(r);  
                    break;
                }
            }
        }
        Path* new_path = new Path(path_roads, path_id++);
        random_device rd;  
        mt19937 gen(rd()); 
        uniform_int_distribution<> dis(1, 10);
        for (int  i = 0 ; i < count ; i++) {
            Car* new_car = new Car(dis(gen), new_path, car_id++);
            cars.push_back(new_car);
        }
        paths.push_back(new_path);
    }
}
