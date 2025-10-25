#include "wave_attack.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

void WaveAttack::set_direction(const godot::Vector3 direction)
{
    ERR_FAIL_COND_MSG(direction.is_zero_approx(), "Attack direction cannot be godot::Vector3{0, 0, 0}");
    this->direction = direction;
}

void WaveAttack::_ready()
{
    if (projectile != nullptr)
    {
        return;
    }

    ERR_FAIL_COND_MSG(this->projectile_scene.is_null(), vformat("\"%s\": no projectile scene or projectile.", this->get_name()));
    ERR_FAIL_COND_MSG((!this->projectile_scene->can_instantiate()), vformat("\"%s\": cannot instantiate prijectile scene and has no projectile.", this->get_name()));

    Projectile* projectile = godot::Object::cast_to<Projectile>(projectile_scene->instantiate());

    ERR_FAIL_COND_MSG(this->projectile_scene.is_null(), vformat("\"%s\": packed scene is not projectile.", this->get_name()));

    this->projectile = projectile;
}

WaveAttack::~WaveAttack()
{
    if (this->projectile)
    {
        memdelete(this->projectile);
    }
}

void WaveAttack::_reset()
{
    this->waves_fired = 0;
}


void WaveAttack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(WaveAttack, projectile_scene, PROPERTY_HINT_RESOURCE_TYPE, "PackedScene", PROPERTY_USAGE_DEFAULT, PackedScene);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(WaveAttack, INT, projectiles_per_wave);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(WaveAttack, INT, waves);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(WaveAttack, FLOAT, wave_cooldown);
}