#include "bt_node3d_in_range.hpp"

#include <godot_cpp/classes/node3d.hpp>

BTTask::Status BTNode3DInRange::_tick(double delta)
{
    godot::Node3D* actor = godot::Object::cast_to<godot::Node3D>(this->get_actor());
    godot::Node3D* node = godot::Object::cast_to<Node3D>(this->get_blackboard()->get_var(this->node_name, godot::Variant(), false));

    TASK_FAIL_COND(node == nullptr);

    TASK_SUCCEED_COND(actor->get_position().distance_to(node->get_position()) <= range);

    return BTTask::Status::FAILURE;
}

void BTNode3DInRange::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTNode3DInRange, FLOAT, range);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTNode3DInRange, STRING, node_name);

}