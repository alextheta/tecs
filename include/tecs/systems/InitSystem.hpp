#pragma once

#include <tecs/Systems/SystemBase.hpp>

namespace te::ecs
{
    class InitSystem : public SystemBase
    {
    public:
        virtual void Init(World &world) = 0;
    };
}