//
// Created by Beniamin on 2017-05-28.
//

#ifndef CPLUSPLUS_INPUTHANDLER_H
#define CPLUSPLUS_INPUTHANDLER_H

#include <math.h>
#include "Timer.h"

const int LEFT_ANALOG = 0;
const int RIGHT_ANALOG = 1;


extern "C"{


struct input_handler_propereties{
    float screen_w;
    float screen_h;
    //InputPointer pointers[2];
};

struct pointer_info{
    float x;
    float y;
};

struct input_handler_data{
    bool active[2];
    pointer_info pointers[2];
    int current_action[2];
    int player_lis;
};



struct analog_input{
    bool active;
    int id;
};

class InputHandler {

private:

    input_handler_data data;
    input_handler_propereties propereties;
    analog_input analogs[2]; //

    bool reset = true;

    void ScreenToRender(float &x, float &y, float screen_w, float screen_h);

public:

    InputHandler() { data.active[0] = false; data.active[1] = false;}
    void Create(float screen_w, float screen_h){
        propereties.screen_h = screen_h;
        propereties.screen_w = screen_w;
        data.active[0] = false;
        data.active[1] = false;
        analogs[LEFT_ANALOG].active = false;
        analogs[RIGHT_ANALOG].active = false;
            data.player_lis = LIS_NOTHING;
    }
    void ProcessInput(float *n_data, float screen_w, float screen_h);
    void SetPointer(int id, float x, float y);

    bool IsActive(int n_id) {  return data.active[n_id];}
    float PointerX(int n_id) { return data.pointers[n_id].x; }
    float PointerY(int n_id) { return data.pointers[n_id].y; }
    bool Reset() {  if(reset){ reset = false; return  true; } else return false; }
    void CheckAnalog(int side, float screen_w, float screen_h);
    int PlayerLIS() { return data.player_lis; }
    void SetPlayerLIS(int l_left_state, bool l_left_hold, int l_right_state, bool l_right_hold);
};

};

#endif //CPLUSPLUS_INPUTHANDLER_H
