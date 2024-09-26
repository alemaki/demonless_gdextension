#include "bt_queue_free_node.hpp"
#include "components/area3d/hitbox.hpp"

BTTask::Status BTQueueFreeNode::_tick(double delta)
{
    godot::Variant var = this->get_blackboard()->get_var(this->var_node, Variant(), this->is_complain_enabled());
    Node* node = godot::Object::cast_to<Hitbox>(var);
    TASK_FAIL_COND(node == nullptr);
    if (node->get_parent() != nullptr)
    {
        node->get_parent()->remove_child(node);
    }
    node->queue_free();
    TASK_SUCCEED();
}

void BTQueueFreeNode::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTQueueFreeNode, STRING_NAME, var_node);
}