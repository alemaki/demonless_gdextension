#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <godot_cpp/classes/node.hpp>
#include "utils/utils.hpp"

class Attack : public godot::Node
{
    GDCLASS(Attack, godot::Node)

private:
    double elapsed = 0;
    double duration = 0;

protected:
    virtual void _reset(){};
    virtual void _step(double delta){};
    virtual void _end(){};

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, duration);

    void reset();
    void step(double delta);
    void end();

protected:
    static void _bind_methods();
};

#endif /* ATTACK_HPP */