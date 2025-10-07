#ifndef HITBOX_HPP
#define HITBOX_HPP

#include <godot_cpp/classes/area3d.hpp>
#include "utils/utils.hpp"
#include "components/area3d/hitbox_blocker.hpp"
#include "components/area3d/hurtbox.hpp"

class Hitbox : public godot::Area3D
{
    GDCLASS(Hitbox, godot::Area3D);

protected:
    void _on_area_entered(godot::Area3D* area3d);

public:
    virtual void _ready() override;

protected:
    static void _bind_methods();
};


#endif