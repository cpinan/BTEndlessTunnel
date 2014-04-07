//
//  Constants.h
//  BTEndlessTunnel
//
//  Created by NSS on 3/21/14.
//
//

#ifndef BTEndlessTunnel_Constants_h
#define BTEndlessTunnel_Constants_h

#define SHOW_FPS true

// Definición de ID's para Local Storage

#define USER_CONTROL_IS_JOYPAD "KEY_CONTROL_IS_JOYPAD"
#define USER_SAVE_REPLAY "KEY_REPLAY_NUMBER_%d"
#define USER_PLAYER_SELECTED "KEY_PLAYER_SELECTED"
#define USER_MAX_SCORE "KEY_MAX_SCORE"
#define USER_TOTAL_GAMES_PLAYED "KEY_TOTAL_GAMES_PLAYED"
#define USER_TOTAL_SCORE "KEY_TOTAL_SCORE"

// Notifications
#define NOTIFICATION_PAUSE_GAME "NOT_PAUSE_GAME"
#define NOTIFICATION_RESUME_GAME "NOT_RESUME_GAME"
#define NOTIFICATION_PLAY_AGAIN "NOT_PLAY_AGAIN"
#define NOTIFICATION_GO_HOME "NOT_GO_HOME"

// Musics and SFX
#define BG_MUSIC_01 "bg_music.mp3"
#define SFX_JUMP "jump.mp3"
#define SFX_SMASH "smash.mp3"
#define SFX_SWOOSH "swoosh.mp3"

// Game constants
#define MAX_PARALLAX 5
#define START_X_OBSTACLES designResolutionSize.width * 2.0f
#define START_WORLD_SPEED 250.0f
#define MAX_OBSTACLES 10
#define MIN_DISTANCE_OBSTACLES 200
#define DRAW_COLLISIONS false

#endif
