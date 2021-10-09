#pragma once
#ifndef _ANIMATORSYSTEM_H_
#define _ANIMATORSYSTEM_H_

#include "../Components/Animator.h"
#include "../Components/Mesh.h"

struct animator_system : paperback::system::instance
{
	RenderResourceManager* m_Resources;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "animator_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		m_Resources = &RenderResourceManager::GetInstanced();
	}

	PPB_FORCEINLINE
	void PreUpdate(void) noexcept
	{
	}

	PPB_FORCEINLINE
	void Update(void) noexcept
	{
		//just testing stuff
		tools::query Query;
		Query.m_Must.AddFromComponents<animator, mesh>();


		ForEach(Search(Query), [&](animator& ator, mesh& model) noexcept
		{
			auto& anims = m_Resources->m_Models[model.m_Model].GetAnimations();

			if (anims.find(ator.m_CurrentAnimationName) != anims.end())
			{
				auto& current_anim{ anims[ator.m_CurrentAnimationName] };
				
				ator.m_CurrentTime += current_anim.GetTicksPerSecond() * DeltaTime();
				ator.m_CurrentTime = fmod(ator.m_CurrentTime, current_anim.GetDuration());
				CalculateBoneTransform(&current_anim.GetRootNode(), glm::mat4{ 1.0f }, current_anim, ator);
			}

		});
	}

	void CalculateBoneTransform(const NodeData* node, glm::mat4 parent_transform, Animation& current_anim, animator& ator)
	{
		Bone* bone{ current_anim.FindBone(node->name) };
		glm::mat4 global_transformation{ parent_transform * node->transformation };

		if (bone)
		{
			bone->Update(ator.m_CurrentTime);
			global_transformation = parent_transform * bone->GetLocalTransform();
		}

		auto bone_info_map{ current_anim.GetBoneIDMap() };

		if (bone_info_map.find(node->name) != bone_info_map.end())
		{
			int index{ bone_info_map[node->name].id };
			glm::mat4 offset{ bone_info_map[node->name].offset };
			ator.m_FinalBoneMatrices[index] = global_transformation * offset;
		}

		for (int i = 0; i < node->children.size(); ++i)
		{
			CalculateBoneTransform(&node->children[i], global_transformation, current_anim, ator);
		}
	}
};

#endif