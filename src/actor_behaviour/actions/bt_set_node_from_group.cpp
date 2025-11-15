#include "bt_set_node_from_group.hpp"
#include "utils/utils.hpp"

BTTask::Status BTSetNodeFromGroup::_tick(double delta)
{
    TASK_FAIL_COND_COMP_MSG(this->group_name.is_empty(), "Group name is empty");
    TASK_FAIL_COND_COMP_MSG(this->node_name.is_empty(), "Node name is empty");
    TASK_FAIL_COND_COMP_MSG(this->set_name.is_empty(), "Set name is empty");

    Node* node = ::get_scene_tree()->get_first_node_in_group(this->group_name); //TODO!: get specific node in group? or random
    TASK_FAIL_COND_COMP(node == nullptr);

    this->get_blackboard()->set_var(this->set_name, node);

    TASK_SUCCEED();
}

void BTSetNodeFromGroup::_bind_methods()
{
    using namespace std;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetNodeFromGroup, STRING_NAME, group_name);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetNodeFromGroup, STRING_NAME, node_name);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetNodeFromGroup, STRING_NAME, set_name);
}


//TODO: write tests.