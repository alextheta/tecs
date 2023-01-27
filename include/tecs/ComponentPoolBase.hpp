#pragma once

#include <boost/dynamic_bitset.hpp>

namespace te::ecs
{
    class World;

    class ComponentPoolBase
    {
    public:
        ComponentPoolBase() = default;
        ComponentPoolBase(World &world);
        virtual ~ComponentPoolBase() = default;

        virtual bool Has(size_t entityId) const = 0;
        virtual void Remove(size_t entityId) = 0;
        virtual boost::dynamic_bitset<size_t> GetEntities() const = 0;

    protected:
        World &_world;

        bool EntityIsValid(size_t entityId) const;
    };
}