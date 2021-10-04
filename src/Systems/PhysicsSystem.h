#pragma once
#include "Components/Transform.h"
#include "Math/Math_includes.h"
#include "Systems/DebugSystem.h"

// physics************************************************************ boundary system aka map
struct physics_system : paperback::system::instance
{
private:

    struct Force
    {

        std::string m_Tag; //default tagging to allow for overwriting existing force
        float m_Magnitude; //magnitude of the force to be applied
        float m_Lifetime; //how long the force will be exerted
        xcore::vector3 m_Direction; //direction of force, ideally to be unit vector, but can be any value
    };

    struct ForceHandler
    {

        size_t m_ID; // id tag for the entity that the forces are assigned to
        std::vector<Force> m_Forces; // contains all forces acting on instance with id
        xcore::vector3 m_TotalForces; // contains the summation of all sources acting on the entity

        // updates the current acting total forces and handles the lifetime of all the forces
        void GetForces(float deltatime)
        {

            m_TotalForces.setZero();

            for (Force& force : m_Forces)
            {

                force.m_Lifetime -= deltatime;

                m_TotalForces += force.m_Direction * force.m_Magnitude;
            }

            //remove forces that are now not live
            auto end = std::remove_if(std::begin(m_Forces), std::end(m_Forces), [](Force& force) { return force.m_Lifetime < 0.0f; });

            if (end != std::end(m_Forces))
            {

                m_Forces.erase(end, m_Forces.end());
            }
        }
    };
       
    
public:

    std::vector<ForceHandler> m_AllForces{};

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "physics_system"
    };

    xcore::vector3 GetForceForEntity(size_t id)
    {

        for (ForceHandler& handler : m_AllForces)
        {

            if (handler.m_ID == id)
                return handler.m_TotalForces;
        }

        return {};
    }

    void AddForceToEntity(float dir_x, float dir_y, float dir_z, float magnitude, size_t id, std::string tag = "",  float lifetime = 1.0f)
    {

        //check if there's any existing handler
        auto check = std::find_if(std::begin(m_AllForces), std::end(m_AllForces), [id](ForceHandler& handler) { return handler.m_ID == id; });
        std::vector<Force>* forces;


        if (check == std::end(m_AllForces))
        {

            m_AllForces.push_back({});

            m_AllForces.back().m_ID = id;

            forces = &m_AllForces.back().m_Forces;
        }
        else
        {

            forces = &check->m_Forces;
        }

        if (tag == "")
        {

            //tagless force, just add new
            forces->push_back({});

            forces->back().m_Direction.m_X = dir_x;
            forces->back().m_Direction.m_Y = dir_y;
            forces->back().m_Direction.m_Z = dir_z;

            forces->back().m_Magnitude = magnitude;

            forces->back().m_Lifetime = lifetime;
        }
        else
        {

            //tagged force, check if it exists
            auto tagged = std::find_if(std::begin(*forces), std::end(*forces), [tag](Force& single) { return tag == single.m_Tag; });

            if (tagged != std::end(*forces))
            {

                tagged->m_Direction.m_X = dir_x;
                tagged->m_Direction.m_Y = dir_y;
                tagged->m_Direction.m_Z = dir_z;

                tagged->m_Magnitude = magnitude;

                tagged->m_Lifetime = lifetime;
            }
            else
            {

                forces->push_back({});

                forces->back().m_Tag = tag;
                forces->push_back({});

                forces->back().m_Direction.m_X = dir_x;
                forces->back().m_Direction.m_Y = dir_y;
                forces->back().m_Direction.m_Z = dir_z;

                forces->back().m_Magnitude = magnitude;

                forces->back().m_Lifetime = lifetime;
            }
        }
    }

    void OnFrameStart(void) noexcept 
    {
        
        GetSystem<debug_system>().BeginTime(1);
    }

    void PreUpdate(void) noexcept 
    {

        for (ForceHandler& handler : m_AllForces)
        {

            handler.GetForces(DeltaTime());
        }
    }

    void PostUpdate(void) noexcept
    {

        auto end =std::remove_if(std::begin(m_AllForces), std::end(m_AllForces), [](ForceHandler& handler) { return handler.m_Forces.empty(); });
        
        if (end != std::end(m_AllForces))
        {

            m_AllForces.erase(end, m_AllForces.end());
        }
    }

    void OnFrameEnd(void) noexcept 
    {
        
        GetSystem<debug_system>().EndTime(1);
    }


    // map check collision out of bounds check
    void operator()(paperback::component::entity& Entity, transform& Transform, rigidbody& RigidBody ) noexcept
    {



        /*
        // X-Out-Of-Bounds
        if  (Transform.m_Position.m_X < 0.0f)
        {
            Transform.m_Position.m_X = 0.0f;
            RigidBody.m_Velocity.m_X = -RigidBody.m_Velocity.m_X;
        }
        else if (Transform.m_Position.m_X >= m_Engine.m_Width)
        {
            Transform.m_Position.m_X = m_Engine.m_Width;
            RigidBody.m_Velocity.m_X = -RigidBody.m_Velocity.m_X;
        }

        // Y-Out-Of-Bounds
        if (Transform.m_Position.m_Y < 0.0f)
        {
            Transform.m_Position.m_Y = 0.0f;
            RigidBody.m_Velocity.m_Y = -RigidBody.m_Velocity.m_Y;
        }
        else if (Transform.m_Position.m_Y >= m_Engine.m_Height)
        {
            Transform.m_Position.m_Y = m_Engine.m_Height;
            RigidBody.m_Velocity.m_Y = -RigidBody.m_Velocity.m_Y;
        }*/

        RigidBody.m_Velocity = GetForceForEntity(Entity.m_UID);

        Transform.m_Position += RigidBody.m_Velocity * m_Coordinator.DeltaTime();

        xcore::vector3 new_pos = Transform.m_Position + Transform.m_Offset;

        Transform.fakeSphere.setCenter({ new_pos.m_X, new_pos.m_Y, new_pos.m_Z });
    }
};