#ifndef CHARACTER_MOVEMENT_COMPONENT
#define CHARACTER_MOVEMENT_COMPONENT

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/character_body3d.hpp>

#include "utils/utils.hpp"

class CharacterMovementComponent : public godot::Node
{
    GDCLASS(CharacterMovementComponent, godot::Node);

private:
    double speed = 1;
    double friction = 1;

    godot::Vector3 current_velocity = godot::Vector3(0, 0, 0);
    godot::Vector3 target_velocity = godot::Vector3(0, 0, 0);

    godot::CharacterBody3D* character = nullptr;

public:
    void _physics_process(double delta) override;

    CREATE_GETTER_SETTER_DEFAULT(godot::Vector3, target_velocity);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, speed);
    CREATE_GETTER_SETTER_DEFAULT(godot::CharacterBody3D*, character);

    void set_target_direction(godot::Vector3 direction);

    void set_friction(double friction);
    _FORCE_INLINE_ double get_friction() const
    {
        return this->friction;
    }

protected:
    static void _bind_methods();
};

#endif // CHARACTER_MOVEMENT_COMPONENT