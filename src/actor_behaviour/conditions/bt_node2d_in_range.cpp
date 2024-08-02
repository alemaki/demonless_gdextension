#include "bt_node2d_in_range.hpp"

#include <godot_cpp/classes/node2d.hpp>

BTTask::Status BTNode2DInRange::_tick(double delta)
{
    godot::Node2D* actor = godot::Object::cast_to<godot::Node2D>(this->get_actor());
    godot::Node2D* node = godot::Object::cast_to<Node2D>(this->get_blackboard()->get_var(this->node_name, godot::Variant(), false));

    TASK_FAIL_COND(node == nullptr);

    TASK_SUCCEED_COND(actor->get_position().distance_to(node->get_position()) <= range);

    return BTTask::Status::FAILURE;
}

void BTNode2DInRange::set_range(double range)
{
    this->range = range;
}

void BTNode2DInRange::set_node_name(const godot::String& node_name)
{
    this->node_name = node_name;
}

void BTNode2DInRange::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_range", "range"), &BTNode2DInRange::set_range);
    ClassDB::bind_method(D_METHOD("get_range"), &BTNode2DInRange::get_range);
    ClassDB::bind_method(D_METHOD("set_node_name", "node_name"), &BTNode2DInRange::set_node_name);
    ClassDB::bind_method(D_METHOD("get_node_name"), &BTNode2DInRange::get_node_name);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "range"), "set_range", "get_range");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "node_name"), "set_node_name", "get_node_name");

}