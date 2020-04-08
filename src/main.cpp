/*this code was written by creckeryop*/
#include "vitasdk.h"
#include "vita2d.h"
#include "core.h"
#include "utils.h"
#include <stack>
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_speech.h"
#define click(key) (pad.buttons & key && !(oldpad.buttons & key))
#define release(key) (oldpad.buttons & key && !(pad.buttons & key))
#define hold(key) (pad.buttons & key)
vita2d_texture* sprites = NULL;
bool start_flag = false, butt_flag = false;
float fade = 1, tap_fade = 0, blink = 0;
long long death_timer = get_time();
#define get_num_width(num) (num == 1 ? 8 : 12)
void draw_score(float offset_x)
{
    int len = 1, score = get_score(), score_width = 0;
    while (true)
    {
        score_width += get_num_width(score % 10) * 2 - 2;
        if (score > 9)
            len++, score/=10;
        else
            break;
    }
    float x = 144 - score_width / 2;
    score = get_score();
    std::stack<int> nums;
    while (len--) { nums.push(score % 10); score /= 10;}
    while (!nums.empty())
    {
        vita2d_draw_texture_part_scale(sprites, offset_x + (int)x, 16 + 80, 292 + nums.top() * 13, 160, 12, 18, 2, 2);
        x += get_num_width(nums.top()) * 2 - 4;
        nums.pop();
    }
}
void draw_score_small(float offset_x, float x, float y, int n)
{
    int len = 1, score = n, score_width = 0;
    while (true)
    {
        score_width += 8 * 2;
        if (score > 9)
            len++, score/=10;
        else
            break;
    }
    score = n;
    while (len--) 
    { 
        int num = score % 10;
        x -= 8 * 2;
        vita2d_draw_texture_part_scale(sprites, offset_x + x, y, 291 + num * 8, 179, 7, 10, 2, 2);
        score /= 10;
    }
}
void draw_pipes(float offset_x, float d, float y)
{
    if (d - get_distance()-13 < 144)
    {
        vita2d_draw_texture_part_scale(sprites, offset_x - 26 + 2 * (d - get_distance()), y * 2 - 354, 514, 0, 26, 160, 2, 2);
        vita2d_draw_texture_part_scale(sprites, offset_x - 26 + 2 * (d - get_distance()), 66 + y * 2, 542, 0, 26, 160, 2, 2);
    }
}

