#ifndef HITBOX_HPP
#define HITBOX_HPP

#include <godot_cpp/classes/area3d.hpp>
#include "utils/utils.hpp"

class Hitbox : public godot::Area3D
{
    GDCLASS(Hitbox, godot::Area3D);

    bool blockable = false;

protected:
    void _on_area_entered(godot::Area3D* area3d);
    virtual void _on_block();

public:
    CREATE_GETTER_SETTER_BOOL_DEFAULT(blockable);

    virtual void _ready() override;
    void trigger_block();

protected:
    static void _bind_methods();
};


#endif