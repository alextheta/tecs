#pragma once

#include <tecs/ComponentPoolBase.hpp>
#include <tecs/World.hpp>

namespace te::ecs
{
    ComponentPoolBase::ComponentPoolBase(World &world) : _world(world)
    {}

    bool ComponentPoolBase::EntityIsValid(size_t entityId) const
    {
        return _world.HasEntity(entityId);
    }
}