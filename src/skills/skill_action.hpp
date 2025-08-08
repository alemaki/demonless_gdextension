#ifndef SKILL_ACTION_HPP
#define SKILL_ACTION_HPP

#include <godot_cpp/classes/node.hpp>
#include "utils/utils.hpp"
#include "godot_cpp/classes/node3d.hpp"

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
    SkillPhase phase = SkillPhase::None;
    bool cancellable = true;

protected:
    godot::Node3D* actor_source = nullptr;
    virtual void _reset(){};
    virtual void _step(double delta){};
    virtual void _end(){};

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, duration);
    CREATE_GETTER_SETTER_DEFAULT(godot::Node3D*, actor_source);
    CREATE_GETTER_SETTER_BOOL_DEFAULT(cancellable);

    void reset();
    void step(double delta);

    _FORCE_INLINE_ double get_time_accumulated() const
    {
        return this->time_accumulated;
    }
    _FORCE_INLINE_ bool is_finished() const
    {
        return (this->time_accumulated >= this->duration);
    }

protected:
    static void _bind_methods();
};

#endif /* SKILL_ACTION_HPP */