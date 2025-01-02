#include "gamescene.h"
#include "scene.h"
#define BUTTON_NUM 4
/*
   [GameScene function]
*/

GameScene::GameScene(int label) : Scene(label)
{
    // setting derived object member
    // 設定字體
    font = al_load_ttf_font("assets/font/GenSenRounded-M.ttc", 32, 0);
    title_x = 400;
    title_y = 60;
    // 設定背景圖片
    background = al_load_bitmap("assets/image/background.png");
    // 設定音樂
    BGM = al_load_sample("assets/sound/game/GameSceneBGM.wav");
    al_reserve_samples(20);
    sample_instance = al_create_sample_instance(BGM);
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    // 設定BGM音量
    al_set_sample_instance_gain(sample_instance, 0.5);

    // 設定按鈕音效
    ButtonClick = al_load_sample("assets/sound/button_press_sound.wav");
    al_reserve_samples(20);
    Click_sample_instance = al_create_sample_instance(ButtonClick);
    al_set_sample_instance_playmode(Click_sample_instance, ALLEGRO_PLAYMODE_ONCE);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(Click_sample_instance, al_get_default_mixer());
    // 設定音效音量
    al_set_sample_instance_gain(Click_sample_instance, 1);

    // 設定遊戲主畫面的功能鍵&金錢條(先暫時用圖片)
    // 圖鑑[0]、商店[1]、訂單[2]、回到主選單[3]
    button[0] = al_load_bitmap("assets/image/MainGame/BookIconClose.png");
    highlight[0] = al_load_bitmap("assets/image/MainGame/BookIconOpen.png");
    button_w[0] = al_get_bitmap_width(button[0]);
    button_h[0] = al_get_bitmap_height(button[0]);
    buttonH_w[0] = al_get_bitmap_width(highlight[0]);
    buttonH_h[0] = al_get_bitmap_height(highlight[0]);

    button[1] = al_load_bitmap("assets/image/MainGame/ShopIconClose.png");
    highlight[1] = al_load_bitmap("assets/image/MainGame/ShopIconOpen.png");
    button_w[1] = al_get_bitmap_width(button[1]);
    button_h[1] = al_get_bitmap_height(button[1]);
    buttonH_w[1] = al_get_bitmap_width(highlight[1]);
    buttonH_h[1] = al_get_bitmap_height(highlight[1]);

    button[2] = al_load_bitmap("assets/image/MainGame/OrderIconClose.png");
    highlight[2] = al_load_bitmap("assets/image/MainGame/OrderIconOpen.png");
    button_w[2] = al_get_bitmap_width(button[2]);
    button_h[2] = al_get_bitmap_height(button[2]);
    buttonH_w[2] = al_get_bitmap_width(highlight[2]);
    buttonH_h[2] = al_get_bitmap_height(highlight[2]);

    button[3] = al_load_bitmap("assets/image/MainGame/BackIconClose.png");
    highlight[3] = al_load_bitmap("assets/image/MainGame/BackIconOpen.png");
    button_w[3] = al_get_bitmap_width(button[3]);
    button_h[3] = al_get_bitmap_height(button[3]);
    buttonH_w[3] = al_get_bitmap_width(highlight[3]);
    buttonH_h[3] = al_get_bitmap_height(highlight[3]);

    // 設定按鈕位置
    int increase = 750;
    X[0] = increase;
    Y[0] = 30;
    XH[0] = increase - 100; // 針對圖鑑按鈕的微調
    YH[0] = 30;
    increase += 180;
    for (int i = 1; i < BUTTON_NUM; i++)
    {
        X[i] = increase;
        Y[i] = 30;
        XH[i] = increase;
        YH[i] = 30;

        increase += 180;
    }
    X[2] += 15;
    XH[2] += 20;

    Money = al_load_bitmap("assets/image/MainGame/MoneyIcon.png");

    // pDerivedObj->notify = al_load_bitmap("assets/image/GameButton/notify.png"); //新事件通知按鈕(尚未設定完全)

    // 一進來初始化先把滑鼠點擊狀態清空
    mouse_state[1] = false;

    click = 0;

    // 設定遊戲功能頁面(初始化為預設)
    gameFunction = -1;

    // 初始化貓咪種類新舊、是否擁有、數量
    for (int i = 0; i < CatTotalKind; i++)
    {
        CatNumber[i] = 0;
        NewCatOrNot[i] = true;
        Own[i] = false;
    }

    // 根據之前在menu setting決定的遊玩模式改變初始金額
    if (ModeCondition[0])
    {
        TotalMoney = 99999;
    }
    else if (ModeCondition[1])
    {
        TotalMoney = 1000;
    }
    else if (ModeCondition[2])
    {
        TotalMoney = 100;
    }

    // register 遊戲物件
    //RegisterElement(New_Floor(Floor_L));
    //RegisterElement(New_Teleport(Teleport_L));
    //RegisterElement(New_Tree(Tree_L));
    //RegisterElement(New_Character(Character_L));
    RegisterElement(New_Basket(Basket_L));
    RegisterElement(New_Meat(Meat_L));
    //RegisterElement(New_catT(CatT_L));
    RegisterElement(New_Capture(Capture_L));
    RegisterElement(New_Cat(Cat_L));

    // register 遊戲功能頁面
    // 圖鑑
    RegisterElement(New_Book(Book_L));
    RegisterElement(New_PageOne(PageOne_L));
    RegisterElement(New_PageTwo(PageTwo_L));
    // 商店
    RegisterElement(New_Shop(Shop_L));
    RegisterElement(New_Buy(Buy_L));
    // 訂單
    RegisterElement(New_OrderControl(OrderControl_L));

    // RegisterElement(New_Exit(Exit_L));
}

