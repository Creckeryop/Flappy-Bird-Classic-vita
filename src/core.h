/*this code was written by creckeryop*/
#pragma once
#include "utils.h"
#include "collision.h"

enum states
{
    state_intro, state_menu, state_tap, state_play, state_death
};

class bird : public obj
{
public:
    float rot = 0;
    int color = 0;
    float velocity = 0;
    bird();
    float rotation();
    ~bird() = default;
};

struct pipe
{
    int x = -100, y = 0;
    pipe() = default;
    pipe(int x, int y);
};

extern pipe pipes[3];
extern bird BIRD;

bool get_day();
void load(long long score);
long long save();
long long get_score();
long long get_state();
long long get_record();
float get_distance();
void update(float d = 1);
void tap(int t = 1);