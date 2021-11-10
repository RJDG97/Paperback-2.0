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

				if (Ator.m_PlayOnce && Ator.m_CurrentTime >= current_anim.GetDuration())
				{
					Ator.m_FinishedAnimating = true;
				}

				else
				{
					Ator.m_CurrentTime = fmod(Ator.m_CurrentTime, current_anim.GetDuration());

					std::vector<std::pair<socketed*, scale*>> children_data;

					if (Parent)
					{
						for (const auto& ChildGlobalIndex : Parent->m_ChildrenGlobalIndexes)
						{
							auto& ChildInfo = GetEntityInfo(ChildGlobalIndex);
							auto [CSocketed, CScale] = ChildInfo.m_pArchetype->FindComponents<socketed, scale>(ChildInfo.m_PoolDetails);
							children_data.push_back({CSocketed, CScale});
						}
					}

					CalculateBoneTransform(&current_anim.GetRootNode(), glm::mat4{ 1.0f }, current_anim, Ator, children_data, &Scale);
				}
			}

		});
	}

	void CalculateBoneTransform(const NodeData* node, glm::mat4 parent_transform, Animation& current_anim, animator& ator, std::vector<std::pair<socketed*, scale*>>& children_data, scale* scale)
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

			for (auto& child_data : children_data)
			{
				if (child_data.first && child_data.second && node->name == child_data.first->m_ParentSocket)
				{
					glm::mat4 socket_offset{ glm::mat4{1.0f} };
					paperback::Vector3f	 socket_pos_offset { child_data.first->m_SocketPosOffset };
					paperback::Vector3f	 socket_rot_offset { child_data.first->m_SocketRotOffset };
					paperback::Vector3f	 socket_scale_offset { child_data.first->m_SocketScaleOffset };

					socket_offset = glm::translate(socket_offset, glm::vec3{ socket_pos_offset.x, socket_pos_offset.y, socket_pos_offset.z });
					socket_offset = glm::rotate(socket_offset, glm::radians(socket_rot_offset.x), glm::vec3{ 1.f, 0.f, 0.f });
					socket_offset = glm::rotate(socket_offset, glm::radians(socket_rot_offset.y), glm::vec3{ 0.f, 1.f, 0.f });
					socket_offset = glm::rotate(socket_offset, glm::radians(socket_rot_offset.z), glm::vec3{ 0.f, 0.f, 1.f });
					socket_offset = glm::scale(socket_offset, glm::vec3{ socket_scale_offset.x, socket_scale_offset.y, socket_scale_offset.z });

					child_data.first->m_BoneTransform = global_transformation * offset * socket_offset;
				}
			}
		}

		for (int i = 0; i < node->children.size(); ++i)
		{
			CalculateBoneTransform(&node->children[i], global_transformation, current_anim, ator, children_data, scale);
		}
	}
};

#endif