#include "sceneManager.h"
//主選單相關的頁面的.h檔incoude
#include "menu.h"
#include "gamescene.h"
#include "Information.h"
#include "leave.h"
#include "Setscene.h"

Scene *scene = nullptr;
void create_scene(SceneType type)
{
    if(scene != nullptr){
        delete scene;
        scene = nullptr;
    }
    switch (type)
    {
    case Menu_L:
        scene = new Menu(Menu_L);
        break;

    case GameScene_L:
        scene = new GameScene(GameScene_L);
        break;

    case Set_L:
        scene = new Setting(Set_L);
        break;

    case Information_L:
        scene = new Info(Information_L);
        break;

    case Quit_L:
        scene = new Leave(Quit_L);
        break;

    default:
        break;
    }
}