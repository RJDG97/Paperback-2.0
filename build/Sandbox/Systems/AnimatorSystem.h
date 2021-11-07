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
		tools::query Query;
		Query.m_Must.AddFromComponents<animator, mesh, scale>();
		Query.m_OneOf.AddFromComponents<parent, animator>();

		ForEach(Search(Query), [&](animator& Ator, mesh& Model, scale& Scale, parent* Parent) noexcept
		{
			auto& anims = m_Resources->m_Models[Model.m_Model].GetAnimations();

			if (anims.find(Ator.m_CurrentAnimationName) != anims.end())
			{
				auto& current_anim{ anims[Ator.m_CurrentAnimationName] };
				
				Ator.m_CurrentTime += current_anim.GetTicksPerSecond() * DeltaTime();
				Ator.m_CurrentTime = fmod(Ator.m_CurrentTime, current_anim.GetDuration());

				std::vector<socketed*> socketeds;

				if (Parent)
				{
					for (const auto& ChildGlobalIndex : Parent->m_ChildrenGlobalIndexes)
					{
						auto& ChildInfo = GetEntityInfo(ChildGlobalIndex);
						auto [CSocketed] = ChildInfo.m_pArchetype->FindComponents<socketed>(ChildInfo.m_PoolDetails);
						socketeds.push_back({CSocketed});
					}
				}

				CalculateBoneTransform(&current_anim.GetRootNode(), glm::mat4{ 1.0f }, current_anim, Ator, socketeds);
			}

		});
	}

	void CalculateBoneTransform(const NodeData* node, glm::mat4 parent_transform, Animation& current_anim, animator& ator, std::vector<socketed*>& socketeds)
	{
		Bone* bone{ current_anim.FindBone(node->name) };
		glm::mat4 global_transformation{ parent_transform * node->transformation };

		if (bone)
		{
			global_transformation = parent_transform * bone->Update(ator.m_CurrentTime);
		}

		auto bone_info_map{ current_anim.GetBoneIDMap() };

		if (bone_info_map.find(node->name) != bone_info_map.end())
		{
			int index{ bone_info_map[node->name].id };
			glm::mat4 offset{ bone_info_map[node->name].offset };
			ator.m_FinalBoneMatrices[index] = global_transformation * offset;

			for (auto& socketed : socketeds)
			{
				if (socketed && node->name == socketed->parent_socket)
				{
					socketed->bone_transform = global_transformation;
				}
			}
		}

		for (int i = 0; i < node->children.size(); ++i)
		{
			CalculateBoneTransform(&node->children[i], global_transformation, current_anim, ator, socketeds);
		}
	}
};

#endif