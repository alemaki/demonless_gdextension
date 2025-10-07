#ifndef HITBOX_BLOCKER_HPP
#define HITBOX_BLOCKER_HPP

#include <godot_cpp/classes/area3d.hpp>
#include "utils/utils.hpp"

class HitboxBlocker : public godot::Area3D
{
    GDCLASS(HitboxBlocker, godot::Area3D);

    friend class Hitbox;

private:
    bool active = true;

protected:
    void hitbox_entered(godot::Area3D* hitbox);

public:
    CREATE_GETTER_SETTER_BOOL_DEFAULT(active);

protected:
    static void _bind_methods();

};


#endif // HITBOX_BLOCKER_HPP