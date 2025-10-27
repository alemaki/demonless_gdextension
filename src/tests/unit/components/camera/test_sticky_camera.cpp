#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"

#include "components/camera/sticky_camera.hpp"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/vector3.hpp>

struct StickyCameraFixture
{
    StickyCamera* camera = memnew(StickyCamera);
    godot::Node3D* parent = memnew(godot::Node3D);

    StickyCameraFixture()
    {
        parent->add_child(camera);
        ::get_scene_root()->add_child(parent);
    }

    ~StickyCameraFixture()
    {
        parent->remove_child(camera);
        ::get_scene_root()->remove_child(parent);

        memdelete(camera);
        memdelete(parent);
    }
};

TEST_SUITE("StickyCamera")
{
    TEST_CASE_FIXTURE(StickyCameraFixture, "_ready sets top-level and initializes vector_stick")
    {
        CHECK(camera->is_set_as_top_level());
        CHECK_EQ(camera->get_position(), camera->get_vector_stick());
    }

    TEST_CASE_FIXTURE(StickyCameraFixture, "_physics_process applies parent offset correctly")
    {
        // parent moves to some position
        parent->set_position(godot::Vector3(10, 20, 30));

        // camera’s vector_stick is initially its starting pos
        camera->set_vector_stick(godot::Vector3(2, 5, 3));

        camera->_physics_process(0.016);

        godot::Vector3 expected = parent->get_position();
        expected.x += 2;
        expected.z += 3;
        expected.y = 5;

        CHECK_VECTORS_EQ(camera->get_position(), expected);
    }

    TEST_CASE_FIXTURE(StickyCameraFixture, "Camera stays attached if parent moves between frames")
    {
        parent->set_position(godot::Vector3(0, 0, 0));
        camera->set_vector_stick(godot::Vector3(1, 2, 3));
        camera->_physics_process(0.016);

        godot::Vector3 first = camera->get_position();

        parent->set_position(godot::Vector3(5, 5, 5));
        camera->_physics_process(0.016);

        godot::Vector3 second = camera->get_position();

        CHECK_VECTORS_NE(first, second);
        CHECK_EQ(second.x - first.x, doctest::Approx(5.0));
        CHECK_EQ(second.z - first.z, doctest::Approx(5.0));
        // y should remain same because it comes from vector_stick
        CHECK_EQ(second.y, doctest::Approx(2.0));
    }

    TEST_CASE_FIXTURE(StickyCameraFixture, "Setting different vector_stick changes follow offset")
    {
        parent->set_position(godot::Vector3(0, 0, 0));
        camera->set_vector_stick(godot::Vector3(10, 2, -5));
        camera->_physics_process(0.016);

        godot::Vector3 expected(10, 2, -5);
        CHECK_VECTORS_EQ(camera->get_position(), expected);
    }
}

TEST_SUITE("[errors] StickyCamera")
{
    TEST_CASE("Fails if no parent")
    {
        StickyCamera* cam = memnew(StickyCamera);
        ::get_scene_root()->add_child(cam);

        CHECK_GODOT_ERROR(cam->_physics_process(0.016));

        ::get_scene_root()->remove_child(cam);
        memdelete(cam);
    }

    TEST_CASE("Fails if parent is not Node3D")
    {
        godot::Node* non3DParent = memnew(godot::Node);
        StickyCamera* cam = memnew(StickyCamera);
        non3DParent->add_child(cam);
        ::get_scene_root()->add_child(non3DParent);

        CHECK_GODOT_ERROR(cam->_physics_process(0.016));

        memdelete(cam);
        memdelete(non3DParent);
    }
}