#pragma once
#ifndef _ANIMATORSYSTEM_H_
#define _ANIMATORSYSTEM_H_

#include "../Components/Animator.h"
#include "../Components/Mesh.h"

struct animator_system : paperback::system::pausable_instance
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
		Query.m_Must.AddFromComponents<animator, mesh>();
		Query.m_OneOf.AddFromComponents<parent, animator>();
		Query.m_NoneOf.AddFromComponents<prefab>();

		ForEach(Search(Query), [&](animator& Ator, mesh& Model, parent* Parent) noexcept
		{
			UpdateAnimator(Ator, Model, Parent);
		});
	}

	void UpdateAnimator(animator& Ator, mesh& Model, parent* Parent)
	{
		auto& anims = m_Resources->m_Models[Model.m_Model].GetAnimations();

		if (anims.find(Ator.m_CurrentAnimationName) != anims.end())
		{
			if (!(Ator.m_PlayOnce && Ator.m_FinishedAnimating) && !Ator.m_PauseAnimation)
			{
				auto& current_anim{ anims[Ator.m_CurrentAnimationName] };

				if (Ator.m_Reversed)
				{
					Ator.m_CurrentTime -= current_anim.GetTicksPerSecond() * DeltaTime();
				}

				else
				{
					Ator.m_CurrentTime += current_anim.GetTicksPerSecond() * DeltaTime();
				}

				// if it is the end of the animation
				if (!Ator.m_Reversed && Ator.m_CurrentTime >= current_anim.GetDuration())
				{
					Ator.m_CurrentTime = fmod(Ator.m_CurrentTime, current_anim.GetDuration());
					Ator.m_FinishedAnimating = true;
				}

				else if (Ator.m_Reversed && Ator.m_CurrentTime < 0.0f)
				{
					Ator.m_CurrentTime = current_anim.GetDuration();
					Ator.m_FinishedAnimating = true;
				}

				else
				{
					Ator.m_FinishedAnimating = false;
				}

				std::vector<std::tuple<socketed*, animator*, mesh*, parent*>> children_data;

				if (Parent)
				{
					for (const auto& ChildGlobalIndex : Parent->m_ChildrenGlobalIndexes)
					{
						auto& ChildInfo = GetEntityInfo(ChildGlobalIndex);
						auto [CSocketed, CAnimator, CMesh, CParent] = ChildInfo.m_pArchetype->FindComponents<socketed, animator, mesh, parent>(ChildInfo.m_PoolDetails);
						children_data.push_back({ CSocketed, CAnimator, CMesh, CParent });
					}
				}

				CalculateBoneTransform(&current_anim.GetRootNode(), glm::mat4{ 1.0f }, current_anim, Ator, children_data);
			}
		}
	}

	void CalculateBoneTransform(const NodeData* node, glm::mat4 parent_transform, Animation& current_anim, animator& ator, std::vector<std::tuple<socketed*, animator*, mesh*, parent*>>& children_data)
	{
		Bone* bone{ current_anim.FindBone(node->name) };
		glm::mat4 global_transformation{ parent_transform * node->transformation };
	
		if (bone)
		{
			global_transformation = parent_transform * bone->Update(ator.m_CurrentTime, ator.m_PauseAtFrame, ator.m_PauseAnimation);
		}

		auto bone_info_map = current_anim.FindBoneIDMap();

		if (bone_info_map->find(node->name) != bone_info_map->end())
		{
			int index{ (*bone_info_map)[node->name].id };
			glm::mat4 offset{ (*bone_info_map)[node->name].offset };

			glm::mat4 transform{};

			if (offset == glm::mat4{ 0.0f })
			{
				transform = global_transformation;
			}

			else
			{
				transform = global_transformation * offset;
			}

			ator.m_FinalBoneMatrices[index] = transform;

			for (auto& child_data : children_data)
			{
				if (std::get<0>(child_data) && node->name == std::get<0>(child_data)->m_ParentSocket)
				{
					glm::mat4 socket_offset{ glm::mat4{1.0f} };
					paperback::Vector3f	 socket_pos_offset { std::get<0>(child_data)->m_SocketPosOffset };
					paperback::Vector3f	 socket_rot_offset { std::get<0>(child_data)->m_SocketRotOffset };
					paperback::Vector3f	 socket_scale_offset { std::get<0>(child_data)->m_SocketScaleOffset };

					socket_offset = glm::translate(socket_offset, glm::vec3{ socket_pos_offset.x, socket_pos_offset.y, socket_pos_offset.z });
					socket_offset = glm::rotate(socket_offset, glm::radians(socket_rot_offset.x), glm::vec3{ 1.f, 0.f, 0.f });
					socket_offset = glm::rotate(socket_offset, glm::radians(socket_rot_offset.y), glm::vec3{ 0.f, 1.f, 0.f });
					socket_offset = glm::rotate(socket_offset, glm::radians(socket_rot_offset.z), glm::vec3{ 0.f, 0.f, 1.f });
					socket_offset = glm::scale(socket_offset, glm::vec3{ socket_scale_offset.x, socket_scale_offset.y, socket_scale_offset.z });

					std::get<0>(child_data)->m_BoneTransform = transform * socket_offset;
				}

				if (std::get<0>(child_data) && std::get<1>(child_data) && std::get<0>(child_data)->m_SyncAnimationWithParent && !std::get<1>(child_data)->m_FinishedAnimating)
				{
					std::get<1>(child_data)->m_CurrentTime = ator.m_CurrentTime;
					UpdateAnimator(*std::get<1>(child_data), *std::get<2>(child_data), std::get<3>(child_data));
				}
			}
		}

		for (int i = 0; i < node->children.size(); ++i)
		{
			CalculateBoneTransform(&node->children[i], global_transformation, current_anim, ator, children_data);
		}
	}
};

#endif