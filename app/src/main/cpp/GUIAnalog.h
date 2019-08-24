//
// Created by Beniamin on 2017-05-30.
//

#ifndef CPLUSPLUS_GUIANALOG_H
#define CPLUSPLUS_GUIANALOG_H

#include <math.h>
#include <android/log.h>
#include <string>
#include "Timer.h"


extern "C"{

struct analog_data{
    bool is_active;
    int pointer_id;
    float inside_x;
    float inside_y;
    int current_section;
    int last_section;
    bool hold;
};

struct analog_propereties{
    bool active;
    int id;
    int square_id;
    float x;
    float y;
    float radius;
    float screen_w;
    float screen_h;
};

class GUIAnalog {
private:
    analog_data data;
    analog_propereties propereties;

    void ScreenToRender(float &x, float &y);

public:

    GUIAnalog() { propereties.active = false;}
    void Create(int id, int square_id, float x, float y, float radius, float screen_w, float screen_h);
    bool Check(float x, float y, int pointer_id);
    void Reset();
    void Set(int n_pointer_id, float n_x, float n_y);

    bool IsActive(){ return data.is_active; }

    float RenderX() { return propereties.x; }
    float RenderY() { return propereties.y; }
    float RenderInsideX() { return data.inside_x; }
    float RenderInsideY()  { return data.inside_y; }
    int SquareId() { return propereties.square_id; }
   // int PointerId() { return data.pointer_id; }
    void SetInputState();
    int InputState() { return data.current_section; }
    bool Hold() { return data.hold; }
    int LastState() { return data.last_section; }
};

};


#endif //CPLUSPLUS_GUIANALOG_H
