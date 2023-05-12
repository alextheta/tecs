#pragma once

#include <tecs/Systems/SystemBase.hpp>

namespace te::ecs
{
    class UpdateSystem : public SystemBase
    {
    public:
        virtual void Update(World &world, float deltaTime = 0.0f) = 0;
    };
}