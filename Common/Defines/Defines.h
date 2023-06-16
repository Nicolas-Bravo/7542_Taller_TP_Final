#ifndef COMMON_DEFINES_H_
#define COMMON_DEFINES_H_

#define PATH "assets/config/config.yaml"
#define WINDOW_WIDTH 1920 * 2
#define WINDOW_HEIGTH 1080

#define FRAME_RATE 60
#define SIZE_QUEUE 256

#define CREATE_MENU 1
#define JOIN_MENU 2

// Animation SDL
#define SIZE_FRAME 128
#define SIZE_FRAME_ENEMY 96
#define SIZE_SPRITE_X 64
#define SIZE_SPRITE_Y 68
#define SPEED_IDLE 150
#define SPEED_RUN 100

#define CREATE_CODE 0x01
#define JOIN_CODE 0x02
#define START_CODE 0x03
#define PLAYING_CODE 0x04
#define MOVE_CODE 0x05
#define STOP_MOVE_CODE 0x06
#define THROW_SMOKE_CODE 0x07
#define STOP_SMOKE_CODE 0x08
#define THROW_GRENADE_CODE 0x09
#define STOP_GRENADE_CODE 0x10
#define BLITZ_ATACK_CODE 0x11
#define SHOOT_CODE 0x12
#define STOP_SHOOT_CODE 0x13
#define LEAVE_CODE 0x14
#define END_CODE 0x15
#define STATS_CODE 0x16

#define IDF_CODE 0x01
#define P90_CODE 0x02
#define SCOUT_CODE 0x03

#define SURVIVAL_CODE 0x01
#define CLEAR_ZONE_CODE 0x02

#define RIGHT_CODE 0x01
#define LEFT_CODE 0x02
#define UP_CODE 0x03
#define DOWN_CODE 0x04

#define STATE_IDLE 0x00
#define STATE_MOVING 0x01
#define STATE_ATACK 0x02
#define STATE_INJURE 0x03
#define STATE_HABILITY 0x04
#define STATE_RECHARGE 0x05

#define INFECTED_ZOMBIE 0x00
#define INFECTED_JUMPER 0x01
#define INFECTED_WITCH 0x02
#define INFECTED_SPEAR 0x03
#define INFECTED_VENOM 0x04

// Server
#define INITIAL_PLAYERS_AMOUNT 0x00
#define INITIAL_PLAYER_ID 0x00
#define INITIAL_INFECTED_ID 50
#define INITIAL_OBSTACLE_ID 240

#define OBSTACLE_TIRE 0x00
#define OBSTACLE_CRATER 0x01

#define CLOSE_DISTANCE 50


#endif
