#pragma once
#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_
#include "Animation.h"

class Animator
{
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;

public:

    Animator() {}

    Animator(Animation* Animation)
        : m_FinalBoneMatrices{},
          m_CurrentTime {0.0f},
          m_CurrentAnimation { Animation }
    {
        for (int i = 0; i < 100; ++i)
        {
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
        }
    }

    void SetAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }

    void UpdateAnimation(float dt)
    {
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4{1.0f});
        }
    }

    void CalculateBoneTransform(const NodeData* node, glm::mat4 parent_transform)
    {
        Bone* bone{m_CurrentAnimation->FindBone(node->name)};
        glm::mat4 global_transformation{ parent_transform * node->transformation };

        if (bone)
        {
            bone->Update(m_CurrentTime);
            global_transformation = parent_transform * bone->GetLocalTransform();
        }

        auto bone_info_map{ m_CurrentAnimation->GetBoneIDMap() };

        if (bone_info_map.find(node->name) != bone_info_map.end())
        {
            int index{ bone_info_map[node->name].id };
            glm::mat4 offset{ bone_info_map[node->name].offset };
            m_FinalBoneMatrices[index] = global_transformation * offset;
        }

        for (int i = 0; i < node->children.size(); ++i)
        {
            CalculateBoneTransform(&node->children[i], global_transformation);
        }
    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }
};

#endif