#ifndef MOVEMENT_CONTEXT_HPP
#define MOVEMENT_CONTEXT_HPP

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include "utils/utils.hpp"

class MovementContext : public godot::Resource
{
    GDCLASS(MovementContext, godot::Resource);

protected:
    godot::Vector3 position = godot::Vector3(0, 0, 0);
    godot::Vector3 direction = godot::Vector3(1, 0, 0);
    double speed = 1;
    godot::Node3D* target = nullptr;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::Vector3, position);
    void set_direction(const godot::Vector3 direction);
    _FORCE_INLINE_ godot::Vector3 get_direction() const
    {
        return this->direction;
    }


    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, speed);
    CREATE_GETTER_SETTER_DEFAULT(godot::Node3D*, target);

protected:
    static void _bind_methods();
};

#endif // MOVEMENT_CONTEXT_HPP