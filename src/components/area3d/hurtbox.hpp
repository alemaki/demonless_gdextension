#ifndef HURTBOX_HPP
#define HURTBOX_HPP

#include <godot_cpp/classes/area3d.hpp>
#include "attacks/damage_info.hpp"

class Hurtbox : public godot::Area3D
{
    GDCLASS(Hurtbox, godot::Area3D);

    friend class Hitbox;
protected:
    void hitbox_entered(godot::Area3D* hitbox);

protected:
    static void _bind_methods();

};


#endif // HURTBOX_HPP