GameScene::~GameScene()
{
    //Destructor
}


void GameScene::Update()
{
    // update every element
    std::vector<Elements*> allEle = GetAllElements();
    for (Elements* ele : allEle)
    {
        ele->Update(ele);
    }

    // run interact for every element
    for (Elements* ele : allEle)
    {
        // run every interact object
        for (int j = 0; j < ele->inter_len; j++)
        {
            int inter_label = ele->inter_obj[j];
            std::vector<Elements*> labelEle = GetLabelElements(inter_label);
            for (Elements* inter_ele : labelEle)
            {
                // printf("-> Interact with %d\n", inter_label);
                ele->Interact(ele, inter_ele);
            }
        }
    }

    // remove element
    for (size_t i = 0; i < allEle.size(); i++)
    {
        Elements* ele = allEle[i];
        if (ele->dele){
            RemoveElement(ele);
        }
    }

    // 遊戲內主功能(需要當下是"gameFunction == -1)
    if (gameFunction == -1)
    {
        // 撥放bgm
        al_play_sample_instance(sample_instance);

        // 感測各功能按鈕狀態，並切換功能
        Game_DetectButtonOn();
        if (mouse_state[1])
        { // 當按鈕按下且在遊戲介面時
            if (over_button[0])
            { // 進入圖鑑[0]
                al_play_sample_instance(Click_sample_instance);
                printf("In to Book\n");
                click = 1;
            }
            else if (over_button[1])
            { // 進入商店[1]
                al_play_sample_instance(Click_sample_instance);
                printf("In to Shop\n");
                 click = 1;
            }
            else if (over_button[2])
            { // 進入訂單管理頁面[2]
                al_play_sample_instance(Click_sample_instance);
                printf("In to Order\n");
                // 不切換頁面，而是利用全域變數操控跳出來的畫面，同時截斷底下頁面的感應功能
                 click = 1;
            }
            else if (over_button[3])
            { // 關閉遊戲(4)
                // 如果滑鼠按下的時候在按鈕上 -> 準備要離開
                printf("Quit\n");
                 click = 1;
            }
        }
        else
        {
            if (over_button[0] && click)
            { // 進入圖鑑[0]
                gameFunction = 0;
            }
            else if (over_button[1] && click)
            {                                                  // 進入商店[1]
                al_stop_sample_instance(sample_instance); // 如果進入商店，就停止遊戲主畫面內的bgm
                gameFunction = 1;
            }
            else if (over_button[2] && click)
            { // 進入訂單管理頁面[2]
                // 不切換頁面，而是利用全域變數操控跳出來的畫面，同時截斷底下頁面的感應功能
                //  gameFunction = 2;
                if (client_set != 3)
                {
                    gameFunction = 2;
                }
            }
            else if (over_button[3] && click)
            { // 關閉遊戲(4)
                // 如果滑鼠按下的時候在按鈕上 -> 準備要離開
                printf("Quit\n");
                if (TotalOwnCat == 0)
                {
                   SetSceneEnd(true);
                    window = 4;
                }
                else
                { // 如果總持有數>0，就進入結尾彩蛋
                    al_stop_sample_instance(sample_instance);
                    gameFunction = 3;
                }
            }
            click = 0;
        }
    }
}

