#pragma once

#include <memory>

#include <tecs/World.hpp>
#include <tecs/ComponentPool.hpp>

namespace te::ecs
{
    size_t World::CreateEntity()
    {
        auto newEntity = _recycledEntities.find_first();
        if (newEntity != boost::dynamic_bitset<size_t>::npos)
        {
            _recycledEntities.reset(newEntity);
            _entities.set(newEntity);
            return newEntity;
        }

        newEntity = _entities.count();

        if (_entities.count() <= newEntity)
        {
            _entities.resize(newEntity + 1);
        }

        _entities.set(newEntity);
        return newEntity;
    }

    bool World::HasEntity(const size_t entityId) const
    {
        return entityId < _entities.size() && _entities.test(entityId);
    }

    void World::DestroyEntity(const size_t entityId)
    {
        if (_entities.test(entityId))
        {
            _entities.reset(entityId);

            if (_recycledEntities.count() <= entityId)
            {
                _recycledEntities.resize(entityId + 1);
            }

            _recycledEntities.set(entityId);

            for (auto &[_, pool]: _componentPools)
            {
                pool->Remove(entityId);
            }
        }
    }

    const boost::dynamic_bitset<size_t> &World::GetEntities() const
    {
        return _entities;
    }

    template<typename T>
    const std::shared_ptr<ComponentPool<T>> World::GetPool()
    {
        std::type_index typeId = typeid(T);

        if (!HasPool(typeId))
        {
            BindPool(typeId, std::make_shared<ComponentPool<T>>(*this));
        }

        return std::static_pointer_cast<ComponentPool<T>>(GetPool(typeId));
    }

    template<typename... IncludeComponents>
    Filter<IncludeComponents...> World::MakeFilter()
    {
        return Filter<IncludeComponents...>(*this);
    }

    void World::BindPool(std::type_index typeId, std::shared_ptr<ComponentPoolBase> pool)
    {
        _componentPools[typeId] = pool;
    }

    bool World::HasPool(std::type_index typeId) const
    {
        return _componentPools.count(typeId) != 0;
    }

    const std::shared_ptr<ComponentPoolBase> World::GetPool(std::type_index typeId) const
    {
        if (!HasPool(typeId))
        {
            return nullptr;
        }

        return _componentPools.at(typeId);
    }
}
