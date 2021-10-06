// ----- Start of header guard
#ifndef PHYSICS_STRUCTS_H
#define PHYSICS_STRUCTS_H

#include "Math/Math_includes.h"
#include <vector>

struct Force
{

    std::string m_Tag; //default tagging to allow for overwriting existing force
    float m_Magnitude; //magnitude of the force to be applied
    float m_Lifetime; //how long the force will be exerted
    xcore::vector3 m_Direction; //direction of force, ideally to be unit vector, but can be any value
};

struct ForceHandler
{

    //size_t m_ID; // id tag for the entity that the forces are assigned to
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

    void AddForce(float dir_x, float dir_y, float dir_z, float magnitude, std::string tag = "", float lifetime = 1.0f)
    {

        if (tag == "")
        {

            //tagless force, just add new
            m_Forces.push_back({});

            m_Forces.back().m_Direction.m_X = dir_x;
            m_Forces.back().m_Direction.m_Y = dir_y;
            m_Forces.back().m_Direction.m_Z = dir_z;

            m_Forces.back().m_Magnitude = magnitude;

            m_Forces.back().m_Lifetime = lifetime;
        }
        else
        {

            //tagged force, check if it exists
            auto tagged = std::find_if(std::begin(m_Forces), std::end(m_Forces), [tag](Force& single) { return tag == single.m_Tag; });

            if (tagged != std::end(m_Forces))
            {

                tagged->m_Direction.m_X = dir_x;
                tagged->m_Direction.m_Y = dir_y;
                tagged->m_Direction.m_Z = dir_z;

                tagged->m_Magnitude = magnitude;

                tagged->m_Lifetime = lifetime;
            }
            else
            {

                m_Forces.push_back({});
                        
                m_Forces.back().m_Tag = tag;
                m_Forces.push_back({});
                        
                m_Forces.back().m_Direction.m_X = dir_x;
                m_Forces.back().m_Direction.m_Y = dir_y;
                m_Forces.back().m_Direction.m_Z = dir_z;
                        
                m_Forces.back().m_Magnitude = magnitude;
                        
                m_Forces.back().m_Lifetime = lifetime;
            }
        }
    }
};


#endif