//
// Created by Beniamin on 2017-06-06.
//

#include "Player.h"

using namespace std;

extern "C"{

void Player::Create(player_data l_player_data, player_propereties l_player_prop) {
    data = l_player_data;
    data.right = true;
    propereties = l_player_prop;
    propereties.movement_velocity /= 1000.0f;
    propereties.jump_power /= 1000.0f;
    data.air_jump = 0;
    data.animation = 0;
    data.ground_state = GS_NONE;
    data.logic_input_state = LIS_NOTHING;
    data.movement_state = PLAYER_STAY;
    data.timer = new Timer;
    data.request_input = false;
    data.timer->Start();
    forces.force_x = 0;
    forces.force_y = 0;

    data.input_state = new bool[INPUT_STATE_AMOUNT];
    for(int i = 0; i < INPUT_STATE_AMOUNT; i++){
        data.input_state[i] = false;
    }
}

void Player::AddFrames() {
    /*frames.amount = l_amount;
    frames.frames = new player_frame[l_amount];
    frames.delta = 0;
    frames.current = -1;
    for(int i = 0; i < l_amount; i++){
        frames.frames[i] = l_frames[i];
    }
    frames.next = 0;
    SetFrame();*/

    //Current Settings
    int l_ground_speed = 400;
    int l_air_speed = 200;
    int l_jump_power = 1000;
    int l_dash_velocity = 15000;

    int *l_data = new int[7];
    int **l_input_data = new int*[20];
    for(int i = 0; i < 20; i++) l_input_data[i] = new int[2];

    frames.amount = 20;
    frames.action_frames = new PlayerFrame[20];

    //frame 0 STAY ON GROUND
    l_data[0] = 0; //ID
    l_data[1] = 0; // Animation
    l_data[2] = 30.0f; //Delay
    l_data[3] = PLAYER_STAY; //Movement State
    l_data[4] = 0; //ground vel
    l_data[5] = 0; //air vel
    l_data[6] = 0; // jump vel

    l_input_data[0][0] = KEEP_FRAME; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[0].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 1 STAY IN AIR
    l_data[0] = 1; //ID
    l_data[1] = 2; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_STAY; //Movement State
    l_data[4] = 0; //ground vel
    l_data[5] = 0; //air vel
    l_data[6] = 0; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = KEEP_FRAME; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[1].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 2 LEFT ON GROUND
    l_data[0] = 2; //ID
    l_data[1] = 1; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_MOVE_LEFT; //Movement State
    l_data[4] = l_ground_speed; //ground vel
    l_data[5] = 0; //air vel
    l_data[6] = 0; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = KEEP_FRAME; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[2].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 3 LEFT IN AIR
    l_data[0] = 3; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_MOVE_LEFT; //Movement State
    l_data[4] = 0; //ground vel
    l_data[5] = l_air_speed; //air vel
    l_data[6] = 0; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = KEEP_FRAME; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[3].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 4 RIGHT ON GROUND
    l_data[0] = 4; //ID
    l_data[1] = 1; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_MOVE_RIGHT; //Movement State
    l_data[4] = l_ground_speed; //ground vel
    l_data[5] = 0; //air vel
    l_data[6] = 0; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = KEEP_FRAME; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[4].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 5 RIGHT IN AIR
    l_data[0] = 5; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_MOVE_RIGHT; //Movement State
    l_data[4] = 0; //ground vel
    l_data[5] = l_air_speed; //air vel
    l_data[6] = 0; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = KEEP_FRAME; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[5].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 6 Jump
    l_data[0] = 6; //ID
    l_data[1] = 2; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_JUMP; //Movement State
    l_data[4] = 0; //ground vel
    l_data[5] = 0; //air vel
    l_data[6] = l_jump_power; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = SKIP_FRAME; //Jump on ground
    l_input_data[3][1] = SKIP_FRAME; //Jump in Air
    l_input_data[4][0] = SKIP_FRAME; //Jump Left on ground
    l_input_data[4][1] = SKIP_FRAME; //Jump Left in Air
    l_input_data[5][0] = SKIP_FRAME; //Jump Right on ground
    l_input_data[5][1] = SKIP_FRAME; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[6].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 7 Jump Left
    l_data[0] = 7; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_JUMP_LEFT; //Movement State
    l_data[4] = l_ground_speed; //ground vel
    l_data[5] = l_air_speed; //air vel
    l_data[6] = l_jump_power; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = SKIP_FRAME; //Jump on ground
    l_input_data[3][1] = SKIP_FRAME; //Jump in Air
    l_input_data[4][0] = SKIP_FRAME; //Jump Left on ground
    l_input_data[4][1] = SKIP_FRAME; //Jump Left in Air
    l_input_data[5][0] = SKIP_FRAME; //Jump Right on ground
    l_input_data[5][1] = SKIP_FRAME; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[7].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 8 Jump Right
    l_data[0] = 8; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_JUMP_RIGHT; //Movement State
    l_data[4] = l_ground_speed; //ground vel
    l_data[5] = l_air_speed; //air vel
    l_data[6] = l_jump_power; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = SKIP_FRAME; //Jump on ground
    l_input_data[3][1] = SKIP_FRAME; //Jump in Air
    l_input_data[4][0] = SKIP_FRAME; //Jump Left on ground
    l_input_data[4][1] = SKIP_FRAME; //Jump Left in Air
    l_input_data[5][0] = SKIP_FRAME; //Jump Right on ground
    l_input_data[5][1] = SKIP_FRAME; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground cant fall on ground (for now)
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[8].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 9 Fall (ONLY IN AIR)
    l_data[0] = 9; //ID
    l_data[1] = 2; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_FALL; //Movement State
    l_data[4] = 0; //ground vel
    l_data[5] = 0; //air vel
    l_data[6] = l_jump_power; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = KEEP_FRAME; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[9].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 10 Fall Left (ONLY IN AIR)
    l_data[0] = 10; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_FALL_LEFT; //Movement State
    l_data[4] = l_ground_speed; //ground vel
    l_data[5] = l_air_speed; //air vel
    l_data[6] = l_jump_power; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = KEEP_FRAME; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[10].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 11 Fall Right (ONLY IN AIR)
    l_data[0] = 11; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 30.0f;; //Delay
    l_data[3] = PLAYER_FALL_RIGHT; //Movement State
    l_data[4] = l_ground_speed; //ground vel
    l_data[5] = l_air_speed; //air vel
    l_data[6] = l_jump_power; // jump vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = KEEP_FRAME; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[11].Create(MOVEMENT_FRAME, l_data, l_input_data);

    //Frame 12 DASH LEFT GROUND
    l_data[0] = 12; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 250.0f; //Delay
    l_data[3] = PLAYER_DASH_LEFT; //Movement State
    l_data[4] = 800.0f; //ground vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[12].Create(DASH_FRAME, l_data, l_input_data);

    //Frame 13 DASH LEFT AIR
    l_data[0] = 13; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 250.0f; //Delay
    l_data[3] = PLAYER_DASH_LEFT; //Movement State
    l_data[4] = 800.0f; //ground vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = SKIP_FRAME; //Dash left on ground
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = 15; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[13].Create(DASH_FRAME, l_data, l_input_data);


    //Frame 14 DASH RIGHT GROUND
    l_data[0] = 14; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 250.0f; //Delay
    l_data[3] = PLAYER_DASH_RIGHT; //Movement State
    l_data[4] = 800.0f; //ground vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[10][1] = 15; //Dash left air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[14].Create(DASH_FRAME, l_data, l_input_data);

    //Frame 15 DASH RIGHT AIR
    l_data[0] = 15; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 250.0f; //Delay
    l_data[3] = PLAYER_DASH_RIGHT; //Movement State
    l_data[4] = 800.0f; //ground vel

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = 12; //Dash left on ground
    l_input_data[9][1] = SKIP_FRAME; //Dash left on ground
    l_input_data[10][0] = 14; //Dash right on ground
    l_input_data[10][1] = SKIP_FRAME; //Dash right air
    l_input_data[11][0] = 16; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[15].Create(DASH_FRAME, l_data, l_input_data);


    //Frame 16 DASH UP GROUND
    l_data[0] = 16; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 250.0f; //Delay
    l_data[3] = PLAYER_DASH_UP; //Movement State
    l_data[4] = 800.0f; //ground velocity

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[10][1] = 15; //Dash left air
    l_input_data[11][0] = SKIP_FRAME; //Dash left air
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[16].Create(DASH_FRAME, l_data, l_input_data);

    //Frame 17 DASH UP AIR
    l_data[0] = 17; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 250.0f; //Delay
    l_data[3] = PLAYER_DASH_UP; //Movement State
    l_data[4] = 800.0f; //ground velocity

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[10][1] = 15; //Dash left air
    l_input_data[11][0] = 16; //Dash up ground
    l_input_data[11][1] = SKIP_FRAME; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[17].Create(DASH_FRAME, l_data, l_input_data);

    //Frame 18 DASH DOWN GFROUND
    l_data[0] = 18; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 250.0f; //Delay
    l_data[3] = PLAYER_DASH_DOWN; //Movement State
    l_data[4] = 800.0f; //ground velocity

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[10][1] = 15; //Dash left air
    l_input_data[11][0] = 16; //Dash up ground
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = SKIP_FRAME; //Dash down ground
    l_input_data[12][1] = 19; //Dash down air
    frames.action_frames[18].Create(DASH_FRAME, l_data, l_input_data);

    //Frame 19 DASH DOWN AIR
    l_data[0] = 19; //ID
    l_data[1] = 3; // Animation
    l_data[2] = 250.0f; //Delay
    l_data[3] = PLAYER_DASH_DOWN; //Movement State
    l_data[4] = 800.0f; //ground velocity

    l_input_data[0][0] = 0; //Stay On Ground
    l_input_data[0][1] = 1; //Stay In Air
    l_input_data[1][0] = 2; //Left on ground
    l_input_data[1][1] = 3; //Left in Air
    l_input_data[2][0] = 4; //Right on ground
    l_input_data[2][1] = 5; //Right in Air
    l_input_data[3][0] = 6; //Jump on ground
    l_input_data[3][1] = 6; //Jump in Air
    l_input_data[4][0] = 7; //Jump Left on ground
    l_input_data[4][1] = 7; //Jump Left in Air
    l_input_data[5][0] = 8; //Jump Right on ground
    l_input_data[5][1] = 8; //Jump Right in Air
    l_input_data[6][0] = SKIP_FRAME; //Fall on ground
    l_input_data[6][1] = 9; //Fall in Air
    l_input_data[7][0] = 2; //Fall left on ground
    l_input_data[7][1] = 10; //Fall left in Air
    l_input_data[8][0] = 4; //Fall right on ground
    l_input_data[8][1] = 11; //Fall right in Air
    l_input_data[9][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[9][1] = 13; //Dash left air
    l_input_data[10][0] = SKIP_FRAME; //Dash left on ground
    l_input_data[10][1] = 15; //Dash left air
    l_input_data[11][0] = 16; //Dash up ground
    l_input_data[11][1] = 17; //Dash up air
    l_input_data[12][0] = 18; //Dash down ground
    l_input_data[12][1] = SKIP_FRAME; //Dash down air
    frames.action_frames[19].Create(DASH_FRAME, l_data, l_input_data);


    frames.next = 0;
    SetFrame();
}

void Player::SetGroundState(int l_platform_id, int l_state, float l_platform_surface){
    data.platform_id = l_platform_id; data.ground_state = l_state;
    if(data.ground_state == GS_ON_PLATFORM){
        data.last_y = l_platform_surface;
        data.y = l_platform_surface;
        forces.force_y = 0;
        data.air_jump = 0;
    }
}

void Player::CheckFrames() {

    if(data.request_input) {
        switch (data.logic_input_state) {
            case LIS_NOTHING:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "NOTHING", 1);
                if (data.ground_state == GS_ON_PLATFORM) {
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
                } else {
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
                    //__android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "NOTHING IN AIR", 1);
                }
                break;

            case LIS_MOVE_LEFT:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "MOVE LEFT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.move_left.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.move_left.in_air;
                break;

            case LIS_MOVE_RIGHT:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "MOVE RIGHT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.move_right.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.move_right.in_air;
                break;

            case LIS_JUMP:
                frames.next = KEEP_FRAME;
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.jump.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.jump.in_air;
                break;

            case LIS_JUMP_LEFT:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "MOVE JUMP LEFT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.jump_left.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.jump_left.in_air;
                break;

            case LIS_JUMP_RIGHT:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "MOVE JUMP RIGHT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.jump_right.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.jump_right.in_air;
                break;

            case LIS_FALL:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "FALL", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.fall.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.fall.in_air;
                break;

            case LIS_FALL_LEFT:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "MOVE FALL LEFT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.fal_left.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.fal_left.in_air;
                break;

            case LIS_FALL_RIGHT:
                frames.next = KEEP_FRAME;
                //__android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "MOVE FALL RIGHT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.fall_right.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.fall_right.in_air;
                break;

            case LIS_LIGHT_ATTACK:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "LIGHT ATTACK", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
                break;

            case LIS_SPECIAL_ATTACK:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "SPECIAL ATTACK", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
                break;

            case LIS_SHIELD:
                frames.next = KEEP_FRAME;
              //  __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "SHIELD", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
                break;

            case LIS_DASH_LEFT:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "DASH LEFT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.dash_left.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.dash_left.in_air;
                break;

            case LIS_DASH_RIGHT:
                frames.next = KEEP_FRAME;
              //  __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "DASH RIGHT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.dash_right.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.dash_right.in_air;
                break;

            case LIS_DASH_UP:
                frames.next = KEEP_FRAME;
              //  __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "DASH UP", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.dash_up.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.dash_up.in_air;
                break;

            case LIS_DASH_DOWN:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "DASH DOWN", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.dash_down.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.dash_down.in_air;
                break;

            case LIS_LIGHT_ATTACK_LEFT:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "LIGHT ATTACK LEFT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
                break;

            case LIS_LIGHT_ATTACK_RIGHT:
                frames.next = KEEP_FRAME;
                //__android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "LIGHT ATTACK RIGHT", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
                break;

            case LIS_LIGHT_ATTACK_UP:
                frames.next = KEEP_FRAME;
               // __android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "LIGHT ATTACK UP", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
                break;

            case LIS_LIGHT_ATTACK_DOWN:
                frames.next = KEEP_FRAME;
                //__android_log_print(ANDROID_LOG_VERBOSE, "PLAYER MOVE: ", "LIGHT ATTACK DOWN", 1);
                if (data.ground_state == GS_ON_PLATFORM)
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
                else
                    frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
                break;
        }
    }

    if(data.request_input){
        if(frames.next == KEEP_FRAME){



        }else if(frames.next == SKIP_FRAME) {
            //DO THE SAME THING AS IN LIS_NOTHING THEN SET FRAME
            if(data.ground_state == GS_ON_PLATFORM)
                frames.next = frames.action_frames[frames.current].Data().input.nothing.on_ground;
            else
                frames.next = frames.action_frames[frames.current].Data().input.nothing.in_air;
            SetFrame();
        }else{
            SetFrame();
        }
    }

    if(frames.timer.Current() >= frames.current_delay) {


        //If the last was dash, stop movement
        if(frames.action_frames[frames.current].Data().type == DASH_FRAME){
            if(frames.current_delay > frames.action_frames[frames.current].Data().delay){
                data.request_input = true;
            }else{
                if(data.movement_state == PLAYER_DASH_LEFT || data.movement_state == PLAYER_DASH_RIGHT){
                    forces.force_x = 0.0f;
                }else{
                    forces.force_y = 0.0f;
                }
                frames.current_delay += 0.2f;
                data.movement_state = PLAYER_STAY;
                if(data.ground_state == GS_ON_PLATFORM)
                    data.animation = 0;
                else
                    data.animation = 2;
            }
        }else{
            data.request_input = true;
        }


    }

}

void Player::Update(){

    CheckFrames();

}

void Player::Gravity(float n_gravity_vel, float n_min_gravity) {
    if(data.ground_state == GS_IN_AIR){
        if(forces.force_y <= 0 && forces.force_y > -n_min_gravity) forces.force_y = -n_min_gravity;
        forces.force_y -= n_gravity_vel * ( data.timer->Current());
    }
}

void Player::Friction(float n_air, float n_ground) {
    if(data.ground_state == GS_ON_PLATFORM){
        if(forces.force_x > 0){
            forces.force_x -= n_ground * ( data.timer->Current());
            if(forces.force_x < 0) forces.force_x = 0.0f;
        }else if(forces.force_x < 0){
            forces.force_x += n_ground * ( data.timer->Current());
            if(forces.force_x > 0) forces.force_x = 0.0f;
        }
    }
    if(data.ground_state == GS_IN_AIR){
        if(forces.force_x > 0){
            forces.force_x -= n_air * ( data.timer->Current());
            if(forces.force_x < 0) forces.force_x = 0.0f;
        }else if(forces.force_x < 0){
            forces.force_x += n_air * ( data.timer->Current());
            if(forces.force_x > 0) forces.force_x = 0.0f;
        }
    }
}

void Player::Move() {

    data.last_x = data.x;
    data.last_y = data.y;
    data.last_z = data.z;

    if(data.movement_state == PLAYER_MOVE_LEFT){
                data.right = false;
        if(data.ground_state == GS_ON_PLATFORM){
            if(forces.force_x < frames.action_frames[frames.current].MovementData().ground_x_vel)
            forces.force_x = frames.action_frames[frames.current].MovementData().ground_x_vel;
        } else{
            if(forces.force_x < frames.action_frames[frames.current].MovementData().air_x_vel)
                forces.force_x = frames.action_frames[frames.current].MovementData().air_x_vel;
        }
    }else if(data.movement_state == PLAYER_MOVE_RIGHT){
        data.right = true;
        if(data.ground_state == GS_ON_PLATFORM){
            if(forces.force_x > -frames.action_frames[frames.current].MovementData().ground_x_vel)
                forces.force_x = -frames.action_frames[frames.current].MovementData().ground_x_vel;
        } else{
            if(forces.force_x > -frames.action_frames[frames.current].MovementData().air_x_vel)
                forces.force_x = -frames.action_frames[frames.current].MovementData().air_x_vel;
        }
    }else if(data.movement_state == PLAYER_JUMP_LEFT){

        data.right = false;
        if(data.ground_state == GS_ON_PLATFORM){
            if(forces.force_x < frames.action_frames[frames.current].MovementData().ground_x_vel)
                forces.force_x = frames.action_frames[frames.current].MovementData().ground_x_vel;
        } else{
                if(forces.force_x < frames.action_frames[frames.current].MovementData().air_x_vel)
                    forces.force_x = frames.action_frames[frames.current].MovementData().air_x_vel;
        }
        if(data.air_jump < propereties.air_jump_amount){
            forces.force_y = frames.action_frames[frames.current].MovementData().y_vel;
            data.air_jump++;
        }
        data.movement_state = PLAYER_STAY;
    }else if(data.movement_state == PLAYER_JUMP_RIGHT){
        data.right = true;
        if(data.ground_state == GS_ON_PLATFORM){
            if(forces.force_x > -frames.action_frames[frames.current].MovementData().ground_x_vel)
                forces.force_x = -frames.action_frames[frames.current].MovementData().ground_x_vel;
        } else{
            if(forces.force_x > -frames.action_frames[frames.current].MovementData().air_x_vel)
                forces.force_x = -frames.action_frames[frames.current].MovementData().air_x_vel;
        }
        if(data.air_jump < propereties.air_jump_amount){
            forces.force_y = frames.action_frames[frames.current].MovementData().y_vel;
            data.air_jump++;
        }
        data.movement_state = PLAYER_STAY;
    }else if(data.movement_state == PLAYER_JUMP){
        if(data.air_jump < propereties.air_jump_amount){
            forces.force_y = frames.action_frames[frames.current].MovementData().y_vel;
            data.air_jump++;
        }
        data.movement_state = PLAYER_STAY;
    }else if(data.movement_state == PLAYER_FALL){
        forces.force_y -= frames.action_frames[frames.current].MovementData().y_vel * ( data.timer->Current());
    }else if(data.movement_state == PLAYER_FALL_LEFT){
        data.right = false;
        if(data.ground_state == GS_ON_PLATFORM){
            if(forces.force_x < frames.action_frames[frames.current].MovementData().ground_x_vel)
                forces.force_x = frames.action_frames[frames.current].MovementData().ground_x_vel;
        } else{
            if(forces.force_x < frames.action_frames[frames.current].MovementData().air_x_vel)
                forces.force_x = frames.action_frames[frames.current].MovementData().air_x_vel;
        }
        forces.force_y -= frames.action_frames[frames.current].MovementData().y_vel * ( data.timer->Current());
    }else if(data.movement_state == PLAYER_FALL_RIGHT){
        data.right = true;
        if(data.ground_state == GS_ON_PLATFORM){
            if(forces.force_x > -frames.action_frames[frames.current].MovementData().ground_x_vel)
                forces.force_x = -frames.action_frames[frames.current].MovementData().ground_x_vel;
        } else{
            if(forces.force_x > -frames.action_frames[frames.current].MovementData().air_x_vel)
                forces.force_x = -frames.action_frames[frames.current].MovementData().air_x_vel;
        }
        forces.force_y -= frames.action_frames[frames.current].MovementData().y_vel * ( data.timer->Current());
    }else if(data.movement_state == PLAYER_DASH_LEFT){
        data.right = false;
        forces.force_x = frames.action_frames[frames.current].DashData().velocity;
    }else if(data.movement_state == PLAYER_DASH_RIGHT){
        data.right = true;
        forces.force_x = -frames.action_frames[frames.current].DashData().velocity;
    }else if(data.movement_state == PLAYER_DASH_UP){

        forces.force_y = frames.action_frames[frames.current].DashData().velocity;
    }
    else if(data.movement_state == PLAYER_DASH_DOWN){

        forces.force_y = -frames.action_frames[frames.current].DashData().velocity;
    }

    data.x += forces.force_x * ( data.timer->Current());
    data.y += forces.force_y * ( data.timer->Current());
    data.timer->Start();
}

void Player::SetFrame() {
        frames.timer.Start();

        frames.current = frames.next;
        frames.next = -1;
        frames.current_delay = frames.action_frames[frames.current].Data().delay;
        data.request_input = false;
        data.animation = frames.action_frames[frames.current].Data().animation;
        if(frames.action_frames[frames.current].Data().type == MOVEMENT_FRAME){
            data.movement_state = frames.action_frames[frames.current].MovementData().movement_state;
        }else if(frames.action_frames[frames.current].Data().type == DASH_FRAME){
            data.movement_state = frames.action_frames[frames.current].DashData().movement_state;
        }
}

}
