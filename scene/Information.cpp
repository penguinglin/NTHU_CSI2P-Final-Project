#include <allegro5/allegro_primitives.h>
#include "Information.h"
#include <stdbool.h>

/*
   [Info function]
*/
Info::Info(int label) : Scene(label) {
    // setting derived object member
    font = al_load_ttf_font("assets/font/GenSenRounded-M.ttc", 24, 0);
    title_x = WIDTH / 2;
    title_y = HEIGHT / 2;

    background_image = al_load_bitmap("assets/image/info.png"); //設定背景

    //【設定按鈕圖案】
    button = al_load_bitmap("assets/image/Menu/BackIconClose.png"); //回到菜單按鈕
    hightlight_button = al_load_bitmap("assets/image/Menu/BackIconOpen.png");
    button_W = al_get_bitmap_width(button);
    button_H = al_get_bitmap_height(button);
    buttonH_W = al_get_bitmap_width(hightlight_button);
    buttonH_H = al_get_bitmap_height(hightlight_button);

    //設定按鈕放置位置
    X = 1250;
    Y = 50;
    XH = 1245;
    YH = 50;

    //設定按鈕音效
    ButtonClick = al_load_sample("assets/sound/button_press_sound.wav");
    al_reserve_samples(20);
    Click_sample_instance = al_create_sample_instance(ButtonClick);
    al_set_sample_instance_playmode(Click_sample_instance, ALLEGRO_PLAYMODE_ONCE);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(Click_sample_instance, al_get_default_mixer());
    //設定音效音量
    al_set_sample_instance_gain(Click_sample_instance, 1);
    
    //一進來初始化先把滑鼠點擊狀態清空
    mouse_state[1] = false;

    click = 0;
}

Info::~Info() {
    //Destructor
}


void Info::Update() {
    DetectButtonOn();
    if (mouse_state[1]) //檢查滑鼠左鍵按下的當下是否在按鈕上
    {
        if(over_button){ //回到主選單(0)
            al_play_sample_instance(Click_sample_instance);
            printf("back to menu\n");
            click = 1;
        }
    }
    else{
         if(over_button && click){ //回到主選單(0)
            SetSceneEnd(true);
            window = 0;
        }
    }
    
    return;
}

void Info::Draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    printf("in Info: %d\n", al_get_bitmap_width(background_image));

    al_draw_bitmap(background_image, 0, 0, 0); //【先畫出背景才不會被覆蓋】

    DetectButtonOn(); //畫完正常按鈕後，檢查滑鼠是否停在按鈕上，並更改狀態
}

void Info::DetectButtonOn(){
    if((mouse.x >= X)&&(mouse.x <= X+button_W)&&(mouse.y >= Y)&&(mouse.y <= Y+button_H)){ //如果滑鼠在按鈕範圍內
        al_draw_bitmap(hightlight_button, XH, YH, 0);
        over_button = true;
    }
    else{
        al_draw_bitmap(button, X, Y, 0);
        over_button = false;
    }
}

void Info::Destroy() {
    ALLEGRO_BITMAP *background = background_image;
    al_destroy_bitmap(background); //【destory背景圖片(先宣告才能刪除)】
    al_destroy_bitmap(button);
    al_destroy_bitmap(hightlight_button);
    al_destroy_font(font);
    al_destroy_sample_instance(Click_sample_instance);
    al_destroy_sample(ButtonClick);
}