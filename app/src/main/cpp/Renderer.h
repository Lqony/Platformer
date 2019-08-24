//
// Created by Beniamin on 2017-05-17.
//

#ifndef CPLUSPLUS_RENDERER_H
#define CPLUSPLUS_RENDERER_H


#include "Square.h"
#include "GUIAnalog.h"
#include "Player.h"
#include "Platform.h"


extern "C"{

const int PLAYER_FRAME_DATA_AMOUNT = 23;

const short COORDS_PER_VERTEX = 3;
const short VERTEX_COUNT = 4;
const short VERTEX_STRIDE = 12;
const short MAX_SQUARES_AMOUNT = 50;
const short DRAW_ORDER[] = {0, 1, 2, 0, 2 ,3};
const int ANALOG_OUTSIDE = 0;
const int ANALOG_INSIDE = 1;
const int ANALOG_NONE = -1;
const int GAME_SCENE = 0;
const int FRONT_GUI = 1;
const int MAX_PLAYERS = 4;

class Square;
class Animation;

struct screen_info{
    float screen_w;
    float screen_h;
};

struct draw_info{
    int position;
    int draw_id;
    int count;
    bool draw;
};

struct scene_matrix{
    float mvp_matrix[16];
    float projection_matrix[16];
    float view_vatrix[16];
};

struct render_matrix{
    float texture_vs[8];
    float square_vs[12];
};

struct squares{
    int current_amount;
    bool is_taken[MAX_SQUARES_AMOUNT];
    Square* squares[MAX_SQUARES_AMOUNT];
};

struct guianalogs{
    GUIAnalog* analogs;
    int created;
};

struct texutres{
    int current_amount;
    int **bmp_ids;
};

struct time_deltas{
    float fps;
    float update;
    float draw;

};

struct game_players{
    int amount;
    Player* players;
};

struct game_platforms{
    int amount;
    Platform* platforms;
    int max;
};

class Renderer {

private:
    Timer timer;
    InputHandler input;
    int FPS;
    squares square_objects;
    screen_info screen;
    draw_info draw;
    render_matrix matrix;
    guianalogs guianalog_objects;
    game_players players;
    game_platforms platforms;

public:
    void SetUp();
    int CreateSquares(int amount, float **xyz);
    void LoadAnimations(int square_id, int amount, float** data);
    void SetScreen(float w, float h) { screen.screen_w = w; screen.screen_h = h; CreateInputHandler(); }
    bool ToDraw(int drawed_scene);
    void ReadDrawedSquareRect();
    float DrawedSquareRect(int n) { return matrix.square_vs[n]; }
    void ReadDrawedTextureRect();
    float DrawedTextureRect(int n) { return matrix.texture_vs[n]; }
    void UpdateSquares();
    void StartFps() { timer.Start(); FPS = 0; }
    void CreateInputHandler() { input.Create(screen.screen_w, screen.screen_h);}
    void ProcessInput(float *n_data);
    void CreateAnalogs(int amount, float** data);
    void HandleInput();
    void HandleAnalogs();
    void AddPlayer(player_data l_data, player_propereties l_prop, int l_frames_amount, player_frame* l_frames);
    void Logic();
    void Physic();
    void SetPlatforms(int amount) { platforms.platforms = new Platform[amount]; platforms.max = amount; }
    void AddPlatform(int square_id, int type, float* rect);
    void LoadPlayerFrames(player_frame* n_p_frames, int* n_frames, int n_amount);

    void ScreenToRender(int drawed_scene, float &x, float &y);
    float RenderToScreen(float &x, float &y);

    bool AddThatSquare() { if(input.Reset() && input.IsActive(0)) return true;
        return false;}
    float InputX() { return input.PointerX(0);}
    float InputY() { return input.PointerY(0);}
};

};
#endif //CPLUSPLUS_RENDERER_H
