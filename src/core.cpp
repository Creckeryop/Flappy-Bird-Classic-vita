/*this code was written by creckeryop*/
#include "core.h"
#include <cmath>
bool day = true;
long long score = 0;
long long record = 0;
int state = state_menu;
float distance = 0;
float distance_offset = 0;
pipe pipes[3] = { pipe(320, rand() % 100 + 50), pipe(400, rand() % 100 + 50), pipe(480, rand() % 100 + 50)};
bird BIRD;
bird::bird() : obj(72, 96, 14, 10), color(0) { }
float bird::rotation()
{
    return rot;
}
pipe::pipe(int x, int y) : x(x), y(y) {};


bool get_day()
{
    return day;
}

void load(long long score)
{
    record = score;
}

long long save()
{
    return record;
}

long long get_state()
{
    return state;
}

long long get_score()
{
    return score;
}

long long get_record()
{
    return record;
}

float get_distance()
{
    return distance;
}

void update(float d)
{
    switch(state)
    {
        case state_intro:
        break;
        case state_menu:
            distance += d * 1.2f;
            if (distance > 144) 
                distance -= 144;

            BIRD.position.x = 72;
            BIRD.position.y = 100 + sin(8 * 3.141592f * distance / 144);
            BIRD.velocity = 0;
            BIRD.rot = 0;

        break;
        case state_tap:
            distance += d * 1.2f;
            if (distance > 144)
                distance -= 144;
            
            BIRD.position.y = 128 + sin(8 * 3.141592f * distance / 144);
            BIRD.velocity = 0;
            BIRD.rot = 0;
            BIRD.position.x = 36;
        break;
        case state_play:
            distance += d * 1.2f;
            if (distance >= 160 + 36 + distance_offset)
                score = ((int)distance - 160 - 36 - distance_offset) / 80;
            else
                score = 0;
            if (distance > pipes[0].x + 13)
            {
                pipes[0] = pipes[1];
                pipes[1] = pipes[2];
                pipes[2] = pipe(pipes[2].x + 80, rand() % 100 + 50);
            }
            {
                obj up_pipe(pipes[0].x, pipes[0].y - 25 - 80, 26, 160);
                obj dw_pipe(pipes[0].x, pipes[0].y + 25 + 80, 26, 160);
                BIRD.points[0] = point(-7, -5);
                BIRD.points[1] = point(7, -5);
                BIRD.points[2] = point(7, 5);
                BIRD.points[3] = point(-7, 5);
                BIRD.rotate(BIRD.rot);
                BIRD.position.x = distance + 36;
                point res = res_vec(&BIRD, &up_pipe);
                point res2 = res_vec(&up_pipe, &BIRD);
                if (res.x == 0 & res.y == 0 & res2.x == 0 & res2.y == 0)
                {
                    res = res_vec(&BIRD, &dw_pipe);
                    res2 = res_vec(&dw_pipe, &BIRD);
                }
                if ((res.x != 0 || res.y != 0) & (res2.x !=0 || res2.y!=0))
                {
                    state = state_death;
                }
                BIRD.position.x = 36;
            }
        case state_death:
            {
                float new_rot = BIRD.velocity <= 0.6f ? -3.141592f / 8 : (pow(BIRD.velocity - 0.6f, 3) / 24 - 3.141592f / 8);
                if (new_rot > 3.141592f / 2) new_rot = 3.141592f / 2;
                BIRD.rot -= (BIRD.rot - new_rot) / 4;
                BIRD.position.y += BIRD.velocity * d;
                BIRD.velocity += 0.15f * d;
                if (BIRD.velocity > 6)
                    BIRD.velocity = 6;
                if (BIRD.position.y > 195)
                {
                    state = state_death;
                    BIRD.position.y = 195;
                }
            }
        break;
        default:
        break;
    }
}

void tap(int t)
{
    if (t == 0) return;
    switch(state)
    {
        case state_tap:
            distance_offset = distance;
            pipes[0] = pipe(320 + distance_offset, rand() % 100 + 50);
            pipes[1] = pipe(400 + distance_offset, rand() % 100 + 50);
            pipes[2] = pipe(480 + distance_offset, rand() % 100 + 50);
            BIRD.velocity = -2.6f;
            state = state_play;
            break;
        case state_play:
            if (BIRD.position.y > 0)
                BIRD.velocity = -2.6f;
            break;
        case state_menu:
            srand(get_time() & 0xFFFFFFFF);
        case state_death:
            day = rand() % 2;
            BIRD.color = rand() % 3;;
            state = state_tap;
            if (score > record)
                record = score;
            score = 0;
            break;
        default:
            break;
    }
}