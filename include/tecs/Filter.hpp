#pragma once

#include <boost/dynamic_bitset.hpp>

#include <tecs/World.hpp>

namespace te::ecs
{
    template<typename... IncludeComponents>
    class Filter
    {
    public:
//        static_assert(sizeof...(IncludeComponents) > 0, "There are no components to filter");

        class Iterator
        {
        public:
            Iterator() = delete;
            Iterator(Filter &filter, size_t startEntity);

            bool operator!=(const Iterator &other);
            Iterator &operator++();
            std::tuple<size_t, IncludeComponents &...> operator*();

        private:
            Filter &_filter;
            size_t _currentEntity;

            template<std::size_t... Is>
            std::tuple<size_t, IncludeComponents &...> BuildTuple(int index, std::index_sequence<Is...>);
        };

        Filter(World &world);

        template<typename... ExcludeComponents>
        Filter &Exclude();

        void Update();

        Iterator begin();
        Iterator end();

        boost::dynamic_bitset<size_t> _entities;

    private:
        World &_world;

        std::vector<std::shared_ptr<ComponentPoolBase>> _includedPoolsCache;
        std::vector<std::shared_ptr<ComponentPoolBase>> _excludedPoolsCache;

        void EqualizeBitsets(boost::dynamic_bitset<size_t> &first, boost::dynamic_bitset<size_t> &second);
        void ApplyIncludes();
        void ApplyExcludes();
    };
}