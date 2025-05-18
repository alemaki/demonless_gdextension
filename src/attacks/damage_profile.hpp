#ifndef DAMAGE_PROFILE_HPP
#define DAMAGE_PROFILE_HPP

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/node.hpp>
#include "attacks/damage_info.hpp"
#include "utils/utils.hpp"

class DamageProfile : public godot::Resource
{
    GDCLASS(DamageProfile, godot::Resource)

private:
    double base_damage = 1;

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, base_damage);

    godot::Ref<DamageInfo> construct_damage_info(godot::Node* actor_source);

protected:
    static void _bind_methods();
};

#endif /* DAMAGE_INFO_HPP */