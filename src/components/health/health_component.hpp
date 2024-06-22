#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP


#include <godot_cpp/classes/node.hpp>

class HealthComponent : public godot::Node
{
    GDCLASS(HealthComponent, godot::Node);
    
public:
    double max_hp = 0;
    double current_hp = 0;

public:
    void set_max_hp(double max_hp);
    _FORCE_INLINE_ double get_max_hp() const
    {
        return this->max_hp;
    }
    void set_current_hp(double current_hp);
    _FORCE_INLINE_ double get_current_hp() const
    {
        return this->current_hp;
    }

    void take_damage(double amount);
    void heal(double amount);
    

protected:
    static void _bind_methods();
};

#endif