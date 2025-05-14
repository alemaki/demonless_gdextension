#ifndef COMBATANT_HPP
#define COMBATANT_HPP

#include <godot_cpp/classes/character_body3d.hpp>
#include "components/area3d/hurtbox.hpp"
#include "components/area3d/hitbox.hpp"
#include "components/health/health_component.hpp"
#include "utils/utils.hpp"

/**
 * @brief Base class for any CharacterBody3D that can take damage: wires a Hurtbox's
 * "hurtbox_hit" signal into the HealthComponent. Fields are either set by adding a
 * child to the node with the default name and proper class or by force setting them
 * as properties.
 */
class Combatant : public godot::CharacterBody3D
{
    GDCLASS(Combatant, godot::CharacterBody3D)

protected:
    Hurtbox* hurtbox = nullptr;
    HealthComponent* health_component = nullptr;

    virtual void on_hit(const Hitbox* hitbox);

private:
    void _on_hurtbox_hit(const godot::Area3D* hitbox);

public:
    CREATE_GETTER_SETTER_DEFAULT(Hurtbox*, hurtbox);
    CREATE_GETTER_SETTER_DEFAULT(HealthComponent*, health_component);

    void _ready() override;

protected:
    static void _bind_methods();
};

#endif /* COMBATANT_HPP */
