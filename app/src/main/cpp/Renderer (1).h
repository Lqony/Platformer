//
// Created by Beniamin on 2017-05-17.
//

#include "Renderer.h"
#include <jni.h>
#include <android/log.h>
#include <string>

using namespace std;

extern "C" {

Renderer* RENDERER;
Timer TIMER;

void Renderer::SetUp() {
    square_objects.current_amount = 0;
    for(int i = 0; i < MAX_SQUARES_AMOUNT; i++){
        square_objects.is_taken[i] = false;
    }

    draw.draw = false;
    draw.draw_id = 0;
    draw.position = 0;
    draw.count = 0;

    guianalog_objects.created = 0;

    players.amount = 0;
    players.players = new Player[MAX_PLAYERS];

    SetPlatforms(20);
    platforms.amount = 0;
}

int Renderer::CreateSquares(int amount, float **dsxyz) {
    int i = 0;
    int count = 0;
    while(count < amount && i < MAX_SQUARES_AMOUNT){
        if(!square_objects.is_taken[i])
        {
            square_objects.is_taken[i] = true;
            square_objects.current_amount++;
            square_objects.squares[i] = new Square(i);
            square_objects.squares[i]->Create(dsxyz[count][0], dsxyz[count][1], dsxyz[count][2], dsxyz[count][3]);
            count++;
        }
        i++;
    }

    return square_objects.current_amount - 1;
}

void Renderer::LoadAnimations(int square_id, int amount, float **data) {

            if(square_id < MAX_SQUARES_AMOUNT){
                if(square_objects.is_taken[square_id]){
                    square_objects.squares[square_id]->LoadAnimations(amount, data);
                }
            }
}

bool Renderer::ToDraw(int drawed_scene) {

    if(draw.count < square_objects.current_amount){

        for(int i = draw.position; i  < MAX_SQUARES_AMOUNT; i++){
            if(square_objects.is_taken[i]){
                draw.count++;

                if(square_objects.squares[i]->Visible()
                   && square_objects.squares[i]->DrawedScene() == drawed_scene){
                    //__android_log_print(ANDROID_LOG_VERBOSE, "SCENE", to_string(drawed_scene).c_str(), 1);
                    draw.draw_id = i;
                    draw.draw = true;
                    draw.position = i+1;
                    return draw.draw;
                }
            }
        }
        draw.count = 0;
        draw.position = 0;
        draw.draw = false;
        return draw.draw;
    }else{
        draw.count = 0;
        draw.position = 0;
        draw.draw = false;
        return draw.draw;
    }
}

void Renderer::ReadDrawedSquareRect(){
    if(draw.draw)
   square_objects.squares[draw.draw_id]->SquareRect(matrix.square_vs);
}

void Renderer::ReadDrawedTextureRect(){
    if(draw.draw)
   square_objects.squares[draw.draw_id]->TextureRect(matrix.texture_vs);

}

void Renderer::ScreenToRender(int drawed_scene, float &x, float &y){

    int l_l;
    int l_r;
    if(drawed_scene == GAME_SCENE){
        l_l = 1;
        l_r = -1;
    }else {
        l_l = -1;
        l_r = 1;
    }

    float sourceTopLeftX = 0.0f;
    float sourceTopLeftY = 0.0f;
    float sourceBottomRightX = screen.screen_w;
    float sourceBottomRightY = screen.screen_h;

    float targetTopLeftX = l_l * (float)(screen.screen_w/screen.screen_h);
    float targetTopLeftY = 1;
    float targetBottomRightX = l_r * (float)(screen.screen_w/screen.screen_h);
    float targetBottomRightY = -1;

    x = targetTopLeftX +
            ((x - sourceTopLeftX) / (sourceBottomRightX-sourceTopLeftX))*(targetBottomRightX-targetTopLeftX);
    y = targetTopLeftY +
            ((y - sourceTopLeftY) / (sourceBottomRightY-sourceTopLeftY))*(targetBottomRightY-targetTopLeftY);
}

void Renderer::UpdateSquares(){
    int count = 0;
    int i = 0;
    int l_square_id;


    //Update All Animations
    while(count < square_objects.current_amount && i < MAX_SQUARES_AMOUNT){

        if(square_objects.is_taken[i]){
            if(square_objects.squares[i]->Visible()){
                square_objects.squares[i]->Update();
            }
            count++;
        }
        i++;
    }

    //Update Position of Analog Squares
    for(int i = 0; i < guianalog_objects.created; i++){
        l_square_id = guianalog_objects.analogs[i].SquareId();
        if(square_objects.squares[l_square_id]->Visible()){
            square_objects.squares[l_square_id]->SetCoords(guianalog_objects.analogs[i].RenderInsideX(),
                                                             guianalog_objects.analogs[i].RenderInsideY(),

            0.0f);
        }
    }

    //Update position of player squares
    for(int i = 0; i < players.amount; i++){
        l_square_id = players.players[i].SquareID();
        if(square_objects.squares[l_square_id]->Visible()){
            if(players.players[i].AnimationId() != square_objects.squares[l_square_id]->AnimationId()){
                square_objects.squares[l_square_id]->Show(players.players[i].AnimationId());
            }
            square_objects.squares[l_square_id]->SetSide(players.players[i].RightSide());
            square_objects.squares[l_square_id]->SetCoords(players.players[i].X(),
                                                           players.players[i].Y(),
                                                           players.players[i].Z());
        }
    }

    FPS++;
    if(timer.Current() >= 1){
        __android_log_print(ANDROID_LOG_VERBOSE, "FPS", to_string(FPS).c_str(), 1);
        timer.Start();
        FPS = 0;
    }
}

void Renderer::CreateAnalogs(int amount, float **data) {
    guianalog_objects.created = amount;
    guianalog_objects.analogs = new GUIAnalog[amount];

    for(int i = 0; i < amount; i++){
        guianalog_objects.analogs[i].Create(i, data[i][0], data[i][1],
                                            data[i][2], data[i][3], data[i][4], data[i][5]);
    }


}

void Renderer::HandleAnalogs(){
    int l_pointer_id;
    float l_render_x;
    float l_render_y;
    bool l_found = false;
    for(int i = 0; i < guianalog_objects.created; i++){
        l_found = false;
        if(input.IsActive(0)){
            l_pointer_id = 0;
            l_render_x = input.PointerX(l_pointer_id);
            l_render_y = input.PointerY(l_pointer_id);
            ScreenToRender(FRONT_GUI, l_render_x, l_render_y);
            l_found = guianalog_objects.analogs[i].Check(l_render_x, l_render_y, l_pointer_id);
        }
        if(!l_found && input.IsActive(1)){
            l_pointer_id = 1;
            l_render_x = input.PointerX(l_pointer_id);
            l_render_y = input.PointerY(l_pointer_id);
            ScreenToRender(FRONT_GUI, l_render_x, l_render_y);
            l_found = guianalog_objects.analogs[i].Check(l_render_x, l_render_y, l_pointer_id);
        }
        if(!l_found) guianalog_objects.analogs[i].Reset();
        guianalog_objects.analogs[i].SetInputState();
    }


    //0 IS LEFT AND 1 IS RIGHT
}

void Renderer::ProcessInput(float *n_data) {
    input.ProcessInput(n_data, screen.screen_w, screen.screen_h);
    HandleAnalogs();
}

void Renderer::AddPlayer(player_data l_data, player_propereties l_prop, int l_frames_amount,
                         player_frame *l_frames) {
    if(players.amount < MAX_PLAYERS){
        l_data.id = players.amount;
        players.players[players.amount].Create(l_data, l_prop);
        players.players[players.amount].AddFrames();
        players.amount++;
    }
}


void Renderer::HandleInput() {

    if(players.amount > 0) {
            input.SetPlayerLIS(guianalog_objects.analogs[0].InputState(), guianalog_objects.analogs[0].Hold(),
                               guianalog_objects.analogs[1].InputState(), guianalog_objects.analogs[1].Hold());
        if(players.players[0].RequestInput()){
            players.players[0].SetLogicInputState(input.PlayerLIS());
        }
    }
}

void Renderer::Logic(){
    for(int i = 0; i < players.amount; i++){
        players.players[i].Update();
    }
    for(int i = 0; i < players.amount; i++){
        players.players[i].Move();
    }
}

void Renderer::Physic() {
    //Player Ground State
    float *l_player_last_xyz = new float[3];
    float *l_player_current_xyz = new float[3];
    float *l_player_size = new float[2];
    for(int i = 0; i < players.amount; i++){
        l_player_last_xyz[0] = players.players[i].LastX();
        l_player_last_xyz[1] = players.players[i].LastY();
        l_player_last_xyz[2] = players.players[i].LastZ();

        l_player_current_xyz[0] = players.players[i].X();
        l_player_current_xyz[1] = players.players[i].Y();
        l_player_current_xyz[2] = players.players[i].Z();

        l_player_size[0] = 0.2f;
        l_player_size[1] = 0.2f;

        switch (players.players[i].GroundState()){
            case GS_NONE:

                players.players[i].SetGroundState(0, GS_IN_AIR, 0);
                for(int j = 0; j < platforms.amount; j++){
                    if(platforms.platforms[j].Check(l_player_last_xyz, l_player_current_xyz, l_player_size))
                    {

                        players.players[i].SetGroundState(j, GS_ON_PLATFORM, platforms.platforms[j].Y());
                        j = platforms.amount;
                    }
                }
                //check if he stands at any platform

                break;

            case GS_IN_AIR:
                for(int j = 0; j < platforms.amount; j++){
                    if(platforms.platforms[j].Check(l_player_last_xyz, l_player_current_xyz, l_player_size))
                    {

                        players.players[i].SetGroundState(j, GS_ON_PLATFORM, platforms.platforms[j].Y());
                        j = platforms.amount;
                    }
                }
                break;

            case GS_ON_PLATFORM:
                if(!platforms.platforms[players.players[i].PlatformId()].Check(l_player_last_xyz, l_player_current_xyz, l_player_size))
                {
                    players.players[i].SetGroundState(0, GS_IN_AIR, 0);
                }
                break;

            default:
                //something is wrong ground platform should be only one of above
                break;
        }
    }

    for(int i = 0; i < players.amount; i++){
        players.players[i].Gravity( 2, 1);
        players.players[i].Friction(0.1f, 6.0f);
    }

    //CLEANUP
    delete [] l_player_last_xyz;
    delete [] l_player_current_xyz;
    delete [] l_player_size;
}

void Renderer::AddPlatform(int square_id, int type, float *rect) {

    if(platforms.amount < platforms.max){
        platforms.platforms[platforms.amount].Create(platforms.amount, square_id, type, rect);
        platforms.amount++;
    }
};

void Renderer::LoadPlayerFrames(player_frame* n_p_frames, int *n_frames, int n_amount) {
    for(int i = 0; i < n_amount; i++){
        //MAMY 10 FALL
        /*n_p_frames[i].propereties.movement_state = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+0];
        n_p_frames[i].propereties.animation = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+1];

        n_p_frames[i].no_input.delta = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+2] / 1000.0f;
        n_p_frames[i].no_input.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+3];
        n_p_frames[i].no_input.in_air_frame_id= n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+4];

        n_p_frames[i].left.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+5];
        n_p_frames[i].left.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+6];

        n_p_frames[i].right.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+7];
        n_p_frames[i].right.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+8];

        n_p_frames[i].up_left.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+9];
        n_p_frames[i].up_left.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+10];

        n_p_frames[i].up_right.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+11];
        n_p_frames[i].up_right.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+12];

        n_p_frames[i].down_left.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+13];
        n_p_frames[i].down_left.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+14];

        n_p_frames[i].down_right.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+15];
        n_p_frames[i].down_right.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+16];

        n_p_frames[i].up.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+17];
        n_p_frames[i].up.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+18];

        n_p_frames[i].down.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+19];
        n_p_frames[i].down.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+20];

        n_p_frames[i].push_a.on_ground_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+21];
        n_p_frames[i].push_a.in_air_frame_id = n_frames[(i*PLAYER_FRAME_DATA_AMOUNT)+22];*/
    }
}

// ------------------------------------------------------------------------------------------------

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_UpdateSquares(JNIEnv *env, jobject instance) {
    RENDERER->UpdateSquares();
}

JNIEXPORT jfloatArray JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_DrawSquareCoords(JNIEnv *env, jobject instance) {

    jfloatArray result;
    jfloat *fill = new float[12];
    result = (*env).NewFloatArray(12);

    RENDERER->ReadDrawedSquareRect();
    for(int i = 0; i < 12; i++){
        fill[i] = RENDERER->DrawedSquareRect(i);
    }
    (*env).SetFloatArrayRegion(result, 0, 12, fill);

    delete [] fill;

    return result;
}

JNIEXPORT jfloatArray JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_DrawTextureCoords(JNIEnv *env, jobject instance) {

    jfloatArray result;
    jfloat *fill = new float[8];
    result = (*env).NewFloatArray(8);

    RENDERER->ReadDrawedTextureRect();
    for(int i = 0; i < 8; i++){
        fill[i] = RENDERER->DrawedTextureRect(i);
    }
    (*env).SetFloatArrayRegion(result, 0, 8, fill);

    delete [] fill;

    return result;

}

JNIEXPORT jshortArray JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_DrawOrder(JNIEnv *env, jobject instance) {
    jshortArray result;
    jshort *fill = new jshort[6];
    result = (*env).NewShortArray(6);
    for(int i = 0; i < 6; i++){
        fill[i] = DRAW_ORDER[i];
    }
    (*env).SetShortArrayRegion(result, 0, 6, fill);

    delete [] fill;

    return result;
}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_InitRender(JNIEnv *env, jobject instance) {

    RENDERER = new Renderer;
    RENDERER->SetUp();
    RENDERER->StartFps();

}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_DeleteRender(JNIEnv *env, jobject instance) {

    delete  RENDERER;

}

JNIEXPORT jint JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_AddSquares(JNIEnv *env, jobject instance,
                                                             jint amount, jfloatArray data_) {

    jfloat *data = env->GetFloatArrayElements(data_, NULL);
    float **l_data = new float*[amount]; // there is 11 data elements to pass ATM
    int result;
    for(int i = 0; i < amount; i++){
        l_data[i] = new float[3];
        for(int j = 0; j < 3; j++){
            l_data[i][j] = data[j];
        }
    }

    result = RENDERER->CreateSquares(amount, l_data);

    delete [] l_data;
    env->ReleaseFloatArrayElements(data_, data, 0);

    return result;
}


JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_SetScreen(JNIEnv *env, jobject instance, jfloat w,
                                                            jfloat h) {

    RENDERER->SetScreen(w, h);

}
JNIEXPORT jboolean JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_ToDraw(JNIEnv *env, jobject instance,
                                                         jint drawed_scene) {

    return RENDERER->ToDraw(drawed_scene);

}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_LoadSquareAnimations(JNIEnv *env,
                                                                        jobject instance,
                                                                       jint id,
                                                                        jint amount,
                                                                        jfloatArray data_) {
    jfloat *data = env->GetFloatArrayElements(data_, NULL);

    float** l_data = new float*[amount];


    for(int i = 0; i < amount; i++){
        l_data[i] = new float[12];
        l_data[i][0] = data[(i*12)+0];
        l_data[i][1] = data[(i*12)+1];
        l_data[i][2] = data[(i*12)+2];
        l_data[i][3] = data[(i*12)+3];
        l_data[i][4] = data[(i*12)+4];
        l_data[i][5] = data[(i*12)+5];
        l_data[i][6] = data[(i*12)+6];
        l_data[i][7] = data[(i*12)+7];
        l_data[i][8] = data[(i*12)+8];
        l_data[i][9] = data[(i*12)+9];
        l_data[i][10] = data[(i*12)+10];
        l_data[i][11] = data[(i*12)+11];
    }

    RENDERER->LoadAnimations(id, amount, l_data);

    delete [] l_data;

    env->ReleaseFloatArrayElements(data_, data, 0);
}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_ProcessInput(JNIEnv *env, jobject instance,
                                                               jfloatArray data_) {
    jfloat *data = env->GetFloatArrayElements(data_, NULL);
    float *l_data = new float[5];

    for(int i = 0; i < 5; i++){
        l_data[i] = data[i];
    }

    RENDERER->ProcessInput(data);

    delete [] l_data;
    env->ReleaseFloatArrayElements(data_, data, 0);
}

JNIEXPORT jboolean JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_AddThatSquare(JNIEnv *env, jobject instance) {

    return RENDERER->AddThatSquare();

}

JNIEXPORT jfloatArray JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_NewBorn(JNIEnv *env, jobject instance) {

    jfloatArray result;
    jfloat *fill = new float[2];
    result = (*env).NewFloatArray(2);

    fill[0] = RENDERER->InputX();
    fill[1] = RENDERER->InputY();
    RENDERER->ScreenToRender(GAME_SCENE, fill[0], fill[1]);
    (*env).SetFloatArrayRegion(result, 0, 2, fill);

    delete [] fill;

    return result;

}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_CreateAnalogs(JNIEnv *env, jobject instance,
                                                                jint amount, jfloatArray data_) {
    jfloat *data = env->GetFloatArrayElements(data_, NULL);
    float** l_data = new float*[amount];
    for(int i = 0; i < amount; i++){
        l_data[i] = new float[6];
        for(int j = 0; j < 6; j++){
            l_data[i][j] = data[(i*6)+j];
        }
    }
    RENDERER->CreateAnalogs(amount, l_data);

    env->ReleaseFloatArrayElements(data_, data, 0);
    delete [] l_data;
}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_HandleInput__(JNIEnv *env, jobject instance) {

    RENDERER->HandleInput();

}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_CreatePlayer(JNIEnv *env, jobject instance,
                                                               jfloatArray n_player_data_,
                                                               jint n_frames_amount,
                                                               jintArray n_frames_) {
    jfloat *n_player_data = env->GetFloatArrayElements(n_player_data_, NULL);
    jint *n_frames = env->GetIntArrayElements(n_frames_, NULL);

    player_data l_pdata;
    player_propereties l_pprop;
    player_frame* l_pframes;
    l_pframes = new player_frame[n_frames_amount];

    //CREATE PLAYER DATA
    l_pdata.id = 0; //in a table
    l_pdata.x = n_player_data[5];
    l_pdata.y = n_player_data[6];
    l_pdata.z = n_player_data[7];
    l_pdata.last_x = n_player_data[5];
    l_pdata.last_y = n_player_data[6];
    l_pdata.last_z = n_player_data[7];
    l_pdata.suqare_id = n_player_data[8];
    l_pdata.air_jump = 0;
    l_pdata.ground_state = 0;
    l_pdata.movement_state = 0;
    l_pdata.animation = 0;

    //CREATE PLAYER PROP
    l_pprop.movement_velocity = n_player_data[0];
    l_pprop.jump_power = n_player_data[1];
    l_pprop.air_jump_amount = n_player_data[2];
    l_pprop.air_vertical_velocioty = n_player_data[3];
    l_pprop.fall_velocity = n_player_data[4];

    //LOAD PLAYER FRAMES
    RENDERER->LoadPlayerFrames(l_pframes, n_frames, n_frames_amount);

    RENDERER->AddPlayer(l_pdata, l_pprop, n_frames_amount, l_pframes);

    delete [] l_pframes;

    env->ReleaseFloatArrayElements(n_player_data_, n_player_data, 0);
    env->ReleaseIntArrayElements(n_frames_, n_frames, 0);
}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_Logic(JNIEnv *env, jobject instance) {

    RENDERER->Logic();

}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_CreatePlatform(JNIEnv *env, jobject instance,
                                                                 jint square_id, jint type,
                                                                 jfloatArray rect_) {
    jfloat *rect = env->GetFloatArrayElements(rect_, NULL);
    float* l_rect = new float[5];
    for(int i = 0; i < 5; i++){
        l_rect[i] = rect[i];
    }
    RENDERER->AddPlatform(square_id, type, l_rect);
    // TODO

    env->ReleaseFloatArrayElements(rect_, rect, 0);
}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_Physic(JNIEnv *env, jobject instance) {

    // TODO
    RENDERER->Physic();

}

JNIEXPORT void JNICALL
Java_com_natywna_aplikacja_cplusplus_MyGLRenderer_Pause(JNIEnv *env, jobject instance, jint mss) {
    //float l_delta;

    TIMER.Start();
    while(TIMER.Current() < mss){

        // l_delta = TIMER.Current();
        //__android_log_print(ANDROID_LOG_VERBOSE, "DELTA", to_string(l_delta).c_str(), 1);

    }

    TIMER.Stop();

}

}



