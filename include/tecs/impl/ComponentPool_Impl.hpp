#pragma once

#include <tecs/ComponentPool.hpp>

namespace te::ecs
{
    template<typename T>
    template<typename... Args>
    inline T &ComponentPool<T>::Add(size_t entityId, Args &&... args)
    {
        if (!EntityIsValid(entityId))
        {
            throw std::runtime_error(std::format("Entity {} is not exists in world", entityId));
        }

        if (_components.Test(entityId))
        {
            throw std::runtime_error(std::format("Component {} is already attached to entity {}", typeid(T).name(), entityId));
        }

        auto newItem = T(std::forward<Args>(args)...);
        return *_components.Insert(entityId, newItem);
    }

    template<typename T>
    inline T &ComponentPool<T>::Get(size_t entityId)
    {
        return *_components.Get(entityId);
    }

    template<typename T>
    inline void ComponentPool<T>::Remove(size_t entityId)
    {
        _components.Remove(entityId);
    }

    template<typename T>
    inline bool ComponentPool<T>::Has(size_t entityId) const
    {
        return _components.Test(entityId);
    }

    template<typename T>
    inline boost::dynamic_bitset<size_t> ComponentPool<T>::GetEntities() const
    {
        return _components.GetBitset();
    }
}