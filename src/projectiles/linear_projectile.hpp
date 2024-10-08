#ifndef LINEAR_PROJECTILE_HPP
#define LINEAR_PROJECTILE_HPP

#include "projectiles/projectile.hpp"

#include "utils/utils.hpp"

class LinearProjectile : public Projectile
{
    GDCLASS(LinearProjectile, Projectile);

protected:

    float speed = 0;
    godot::Vector3 direction = godot::Vector3(0, 0, 0);

public:
    virtual void _physics_process(double delta) override;

    void set_direction(godot::Vector3 direction);
    _FORCE_INLINE_ godot::Vector3 get_direction() const
    {
        return this->direction;
    }

    CREATE_GETTER_SETTER_DEFAULT(double, speed);

protected:
    static void _bind_methods();
};

#endif // LINEAR_PROJECTILE_HPP