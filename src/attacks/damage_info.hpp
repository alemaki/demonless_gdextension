#ifndef DAMAGE_INFO_HPP
#define DAMAGE_INFO_HPP

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/node.hpp>
#include "utils/utils.hpp"

class DamageInfo : public godot::Resource
{
    GDCLASS(DamageInfo, godot::Resource)

private:
    double damage = 1;
    godot::Node* source = nullptr;

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, damage);
    CREATE_GETTER_SETTER_DEFAULT(godot::Node*, source);

protected:
    static void _bind_methods();
};

#endif /* DAMAGE_INFO_HPP */