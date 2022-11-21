#pragma once


class Interaction_Object {
public:
    int x;
    int y;
    RECT collisionBox;
    bool isPulling = false;
    CImage draw_image;
    INTER_OBJ type;

    //나중에 시간함수로 바꾸3
    int current_time = 0;
    int max_time = 100;
public:
    void SetCollsionBox();
    void Update();
    void Collsion();

    void SetImage()
    {
        draw_image = button_normal;
        isPulling = false;
    }
};
