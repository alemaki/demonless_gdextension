#ifndef HITBOX_BLOCKER_HPP
#define HITBOX_BLOCKER_HPP

#include <godot_cpp/classes/area3d.hpp>

class HitboxBlocker : public godot::Area3D
{
    GDCLASS(HitboxBlocker, godot::Area3D);

protected:
    void _on_area_entered(godot::Area3D* area3d);
    
public:
    virtual void _ready() override;

protected:
    static void _bind_methods();

};


#endif // HITBOX_BLOCKER_HPP