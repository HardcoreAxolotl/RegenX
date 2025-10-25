//
// Created by liad on 10/25/25.
//

#ifndef GAMEENGINE_TESTOBJECT_H
#define GAMEENGINE_TESTOBJECT_H
#include <Objects.h>
#include <Behaviors.h>

class Player : public Object, public Transform, public Texture2D{
    public:
    void update() override {

    }
};
#endif //GAMEENGINE_TESTOBJECT_H