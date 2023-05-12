#pragma once

#include <vector>
#include <boost/dynamic_bitset.hpp>

namespace te::ecs
{
    template<typename T>
    class SparseContainer
    {
    public:
        static const size_t defaultCapacity = 512;

        SparseContainer();
        explicit SparseContainer(size_t initialCapacity);

        T *Insert(size_t index, T &value);
        T *Get(size_t index);
        bool Test(size_t index) const;
        void Remove(size_t index);

        size_t Count() const;

        void Reserve(size_t capacity);

        boost::dynamic_bitset<size_t> GetBitset() const;

    private:
        boost::dynamic_bitset<size_t> _sparseBitset;
        std::vector<size_t> _sparseOrder;
        std::vector<size_t> _dense;
        std::vector<T> _denseData;
    };
}