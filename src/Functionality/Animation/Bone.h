#pragma once
#include "glew/inc/glew.h"
#include "glm/inc/gtx/transform.hpp"
#include "glm/inc/gtx/quaternion.hpp"
#include <vector>
#include <string>

struct KeyPosition
{
    glm::vec3 m_Position;
    float m_TimeStamp;
};

struct KeyRotation
{
    glm::quat m_Orientation;
    float m_TimeStamp;
};

struct KeyScale
{
    glm::vec3 m_Scale;
    float m_TimeStamp;
};

class Bone
{
public:

    Bone() = default;

    Bone(std::vector<KeyPosition> positions, std::vector<KeyRotation> rotations, std::vector<KeyScale> scales,
        glm::mat4 local_transform, std::string name, int id)
        : m_Positions{ positions },
        m_Rotations{ rotations },
        m_Scales{ scales },
        m_LocalTransform{ local_transform },
        m_Name{ name },
        m_ID{ id }
    {

    }

private:

    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;

    glm::mat4 m_LocalTransform;
    std::string m_Name;
    int m_ID;

    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    glm::mat4 InterpolatePosition(float animationTime, int pause_at_frame, bool& paused)
    {
        if (m_Positions.size() == 1)
        {
            return glm::translate( glm::mat4(1.0f), m_Positions[0].m_Position );
        }

        int p0Index = GetPositionIndex(animationTime, pause_at_frame, paused);
        int p1Index = p0Index + 1;

        float scaleFactor = GetScaleFactor(m_Positions[p0Index].m_TimeStamp,
                                           m_Positions[p1Index].m_TimeStamp, animationTime);

        glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].m_Position,
                                           m_Positions[p1Index].m_Position, scaleFactor);

        return glm::translate(glm::mat4(1.0f), finalPosition);
    }

    glm::mat4 InterpolateRotation(float animationTime, int pause_at_frame, bool& paused)
    {
        if (m_Rotations.size() == 1)
        {
            auto rotation = glm::normalize(m_Rotations[0].m_Orientation);
            return glm::toMat4(rotation);
        }

        int p0Index = GetRotationIndex(animationTime, pause_at_frame, paused);
        int p1Index = p0Index + 1;

        float scaleFactor = GetScaleFactor(m_Rotations[p0Index].m_TimeStamp,
                                           m_Rotations[p1Index].m_TimeStamp, animationTime);

        glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].m_Orientation,
                                             m_Rotations[p1Index].m_Orientation, scaleFactor);

        return glm::toMat4(glm::normalize(finalRotation));
    }

    glm::mat4 InterpolateScaling(float animationTime, int pause_at_frame, bool& paused)
    {
        if (m_Scales.size() == 1)
        {
            return glm::scale(glm::mat4(1.0f), m_Scales[0].m_Scale);
        }

        int p0Index = GetScaleIndex(animationTime, pause_at_frame, paused);
        int p1Index = p0Index + 1;

        float scaleFactor = GetScaleFactor(m_Scales[p0Index].m_TimeStamp,
                                           m_Scales[p1Index].m_TimeStamp, animationTime);

        glm::vec3 finalScale = glm::mix(m_Scales[p0Index].m_Scale, m_Scales[p1Index].m_Scale,
                                        scaleFactor);

        return glm::scale(glm::mat4(1.0f), finalScale);
    }

public:

    Bone(std::vector<KeyPosition> positions, std::vector<KeyRotation> rotations,
         std::vector<KeyScale> scales, std::string name, int id)
        : m_Positions { positions }, m_Rotations { rotations }, m_Scales {scales},
            m_LocalTransform{ 1.0f }, m_Name{ name }, m_ID{ id }
    {

    }

    glm::mat4 Update(float current_time, int pause_at_frame, bool& paused)
    {
        glm::mat4 translation{ InterpolatePosition(current_time, pause_at_frame, paused) };
        glm::mat4 rotation{ InterpolateRotation(current_time, pause_at_frame, paused) };
        glm::mat4 scale{ InterpolateScaling(current_time, pause_at_frame, paused) };
        return translation * rotation * scale;
    }

    int GetPositionIndex(float animationTime, int pause_at_frame, bool& paused)
    {
        for (int index = 0; index < m_Positions.size() - 1; ++index)
        {
            if (animationTime < m_Positions[index + 1].m_TimeStamp)
            {
                if (pause_at_frame != -1 && index > pause_at_frame)
                {
                    paused = true;
                    return pause_at_frame;
                }

                else
                {
                    return index;
                }
            }
        }

        return 0;
    }

    int GetRotationIndex(float animationTime, int pause_at_frame, bool& paused)
    {
        for (int index = 0; index < m_Rotations.size() - 1; ++index)
        {
            if (animationTime < m_Rotations[index + 1].m_TimeStamp)
            {
                if (pause_at_frame != -1 && index > pause_at_frame)
                {
                    paused = true;
                    return pause_at_frame;
                }

                else
                {
                    return index;
                }
            }
        }

        return 0;
    }

    int GetScaleIndex(float animationTime, int pause_at_frame, bool& paused)
    {
        for (int index = 0; index < m_Scales.size() - 1; ++index)
        {
            if (animationTime < m_Scales[index + 1].m_TimeStamp)
            {
                if (pause_at_frame != -1 && index > pause_at_frame)
                {
                    paused = true;
                    return pause_at_frame;
                }

                else
                {
                    return index;
                }
            }
        }

        return 0;
    }

    std::vector<KeyPosition>& GetPositions() { return m_Positions; }
    std::vector<KeyRotation>& GetRotations() { return m_Rotations; }
    std::vector<KeyScale>& GetScales() { return m_Scales; }
    glm::mat4 GetLocalTransform() { return m_LocalTransform; }
    std::string& GetBoneName() { return m_Name; }
    int& GetBoneID() { return m_ID; }
};