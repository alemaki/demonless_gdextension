#include "logger.hpp"

void utils::Logger::disable_class(const godot::StringName &name)
{
    this->disabled_classes.insert(name);
    this->enabled_classes.erase(name);
}

void utils::Logger::enable_class(const godot::StringName &name)
{
    this->enabled_classes.insert(name);
    this->disabled_classes.erase(name);
}

void utils::Logger::enable_level(Level level)
{
    this->enabled |= level;
}

void utils::Logger::disable_level(Level level)
{
    this->enabled &= ~level;
}

void utils::Logger::enable_all_levels()
{
    this->enabled = CRIT_ERR | INFO | DEBG;
}