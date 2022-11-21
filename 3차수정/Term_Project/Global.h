#pragma once

enum class STATE { IDLE, MOVE, JUMP, FALL, ATTACK, DEAD };
enum class INTER_OBJ { REVER, BUTTON, PORTAL };
enum class MONSTER_TYPE { MONSTER_MOVE, MONSTER_PEACE };

struct Bullet {
    int x;
    int y;
    int dir;
    RECT collisionBox;
    bool isAttack = false;
    CImage draw_image;
};