void draw_game()
{
    vita2d_draw_texture_part_scale(sprites, 336, 16, 146*!get_day(), 0, 144, 256, 2, 2);
    for (int i = 0; i < 3; ++i)
        draw_pipes(336, pipes[i].x, pipes[i].y);
    int sprite = (get_time() / 80) % 4;
    if (sprite == 3 || BIRD.velocity > 1.5f) sprite = 1;
    sprite += BIRD.color + BIRD.color + BIRD.color;
    vita2d_draw_texture_part_scale_rotate(sprites, BIRD.position.x*2 + 336, 16 + BIRD.position.y+BIRD.position.y,295 + 28 * sprite,242,19,14,2,2,BIRD.rot);
    vita2d_draw_texture_part_scale(sprites, 336-mod(get_distance()+get_distance(),288), 416, 292, 0, 144, 56, 2, 2);
    vita2d_draw_texture_part_scale(sprites, 624-mod(get_distance()+get_distance(),288), 416, 292, 0, 144, 56, 2, 2);
    if (get_state() != state_menu && get_time() - death_timer<1000) draw_score(336);
    static int n = 0;
    if (get_time() - death_timer>1000)
    {
        float temp = (get_time() - death_timer)>1300?1:(get_time() - death_timer - 1000)/300.0f;
        vita2d_draw_texture_tint_part_scale(sprites,336+144-96,16+256-21-110-15*sin(temp*3.141592f),395,59,96,21,2,2,RGBA8(255,255,255,(int)(255*temp)));
        if (get_time() - death_timer>1600)
        {
            float temp = 1 - ((get_time() - death_timer)>2200?1:(get_time() - death_timer - 1600)/600.0f);
            vita2d_draw_texture_part_scale(sprites,336+144-113,16+256-57+(256+57)*(temp*temp*temp),397,180,113,57,2,2);
            int score = 0;
            if (get_time() - death_timer>2200)
            {
                float temp = (get_time() - death_timer)>2600?1:(get_time() - death_timer - 2200)/400.0f;
                score = get_score()*temp;
                if (get_time() - death_timer > 2600)
                {
                    vita2d_draw_texture_tint_part_scale(sprites,336+144-52,16+256-29+110,398,128,52,29,2,2,RGBA8(255-100*butt_flag,255-100*butt_flag,255-100*butt_flag,255));
                }
            }
            int medal = -1;
            if (get_score()>=10) medal = 0;
            if (get_score()>=20) medal = 1;
            if (get_score()>=30) medal = 2;
            if (get_score()>=40) medal = 3;
            if (medal>=0){
                vita2d_draw_texture_part_scale(sprites,336+144+113-20-180,16+256-57+(256+57)*(temp*temp*temp)+32+10,291+23*medal,198,22,22,2,2);
                static point blink = point(rand()%10-5,rand()%10-5);
                if ((get_time() - death_timer - 1000)/700 > n)
                {
                    n = (get_time() - death_timer - 1000)/700;
                    blink = point(rand()%10-5,rand()%10-5);
                    blink = blink / blink.length();
                    blink = blink * (rand()%22);
                }
                int sprite = 0;
                sprite = ((get_time()- death_timer-1000)/100)%7;
                if (sprite == 3) sprite = 1;
                if (sprite < 3)
                {
                    vita2d_draw_texture_part_scale(sprites,336+144+113-20-180+20+blink.x,16+256-57+(256+57)*(temp*temp*temp)+32+10+20+blink.y,384,94+sprite*26,5,5,2,2);
                }
            }
            draw_score_small(336,144+113-20,16+256-57+(256+57)*(temp*temp*temp)+32,score);
            draw_score_small(336,144+113-20,16+256-57+(256+57)*(temp*temp*temp)+32+42,score<get_record()?get_record():score);
            if (score > get_record())
                vita2d_draw_texture_part_scale(sprites,336+144+113-90,16+256-57+32+26,365,148,16,7,2,2);
        }
    }
    else
    {
        n = 0;
    }
    vita2d_draw_texture_tint_part_scale(sprites,336+144-92,16+256-25-90,295,59,92,25,2,2,RGBA8(255,255,255,(int)(255*tap_fade*tap_fade)));
    vita2d_draw_texture_tint_part_scale(sprites,336+144-57,16+256-33,292,91,57,49,2,2,RGBA8(255,255,255,(int)(255*tap_fade*tap_fade)));
    if (get_state() == state_menu)
    {
        vita2d_draw_texture_part_scale(sprites,336+144-89,16+256-25-120,408,96,89,25,2,2);
        vita2d_draw_texture_tint_part_scale(sprites,336+144-52,16+256-29+10,398,128,52,29,2,2,RGBA8(255-100*butt_flag,255-100*butt_flag,255-100*butt_flag,255));
    }
    if (fade > 0)
        vita2d_draw_rectangle(336,16,288,512,RGBA8(0,0,0,(int)(255*fade*fade)));
    if (blink > 0)
        vita2d_draw_rectangle(336,16,288,512,RGBA8(255,255,255,(int)(255*blink*blink)));
    vita2d_draw_rectangle(0, 0, 336, 544, RGBA8(0, 0, 0, 255));
    vita2d_draw_rectangle(624, 0, 336, 544, RGBA8(0, 0, 0, 255));
    vita2d_draw_rectangle(0, 0, 960, 16, RGBA8(0, 0, 0, 255));
    vita2d_draw_rectangle(0, 528, 960, 16, RGBA8(0, 0, 0, 255));
}
int main(int argc, char *argv[])
{
    SoLoud::Soloud gSoloud;
	SoLoud::Wav die;
	SoLoud::Wav hit;
	SoLoud::Wav point;
	SoLoud::Wav swooshing;
	SoLoud::Wav wing;
    gSoloud.init();
	die.load("app0:sounds/sfx_die.ogg");
	hit.load("app0:sounds/sfx_hit.ogg");
	point.load("app0:sounds/sfx_point.ogg");
	swooshing.load("app0:sounds/sfx_swooshing.ogg");
	wing.load("app0:sounds/sfx_wing.ogg");
    srand(get_time() & 0xFFFFFFFF);
	SceCtrlData pad, oldpad;
	vita2d_init();
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
    sprites = vita2d_load_PNG_file("app0:assets.png");
    vita2d_set_vblank_wait(1);
    float d = 1;
    int oldstate = -1;
    long long oldscore = 0;
    bool old_butt_flag = butt_flag;
    SceUID fs = sceIoOpen("ux0:data/flappy_bird.save", SCE_O_RDONLY | SCE_O_CREAT,0777);
	int size = sceIoLseek32(fs, 0, SCE_SEEK_END);
	sceIoLseek32(fs, 0, SCE_SEEK_SET);
	if (fs >= 0) {
		char *save = new char[256];
		sceIoRead(fs, save, size);
        load(atoi_m(save));
		sceIoClose(fs);
	}
	SceTouchData touch, oldtouch;
    sceTouchPeek(0, &oldtouch, 1);
    while (true)
    {
        long long d_t = get_time();
        sceCtrlPeekBufferPositive(0, &pad, 1);
        touch.report[0].x = 0;
        sceTouchPeek(0, &touch, 1);
        if (d>2.5f) d = 1;
        update(d);
        vita2d_start_drawing();
        vita2d_clear_screen();
        draw_game();
        vita2d_end_drawing();
        vita2d_swap_buffers();
        vita2d_wait_rendering_done();
        butt_flag = hold(SCE_CTRL_CROSS);
        butt_flag|=(get_state()==state_menu && touch.reportNum > 0 && touch.report[0].x/2>336+144-52 && touch.report[0].y/2>16+256-29+10 && touch.report[0].x/2<336+144+52 && touch.report[0].y/2<16+256+29+10);
        butt_flag|=(get_state()==state_death && touch.reportNum > 0 && touch.report[0].x/2>336+144-52 && touch.report[0].y/2>16+256-29+110 && touch.report[0].x/2<336+144+52 && touch.report[0].y/2<16+256+29+110);
        butt_flag|=(get_state()==state_menu && touch.reportNum > 1 && touch.report[1].x/2>336+144-52 && touch.report[1].y/2>16+256-29+10 && touch.report[1].x/2<336+144+52 && touch.report[1].y/2<16+256+29+10);
        butt_flag|=(get_state()==state_death && touch.reportNum > 1 && touch.report[1].x/2>336+144-52 && touch.report[1].y/2>16+256-29+110 && touch.report[1].x/2<336+144+52 && touch.report[1].y/2<16+256+29+110);
        bool pressed = !(pad.buttons & SCE_CTRL_CROSS) && oldpad.buttons & SCE_CTRL_CROSS;
        pressed |= (get_state()==state_menu && touch.reportNum < 1 && oldtouch.reportNum == 1 && oldtouch.report[0].x/2>336+144-52 && oldtouch.report[0].y/2>16+256-29+10 && oldtouch.report[0].x/2<336+144+52 && oldtouch.report[0].y/2<16+256+29+10);
        pressed |= (get_state()==state_death && touch.reportNum < 1 && oldtouch.reportNum == 1 && oldtouch.report[0].x/2>336+144-52 && oldtouch.report[0].y/2>16+256-29+110 && oldtouch.report[0].x/2<336+144+52 && oldtouch.report[0].y/2<16+256+29+110);
        pressed |= (get_state()==state_menu && touch.reportNum < 2 && oldtouch.reportNum == 2 && oldtouch.report[1].x/2>336+144-52 && oldtouch.report[1].y/2>16+256-29+10 && oldtouch.report[1].x/2<336+144+52 && oldtouch.report[1].y/2<16+256+29+10);
        pressed |= (get_state()==state_death && touch.reportNum < 2 && oldtouch.reportNum == 2 && oldtouch.report[1].x/2>336+144-52 && oldtouch.report[1].y/2>16+256-29+110 && oldtouch.report[1].x/2<336+144+52 && oldtouch.report[1].y/2<16+256+29+110);
        if (blink == 1)
        {
            gSoloud.play(hit);
            SceUID fs = sceIoOpen("ux0:data/flappy_bird.save", SCE_O_WRONLY | SCE_O_CREAT, 0777);
            if (fs >= 0) {
                char buff[64];
                for (int i=0;i<64;i++) buff[i] = '\0';
                itoa_m(get_score()>get_record()?get_score():get_record(), buff);
                sceIoWrite(fs, buff, 64);
                sceIoClose(fs);
                d_t = get_time();
            }
        }
        if (start_flag)
        {
            fade += d * 0.05f;
            if (fade >= 1)
            {
                fade = 1;
                tap(1);
                start_flag = false;
            }
        }
        else
        {
            fade -= d*0.05f;
            if (fade <= 0)
                fade = 0;
        }
        blink -= d*0.05f;
        if (blink<=0)
            blink = 0;
        static bool played_sound1 = false;
        static bool played_sound2 = false;
        if (get_state() != state_death)
        {
            played_sound1 = false;
            played_sound2 = false;
            death_timer = get_time();
        }
        switch (get_state())
        {
            case state_death:
            if (!played_sound1 && get_time()-death_timer>500)
            {
                played_sound1 = true;
                gSoloud.play(die);
            }
            if (!played_sound2 && get_time()-death_timer>1700)
            {
                played_sound2 = true;
                gSoloud.play(swooshing);
            }
            if (oldstate != state_death)
                blink = 1;
            case state_menu:
            if (pressed)
            {
                if (get_time() - death_timer>2600 && get_state() == state_death || get_state()==state_menu)
                {
                    start_flag = true;
                    gSoloud.play(swooshing);
                }
            }
            break;
            case state_play:
                tap_fade -= d*0.05f;
                if (tap_fade < 0)
                    tap_fade = 0;
                if ((click(SCE_CTRL_CROSS) || touch.reportNum > oldtouch.reportNum))
                {
                    tap(1);
                    gSoloud.play(wing);
                }
            break;
            case state_tap:
                tap_fade += d*0.05f;
                if (tap_fade >= 1){
                    tap_fade = 1;
                    if ((click(SCE_CTRL_CROSS) || touch.reportNum > oldtouch.reportNum))
                    {
                        tap(1);
                        gSoloud.play(wing);
                    }
                }
            break;
        }
        if (oldscore<get_score())
            gSoloud.play(point);
        oldpad = pad;
        oldtouch = touch;
        oldscore = get_score();
        oldstate = get_state();
        old_butt_flag = butt_flag;
        d = (get_time() - d_t) / (1000/60.f);
    }
    vita2d_free_texture(sprites);
    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}