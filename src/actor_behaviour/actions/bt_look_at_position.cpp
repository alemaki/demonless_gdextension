#include "bt_look_at_position.hpp"
#include <godot_cpp/classes/node3d.hpp>

BTTask::Status BTLookAtPosition::_tick(double delta)
{
    godot::Node3D* actor = godot::Object::cast_to<Node3D>(this->get_actor());

    TASK_FAIL_COND_COMP_MSG(actor == nullptr, "Actor is not a Node3D");
    TASK_FAIL_COND_COMP(!(this->get_blackboard()->has_var(this->look_at_position_var)));

    godot::Vector3 look_at_pos = this->get_blackboard()->get_var(this->look_at_position_var);

    actor->look_at(look_at_pos, this->up_vector);
    TASK_SUCCEED();
}

void BTLookAtPosition::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTLookAtPosition, STRING_NAME, look_at_position_var);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTLookAtPosition, VECTOR3, up_vector);
}