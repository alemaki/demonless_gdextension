#include "bt_set_random_position.hpp"

BTTask::Status BTSetRandomPosition::_tick(double delta)
{
    TASK_FAIL_COND_COMP_MSG( this->position_name.is_empty(), "%s: position_name is empty");

    Vector3 random_position;
    /* TODO: make this random position to be in the navMesh */
    random_position.x = godot::UtilityFunctions::randf_range(-15, 15);
    random_position.z = godot::UtilityFunctions::randf_range(-15, 15);

    this->get_blackboard()->set_var(this->position_name, random_position);
    TASK_SUCCEED();
}

void BTSetRandomPosition::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetRandomPosition, STRING_NAME, position_name);
}