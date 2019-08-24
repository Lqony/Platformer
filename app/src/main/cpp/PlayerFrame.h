//
// Created by Beniamin on 2017-08-01.
//

#ifndef CPLUSPLUS_PLAYERFRAME_H
#define CPLUSPLUS_PLAYERFRAME_H

extern "C"{

static int MOVEMENT_FRAME = 0;
static int DASH_FRAME = 1;


struct movement_frame_data{
    int movement_state;
    float ground_x_vel;
    float air_x_vel;
    float y_vel;
};

struct dash_frame_data{
    int movement_state;
    float velocity;
};

struct frame_input_types{
    int in_air;
    int on_ground;
};

struct action_frame_input{
    frame_input_types nothing;
    frame_input_types move_left;
    frame_input_types move_right;
    frame_input_types jump;
    frame_input_types fall;
    frame_input_types jump_left;
    frame_input_types jump_right;
    frame_input_types fal_left;
    frame_input_types fall_right;
    frame_input_types light_attack;
    frame_input_types special_attack;
    frame_input_types shield;
    frame_input_types dash_left;
    frame_input_types dash_right;
    frame_input_types dash_up;
    frame_input_types dash_down;
    frame_input_types light_attack_left;
    frame_input_types light_attack_right;
    frame_input_types light_attack_up;
    frame_input_types light_attack_down;
};

struct frame_data{
    int id;
    int type;
    int animation;
    float delay;
    action_frame_input input;
};


class PlayerFrame {

private:

    frame_data data;
    movement_frame_data movement_data;
    dash_frame_data dash_data;

public:

    void Create(int type, int *data, int **input_data);

    frame_data Data() { return data; }
    movement_frame_data MovementData() { return movement_data; }
    dash_frame_data DashData() { return dash_data; }

};


};


#endif //CPLUSPLUS_PLAYERFRAME_H