void GameScene::Draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    printf("in game: %d, gameFunction = %d\n", al_get_bitmap_width(background), gameFunction);
    printf("Current Money: %d\n", TotalMoney);

    al_draw_bitmap(background, 0, 0, 0);

    // 畫出遊戲物件
    std::vector<Elements*> allEle = GetAllElements();
    for (Elements* ele : allEle)
    {
        ele->Draw(ele);
    }

    if (gameFunction == -1)
    { // 如果當前狀態是在遊戲畫面內，再畫出遊戲主畫面需要的物件
        // 畫出功能按鈕
        Game_DetectButtonOn();

        // 畫完按鈕後，根據現在是否有新事件畫出通知符號 -> 尚未設定
        // al_draw_bitmap(gs->notify, 415, 10, 0);

        // 畫出金錢條&上面的數字(固定)
        al_draw_bitmap(Money, 40, 20, 0);
        HowManyMoneyIHaveGAMESCENE();
    }
}

void GameScene::Game_DetectButtonOn()
{
    for (int i = 0; i < BUTTON_NUM; i++)
    {
        if ((mouse.x >= X[i]) && (mouse.x <= X[i] + button_w[i]) && (mouse.y >= Y[i]) && (mouse.y <= Y[i] + button_h[i]))
        { // 如果滑鼠在按鈕範圍內
            al_draw_bitmap(highlight[i], XH[i] - 10, YH[i] - 5, 0);
            over_button[i] = true;
        }
        else
        {
            al_draw_bitmap(button[i], X[i] - 10, Y[i] - 5, 0);
            over_button[i] = false;
        }
    }
}

void GameScene::HowManyMoneyIHaveGAMESCENE()
{                               // 逐字拆解目前錢錢數字，並畫出來
    int Number[MaxMoney] = {0}; // 最多7位數的陣列
    int temp = TotalMoney;
    int index = 0;
    int gap = 0;
    if (temp > 0)
    {
        while (temp != 0)
        {
            Number[index] = temp % 10;
            temp /= 10;
            index++;
        }
        for (int i = index - 1; i >= 0; i--)
        { // 到著跑，即可畫出從最小位~最高位
            PrintMoneyGAMESCENE(Number[i], gap);
            gap += 20; // 增加x座標往後畫
        }
    }
    else
    {
        PrintMoneyGAMESCENE(0, gap);
    }
}

void GameScene::PrintMoneyGAMESCENE(int num, int gap)
{ // 依照現在的數字是啥就印出誰
    switch (num)
    {
    case 0:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "0");
        break;
    case 1:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "1");
        break;
    case 2:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "2");
        break;
    case 3:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "3");
        break;
    case 4:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "4");
        break;
    case 5:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "5");
        break;
    case 6:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "6");
        break;
    case 7:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "7");
        break;
    case 8:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "8");
        break;
    case 9:
        al_draw_text(font, al_map_rgb(0, 0, 0), title_x + gap, title_y, ALLEGRO_ALIGN_CENTER, "9");
        break;
    default:
        break;
    }
}

void GameScene::Destroy()
{
    ALLEGRO_BITMAP *background = this->background;
    al_destroy_bitmap(background);
    al_destroy_sample(BGM);
    al_destroy_sample_instance(sample_instance);
    al_destroy_sample(ButtonClick);
    al_destroy_sample_instance(Click_sample_instance);
    // 刪除字體
    al_destroy_font(font);
    // 刪除按鈕&暫時的金錢條
    al_destroy_bitmap(button[0]);
    al_destroy_bitmap(button[1]);
    al_destroy_bitmap(button[2]);
    al_destroy_bitmap(button[3]);
    al_destroy_bitmap(highlight[0]);
    al_destroy_bitmap(highlight[1]);
    al_destroy_bitmap(highlight[2]);
    al_destroy_bitmap(highlight[3]);
    al_destroy_bitmap(Money);
    // al_destroy_bitmap(Obj->notify);
    // 刪除遊戲內元素(各功能的頁面、貓、肉泥等等)
    std::vector<Elements*> allEle = GetAllElements();
    for (Elements* ele : allEle)
    {
        ele->Destroy(ele);
    }
}