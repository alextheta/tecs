#pragma once

#include <tecs/Filter.hpp>

namespace te::ecs
{
    template<typename... Components>
    Filter<Components ...>::Filter(World &world) : _world(world)
    {
        Update();
    }

    template<typename... Components>
    void Filter<Components ...>::Update()
    {
        _poolCache.clear();

        (_poolCache.push_back(_world.GetPool<Components>()), ...);

        _entities = _world.GetEntities();

        for (auto &pool: _poolCache)
        {
            auto entitiesInPool = pool->GetEntities();
            _entities.resize(entitiesInPool.size());
            _entities &= entitiesInPool;
        }

        _entities.shrink_to_fit();
    }

    template<typename... Components>
    Filter<Components ...>::Iterator Filter<Components ...>::begin()
    {
        return Iterator(*this, _entities.find_first());
    }

    template<typename... Components>
    Filter<Components ...>::Iterator Filter<Components ...>::end()
    {
        size_t lastEntity = _entities.find_first();
        do
        {
            lastEntity = _entities.find_next(lastEntity);
        } while (lastEntity != boost::dynamic_bitset<>::npos);

        return Iterator(*this, lastEntity);
    }

    template<typename... Components>
    Filter<Components ...>::Iterator::Iterator(Filter<Components ...> &filter, size_t startEntity) : _filter(filter), _currentEntity(startEntity)
    {}

    template<typename... Components>
    bool Filter<Components ...>::Iterator::operator!=(const Iterator &other)
    {
        return _currentEntity != other._currentEntity;
    }

    template<typename... Components>
    typename Filter<Components ...>::Iterator &Filter<Components ...>::Iterator::operator++()
    {
        _currentEntity = _filter._entities.find_next(_currentEntity);
        return *this;
    }

    template<typename... Components>
    std::tuple<size_t, Components &...> Filter<Components ...>::Iterator::operator*()
    {
        return BuildTuple(_currentEntity, std::make_index_sequence<sizeof...(Components)>{});
    }

    template<typename... Components>
    template<std::size_t... Is>
    std::tuple<size_t, Components &...> Filter<Components ...>::Iterator::BuildTuple(int index, std::index_sequence<Is...>)
    {
        return std::make_tuple(index, std::ref(std::static_pointer_cast<ComponentPool<Components>>(_filter._poolCache[Is])->Get(index))...);
    }
}