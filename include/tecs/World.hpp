#pragma once

#include <typeindex>
#include <unordered_map>
#include <boost/dynamic_bitset.hpp>

#include <tecs/ComponentPool.hpp>
#include <tecs/Filter.hpp>

namespace te::ecs
{
    class ComponentPoolBase;

    class World
    {
    public:
        size_t CreateEntity();
        bool HasEntity(const size_t entityId) const;
        void DestroyEntity(const size_t entityId);

        const boost::dynamic_bitset<size_t> &GetEntities() const;

        template<typename T>
        const std::shared_ptr<ComponentPool<T>> GetPool();

        template<typename... IncludeComponents>
        Filter<IncludeComponents...> MakeFilter();

    private:
        std::unordered_map<std::type_index, std::shared_ptr<ComponentPoolBase>> _componentPools;
        boost::dynamic_bitset<size_t> _entities;
        boost::dynamic_bitset<size_t> _recycledEntities;

        void BindPool(std::type_index typeId, std::shared_ptr<ComponentPoolBase> pool);
        bool HasPool(std::type_index typeId) const;
        const std::shared_ptr<ComponentPoolBase> GetPool(std::type_index typeId) const;
    };
}