//
// Created by Beniamin on 2017-05-17.
//

#include "Animation.h"
#include <string>
#include <jni.h>
#include <android/log.h>


using namespace std;

extern "C" {

    void Animation::Create(float *n_data) {

        data.current_frame = 0;
        data.delta = 0;
        data.visible = false;
        data.x_roll = 0;

        //as template
        //LOAD
        properties.first_frame_x = n_data[0];
        properties.first_frame_y = n_data[1];
        properties.source_frame_w = n_data[2];
        properties.source_frame_h = n_data[3];
        properties.roll = n_data[4];
        //RENDER
        properties.dest_frame_w = n_data[5];
        properties.dest_frame_h = n_data[6];
        properties.change_x = n_data[7];
        properties.change_y = n_data[8];
        //Prop
        properties.frame_amount = n_data[9];
        properties.repeat_from = n_data[10];
        properties.delay_time = n_data[11];
      //  __android_log_print(ANDROID_LOG_VERBOSE, "DELAY TIME", to_string(properties.delay_time).c_str(), 1);
    }

    void Animation::Start() {
        timer.Start();
        data.delta = 0;
        data.visible = true;
        data.current_frame = 0;
    }
    void Animation::Stop() {
        timer.Stop();
        data.delta = 0;
        data.visible = false;
        data.current_frame = 0;
    }

    void Animation::Pause() {
        timer.Pause();
    }

    void Animation::UnPause() {
        timer.unPause();
    }

    void Animation::Update(){

       // __android_log_print(ANDROID_LOG_VERBOSE, "DELTA", to_string(timer.Current()).c_str(), 1);
       // __android_log_print(ANDROID_LOG_VERBOSE, "DELAY TIME", to_string(properties.delay_time).c_str(), 1);
        if(properties.frame_amount > 1 &&  properties.delay_time > 0 && timer.Current() + data.delta >= properties.delay_time)
        {
            data.delta += timer.Stop();
            while(data.delta >= properties.delay_time){
                data.delta -= properties.delay_time;
                data.current_frame++;
                if(data.current_frame == properties.frame_amount) data.current_frame = properties.repeat_from;
            }
            timer.Start();
        }
    }

    float* Animation::FrameRect() {
        data.x_roll = 0;

        //BOTTOM LEFT x/y
        data.uvs[0] = properties.first_frame_x + (properties.source_frame_w * data.current_frame);
        data.uvs[1] = properties.first_frame_y;

        if(properties.roll > 0) {
            data.x_roll = data.current_frame / properties.roll;
            data.uvs[1] += properties.source_frame_h * data.x_roll;
            data.uvs[0] -= (properties.source_frame_w * properties.roll) * data.x_roll;
        }

        //TOP LEFT x/y
        data.uvs[2] = data.uvs[0];
        data.uvs[3] = data.uvs[1] - properties.source_frame_h;

        //TOP RIGHT x/y
        data.uvs[4] = data.uvs[0] - properties.source_frame_w;
        data.uvs[5] = data.uvs[1] - properties.source_frame_h;

        //BOTTOM RIGHT x/y
        data.uvs[6] = data.uvs[0] - properties.source_frame_w;
        data.uvs[7] = data.uvs[1];

        /*data.uvs[0] = data.uvs[0] / 1000.0f;
        data.uvs[1] = data.uvs[1] / 1000.0f;
        data.uvs[2] = data.uvs[2] / 1000.0f;
        data.uvs[3] = data.uvs[3] / 1000.0f;
        data.uvs[4] = data.uvs[4] / 1000.0f;
        data.uvs[5] = data.uvs[5] / 1000.0f;
        data.uvs[6] = data.uvs[6] / 1000.0f;
        data.uvs[7] = data.uvs[7] / 1000.0f;*/

    return data.uvs;
    }


};
