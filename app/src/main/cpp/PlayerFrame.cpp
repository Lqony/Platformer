//
// Created by Beniamin on 2017-08-01.
//

#include "PlayerFrame.h"

void PlayerFrame::Create(int type, int *data, int **input_data) {

    this->data.type = type;
    this->data.id = data[0];
    this->data.animation = data[1];
    this->data.delay = data[2] / 1000.0f;

    if(type == MOVEMENT_FRAME){

        movement_data.movement_state = data[3];
        movement_data.ground_x_vel = data[4] / 1000.0f;
        movement_data.air_x_vel = data[5] / 1000.0f;
        movement_data.y_vel = data[6] / 1000.0f;
    }else if(type == DASH_FRAME){
        dash_data.movement_state = data[3];
        dash_data.velocity = data[4] / 1000.0f;
    }

    //Movment Input Data
    this->data.input.nothing.on_ground = input_data[0][0];
    this->data.input.nothing.in_air = input_data[0][1];

    this->data.input.move_left.on_ground = input_data[1][0];
    this->data.input.move_left.in_air = input_data[1][1];

    this->data.input.move_right.on_ground = input_data[2][0];
    this->data.input.move_right.in_air = input_data[2][1];

    this->data.input.jump.on_ground = input_data[3][0];
    this->data.input.jump.in_air = input_data[3][1];

    this->data.input.jump_left.on_ground = input_data[4][0];
    this->data.input.jump_left.in_air = input_data[4][1];

    this->data.input.jump_right.on_ground = input_data[5][0];
    this->data.input.jump_right.in_air = input_data[5][1];

    this->data.input.fall.on_ground = input_data[6][0];
    this->data.input.fall.in_air = input_data[6][1];

    this->data.input.fal_left.on_ground = input_data[7][0];
    this->data.input.fal_left.in_air = input_data[7][1];

    this->data.input.fall_right.on_ground = input_data[8][0];
    this->data.input.fall_right.in_air = input_data[8][1];

    this->data.input.dash_left.on_ground = input_data[9][0];
    this->data.input.dash_left.in_air = input_data[9][1];

    this->data.input.dash_right.on_ground = input_data[10][0];
    this->data.input.dash_right.in_air = input_data[10][1];

    this->data.input.dash_up.on_ground = input_data[11][0];
    this->data.input.dash_up.in_air = input_data[11][1];

    this->data.input.dash_down.on_ground = input_data[12][0];
    this->data.input.dash_down.in_air = input_data[12][1];
}