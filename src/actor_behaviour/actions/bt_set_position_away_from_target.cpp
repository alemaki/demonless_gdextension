#include "bt_set_position_away_from_target.hpp"
#include "godot_cpp/classes/node3d.hpp"
BTTask::Status BTSetPositionAwayFromTarget::_tick(double delta)
{
    godot::Node3D* actor = godot::Object::cast_to<Node3D>(this->get_blackboard()->get_var(this->target_name));
    

    TASK_SUCCEED();
}

void BTSetPositionAwayFromTarget::_enter()
{
    ERR_FAIL_COND_MSG(target_name.is_empty(), vformat("", this->get_custom_name()))
}

void BTSetPositionAwayFromTarget::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetPositionAwayFromTarget, STRING_NAME, position_name);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetPositionAwayFromTarget, STRING_NAME, target_name);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetPositionAwayFromTarget, FLOAT, distance);
}

//TODO: write tests;
