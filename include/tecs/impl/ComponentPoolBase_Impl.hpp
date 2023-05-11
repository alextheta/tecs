#pragma once

#include <tecs/ComponentPoolBase.hpp>
#include <tecs/World.hpp>

namespace te::ecs
{
    inline ComponentPoolBase::ComponentPoolBase(World &world) : _world(world)
    {}

    inline bool ComponentPoolBase::EntityIsValid(size_t entityId) const
    {
        return _world.HasEntity(entityId);
    }
}