#ifndef CHARACTER_INPUT_COMPONENT
#define CHARACTER_INPUT_COMPONENT

#include <godot_cpp/classes/node3d.hpp>

class CharacterInputComponent : public godot::Node3D
{
    GDCLASS(CharacterInputComponent, godot::Node3D);

protected:
    bool attack_pressed = false;
    bool block_pressed = false;
    godot::Vector3 direction_input = godot::Vector3(0, 0, 0);

    void evaluate_input();
    
public:
    CharacterInputComponent();

    _FORCE_INLINE_ godot::Vector3 get_direction_input() const
    {
        return this->direction_input;
    }
    _FORCE_INLINE_ bool is_attack_pressed() const
    {
        return this->attack_pressed;
    }
    _FORCE_INLINE_ bool is_block_pressed() const
    {
        return this->block_pressed;
    }

    godot::Vector3 get_mouse_casted_position();

    void _process(double delta) override;
    void _ready() override;

protected:
    static void _bind_methods();

};

#endif // CHARACTER_INPUT_COMPONENT