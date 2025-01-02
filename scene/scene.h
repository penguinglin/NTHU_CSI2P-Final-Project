#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include "../global.h"
#include "../element/element.h"
#include "../algif5/algif.h"
#include <stdbool.h>
#include <vector>

#define MAX_ELEMENT 100

/*
   [scene object]
*/
class Scene {
public:
    Scene(int label);
    virtual ~Scene();

    int GetLabel() const { return label; }
    bool IsSceneEnd() const { return scene_end; }
    void SetSceneEnd(bool end) { scene_end = end; }

    void RegisterElement(Elements *ele);
    void RemoveElement(Elements *ele);
    std::vector<Elements*> GetAllElements();
    std::vector<Elements*> GetLabelElements(int label);

    // interface for function
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Destroy() = 0;

protected:
    int label;
    bool scene_end;
    int ele_num;

private:
    struct EPNode {
        int id;
        Elements *ele;
        EPNode *next;
    };

    EPNode *ele_list[MAX_ELEMENT];
};

#endif