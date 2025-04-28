#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "utils/logger/logger.hpp"

struct LoggerFixture
{
    utils::Logger* logger = utils::Logger::instance;

    LoggerFixture() { reset(); }
    ~LoggerFixture() { reset(); }

    void reset()
    {
        logger->set_every_class_enabled(true);
        logger->enabled_classes.clear();
        logger->disabled_classes.clear();
        logger->enabled_base_classes.clear();
        logger->disabled_base_classes.clear();
    }
};

TEST_SUITE("LoggerTests")
{
    TEST_CASE_FIXTURE(LoggerFixture, "Every class is enabled by default")
    {
        CHECK(logger->is_class_enabled("Node3D"));
        CHECK(logger->is_class_enabled("SomeUnregisteredClassName"));
    }

    TEST_CASE_FIXTURE(LoggerFixture, "Disabling a class blocks only that class")
    {
        logger->disable_class("Node3D");

        CHECK_FALSE(logger->is_class_enabled("Node3D"));
        CHECK(logger->is_class_enabled("Node2D"));
        CHECK(logger->is_class_enabled("Node"));
    }

    TEST_CASE_FIXTURE(LoggerFixture, "Re-enabling a disabled class allows it again")
    {
        logger->disable_class("Node3D");
        logger->enable_class("Node3D");

        CHECK(logger->is_class_enabled("Node3D"));
    }

    TEST_CASE_FIXTURE(LoggerFixture, "Disabling a base class blocks all of its subclasses")
    {
        logger->disable_base_class("Node");

        CHECK_FALSE(logger->is_class_enabled("Node3D"));
        CHECK_FALSE(logger->is_class_enabled("Node2D"));
        CHECK(logger->is_class_enabled("RefCounted"));   // unrelated hierarchy, unaffected
    }

    TEST_CASE_FIXTURE(LoggerFixture, "An explicit class enable overrides a disabled base class")
    {
        logger->disable_base_class("Node");
        logger->enable_class("Node3D");

        CHECK(logger->is_class_enabled("Node3D"));
        CHECK_FALSE(logger->is_class_enabled("Node2D")); // no explicit override, still blocked
    }

    TEST_CASE_FIXTURE(LoggerFixture, "An explicit class disable overrides an enabled base class")
    {
        logger->set_every_class_enabled(false);
        logger->enable_base_class("Node");
        logger->disable_class("Node3D");

        CHECK_FALSE(logger->is_class_enabled("Node3D"));
        CHECK(logger->is_class_enabled("Node2D")); // no explicit override, still allowed
    }

    TEST_CASE_FIXTURE(LoggerFixture, "Opt-in mode disables everything unless explicitly enabled")
    {
        logger->set_every_class_enabled(false);

        CHECK_FALSE(logger->is_class_enabled("Node3D"));

        logger->enable_class("Node3D");
        CHECK(logger->is_class_enabled("Node3D"));
        CHECK_FALSE(logger->is_class_enabled("Node2D"));
    }

    TEST_CASE_FIXTURE(LoggerFixture, "Opt-in mode with an enabled base class allows all of its subclasses")
    {
        logger->set_every_class_enabled(false);
        logger->enable_base_class("Node");

        CHECK(logger->is_class_enabled("Node3D"));
        CHECK(logger->is_class_enabled("Node2D"));
        CHECK_FALSE(logger->is_class_enabled("RefCounted"));
    }

    TEST_CASE_FIXTURE(LoggerFixture, "Switching modes re-activates the opposite toggle set")
    {
        logger->disable_class("Node3D");
        logger->set_every_class_enabled(false);
        logger->enable_class("Node3D");

        CHECK(logger->is_class_enabled("Node3D"));
        CHECK_FALSE(logger->is_class_enabled("Node2D"));
    }
}
