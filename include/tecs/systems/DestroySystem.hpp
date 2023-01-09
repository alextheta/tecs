#pragma once

#include <tecs/Systems/SystemBase.hpp>

namespace te::ecs
{
    class DestroySystem : public SystemBase
    {
    public:
        virtual void Destroy(World &world) = 0;
    };
}