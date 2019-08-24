//
// Created by Beniamin on 2017-06-12.
//

#include "Platform.h"

extern "C" {

}

void Platform::Create(int id, int square_id, int type, float *rect) {

    data.id = id;
    data .square_id = square_id;
    collision.type = type;

    collision.x = rect[0];
    collision.y = rect[1];
    collision.z = rect[2];
    collision.w = rect[3];
    collision.h = rect[4];
}

bool Platform::Check(float* player_last_xyz, float* player_current_xyz, float* player_size) {
    float l_last_x = player_last_xyz[0];
    float l_last_y = player_last_xyz[1];
    float l_last_z = player_last_xyz[2];
    float l_current_x = player_current_xyz[0];
    float l_current_y = player_current_xyz[1];
    float l_current_z = player_current_xyz[2];
    float l_w = player_size[0];
    float l_h = player_size[1];

    switch (collision.type){
        case PLATFORM_VERTICAL:

            //TODO
            break;

        case PLATFORM_HORIZONTAL:

            if(l_last_y >= collision.y && l_current_y <= collision.y){

                if(l_current_x >= collision.x - (collision.w /2)
                   && l_current_x <= collision.x + (collision.w /2) ){
                    /*__android_log_print(ANDROID_LOG_VERBOSE, "LY", to_string(l_last_y).c_str(), 1);
                    __android_log_print(ANDROID_LOG_VERBOSE, "CY", to_string(l_current_y).c_str(), 1);
                    __android_log_print(ANDROID_LOG_VERBOSE, "COLY", to_string(collision.y).c_str(), 1);*/
                    return true;
                }

            }

            break;
        default:
            //TODO

            break;
    }

    return false;
}
