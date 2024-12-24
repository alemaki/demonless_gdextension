#ifndef CHARACTER_INPUT_COMPONENT
#define CHARACTER_INPUT_COMPONENT

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>

class CharacterInputComponent : public godot::Node3D
{
    GDCLASS(CharacterInputComponent, godot::Node3D);
    
public:
    CharacterInputComponent(){};

    _FORCE_INLINE_ godot::Vector3 get_direction_input() const
    {
        godot::Vector2 direction_input = godot::Input::get_singleton()->get_vector("move_left", "move_right", "move_up", "move_down");
        return {direction_input.x, 0, direction_input.y};
    }
    _FORCE_INLINE_ bool is_attack_pressed() const
    {
        return godot::Input::get_singleton()->is_action_pressed("attack");
    }
    _FORCE_INLINE_ bool is_block_pressed() const
    {
        return godot::Input::get_singleton()->is_action_pressed("block");
    }

    godot::Vector3 get_mouse_casted_position();

protected:
    static void _bind_methods();

};

#endif // CHARACTER_INPUT_COMPONENT