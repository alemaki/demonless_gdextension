#ifndef SRC_UTILS_HPP
#define SRC_UTILS_HPP

#include "behaviour_tree/utils/macros.hpp"
#include "behaviour_tree/utils/utils.hpp"

#include "tests/test_utils/test_macros.hpp"

#define VECTOR_UP godot::Vector3(0, 1, 0)

namespace utils
{

/**
 * @brief Ensures the node is not nullptr. If it is the function searches the path given and if it
 * finds the appropriate node it assigns the value to the member_ptr.
 *
 * @tparam T Any Node type
 * @param member_ptr The pointer reference
 * @param owner Owner of the pointer
 * @param path Path to search if member_ptr is not defined
 */
template <typename T>
inline void ensure_node(T*& member_ptr, const godot::Node* owner, const godot::String& path)
{
    if (member_ptr)
    {
        return;
    }

    Node *node = owner->get_node_or_null(path);

    ERR_FAIL_COND_MSG(!node, vformat("Missing required child '%s' (expected type '%s') in '%s'.",
                                     path, T::get_class_static(), owner->get_name()));

    member_ptr = Object::cast_to<T>(node);

    ERR_FAIL_COND_MSG(!member_ptr, vformat("Node '%s' exists but is not of type '%s' in '%s'.",
                                           path, T::get_class_static(), owner->get_name()));
}

} // namespace utils

#endif /* SRC_UTILS_HPP */
