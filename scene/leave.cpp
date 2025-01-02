#include <allegro5/allegro_primitives.h>
#include "leave.h"
#include <stdbool.h>
#define BUTTON_NUM 2
/*
   [Leave function]
*/

Leave::Leave(int label) : Scene(label)
{
    // setting derived object member
    font = al_load_ttf_font("assets/font/XiaolaiMonoSC-Regular.ttf", 24, 0);
    title_x = WIDTH / 2;
    title_y = HEIGHT / 2;
 
    background_image = al_load_bitmap("assets/image/sad.png"); //【設定背景】

    //【設定按鈕圖案】
    button[0] = al_load_bitmap("assets/image/Menu/Back.png"); //回到菜單
    hightlight_button[0] = al_load_bitmap("assets/image/Menu/Back_H.png");
    button_W[0] = al_get_bitmap_width(button[0]);
    button_H[0] = al_get_bitmap_height(button[0]);
    buttonH_W[0] = al_get_bitmap_width(hightlight_button[0]);
    buttonH_H[0] = al_get_bitmap_height(hightlight_button[0]);

    button[1] = al_load_bitmap("assets/image/Menu/Leave.png"); //離開
    hightlight_button[1] = al_load_bitmap("assets/image/Menu/Leave_H.png");
    button_W[1] = al_get_bitmap_width(button[1]);
    button_H[1] = al_get_bitmap_height(button[1]);
    buttonH_W[1] = al_get_bitmap_width(hightlight_button[1]);
    buttonH_H[1] = al_get_bitmap_height(hightlight_button[1]);

    //設定按鈕位置
    int increase = 150;
    int adjust = 0;
    for(int i = 0 ; i < BUTTON_NUM ; i++){
        X[i] = increase;
        Y[i] = 700+adjust;

        XH[i] = increase-10;
        YH[i] = 700+adjust;

        increase += 700;
        adjust += 20;
    }

    //設定按鈕音效
    ButtonClick = al_load_sample("assets/sound/menu/leave_back_button.wav");
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

Leave::~Leave()
{
    //Destructor
}

void Leave::Update()
{
    DetectButtonOn();
    if (mouse_state[1]) //檢查滑鼠左鍵按下的當下是否在按鈕上
    {
        if(over_button[0]){ //回到主畫面
            al_play_sample_instance(Click_sample_instance);
            printf("From quit back to menu\n");
            click = 1;
        }
        if(over_button[1]){ //離開遊戲
            printf("Finish quit\n");
            //讓事件更新回到讀取新的event時，可以跳出主要的game loop
            click = 1;
        }
    }
    else{
        if(over_button[0] && click){ //回到主畫面
           SetSceneEnd(true);
            window = 0;
        }
        if(over_button[1] && click){ //離開遊戲
            printf("Finish quit\n");
            CloseGame = 1;
            //讓事件更新回到讀取新的event時，可以跳出主要的game loop
        }
    }

    return;
}

void Leave::Draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    printf("in leave: %d\n", al_get_bitmap_width(background_image));

    al_draw_bitmap(background_image, 0, 0, 0);
    //【在顯示選單按鈕前，先畫出背景才不會被覆蓋】

    //依照狀態畫出四個按鈕
    DetectButtonOn();

}

void Leave::DetectButtonOn(){
    for(int i = 0 ; i < BUTTON_NUM ; i++){
        if((mouse.x >= X[i])&&(mouse.x <= X[i]+button_W[i])&&(mouse.y >= Y[i])&&(mouse.y <= Y[i]+button_H[i])){ //如果滑鼠在按鈕範圍內
            al_draw_bitmap(hightlight_button[i], XH[i], YH[i], 0);
            over_button[i] = true;
        }
        else{
            al_draw_bitmap(button[i], X[i], Y[i], 0);
            over_button[i] = false;
        }
    }
}

void Leave::Destroy()
{
    ALLEGRO_BITMAP *background = background_image;
    al_destroy_bitmap(background); //【destory背景圖片(先宣告才能刪除)】
    al_destroy_bitmap(button[0]);
    al_destroy_bitmap(button[1]);
    al_destroy_bitmap(hightlight_button[0]);
    al_destroy_bitmap(hightlight_button[1]);
    al_destroy_font(font);
    al_destroy_sample(ButtonClick);
    al_destroy_sample_instance(Click_sample_instance);
}