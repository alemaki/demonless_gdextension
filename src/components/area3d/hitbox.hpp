#ifndef HITBOX_HPP
#define HITBOX_HPP

#include <godot_cpp/classes/area3d.hpp>

class Hitbox : public godot::Area3D
{
    GDCLASS(Hitbox, godot::Area3D);

protected:
    void _on_area_entered(godot::Area3D* area3d);
    static void _bind_methods();

public:
    virtual void _ready() override;
};


#endif