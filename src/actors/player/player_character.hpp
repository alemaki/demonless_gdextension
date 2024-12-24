#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include "behaviour_tree/behaviour_tree.hpp"
#include "components/health/health_component.hpp"
#include "components/input/character_input_component.hpp"
#include "components/movement/character_movement_component.hpp"
#include "finite_state_machine/fsm.hpp"

class PlayerCharacter : public godot::CharacterBody3D
{
    GDCLASS(PlayerCharacter, CharacterBody3D);

    HealthComponent* health_component = nullptr;
    CharacterInputComponent* input_component = nullptr;
    CharacterMovementComponent* movement_component = nullptr;
    FSM* action_fsm = nullptr;
    FSM* movement_fsm = nullptr;
    godot::Node3D* mesh_instance = nullptr;
    godot::AnimationPlayer* animation_player = nullptr;

private:
void process_action_state();
    void process_movement_state();

    void process_action();
    void process_movement();

public:
    ~PlayerCharacter();

    void _ready() override;
    void _process(double delta) override;
    void _physics_process(double delta) override;

    CREATE_GETTER_SETTER_DEFAULT(HealthComponent*, health_component);
    CREATE_GETTER_SETTER_DEFAULT(CharacterInputComponent*, input_component);
    CREATE_GETTER_SETTER_DEFAULT(FSM*, movement_fsm);
    CREATE_GETTER_SETTER_DEFAULT(FSM*, action_fsm);
    CREATE_GETTER_SETTER_DEFAULT(godot::Node3D*, mesh_instance)

    
    void set_movement_component(CharacterMovementComponent* movement_component);
    _FORCE_INLINE_ CharacterMovementComponent* get_movement_component()
    {
        return this->movement_component;
    }

protected: 
    static void _bind_methods();

};

#endif // PLAYER_CHARACTER_HPP