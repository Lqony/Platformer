//
// Created by Beniamin on 2017-05-20.
//

#ifndef CPLUSPLUS_SQUARE_H
#define CPLUSPLUS_SQUARE_H



#include "Animation.h"

extern "C" {

class Animation;

const int MAX_ANIMATIONS_AMOUNT = 50;

    struct square_data{
        int id; //in table
        int ans_amount; //animations amount
        Animation* animations[MAX_ANIMATIONS_AMOUNT];
        bool current_side; //if false then swap sides

    };

    struct square_properties{
        float x, y, z, w, h;
        bool visible; //is any of its animation visible (only one can be visible at once)
        int visible_id; //id in table of currently visible animation
        int drawed_scene;
    };

class Square {

private:

    square_data data;
    square_properties properties;

public:

    Square(int id = 0) { data.id = id; properties.visible = false; data.current_side = true;}

    void Create(int drawed_scene, float x, float y, float z);
    void LoadAnimations(int amount, float** data);
    void Hide() { properties.visible = false; }
    void Show(int id);
    void Update();
    void SetSide(bool l_side) { data.current_side = l_side;}

    bool Visible() { return properties.visible; }
    void SquareRect(float result[]);
    void TextureRect(float result[]);
    void SetCoords(float x, float y, float z) { properties.x = x; properties.y = y; properties.z = z;}
    int DrawedScene() { return properties.drawed_scene; }
    int AnimationId() { return properties.visible_id; }
};


};




#endif //CPLUSPLUS_SQUARE_H
