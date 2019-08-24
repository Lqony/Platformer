//
// Created by Beniamin on 2017-06-06.
//

#ifndef CPLUSPLUS_PLAYER_H
#define CPLUSPLUS_PLAYER_H

#include <android/log.h>
#include <string>
#include "Timer.h"
#include "PlayerFrame.h"


extern "C"{

//GAME RULES
const float WAIT_TILL_NEXT_FRAME = 0.034f;
const int SKIP_FRAME = -2; //THIS THREATS AN INPUT AS NO_INPUT
const int KEEP_FRAME = -3; //THIS IGNORES ANY INPUT AND CONTINUES WITH CURRENT FRAME

//GROUND STATE
const int GS_IN_AIR = 1;
const int GS_ON_PLATFORM = 0;
const int GS_NONE = 2;

//PLAYER STATE
const int PLAYER_STAY = 0;
const int PLAYER_MOVE_LEFT = 1;
const int PLAYER_MOVE_RIGHT = 2;
const int PLAYER_JUMP = 3;
const int PLAYER_JUMP_LEFT = 4;
const int PLAYER_JUMP_RIGHT = 5;
const int PLAYER_FALL = 6;
const int PLAYER_FALL_LEFT = 7;
const int PLAYER_FALL_RIGHT = 8;
const int PLAYER_DASH_LEFT = 9;
const int PLAYER_DASH_RIGHT = 10;
const int PLAYER_DASH_UP = 11;
const int PLAYER_DASH_DOWN = 12;

//MOVE INPUT STATE
const int IS_NOTHING = 0;
const int IS_LEFT = 1;
const int IS_RIGHT = 2;
const int IS_UP = 3;
const int IS_UP_LEFT = 4;
const int IS_UP_RIGHT = 5;
const int IS_DOWN = 6;
const int IS_DOWN_LEFT = 7;
const int IS_DOWN_RIGHT = 8;


const int INPUT_STATE_AMOUNT = 17;

struct player_frame{
    //TODO delete this shit
};
struct player_data{
    int id; //in a table
    float x;
    float y;
    float z;
    float last_x;
    float last_y;
    float last_z;
    int platform_id;
    int suqare_id;
    int air_jump;
    int ground_state;
    int movement_state;
    int logic_input_state;
    int animation;
    Timer* timer;
    bool right;
    bool *input_state;
    bool request_input;
    //collision object TODO new class
};

struct player_forces{
    Timer timer;
    float force_x;
    float force_y;
};

struct player_propereties{
    float movement_velocity;
    float jump_power;
    int air_jump_amount;
    float air_vertical_velocioty;
    float fall_velocity;
};

struct player_frames{
    int amount;
    int current;
    int next;
    float delta;
    float current_delay;
    PlayerFrame *action_frames;
    Timer timer;
};

class Player {

private:
    player_data data;
    player_propereties propereties;
    player_frames frames;
    player_forces forces;

public:

    Player() {}

    void Create(player_data l_player_data, player_propereties l_player_prop);
    void Update();
    void SetFrame();
    void SetNextFrame();
    void PrepareNextFrame(int l_value);
    void Move();
    void Fall(float n_gravity);

    void AddFrames();
    void SetLogicInputState(int l_state) { data.logic_input_state = l_state; }
    void CheckFrames();
    void SetGroundState(int l_platform_id, int l_state, float l_platform_surface);
    void Gravity(float n_gravity_vel, float n_min_gravity);
    void Friction(float n_air, float n_ground);
    int SquareID() { return data.suqare_id; }
    float X() { return data.x; }
    float Y() { return data.y; }
    float Z() { return data.z; }
    float LastX() { return data.last_x; }
    float LastY() { return data.last_y; }
    float LastZ() { return data.last_z; }
    int GroundState() { return data.ground_state; };
    int PlatformId() { return data.platform_id; }
    int AnimationId() { return data.animation; }
    bool RightSide() { return data.right; }
    bool RequestInput() { return data.request_input; }

};


};


#endif //CPLUSPLUS_PLAYER_H
