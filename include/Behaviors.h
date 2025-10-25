//
// Created by liad on 10/25/25.
//

#ifndef GAMEENGINE_BEHAVIORS_H
#define GAMEENGINE_BEHAVIORS_H
#include <EngineTypes.h>
#include <Sprite.h>
#include <Renderer2D.h>
#include <Renderer.h>
#include <WindowManager.h>
#include <Objects.h>

class Transform {
public:
    RGNX2D::Vector2 position;
    RGNX2D::Vector2 scale;
    float rotation;

    void set_position(const RGNX2D::Vector2& pos) { position = pos; }
    void set_scale(const RGNX2D::Vector2& s) { scale = s; }
    void set_rotation(float r) { rotation = r; }
};

class Texture2D {
    public:

};

#endif //GAMEENGINE_BEHAVIORS_H