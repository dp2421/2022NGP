#include "stdafx.h"
#include "Interaction_Object.h"

void Interaction_Object::SetCollsionBox()
{
    collisionBox.left = x - 25;
    collisionBox.top = y;
    collisionBox.right = x + 25;
    collisionBox.bottom = y + 50;

}

void Interaction_Object::Update()
{
    if (isPulling && type == INTER_OBJ::REVER) {
        if (current_time >= max_time) {
            isPulling = false;
            draw_image = rever_nomal;
        }
        else current_time++;
    }
}

void Interaction_Object::Collsion() {
    if (!isPulling) {
        isPulling = true;
        if (INTER_OBJ::REVER == type) {
            draw_image = rever_pull;
            current_time = 0;
        }
        if (INTER_OBJ::BUTTON == type) {
            draw_image = button_pull;
        }
    }
}