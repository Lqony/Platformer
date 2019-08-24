//
// Created by Beniamin on 2017-05-17.
//

#ifndef CPLUSPLUS_ANIMATION_H
#define CPLUSPLUS_ANIMATION_H


#include <string>
#include "Timer.h"
#include "InputHandler.h"

using namespace std;

extern "C" {

class Timer;


const int START = 0;
const int STOP = 1;
const int PAUSE = 2;
const int UNPAUSE = 3;


struct aniamtion_data{
     int id; //in a table
     int current_frame;
     float delta;
     bool visible;
     int x_roll;
     float uvs[8];
};

struct animation_properties{
    //LOAD
    float first_frame_x;
    float first_frame_y;
    float source_frame_w;
    float source_frame_h;
    int roll;
    //RENDER
    float dest_frame_w;
    float dest_frame_h;
    float change_x;
    float change_y;
    //Prop
    int frame_amount;
    int repeat_from;
    float delay_time;
};

class Animation {

private:
    aniamtion_data data;
    animation_properties properties;
    Timer timer;

public:
      Animation(int id) { data.id = id;  }
      void Create(float* n_data);
    void Start();
    void Stop();
    void Pause();
    void UnPause();
    void Update();
    float* FrameRect();
    float GetFrameRect(int id){ return data.uvs[id];}

    bool Visible() { return data.visible; }

    float DestFrameW() { return properties.dest_frame_w; }
    float DestFrameH() { return properties.dest_frame_h; }
    float ChangeX() { return properties.change_x; }
    float ChangeY() { return properties.change_y; }
};

};

#endif //CPLUSPLUS_ANIMATION_H
