#pragma once

#include <tecs/SystemRegistry.hpp>

namespace te::ecs
{
    inline SystemRegistry::SystemRegistry(const std::shared_ptr<World> &world)
    {
        Clear();
        SetWorld(world);
    }

    inline void SystemRegistry::Clear()
    {
        _systems.clear();

        for (int i = 0; i < SystemType::Last; i++)
        {
            _systems.emplace_back();
        }
    }

    template<typename T, typename... Args>
    inline SystemRegistry &SystemRegistry::Add(Args &&... args)
    {
        auto system = std::make_shared<T>(std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<InitSystem, T>)
        {
            Add(InitSystemType, std::static_pointer_cast<InitSystem>(system));
        }
        if constexpr (std::is_base_of_v<UpdateSystem, T>)
        {
            Add(UpdateSystemType, std::static_pointer_cast<UpdateSystem>(system));
        }
        if constexpr (std::is_base_of_v<DestroySystem, T>)
        {
            Add(DestroySystemType, std::static_pointer_cast<DestroySystem>(system));
        }

        return *this;
    }

    inline void SystemRegistry::Add(SystemType type, const std::shared_ptr<SystemBase> &system)
    {
        _systems[type].push_back(system);
    }

    inline void SystemRegistry::Init() const
    {
        for (auto &system: _systems[InitSystemType])
        {
            std::static_pointer_cast<InitSystem>(system)->Init(*_world);
        }
    }

    inline void SystemRegistry::Update(float deltaTime) const
    {
        for (auto &system: _systems[UpdateSystemType])
        {
            std::static_pointer_cast<UpdateSystem>(system)->Update(*_world, deltaTime);
        }
    }

    inline void SystemRegistry::Destroy()
    {
        for (auto &system: _systems[DestroySystemType])
        {
            std::static_pointer_cast<DestroySystem>(system)->Destroy(*_world);
        }

        Clear();
    }

    inline void SystemRegistry::SetWorld(const std::shared_ptr<World> &world, bool init)
    {
        _world = world;

        if (init)
        {
            Init();
        }
    }

    inline World &SystemRegistry::GetWorld() const
    {
        return *_world;
    }
}