#ifndef HITBOX_HPP
#define HITBOX_HPP

#include <godot_cpp/classes/area3d.hpp>
#include "utils/utils.hpp"
#include "components/area3d/hitbox_blocker.hpp"
#include "components/area3d/hurtbox.hpp"
#include "attacks/damage_profile.hpp"

class Hitbox : public godot::Area3D
{
    GDCLASS(Hitbox, godot::Area3D);

    godot::Ref<DamageProfile> damage_profile = nullptr;
    godot::Node* actor_source = nullptr;

protected:
    void _on_area_entered(godot::Area3D* area3d);

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::Ref<DamageProfile>, damage_profile);
    CREATE_GETTER_SETTER_DEFAULT(godot::Node*, actor_source);

    virtual void _ready() override;
    virtual godot::PackedStringArray _get_configuration_warnings() const override;

protected:
    static void _bind_methods();
};


#endif /* HITBOX_HPP */