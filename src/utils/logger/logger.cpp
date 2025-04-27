#include "logger.hpp"

bool utils::Logger::is_class_enabled(const godot::StringName &name) const
{
    if (enabled_classes.has(name))
    {
        return true;
    }
    else if (disabled_classes.has(name))
    {
        return false;
    }
    
    if (!this->every_class_enabled) /* No need to check both bases */
    {
        for (const godot::StringName& base_class : enabled_base_classes)
        {
            if(is_subclass(name, base_class))
            {
                return true;
            }
        }
    }
    else
    {
        for (const godot::StringName& base_class : disabled_base_classes)
        {
            if(is_subclass(name, base_class))
            {
                return false;
            }
        }
    }

    return this->every_class_enabled;
}

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

void utils::Logger::disable_base_class(const godot::StringName &name)
{
    this->disabled_base_classes.insert(name);
    this->enabled_base_classes.erase(name);
}

void utils::Logger::enable_base_class(const godot::StringName &name)
{
    
    this->enabled_base_classes.insert(name);
    this->disabled_base_classes.erase(name);
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