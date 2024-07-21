#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

#include <godot_cpp/classes/character_body2d.hpp>
#include "behaviour_tree/behaviour_tree.hpp"
#include "components/health/health_component.hpp"
#include "components/input/character_input_component.hpp"
#include "components/movement/character_movement_component.hpp"
#include "finite_state_machine/fsm.hpp"

class PlayerCharacter : public godot::CharacterBody2D
{
    GDCLASS(PlayerCharacter, CharacterBody2D);

    HealthComponent* health_component = nullptr;
    CharacterInputComponent* input_component = nullptr;
    CharacterMovementComponent* movement_component = nullptr;
    FSM* movement_fsm = nullptr;
    FSM* action_fsm = nullptr;
    BehaviourTree* decision_tree = nullptr ;
    godot::Ref<BTTask> task;
    godot::Ref<Blackboard> blackboard;
    
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
    void set_movement_component(CharacterMovementComponent* movement_component);
    _FORCE_INLINE_ CharacterMovementComponent* get_movement_component()
    {
        return this->movement_component;
    }
    void set_movement_fsm(FSM* movement_fsm);
    _FORCE_INLINE_ FSM* get_movement_fsm() const
    {
        return this->movement_fsm;
    }
    void set_action_fsm(FSM* action_fsm);
    _FORCE_INLINE_ FSM* get_action_fsm() const
    {
        return this->action_fsm;
    }
    void set_decision_tree(BehaviourTree* decision_tree);
    _FORCE_INLINE_ BehaviourTree* get_decision_tree() const
    {
        return this->decision_tree;
    }

protected: 
    static void _bind_methods();

};

#endif // PLAYER_CHARACTER_HPP