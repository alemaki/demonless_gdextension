#ifndef MOVEMENT_CONTEXT_HPP
#define MOVEMENT_CONTEXT_HPP

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include "utils/utils.hpp"

class MovementContext : public godot::Object
{
    GDCLASS(MovementContext, godot::Object);

public:
    godot::Vector3 position;
    godot::Vector3 direction;
    double speed;
    godot::Node3D* target;

    CREATE_GETTER_SETTER_DEFAULT(godot::Vector3, position);
    CREATE_GETTER_SETTER_DEFAULT(godot::Vector3, direction);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, speed);
    CREATE_GETTER_SETTER_DEFAULT(godot::Node3D*, target);

protected:
    static void _bind_methods();
};

#endif // MOVEMENT_CONTEXT_HPP