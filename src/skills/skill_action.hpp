#ifndef SKILL_ACTION_HPP
#define SKILL_ACTION_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include "utils/utils.hpp"

class SkillAction : public godot::Node
{
    GDCLASS(SkillAction, godot::Node)

public:
    enum class SkillPhase
    {
        None,
        Active,
        Ended
    };
protected:
    double duration = 1;
    double time_accumulated = 0;
    bool change_direction_allowed = false;
    godot::Vector3 direction = {1, 0, 0};
    SkillPhase phase = SkillPhase::None;
    godot::StringName animation = godot::StringName();
    bool cancellable = false;

protected:
    godot::Node3D* actor_source = nullptr;
    virtual void _reset(){};
    virtual void _step(double delta){};
    virtual void _end(){};

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, duration);
    CREATE_GETTER_SETTER_DEFAULT(godot::Node3D*, actor_source);
    /* TODO: make cancellable virtual check? */
    CREATE_GETTER_SETTER_BOOL_DEFAULT(cancellable);
    CREATE_GETTER_SETTER_BOOL_DEFAULT(change_direction_allowed);
    void set_animation(const godot::StringName& value);
    virtual godot::StringName get_animation() const;

    /* Can change direction only if allowed or before starting the action */
    void set_direction(const godot::Vector3 direction);
    _FORCE_INLINE_ godot::Vector3 get_direction() const
    {
        return this->direction;
    }

    void reset();
    void end();
    void step(double delta);
    _FORCE_INLINE_ SkillPhase get_phase() const
    {
        return this->phase;
    }
    _FORCE_INLINE_ double get_time_accumulated() const
    {
        return this->time_accumulated;
    }
    _FORCE_INLINE_ bool is_active() const
    {
        return (phase == SkillPhase::Active);
    }
    _FORCE_INLINE_ bool is_done() const
    {
        return (phase == SkillPhase::Ended);
    }
    _FORCE_INLINE_ bool is_none() const
    {
        return (phase == SkillPhase::None);
    }
    _FORCE_INLINE_ double get_time_remaining() const
    {
        return this->duration - this->time_accumulated;
    }
    

protected:
    static void _bind_methods();
};

#endif /* SKILL_ACTION_HPP */