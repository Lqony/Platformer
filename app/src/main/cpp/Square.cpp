//
// Created by Beniamin on 2017-05-20.
//

#include "Square.h"
#include <string>
#include <jni.h>
#include <android/log.h>

using namespace std;


extern "C" {

void Square::Create(int drawed_scene, float x, float y, float z) {
    properties.x = x;
    properties.y = y;
    properties.z = z;
    properties.w = 0;
    properties.h = 0;
    properties.drawed_scene = drawed_scene;

}

void Square::LoadAnimations(int amount, float **data) {
    int i = 0;
    this->data.ans_amount = 0;
    while(i < amount && i < MAX_ANIMATIONS_AMOUNT){
        this->data.ans_amount++;
        this->data.animations[i] = new Animation(i);
        this->data.animations[i]->Create(data[i]);
        i++;
    }

    this->Show(0);
}

void Square::Show(int id) {
    if(properties.visible){
        data.animations[properties.visible_id]->Stop();
        properties.visible = false;
    }
    if(id < data.ans_amount){
        properties.w = data.animations[id]->DestFrameW();
        properties.h = data.animations[id]->DestFrameH();
        properties.visible = true;
        properties.visible_id = id;
        data.animations[id]->Start();
    }
}

void Square::Update() {
    if(properties.visible){
        data.animations[properties.visible_id]->Update();
    }
}

void Square::SquareRect(float result[]){

    //Bottom left
    result[0] = properties.x + data.animations[properties.visible_id]->ChangeX()
                - (properties.w / 2.0f);
    result[1] = properties.y + data.animations[properties.visible_id]->ChangeY();
    result[2] = properties.z;

    //Top Left
    result[3] = result[0];
    result[4] = result[1] + properties.h;
    result[5] = properties.z;

    //Top right
    result[6] = result[0] + properties.w;
    result[7] = result[4];
    result[8] = properties.z;

    //Bottom Right
    result[9] = result[6];
    result[10] = result[1];
    result[11] = properties.z;

}

void Square::TextureRect(float result[]){
   data.animations[properties.visible_id]->FrameRect();
    if(data.current_side){
        for(int i = 0; i < 8; i++){
            result[i] = data.animations[properties.visible_id]->GetFrameRect(i);
        }
    }else{
        result[0] = data.animations[properties.visible_id]->GetFrameRect(6);
        result[1] = data.animations[properties.visible_id]->GetFrameRect(7);
                ;
        result[2] = data.animations[properties.visible_id]->GetFrameRect(4);
        result[3] = data.animations[properties.visible_id]->GetFrameRect(5);

        result[4] = data.animations[properties.visible_id]->GetFrameRect(2);
        result[5] = data.animations[properties.visible_id]->GetFrameRect(3);

        result[6] = data.animations[properties.visible_id]->GetFrameRect(0);
        result[7] = data.animations[properties.visible_id]->GetFrameRect(1);
    }

}

};