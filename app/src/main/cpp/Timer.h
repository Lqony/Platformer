//
// Created by Beniamin on 2017-05-24.
//

#ifndef CPLUSPLUS_TIMER_H
#define CPLUSPLUS_TIMER_H

#include <ctime>
#include <chrono>
#include <ratio>

using namespace std;
using namespace chrono;

extern "C"{

//PLAYER LOGIC INPUT
const int LIS_NOTHING = 0;
const int LIS_MOVE_LEFT = 1;
const int LIS_MOVE_RIGHT = 2;
const int LIS_JUMP = 3;
const int LIS_FALL = 4;
const int LIS_JUMP_RIGHT = 5;
const int LIS_JUMP_LEFT = 6;
const int LIS_FALL_RIGHT = 7;
const int LIS_FALL_LEFT = 8;
const int LIS_LIGHT_ATTACK = 9;
const int LIS_SPECIAL_ATTACK = 10;
const int LIS_SHIELD = 11;
const int LIS_DASH_LEFT = 12;
const int LIS_DASH_RIGHT = 13;
const int LIS_DASH_UP = 14;
const int LIS_DASH_DOWN = 15;
const int LIS_LIGHT_ATTACK_LEFT = 16;
const int LIS_LIGHT_ATTACK_RIGHT = 17;
const int LIS_LIGHT_ATTACK_UP = 18;
const int LIS_LIGHT_ATTACK_DOWN = 19;


//ANALOG PHYSIC INPUT
const int SECTION_MIDDLE = 0;
const int SECTION_LEFT = 1;
const int SECTION_LEFT_UP = 2;
const int SECTION_UP = 3;
const int SECTION_RIGHT_UP = 4;
const int SECTION_RIGHT = 5;
const int SECTION_RIGHT_DOWN = 6;
const int SECTION_DOWN = 7;
const int SECTION_LEFT_DOWN = 8;

//Little Timer Class
class Timer{

private:

    high_resolution_clock::time_point start;
    high_resolution_clock::time_point stop;
    duration<double> time_span;
    clock_t clock_ticks;
    double delta;
    bool is_running;


public:

    Timer(){ delta = 0; is_running = false; }

    void Start() { start = high_resolution_clock::now(); delta = 0; is_running = true; }

    double Stop()  { stop = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(stop - start);
        delta += time_span.count() ;
        is_running = false;
        return delta; }

    double Pause() { stop = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(stop - start);
        delta += time_span.count() ;
        is_running = false;
        return delta; }

    void unPause(){
        start = high_resolution_clock::now(); is_running = true;
    }

    double Current() { stop = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(stop - start);

        return time_span.count() ;
    }

};

};

#endif //CPLUSPLUS_TIMER_H
