//
// Created by Beniamin on 2017-05-28.
//

#include <string>
#include "InputHandler.h"
#include <android/log.h>

using namespace std;

extern "C"{

const int ACTION_DOWN = 0;
const int ACTION_UP = 1;
const int ACTION_MOVE = 2;
const int ACTION_POINTER_DOWN = 5;
const int ACTION_POINTER_UP = 6;



void InputHandler::ProcessInput(float *n_data, float screen_w, float screen_h){
  /*  __android_log_print(ANDROID_LOG_VERBOSE, "X1", to_string(n_data[1]).c_str(), 1);
    __android_log_print(ANDROID_LOG_VERBOSE, "Y1", to_string(n_data[2]).c_str(), 1);
    __android_log_print(ANDROID_LOG_VERBOSE, "X2", to_string(n_data[3]).c_str(), 1);
    __android_log_print(ANDROID_LOG_VERBOSE, "Y2", to_string(n_data[4]).c_str(), 1);*/
    switch((int)n_data[0]){
        case ACTION_DOWN:
            //__android_log_print(ANDROID_LOG_VERBOSE, "DELTA", "ACTION DOWN", 1);
            data.active[0] = true;
            data.active[1] = false;
            data.pointers[0].x = n_data[1];
            data.pointers[0].y = n_data[2];
            reset = true;
            break;

        case ACTION_UP:
          //  __android_log_print(ANDROID_LOG_VERBOSE, "DELTA", "ACTION UP", 1);
            data.active[0] = false;
            data.active[1] = false;
            break;

        case ACTION_MOVE:

           if(data.active[0]){
               if(data.pointers[0].x != n_data[1]){
                   data.pointers[0].x = n_data[1];
               }
               if(data.pointers[0].y != n_data[2]){
                   data.pointers[0].y = n_data[2];
               }
           }

            if(data.active[1]){
                if(data.pointers[1].x != n_data[3]){
                    data.pointers[1].x = n_data[3];
                }
                if(data.pointers[1].y != n_data[4]){
                    data.pointers[1].y = n_data[4];
                }
            }

            break;

        case ACTION_POINTER_DOWN:
           // __android_log_print(ANDROID_LOG_VERBOSE, "DELTA", "ACTION POINTER DOWN", 1);
            if(!data.active[1]){
                data.active[0] = true;
                data.active[1] = true;
                data.pointers[0].x = n_data[1];
                data.pointers[0].y = n_data[2];
                data.pointers[1].x = n_data[3];
                data.pointers[1].y = n_data[4];
            }
            break;

        case ACTION_POINTER_UP:
           // __android_log_print(ANDROID_LOG_VERBOSE, "DELTA", "ACTION POINTER UP", 1);
            data.active[0] = true;
            data.active[1] = false;
            data.pointers[0].x = n_data[1];
            data.pointers[0].y = n_data[2];
            break;
    }

    //CHECK FOR FRONT GUI
   // CheckAnalog(LEFT_ANALOG, screen_w, screen_h);
};

void InputHandler::ScreenToRender(float &x, float &y, float screen_w, float screen_h){

    float sourceTopLeftX = 0.0f;
    float sourceTopLeftY = 0.0f;
    float sourceBottomRightX = screen_w;
    float sourceBottomRightY = screen_h;

    float targetTopLeftX = -(float)(screen_w/screen_h);
    float targetTopLeftY = 1;
    float targetBottomRightX = (float)(screen_w/screen_h);
    float targetBottomRightY = -1;

    x = targetTopLeftX +
        ((x - sourceTopLeftX) / (sourceBottomRightX-sourceTopLeftX))*(targetBottomRightX-targetTopLeftX);
    y = targetTopLeftY +
        ((y - sourceTopLeftY) / (sourceBottomRightY-sourceTopLeftY))*(targetBottomRightY-targetTopLeftY);
}

void InputHandler::CheckAnalog(int side, float screen_w, float screen_h){
    float render_x;
    float render_y;
    int id;
    switch (side){
        case LEFT_ANALOG:
            if(analogs[LEFT_ANALOG].active){
                id = analogs[LEFT_ANALOG].id;
            }else id = 0;

            if(data.active[id]){
                render_x = data.pointers[id].x;
                render_y = data.pointers[id].y;
                ScreenToRender(render_x, render_y, screen_w, screen_h);
                if( abs( sqrt( pow(  (render_x - (-1.25f)) , 2) + pow(render_y - ( -0.5f), 2)  ) ) <= 0.5f) {
                    analogs[LEFT_ANALOG].active = true;
                    analogs[LEFT_ANALOG].id = id;
                    //__android_log_print(ANDROID_LOG_VERBOSE, "DELTA", "Mamy Kontakt ID:", 1);
                  //  __android_log_print(ANDROID_LOG_VERBOSE, "DELTA", to_string(id).c_str(), 1);
                    break;
                }
            }
            id == 0 ? id = 1 : id = 0;
            if(data.active[id]){
                render_x = data.pointers[id].x;
                render_y = data.pointers[id].y;
                ScreenToRender(render_x, render_y, screen_w, screen_h);

                if( abs( sqrt( pow(  (render_x - (-1.25f)) , 2) + pow(render_y - ( -0.5f), 2)  ) ) <= 0.5f){
                    analogs[LEFT_ANALOG].active = true;
                    analogs[LEFT_ANALOG].id = id;
                //    __android_log_print(ANDROID_LOG_VERBOSE, "DELTA", "Mamy Kontakt ID:", 1);
              //      __android_log_print(ANDROID_LOG_VERBOSE, "DELTA", to_string(id).c_str(), 1);
                    break;
                }
            }
            analogs[LEFT_ANALOG].active = false;
            //__android_log_print(ANDROID_LOG_VERBOSE, "DELTA", "BRAK KONTAKTU", 1);
            break;

        case RIGHT_ANALOG:
            break;
    }
}

void InputHandler::SetPlayerLIS(int l_left_state, bool l_left_hold, int l_right_state,
                                bool l_right_hold) {


    if(l_left_state == SECTION_MIDDLE && l_right_state == SECTION_MIDDLE){
        data.player_lis = LIS_NOTHING;
    }else
    {
        if(l_left_state != SECTION_MIDDLE && l_right_state == SECTION_MIDDLE) {

            if (l_left_state == SECTION_LEFT) {
                data.player_lis = LIS_MOVE_LEFT;
            } else if (l_left_state == SECTION_RIGHT) {
                data.player_lis = LIS_MOVE_RIGHT;
            } else if (l_left_state == SECTION_UP) {

                if (!l_left_hold) {
                    data.player_lis = LIS_JUMP;
                }else{
                    data.player_lis = LIS_NOTHING;
                }
            } else if (l_left_state == SECTION_DOWN) {
                data.player_lis = LIS_FALL;
            } else if (l_left_state == SECTION_LEFT_UP) {
                if (!l_left_hold) data.player_lis = LIS_JUMP_LEFT;
                else data.player_lis = LIS_MOVE_LEFT;
            } else if (l_left_state == SECTION_RIGHT_UP) {
                if (!l_left_hold) data.player_lis = LIS_JUMP_RIGHT;
                else data.player_lis = LIS_MOVE_RIGHT;
            } else if (l_left_state == SECTION_LEFT_DOWN) {
                data.player_lis = LIS_FALL_LEFT;
            } else if (l_left_state == SECTION_RIGHT_DOWN) {
                data.player_lis = LIS_FALL_RIGHT;
            }

        }else if(l_left_state == SECTION_MIDDLE && l_right_state != SECTION_MIDDLE){
            if (l_right_state == SECTION_LEFT ) {
                if(!l_right_hold)
                data.player_lis = LIS_LIGHT_ATTACK;
                else
                    data.player_lis = LIS_NOTHING;
            } else if (l_right_state == SECTION_RIGHT ) {
                data.player_lis = LIS_SHIELD;
            } else if (l_right_state == SECTION_UP ) {
                if(!l_right_hold)
                 data.player_lis = LIS_SPECIAL_ATTACK;
                else
                data.player_lis = LIS_NOTHING;
            } else if (l_right_state == SECTION_DOWN) {
                data.player_lis = LIS_NOTHING;
            }
        }else if(l_left_state != SECTION_MIDDLE && l_right_state != SECTION_MIDDLE){
            if(l_left_hold && !l_right_hold){
                if (l_right_state == SECTION_LEFT ) {
                    data.player_lis = LIS_LIGHT_ATTACK;
                } else if (l_right_state == SECTION_RIGHT  ) {
                    if(l_left_state == SECTION_LEFT || l_left_state == SECTION_LEFT_UP || l_left_state == SECTION_LEFT_DOWN){
                        if(!l_left_hold || !l_right_hold)
                        data.player_lis = LIS_DASH_LEFT;
                        else
                            data.player_lis = LIS_SHIELD;
                    }else  if(l_left_state == SECTION_RIGHT || l_left_state == SECTION_RIGHT_UP || l_left_state == SECTION_RIGHT_DOWN){
                        if(!l_left_hold || !l_right_hold)
                        data.player_lis = LIS_DASH_RIGHT;
                        else
                            data.player_lis = LIS_SHIELD;
                    }else  if(l_left_state == SECTION_UP){
                        if(!l_left_hold || !l_right_hold)
                        data.player_lis = LIS_DASH_UP;
                        else
                            data.player_lis = LIS_SHIELD;
                    }else  if(l_left_state == SECTION_DOWN){
                        if(!l_left_hold || !l_right_hold)
                        data.player_lis = LIS_DASH_DOWN;
                        else
                            data.player_lis = LIS_SHIELD;
                    }
                } else if (l_right_state == SECTION_UP ) {
                    data.player_lis = LIS_SPECIAL_ATTACK;
                }
            }else if(!l_left_hold && l_right_hold){

                if (l_right_state == SECTION_LEFT ) {

                    if(l_left_state == SECTION_LEFT || l_left_state == SECTION_LEFT_UP || l_left_state == SECTION_LEFT_DOWN){
                        data.player_lis = LIS_LIGHT_ATTACK_LEFT;
                    }
                    else if(l_left_state == SECTION_RIGHT || l_left_state == SECTION_RIGHT_UP||  l_left_state == SECTION_RIGHT_DOWN){
                        data.player_lis = LIS_LIGHT_ATTACK_RIGHT;
                    }
                    else if(l_left_state == SECTION_UP){
                    data.player_lis = LIS_LIGHT_ATTACK_UP;
                    }
                    else if(l_left_state == SECTION_DOWN){
                        data.player_lis = LIS_LIGHT_ATTACK_DOWN;
                    }
                }
                else if (l_right_state == SECTION_RIGHT ) {
                    if(l_left_state == SECTION_LEFT || l_left_state == SECTION_LEFT_UP || l_left_state == SECTION_LEFT_DOWN){
                        if(!l_left_hold || !l_right_hold)
                        data.player_lis = LIS_DASH_LEFT;
                        else
                            data.player_lis = LIS_SHIELD;
                    }else  if(l_left_state == SECTION_RIGHT || l_left_state == SECTION_RIGHT_UP || l_left_state == SECTION_RIGHT_DOWN){
                        if(!l_left_hold || !l_right_hold)
                        data.player_lis = LIS_DASH_RIGHT;
                        else
                        data.player_lis = LIS_SHIELD;
                    }else  if(l_left_state == SECTION_UP){
                        if(!l_left_hold || !l_right_hold)
                        data.player_lis = LIS_DASH_UP;
                        else
                        data.player_lis = LIS_SHIELD;
                    }else  if(l_left_state == SECTION_DOWN){
                        if(!l_left_hold || !l_right_hold)
                        data.player_lis = LIS_DASH_DOWN;
                        else
                        data.player_lis = LIS_SHIELD;
                    }
                } else if (l_right_state == SECTION_UP) {
                    data.player_lis = LIS_SPECIAL_ATTACK;
                }
            }else{
                //BOTH HOLDS

                if (l_right_state == SECTION_LEFT ) {
                    //regardles of stick states, if u hold boths and perform light_attack you need to move your fingers
                    data.player_lis = LIS_NOTHING;
                }

                else if (l_right_state == SECTION_RIGHT ) {
                    //HOLDING SHIELD AND ANY OTHE STCIK ALWAYS SHIELDS YOU
                    data.player_lis = LIS_SHIELD;

                } else if (l_right_state == SECTION_UP ) {

                    //HERE YOU CHARGE YOUR SPECIAL, BUT WE ARE NOT THERE YET
                    data.player_lis = LIS_NOTHING;
                }

            }
        }else{
            data.player_lis = LIS_NOTHING;
        }
    }
}

}
