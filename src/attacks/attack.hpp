#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <godot_cpp/classes/node.hpp>
#include "utils/utils.hpp"
#include "godot_cpp/classes/node3d.hpp"

class Attack : public godot::Node
{
    GDCLASS(Attack, godot::Node)

protected:
    double duration = 0;
    double time_accumulated = 0;

protected:
    godot::Node3D* actor_source;
    virtual void _reset(){};
    virtual void _step(double delta){};

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, duration);
    CREATE_GETTER_SETTER_DEFAULT(godot::Node3D*, actor_source);

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

#endif /* ATTACK_HPP */