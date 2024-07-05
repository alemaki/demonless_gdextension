#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

#include <godot_cpp/classes/character_body2d.hpp>
#include "behaviour_tree/behaviour_tree.hpp"
#include "components/health/health_component.hpp"
#include "components/input/character_input_component.hpp"
#include "finite_state_machine/fsm.hpp"

class PlayerCharacter : public godot::CharacterBody2D
{
    GDCLASS(PlayerCharacter, CharacterBody2D);

    HealthComponent* health_component;
    CharacterInputComponent* input_component;
    FSM* movement_fsm;
    FSM* action_fsm;
    BehaviourTree* behaviour_tree;
    
public:
    void _ready() override;
    void _process(double delta) override;
    void _physics_process(double delta) override;

    void set_health_component(HealthComponent* health_component);
    _FORCE_INLINE_ HealthComponent* get_health_component() const
    {
        return this->health_component;
    }
    void set_input_component(CharacterInputComponent* input_component);
    _FORCE_INLINE_ CharacterInputComponent* get_input_component() const
    {
        return this->input_component;
    }

protected: 
    static void _bind_methods();

};

#endif // PLAYER_CHARACTER_HPP