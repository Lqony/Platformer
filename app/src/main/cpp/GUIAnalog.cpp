//
// Created by Beniamin on 2017-05-30.
//

#include "GUIAnalog.h"

using namespace std;

extern "C"{

void GUIAnalog::Create(int id, int square_id, float x, float y, float radius, float screen_w, float screen_h){
    propereties.active = true;
    propereties.id = id;
    propereties.square_id = square_id; //Outside is squareid, inside is squareid +1
    propereties.x = x;
    propereties.y = y;
    propereties.radius = radius;
    propereties.screen_w = screen_w;
    propereties.screen_h = screen_h;
    data.inside_x = x;
    data.inside_y = y;
    data.is_active = false;
    data.current_section = SECTION_MIDDLE;
    data.hold = false;
    data.last_section = SECTION_MIDDLE;
}

void GUIAnalog::Reset() {
    data.inside_x = propereties.x;
    data.inside_y = propereties.y;
    data.is_active = false;
    data.hold = false;
    data.current_section = SECTION_MIDDLE;
    data.last_section = SECTION_MIDDLE;
}

void GUIAnalog::Set(int n_pointer_id, float n_x, float n_y){
    data.is_active = true;
    data.pointer_id = n_pointer_id;
    data.inside_x = n_x;
    data.inside_y = n_y;
}

bool GUIAnalog::Check(float x, float y, int pointer_id) {
    if( abs( sqrt( pow( propereties.x - x, 2) + pow(propereties.y - y, 2)  ) )
          <= propereties.radius ){
        data.is_active = true;
        data.pointer_id = pointer_id;
        data.inside_x = x;
        data.inside_y = y;
        return true;
    }
    else{

        /*data.is_active = false;
        data.inside_x = propereties.x;
        data.inside_y = propereties.y;*/
        return false;
    }

    //return data.is_active;
}

void GUIAnalog::SetInputState() {
        bool l_left = false, l_right = false, l_up = false, l_down = false;

        if(!data.is_active){
            data.current_section = SECTION_MIDDLE;
            data.last_section = SECTION_MIDDLE;
            data.hold = false;
        }
        else {
            data.last_section = data.current_section;

            if(data.inside_x > propereties.x + (propereties.radius / 3.0f )){
                if(data.inside_x )
                    l_right = true;
            }else if(data.inside_x < propereties.x - (propereties.radius / 3.0f)){
                l_left = true;
            }

            if(data.inside_y > propereties.y + (propereties.radius / 3.0f)){
                l_up = true;
            }else if(data.inside_y < propereties.y - (propereties.radius / 3.0f)){
                l_down = true;
            }

            if(l_left == true){
                if(l_up) {
                    //__android_log_print(ANDROID_LOG_VERBOSE, "ANALOG", "UP", 1);
                    if(data.last_section == SECTION_LEFT_UP){
                        data.hold = true;
                    }else {
                        data.hold = false;
                    }
                    data.current_section = SECTION_LEFT_UP;
                }
                else if(l_down){
                    if(data.last_section == SECTION_LEFT_DOWN){
                        data.hold = true;
                    }else {
                        data.hold = false;
                    }
                    data.current_section = SECTION_LEFT_DOWN;
                }
                else {
                    if(data.last_section == SECTION_LEFT){
                        data.hold = true;
                    }else {
                        data.hold = false;
                    }
                    data.current_section = SECTION_LEFT;
                }

            }else if(l_right == true){
                if(l_up){
                    if(data.last_section == SECTION_RIGHT_UP){
                        data.hold = true;
                    }else {
                        data.hold = false;
                    }
                    data.current_section = SECTION_RIGHT_UP;
                }
                else if(l_down) {
                    if(data.last_section == SECTION_RIGHT_DOWN){
                        data.hold = true;
                    }else {
                        data.hold = false;
                    }
                    data.current_section = SECTION_RIGHT_DOWN;
                }
                else {
                    if(data.last_section == SECTION_RIGHT){
                        data.hold = true;
                    }else {
                        data.hold = false;
                    }
                    data.current_section = SECTION_RIGHT;
                }
            }else if(l_up){
                if(data.last_section == SECTION_UP){
                    data.hold = true;
                }else {
                    data.hold = false;
                }
                data.current_section = SECTION_UP;
            }
            else  if(l_down){
                if(data.last_section == SECTION_DOWN){
                    data.hold = true;
                }else {
                    data.hold = false;
                }
                data.current_section = SECTION_DOWN;
            }
            else {
                data.current_section = SECTION_MIDDLE;
            }
        }
}

void GUIAnalog::ScreenToRender(float &x, float &y){

    float sourceTopLeftX = 0.0f;
    float sourceTopLeftY = 0.0f;
    float sourceBottomRightX = propereties.screen_w;
    float sourceBottomRightY = propereties.screen_h;

    float targetTopLeftX = -(float)(propereties.screen_w/propereties.screen_h);
    float targetTopLeftY = 1;
    float targetBottomRightX = (float)(propereties.screen_w/propereties.screen_h);
    float targetBottomRightY = -1;

    x = targetTopLeftX +
        ((x - sourceTopLeftX) / (sourceBottomRightX-sourceTopLeftX))*(targetBottomRightX-targetTopLeftX);
    y = targetTopLeftY +
        ((y - sourceTopLeftY) / (sourceBottomRightY-sourceTopLeftY))*(targetBottomRightY-targetTopLeftY);
}

}