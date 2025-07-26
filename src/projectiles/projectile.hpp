#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/timer.hpp>

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
    godot::Timer* lifespan_timer = nullptr;
    static void _bind_methods();

public:
    virtual void _on_timeout();
    void _ready() override;
    void set_lifespan_timer(godot::Timer* timer);
    _FORCE_INLINE_ godot::Timer* get_lifespan_timer() const
    {
        return this->lifespan_timer;
    }
};

#endif // PROJECTILE_HPP