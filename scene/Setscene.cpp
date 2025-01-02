#include <allegro5/allegro_primitives.h>
#include "Setscene.h"
#include <stdbool.h>
#define MODENUM 3
/*
   [Setting function]
*/
Setting::Setting(int label) : Scene(label)
{
    // setting derived object member
    font = al_load_ttf_font("assets/font/GenSenRounded-M.ttc", 24, 0);
    title_x = WIDTH / 2;
    title_y = HEIGHT / 2;

    background_image = al_load_bitmap("assets/image/SetTest.png"); //設定背景

    //【設定按鈕圖案】
    button[0] = al_load_bitmap("assets/image/Menu/On.png"); //On
    hightlight_button[0] = al_load_bitmap("assets/image/Menu/On_H.png");
    button_W[0] = al_get_bitmap_width(button[0]);
    button_H[0] = al_get_bitmap_height(button[0]);
    buttonH_W[0] = al_get_bitmap_width(hightlight_button[0]);
    buttonH_H[0] = al_get_bitmap_height(hightlight_button[0]);

    button[1] = al_load_bitmap("assets/image/Menu/Off.png"); //Off
    hightlight_button[1] = al_load_bitmap("assets/image/Menu/Off_H.png");
    button_W[1] = al_get_bitmap_width(button[1]);
    button_H[1] = al_get_bitmap_height(button[1]);
    buttonH_W[1] = al_get_bitmap_width(hightlight_button[1]);
    buttonH_H[1] = al_get_bitmap_height(hightlight_button[1]);

    backbutton = al_load_bitmap("assets/image/Menu/BackIconClose.png"); //back
    highlight_backbutton = al_load_bitmap("assets/image/Menu/BackIconOpen.png");
    backbutton_W = al_get_bitmap_width(backbutton);
    backbutton_H = al_get_bitmap_height(backbutton);
    backbuttonH_W = al_get_bitmap_width(highlight_backbutton);
    backbuttonH_H = al_get_bitmap_height(highlight_backbutton);

    //設定按鈕放置位置(3種模式所以3個)
    int increase = 150;
    for(int i = 0 ; i < MODENUM ; i++){
        X[i] = 800;
        Y[i] = increase;
        XH[i] = 800;
        YH[i] = increase-10;

        increase += 300;
    }
    //回到菜單的按鈕位置設定
    backX = 1250;
    backY = 50;
    backXH = 1250;
    backYH = 50;

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

Setting::~Setting()
{
    //Destructor
}

void Setting::Update()
{
    Setting_DetectButtonOn();
    if (mouse_state[1]) //檢查滑鼠左鍵按下的當下是否在按鈕上
    {
        if(over_button[0]){ //開啟第一模式，其他兩個模式自動關閉
            al_play_sample_instance(Click_sample_instance);
            ModeCondition[0] = true;
            ModeCondition[1] = false;
            ModeCondition[2] = false;
        }
        if(over_button[1]){ //開啟第二模式，其他兩個模式自動關閉
            al_play_sample_instance(Click_sample_instance);
            ModeCondition[0] = false;
            ModeCondition[1] = true;
            ModeCondition[2] = false;
        }
        if(over_button[2]){ //開啟第三模式，其他兩個模式自動關閉
            al_play_sample_instance(Click_sample_instance);
            ModeCondition[0] = false;
            ModeCondition[1] = false;
            ModeCondition[2] = true;
        }

        if(over_backbutton){
            al_play_sample_instance(Click_sample_instance);
            printf("Back to menu\n");
             click = 1;
        }
    }
    else{
        if(over_backbutton && click){
            SetSceneEnd(true);
            window = 0;
        }
    }
    
    return;
}

void Setting::Draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    printf("in Setting: %d\n", al_get_bitmap_width(background_image));

    al_draw_bitmap(background_image, 0, 0, 0); //【先畫出背景才不會被覆蓋】


    Setting_DetectButtonOn(); //畫完正常按鈕後，檢查滑鼠是否停在按鈕上，並更改狀態
}

void Setting::Setting_DetectButtonOn(){
    for(int i = 0 ; i < MODENUM ; i++){
        if((mouse.x >= X[i])&&(mouse.x <= X[i]+button_W[i])&&(mouse.y >= Y[i])&&(mouse.y <= Y[i]+button_H[i])){ //如果滑鼠在按鈕範圍內
            if(ModeCondition[i]){ //根據這個模式的狀態是on/off來決定畫出來的按鈕
                //是On -> 畫出On的highlight樣式
                al_draw_bitmap(hightlight_button[0], XH[i], YH[i], 0);
                over_button[i] = true;
            }
            else{
                //是Off -> 畫出Off的highlight樣式
                al_draw_bitmap(hightlight_button[1], XH[i], YH[i], 0);
                over_button[i] = true;
            }
        }
        else{
            if(ModeCondition[i]){ //根據這個模式的狀態是on/off來決定畫出來的按鈕
                //是On -> 畫出On的highlight樣式
                al_draw_bitmap(button[0], X[i], Y[i], 0);
                over_button[i] = false;
            }
            else{
                //是Off -> 畫出Off的highlight樣式
                al_draw_bitmap(button[1], X[i], Y[i], 0);
                over_button[i] = false;
            }
        }
    }

    if((mouse.x >= backX)&&(mouse.x <= backX+backbutton_W)&&(mouse.y >= backY)&&(mouse.y <= backY+backbutton_H)){ //如果滑鼠在按鈕範圍內
        al_draw_bitmap(highlight_backbutton, backXH, backYH, 0);
        over_backbutton = true;
    }
    else{
        al_draw_bitmap(backbutton, backX, backY, 0);
        over_backbutton = false;
    }

}

void Setting::Destroy()
{
    ALLEGRO_BITMAP *background = background_image;
    al_destroy_bitmap(background); //【destory背景圖片(先宣告才能刪除)】
    al_destroy_bitmap(button[0]);
    al_destroy_bitmap(hightlight_button[0]);
    al_destroy_bitmap(button[1]);
    al_destroy_bitmap(hightlight_button[1]);
    al_destroy_bitmap(backbutton);
    al_destroy_bitmap(highlight_backbutton);
    al_destroy_font(font);
    al_destroy_sample(ButtonClick);
    al_destroy_sample_instance(Click_sample_instance);
}