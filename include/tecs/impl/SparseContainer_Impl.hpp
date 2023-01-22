#pragma once

#include <tecs/SparseContainer.hpp>

namespace te::ecs
{
    template<typename T>
    SparseContainer<T>::SparseContainer() : SparseContainer(defaultCapacity)
    {}

    template<typename T>
    SparseContainer<T>::SparseContainer(size_t initialCapacity)
    {
        Reserve(initialCapacity);
    }

    template<typename T>
    T *SparseContainer<T>::Insert(size_t index, T &value)
    {
        size_t denseIndex;
        if (Test(index))
        {
            denseIndex = _sparseOrder[index];
        }
        else
        {
            denseIndex = Count();
        }

        if (index >= denseIndex)
        {
            _sparseBitset.resize(index + 1);
            _sparseOrder.resize(index + 1);

            _dense.push_back(index);
            _denseData.push_back(value);
        }
        else
        {
            _sparseOrder[index] = denseIndex;
            _dense[denseIndex] = index;
            _denseData[denseIndex] = std::move(value);
        }

        _sparseBitset.set(index);
        _sparseOrder[index] = denseIndex;

        return Get(index);
    }

    template<typename T>
    T *SparseContainer<T>::Get(size_t index)
    {
        if (index >= _sparseBitset.size())
        {
            return nullptr;
        }

        auto sparseIndex = _sparseOrder[index];
        if (_sparseOrder[index] < Count() && _dense[sparseIndex] == index)
        {
            return &_denseData[sparseIndex];
        }

        return nullptr;
    }

    template<typename T>
    bool SparseContainer<T>::Test(size_t index) const
    {
        size_t currentSize = _sparseOrder.size();
        if (index >= currentSize)
        {
            return false;
        }

        return (_sparseOrder[index] < currentSize && _dense[_sparseOrder[index]] == index);
    }

    template<typename T>
    void SparseContainer<T>::Remove(size_t index)
    {
        size_t denseIndex = _sparseOrder[index];
        size_t lastItemIndex = Count() - 1;

        std::swap(_dense[lastItemIndex], _dense[denseIndex]);
        std::swap(_denseData[lastItemIndex], _denseData[denseIndex]);

        _sparseOrder[_dense[denseIndex]] = denseIndex;
        _sparseBitset.reset(index);
    }

    template<typename T>
    void SparseContainer<T>::Reserve(size_t capacity)
    {
        _sparseBitset.reserve(capacity);
        _sparseOrder.reserve(capacity);
        _dense.reserve(capacity);
        _denseData.reserve(capacity);
    }

    template<typename T>
    size_t SparseContainer<T>::Count() const
    {
        return _sparseBitset.count();
    }

    template<typename T>
    boost::dynamic_bitset<size_t> SparseContainer<T>::GetBitset() const
    {
        return _sparseBitset;
    }
}