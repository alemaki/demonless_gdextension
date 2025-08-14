#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include "utils/utils.hpp"
#include "projectiles/projectile.hpp"
#include "skills/projectile_time_jump.hpp"

TEST_SUITE("ProjectileTimeJump")
{
    struct DummyMovementStrategy : public MovementStrategy
    {
        int apply_calls = 0;
        double last_delta = 0;
        godot::Ref<MovementContext> last_ctx;

        void apply(godot::Ref<MovementContext> context, double delta) override
        {
            apply_calls++;
            last_ctx = context;
            last_delta = delta;
            /* Just modify the position for testing purposes */
            context->set_position(context->get_position() + godot::Vector3(42, 0, 0));
        }
    };

    TEST_CASE("Time jump calls movement strategy if present")
    {
        Projectile* projectile = memnew(Projectile);
        DummyMovementStrategy* strategy = memnew(DummyMovementStrategy);
        projectile->set_movement_strategy(strategy);

        godot::Ref<MovementContext> ctx = memnew(MovementContext);
        ctx->set_position(godot::Vector3(0, 0, 0));
        ctx->set_speed(10.0);
        ctx->set_direction(godot::Vector3(1, 0, 0));
        projectile->set_movement_context(ctx);

        ::get_scene_root()->add_child(projectile);

        ProjectileTimeJump* jump = memnew(ProjectileTimeJump);
        jump->set_time_skip(2.0);

        jump->step(0.0);

        CHECK_EQ(strategy->apply_calls, 1);
        CHECK_EQ(strategy->last_delta, 2.0);
        CHECK_EQ(strategy->last_ctx, ctx);

        CHECK_VECTORS_EQ(projectile->get_position(), godot::Vector3(42, 0, 0));

        memdelete(jump);
        memdelete(projectile);
    }

    TEST_CASE("Time jump moves projectile without strategy")
    {
        Projectile* projectile = memnew(Projectile);
        godot::Ref<MovementContext> ctx = memnew(MovementContext);
        ctx->set_position(godot::Vector3(0, 0, 0));
        ctx->set_speed(5.0);
        ctx->set_direction(godot::Vector3(0, 1, 0));
        projectile->set_movement_context(ctx);

        ::get_scene_root()->add_child(projectile);

        ProjectileTimeJump* jump = memnew(ProjectileTimeJump);
        jump->set_time_skip(3.0);

        jump->step(0.0);

        /* Expected displacement = direction * speed * time_skip = (0,1,0) * 5 * 3 */
        godot::Vector3 expected_position = godot::Vector3(0, 15, 0);
        CHECK_VECTORS_EQ(projectile->get_position(), expected_position);
        CHECK_VECTORS_EQ(ctx->get_position(), expected_position);

        memdelete(jump);
        memdelete(projectile);
    }

    TEST_CASE("Time jump ends instantly after step")
    {
        ProjectileTimeJump* jump = memnew(ProjectileTimeJump);
        jump->set_time_skip(1.0);

        /* Also can run with an empty scene */
        jump->step(0.0);

        CHECK(jump->is_finished());

        memdelete(jump);
    }
}