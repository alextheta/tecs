#pragma once

#include <format>

#include <tecs/SparseContainer.hpp>
#include <tecs/ComponentPoolBase.hpp>

namespace te::ecs
{
    template<typename T>
    class ComponentPool : public ComponentPoolBase
    {
    public:
        using ComponentPoolBase::ComponentPoolBase;

        template<typename... Args>
        T &Add(size_t entityId, Args &&... args);
        T &Get(size_t entityId);
        bool Has(size_t entityId) override;
        void Remove(size_t entityId) override;

        boost::dynamic_bitset<size_t> &GetEntities() override;

    private:
        SparseContainer<T> _components;
    };
}