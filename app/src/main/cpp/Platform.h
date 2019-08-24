//
// Created by Beniamin on 2017-06-12.
//

#ifndef CPLUSPLUS_PLATFORM_H
#define CPLUSPLUS_PLATFORM_H

#include <android/log.h>
#include <string>
#include "Timer.h"

extern "C" {

const int PLATFORM_VERTICAL = 0;
const int PLATFORM_HORIZONTAL = 1;

struct platform_collision{
    float x;
    float y;
    float w;
    float h;
    float z;
    int type;
};

struct platform_data{
    int id;
    int square_id;
};

class Platform {

private:
    platform_data data;
    platform_collision collision;

public:

    Platform() {}
    void Create(int id, int square_id, int type, float* rect);
    bool Check(float* player_last_xyz, float* player_current_xyz, float* player_size); /*Return true if players stands on it*/

    float Y() { return collision.y; }
};

};



#endif //CPLUSPLUS_PLATFORM_H
