//
// Created by liad on 10/25/25.
//

#ifndef GAMEENGINE_OBJECTS_H
#define GAMEENGINE_OBJECTS_H
class Object {
public:
    Object() = default;
    virtual ~Object() = default;

    virtual void update();
    virtual void render();
};

#endif //GAMEENGINE_OBJECTS_H