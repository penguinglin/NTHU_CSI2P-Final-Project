#include <allegro5/allegro_primitives.h>
#include "menu.h"
#include <stdbool.h>
#define BUTTON_NUM 4
/*
   [Menu function]
*/

Menu::Menu(int label) : Scene(label)
{
     // setting derived object member
    //字體設定(未使用)
    font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    title_x = WIDTH / 2;
    title_y = HEIGHT / 2;
    
    background_image = al_load_bitmap("assets/image/start.png"); //【設定菜單背景(要跟遊戲視窗設定的大小相等，沒辦法縮放)】

    //【設定按鈕圖案】
    button[0] = al_load_bitmap("assets/image/Menu/Start.png"); //開始
    hightlight_button[0] = al_load_bitmap("assets/image/Menu/Start_H.png");
    button_W[0] = al_get_bitmap_width(button[0]);
    button_H[0] = al_get_bitmap_height(button[0]);
    buttonH_W[0] = al_get_bitmap_width(hightlight_button[0]);
    buttonH_H[0] = al_get_bitmap_height(hightlight_button[0]);


    button[1] = al_load_bitmap("assets/image/Menu/Setting.png"); //設定
    hightlight_button[1] = al_load_bitmap("assets/image/Menu/Setting_H.png");
    button_W[1] = al_get_bitmap_width(button[1]);
    button_H[1] = al_get_bitmap_height(button[1]);
    buttonH_W[1] = al_get_bitmap_width(hightlight_button[1]);
    buttonH_H[1] = al_get_bitmap_height(hightlight_button[1]);

    button[2] = al_load_bitmap("assets/image/Menu/About.png"); //說明
    hightlight_button[2] = al_load_bitmap("assets/image/Menu/About_H.png");    
    button_W[2] = al_get_bitmap_width(button[2]);
    button_H[2] = al_get_bitmap_height(button[2]);
    buttonH_W[2] = al_get_bitmap_width(hightlight_button[2]);
    buttonH_H[2] = al_get_bitmap_height(hightlight_button[2]);

    button[3] = al_load_bitmap("assets/image/Menu/Quit.png"); //離開
    hightlight_button[3] = al_load_bitmap("assets/image/Menu/Quit_H.png");
    button_W[3] = al_get_bitmap_width(button[3]);
    button_H[3] = al_get_bitmap_height(button[3]);
    buttonH_W[3] = al_get_bitmap_width(hightlight_button[3]);
    buttonH_H[3] = al_get_bitmap_height(hightlight_button[3]);

    //設定按鈕放置位置
    int increase = 360;
    for(int i = 0 ; i < BUTTON_NUM ; i++){
        X[i] = (WIDTH/2) - (button_W[i]/2); //X軸置中放置
        Y[i] = increase;

        XH[i] = (WIDTH/2) - (buttonH_W[i]/2); //X軸置中放置
        YH[i] = increase-5;

        increase += 180;
    }

    //設定按鈕音效
    ButtonClick = al_load_sample("assets/sound/button_press_sound.wav");
    al_reserve_samples(20);
    Click_sample_instance = al_create_sample_instance(ButtonClick);
    al_set_sample_instance_playmode(Click_sample_instance, ALLEGRO_PLAYMODE_ONCE);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(Click_sample_instance, al_get_default_mixer());
    //設定音效音量
    al_set_sample_instance_gain(Click_sample_instance, 1);

    // Load sound
    song = al_load_sample("assets/sound/menu/menu.wav");
    al_reserve_samples(20);
    sample_instance = al_create_sample_instance(song);
    // Loop the song until the display closes
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_ONCE);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(sample_instance, 0.3);
    al_play_sample_instance(sample_instance); //放在這裡的話，在還沒有跳轉頁面之前，只會發出一次聲音

    //一進來初始化先把滑鼠點擊狀態清空
    mouse_state[1] = false;

    click = 0;

    timerF = 0; //時間歸零
}

Menu::~Menu()
{
    //Destructor
}

void Menu::Update()
{
    DetectButtonOn();
    if (mouse_state[1] && (window == 0)) //檢查滑鼠左鍵按下的當下是否在按鈕上(且在主選單的介面)
    {
        if(over_button[0]){ //進入遊戲(1)
            al_play_sample_instance(Click_sample_instance);
            printf("Play\n");
            click = 1;
        }
        if(over_button[1]){ //調整設定(2)
            al_play_sample_instance(Click_sample_instance);
            printf("Set\n");
             click = 1;
        }
        if(over_button[2]){ //觀看說明(3)
            al_play_sample_instance(Click_sample_instance);
            printf("read\n");
            click = 1;
        }
        if(over_button[3]){ //關閉遊戲(4)
            //如果滑鼠按下的時候在按鈕上 -> 準備要離開
            al_play_sample_instance(Click_sample_instance);
            printf("Quit\n");
             click = 1;
        }
    }
    else{
        if(over_button[0] && click){ //進入遊戲(1)
             SetSceneEnd(true);
            window = 1;
        }
        if(over_button[1] && click){ //調整設定(2)
            SetSceneEnd(true);
            window = 2;
        }
        if(over_button[2] && click){ //觀看說明(3)
            SetSceneEnd(true);
            window = 3;
        }
        if(over_button[3] && click){ //關閉遊戲(4)
            SetSceneEnd(true);
            window = 4;
        }
    }
    
    return;
}

void Menu::Draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    printf("in menu: %d\n", al_get_bitmap_width(background_image));

    al_draw_bitmap(background_image, 0, 0, 0);

    //依照狀態畫出四個按鈕
    DetectButtonOn();
}

void Menu::DetectButtonOn(){ //檢查滑鼠是否停在按鈕上，並畫出對應狀態的圖案
    for(int i = 0 ; i < BUTTON_NUM ; i++){
        if((mouse.x >= X[i])&&(mouse.x <= X[i]+button_W[i])&&(mouse.y >= Y[i])&&(mouse.y <= Y[i]+button_H[i])){ //如果滑鼠在按鈕範圍內 -> 畫出highlight樣式
            al_draw_bitmap(hightlight_button[i], XH[i], YH[i], 0);
            over_button[i] = true;
        }
        else{ //如果滑鼠不在按鈕範圍內 -> 畫出普通樣式
            al_draw_bitmap(button[i], X[i], Y[i], 0);
            over_button[i] = false;
        }
    }
}

void Menu::Destroy()
{
     ALLEGRO_BITMAP *background = background_image;
    al_destroy_bitmap(background); //【destory背景圖片(先宣告才能刪除)】
    al_destroy_bitmap(button[0]);
    al_destroy_bitmap(button[1]);
    al_destroy_bitmap(button[2]);
    al_destroy_bitmap(button[3]);
    al_destroy_bitmap(hightlight_button[0]);
    al_destroy_bitmap(hightlight_button[1]);
    al_destroy_bitmap(hightlight_button[2]);
    al_destroy_bitmap(hightlight_button[3]);
    al_destroy_font(font);
    al_destroy_sample(song);
    al_destroy_sample_instance(sample_instance);
    al_destroy_sample(ButtonClick);
    al_destroy_sample_instance(Click_sample_instance);
}