#pragma once

#include <tecs/Filter.hpp>

namespace te::ecs
{
    template<typename... IncludeComponents>
    Filter<IncludeComponents ...>::Filter::Filter(World &world) : _world(world)
    {
        _entities.reset();

        _includedPoolsCache.clear();
        (_includedPoolsCache.push_back(_world.GetPool<IncludeComponents>()), ...);
    }

    template<typename... IncludeComponents>
    template<typename... ExcludeComponents>
    Filter<IncludeComponents ...> &Filter<IncludeComponents ...>::Exclude()
    {
        _excludedPoolsCache.clear();
        (_excludedPoolsCache.push_back(_world.GetPool<ExcludeComponents>()), ...);

        return *this;
    }

    template<typename... IncludeComponents>
    Filter<IncludeComponents ...> &Filter<IncludeComponents ...>::Build()
    {
        ApplyIncludes();
        ApplyExcludes();

        _entities.shrink_to_fit();

        return *this;
    }

    template<typename... IncludeComponents>
    Filter<IncludeComponents ...>::Iterator Filter<IncludeComponents ...>::begin()
    {
        return Iterator(*this, _entities.find_first());
    }

    template<typename... IncludeComponents>
    Filter<IncludeComponents ...>::Iterator Filter<IncludeComponents ...>::end()
    {
        size_t lastEntity = _entities.find_first();
        do
        {
            lastEntity = _entities.find_next(lastEntity);
        } while (lastEntity != boost::dynamic_bitset<>::npos);

        return Iterator(*this, lastEntity);
    }

    template<typename... IncludeComponents>
    void Filter<IncludeComponents ...>::EqualizeBitsets(boost::dynamic_bitset<size_t> &first, boost::dynamic_bitset<size_t> &second)
    {
        auto firstSetSize = first.size();
        auto secondSetSize = second.size();
        auto biggerSetSize = std::max(firstSetSize, secondSetSize);

        first.resize(biggerSetSize);
        second.resize(biggerSetSize);
    }

    template<typename... IncludeComponents>
    void Filter<IncludeComponents ...>::ApplyIncludes()
    {
        for (auto &pool: _includedPoolsCache)
        {
            auto entitiesInPool = pool->GetEntities();

            EqualizeBitsets(entitiesInPool, _entities);

            _entities |= entitiesInPool;
        }
    }

    template<typename... IncludeComponents>
    void Filter<IncludeComponents ...>::ApplyExcludes()
    {
        for (auto &pool: _excludedPoolsCache)
        {
            auto entitiesInPool = pool->GetEntities();

            EqualizeBitsets(entitiesInPool, _entities);

            _entities &= ~entitiesInPool;
        }
    }

    template<typename... IncludeComponents>
    Filter<IncludeComponents ...>::Iterator::Iterator(Filter<IncludeComponents ...> &filter, size_t startEntity) : _filter(filter), _currentEntity(startEntity)
    {}

    template<typename... IncludeComponents>
    bool Filter<IncludeComponents ...>::Iterator::operator!=(const Iterator &other)
    {
        return _currentEntity != other._currentEntity;
    }

    template<typename... IncludeComponents>
    typename Filter<IncludeComponents ...>::Iterator &Filter<IncludeComponents ...>::Iterator::operator++()
    {
        _currentEntity = _filter._entities.find_next(_currentEntity);
        return *this;
    }

    template<typename... IncludeComponents>
    std::tuple<size_t, IncludeComponents &...> Filter<IncludeComponents ...>::Iterator::operator*()
    {
        return BuildTuple(_currentEntity, std::make_index_sequence<sizeof...(IncludeComponents)>{});
    }

    template<typename... IncludeComponents>
    template<std::size_t... Is>
    std::tuple<size_t, IncludeComponents &...> Filter<IncludeComponents ...>::Iterator::BuildTuple(int index, std::index_sequence<Is...>)
    {
        return std::make_tuple(index, std::ref(std::static_pointer_cast<ComponentPool<IncludeComponents>>(_filter._includedPoolsCache[Is])->Get(index))...);
    }
}