#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <godot_cpp/classes/node3d.hpp>

class Projectile : public godot::Node3D
{
    GDCLASS(Projectile, godot::Node3D);

protected:
    static void _bind_methods();
};

#endif // PROJECTILE_HPP