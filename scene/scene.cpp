#include "scene.h"
#include <stdbool.h>
#include <stdlib.h> // 引入 stdlib.h 以使用 free 函数
#include <vector>

/*
   [Scene function]
*/

Scene::Scene(int label) : label(label), scene_end(false), ele_num(0) {
    for (int i = 0; i < MAX_ELEMENT; i++) {
        ele_list[i] = nullptr;
    }
}

Scene::~Scene() {
    //Destructor
}

void Scene::RemoveElement(Elements *ele) {
    EPNode *ptr = ele_list[ele->label];
    EPNode *parent_ptr = nullptr;
    while (ptr) {
        if (ptr->id == ele->id) {
            if (parent_ptr == nullptr) {
                ele_list[ele->label] = ptr->next;
                 free(ptr);
                break;
            } else {
                parent_ptr->next = ptr->next;
                free(ptr);
                break;
            }
        }
        parent_ptr = ptr;
        ptr = ptr->next;
    }
    ele_num--;
}

void Scene::RegisterElement(Elements *ele) {
    EPNode *ptr = ele_list[ele->label];
    EPNode *new_node = (EPNode *)malloc(sizeof(EPNode));
    new_node->id = ele_num++;
    new_node->ele = ele;
    new_node->next = nullptr;
    ele->id = new_node->id;
    if (ptr == nullptr) {
        ele_list[ele->label] = new_node;
    } else {
        while (ptr->next != nullptr)
            ptr = ptr->next;
        ptr->next = new_node;
    }
}


std::vector<Elements*> Scene::GetAllElements() {
    std::vector<Elements*> res;
    for (int i = 0; i < MAX_ELEMENT; i++) {
        if (ele_list[i] == nullptr)
            continue;
        EPNode *ptr = ele_list[i];
        while (ptr) {
            res.push_back(ptr->ele);
            ptr = ptr->next;
        }
    }
    return res;
}

std::vector<Elements*> Scene::GetLabelElements(int label) {
    std::vector<Elements*> res;
    EPNode *ptr = ele_list[label];
    while (ptr) {
        res.push_back(ptr->ele);
        ptr = ptr->next;
    }
    return res;
}