#include "bt_set_position_away_from_target.hpp"
#include "godot_cpp/classes/node3d.hpp"

BTTask::Status BTSetPositionAwayFromTarget::_tick(double delta)
{
    godot::Node3D* actor = godot::Object::cast_to<Node3D>(this->get_actor());
    TASK_FAIL_COND_COMP_MSG(actor == nullptr, "Actor is not Node3D.");
    godot::Node3D* target = godot::Object::cast_to<Node3D>(this->get_blackboard()->get_var(this->target_name));
    TASK_FAIL_COND_COMP_MSG(target == nullptr, "Target is not Node3D.");

    Vector3 position = actor->get_global_position() + (actor->get_global_position() - target->get_global_position()).normalized()*distance;
    position.y = 0; // TODO better clear of up vector.
    this->get_blackboard()->set_var(this->position_name, position);

    TASK_SUCCEED();
}

void BTSetPositionAwayFromTarget::_enter()
{
    ERR_FAIL_COND_MSG(this->position_name.is_empty(), "Position name is empty");
    ERR_FAIL_COND_MSG(this->target_name.is_empty(), "Target name is empty");
}

void BTSetPositionAwayFromTarget::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetPositionAwayFromTarget, STRING_NAME, position_name);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetPositionAwayFromTarget, STRING_NAME, target_name);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetPositionAwayFromTarget, FLOAT, distance);
}

//TODO: write tests;
