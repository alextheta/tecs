# tecs — header-only tiny entity-component-system framework
This framework is made to meet my personal API & feature list requirements. No performance tests are ever ran, so it is probably a way slower than [EnTT](https://github.com/skypjack/entt) or [Flecs](https://github.com/SanderMertens/flecs).

This implementation widely uses generics and is based on sparse data storage.
API is ispired by one of the coolest C# ECS frameworks [LeoECS Lite](https://github.com/Leopotam/ecslite)

## How to use?
This is header-only library, so all you need is to copy it to your project (or connect as git submodule), add its folder to include path, then include the ```tecs.hpp``` header.
The framework flow consinsts of 4 major parts that works together.

### 1. World
First of all, you need a ```World``` to store your entities.
```cpp
#include <tecs.hpp>

using namespace te::ecs;

...

World world;
```
### 2. Entities
In this implementation entity is just an integer that is framed in some internal world checks to keep things consistent.

Now you can manage your own entities using the world:
```cpp
auto entity = world.CreateEntity();

...

if (world.HasEntity(entity))
{
    world.DestroyEntity(entity);
}
```

### 3. Components
While entities are just keys or indices, components are where the actual data is stored. Component is a block of data that can be attached to some entity. With tecs you don't need special API to define component:
```cpp
struct Position
{
    float x, y;
}

struct Velocity
{
    float x, y;
}
```
Components are placed in pools that belongs to world. To attach or remove some component to an entity, you need to use these pools:
```cpp
auto positionPool = world.GetPool<Position>();
auto velocityPool = world.GetPool<Velocity>();

auto position = positionPool->Add(entity); // Let's place our entity somewhere in space
position.x = 100.0f;
position.y = 200.0f;

auto velocity = velocityPool->Add(entity); // Let's add gravity to our entity
velocity.x = 0.0f;
velocity.y = 9.8f;

...

if (velocityPool->Has(entity))
{
    velocityPool->Remove(entity);
}

...

if (velocityPool->Has(entity))
{
    auto velocity = velocityPool->Get(entity);
    velocity.x = 0.0f;
}
```
Alternatively, ```ComponentPool->Add(...)``` method supports custom constructor call:
```cpp
struct Position
{
    Position(float x, float y) : x(x), y(y)
    {}

    float x, y;
};

...

positionPool->Add(entity, 100.0f, 200.0f);
```

API details:
* ```ComponentPool::Add```: Exception is thrown if entity is not exist in the world or component is already added to specified entity.
* ```ComponentPool::Get```: Since this method is meant to be fast, no asserts are performed, so ```Get``` of non-existent entity or component will cause crash.
* Note that both ```Add``` and ```Get``` returns reference, so you need to use ```auto&``` instead of ```auto``` if you need your data to be stored in component.

### 4. Systems
The entity. The components. What now? Systems, of course.
Systems in tecs are separated in three interfaces that can be implemented simultaneously on a single system. You won't be forced to implement all three at the time.
```cpp
class GravitySystem : public InitSystem, public UpdateSystem, public DestroySystem
{
    void Init(World &world) override
    {

    }

    void Update(World &world, float deltaTime) override
    {

    }

    void Destroy(World &world) override
    {

    }
}
```
ECS architecture mostly used in game dev, but i do not know what engine you prefer, so update method allows you to pass frame delta time.
Actually, for this example there is no need for ```Init``` & ```Destroy```.

It's time to introduce ```Filter``` and implement ```Update``` method of the system. Filter can separate entities by components that are attached to them. Also filter provides an iterator to access the data.
```cpp
class GravitySystem : public UpdateSystem
{
    void Update(World &world, float deltaTime) override
    {
        auto filter = world.MakeFilter<Velocity, Position>().Build();
    
        for (auto [entity, velocity, position]: filter)
        {
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
        }
    }
}
```
You can define filter using World's ```MakeFilter``` or directly, like ```Filter<Velocity, Position> filter(world)```. After a ```Build``` call, filter will fetch all the entities that have all types of components you mentioned.

If you dont change your entity-component configuration every frame, a point for some specific optimization is to define filter as a system class member (or somewhere on a higher level) and rebuild filter manually when needed:
```cpp
Filter<Velocity, Position> *filter; // Note that Filter has no default constructor and you must pass the World instance on construction

...

filter = new Filter<Velocity, Position>(world);

...

filter->Build();
```
In some cases you need to exclude entities with specific components. ```Exclude``` method of ```Filter``` exsits for this very purpose:
```cpp

auto filter = world.MakeFilter<Velocity, Position>().Exclude<Rotation, Input>().Build();
```
Remember to rebuild your filter after changing the set of components.

Here in tecs systems are stored in ```SystemRegistry``` that attached to some world. The registry is a tool to store and run systems at a desireable time.
```cpp
SystemRegistry systemRegistry(world);

systemRegistry.Add<GravitySystem>();

...

systemRegistry.Init(); // Call this before game start

...

systemRegistry.Update(Time.DeltaTime); // Call this in your game loop. You can skip passing delta time so it will be 0.0f by default

...

systemRegistry.Destroy(); // Call this to free resources and destroy all systems
```

Basically, that's all. Feel free to contact me to ask questions, report bugs and discuss ECS architecture.

## Dependencies

* С++20 STL
* [boost::dynamic_bitset](https://github.com/boostorg/dynamic_bitset)

## What else?

You also can find a list of cool and performant ECS frameworks [here](https://github.com/jslee02/awesome-entity-component-system).
