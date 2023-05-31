#pragma once

#include <vector>
#include <memory>

#include <tecs/Systems/InitSystem.hpp>
#include <tecs/Systems/UpdateSystem.hpp>
#include <tecs/Systems/DestroySystem.hpp>

namespace te::ecs
{
    class SystemRegistry
    {
    public:
        SystemRegistry() = delete;
        explicit SystemRegistry(const std::shared_ptr<World> &world);
        ~SystemRegistry() = default;

        template<typename T, typename... Args>
        SystemRegistry &Add(Args &&... args);

        void Clear();

        void Init() const;
        void Update(float deltaTime = 0.0f) const;
        void Destroy();

        void SetWorld(const std::shared_ptr<World> &world, bool init = false);

        World &GetWorld() const;

    private:
        enum SystemType
        {
            None = -1,
            InitSystemType = 0,
            UpdateSystemType = 1,
            DestroySystemType = 2,
            Last
        };

        void Add(SystemType type, const std::shared_ptr<SystemBase> &system);

        std::shared_ptr<World> _world;
        std::vector<std::vector<std::shared_ptr<SystemBase>>> _systems;
    };
}