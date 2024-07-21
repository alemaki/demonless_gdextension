#ifndef CHARACTER_MOVEMENT_COMPONENT
#define CHARACTER_MOVEMENT_COMPONENT

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/character_body2d.hpp>

class CharacterMovementComponent : public godot::Node
{
    GDCLASS(CharacterMovementComponent, godot::Node);

private:
    float speed = 1;
    float friction = 1;

    godot::Vector2 current_velocity = godot::Vector2(0, 0);
    godot::Vector2 target_velocity = godot::Vector2(0, 0);

    godot::CharacterBody2D* character = nullptr;

public:
    void _physics_process(double delta) override;

    
    void set_target_velocity(godot::Vector2 target_velocity);
    _FORCE_INLINE_ godot::Vector2 get_target_velocity()
    {
        return this->target_velocity;
    }
    void set_speed(float speed);
    _FORCE_INLINE_ float get_speed() const
    {
        return this->speed;
    }
    void set_friction(float friction);
    _FORCE_INLINE_ float get_friction() const
    {
        return this->friction;
    }
    void set_character(godot::CharacterBody2D* character);
    _FORCE_INLINE_ godot::CharacterBody2D* get_character() const
    {
        return this->character;
    }

protected:
    static void _bind_methods();
};

#endif // CHARACTER_MOVEMENT_COMPONENT