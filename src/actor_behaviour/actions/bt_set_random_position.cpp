#include "bt_set_random_position.hpp"

BTTask::Status BTSetRandomPosition::_tick(double delta)
{
    ERR_FAIL_COND_MSG(
        this->position_name.is_empty(),
        vformat("%s: position_name is empty", this->get_name())
    );

    Vector3 random_position;
    /* TODO: make this random position to be in the navMesh */
    random_position.x = godot::UtilityFunctions::randf_range(-100, 100);
    random_position.y = godot::UtilityFunctions::randf_range(-100, 100);

    this->get_blackboard()->set_var(this->position_name, random_position);
    TASK_SUCCEED();
}

void BTSetRandomPosition::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetRandomPosition, STRING_NAME, position_name);
}