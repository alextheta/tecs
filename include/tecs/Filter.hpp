#pragma once

#include <boost/dynamic_bitset.hpp>

#include <tecs/World.hpp>

namespace te::ecs
{
    template<typename... Components>
    class Filter
    {
    public:
        class Iterator
        {
        public:
            Iterator() = delete;
            Iterator(Filter &filter, size_t startEntity);

            bool operator!=(const Iterator &other);
            Iterator &operator++();
            std::tuple<size_t, Components &...> operator*();

        private:
            Filter &_filter;
            size_t _currentEntity;

            template<std::size_t... Is>
            std::tuple<size_t, Components &...> BuildTuple(int index, std::index_sequence<Is...>);
        };

        Filter(World &world);

        void Update();

        Iterator begin();
        Iterator end();

    private:
        World &_world;
        boost::dynamic_bitset<size_t> _entities;
        std::vector<std::shared_ptr<ComponentPoolBase>> _poolCache;
    };
}