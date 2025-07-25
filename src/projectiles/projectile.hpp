#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <godot_cpp/classes/node3d.hpp>

class Projectile : public godot::Node3D
{
    GDCLASS(Projectile, godot::Node3D);

protected:
    godot::Vector3 direction = godot::Vector3(1, 0, 0);

public:

    void set_direction(godot::Vector3 direction);
    _FORCE_INLINE_ godot::Vector3 get_direction() const
    {
        return this->direction;
    }

protected:
    static void _bind_methods();
};

#endif // PROJECTILE_HPP