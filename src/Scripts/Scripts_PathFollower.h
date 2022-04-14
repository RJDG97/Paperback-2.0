/**********************************************************************************
*\file         Scripts_PathFollower.h
*\brief        Scripts_PathFollower.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_PATHFOLLOWER
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_path_follower = m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_path_follower)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no PathFollower component." << std::endl;
		}
#endif

		return m_path_follower;
	}

	MONO_EXPORT int GetPathID(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_PathID;

		return {};
	}

	MONO_EXPORT float GetDistance(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_Distance;

		return {};
	}

	MONO_EXPORT float GetTravelSpeed(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_TravelSpeed;

		return {};
	}

	MONO_EXPORT bool GetBackAndForth(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_BackAndForth;

		return {};
	}

	MONO_EXPORT bool GetReversed(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_Reversed;

		return {};
	}

	MONO_EXPORT bool GetPauseTravel(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			return m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_PauseTravel;

		return {};
	}

	MONO_EXPORT void SetPathID(uint32_t ID, int path_id)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_PathID = path_id;
	}

	MONO_EXPORT void SetDistance(uint32_t ID, float distance)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_Distance = distance;
	}

	MONO_EXPORT void SetTravelSpeed(uint32_t ID, float travel_speed)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_TravelSpeed = travel_speed;
	}

	MONO_EXPORT void SetBackAndForth(uint32_t ID, bool back_and_forth)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_BackAndForth = back_and_forth;
	}

	MONO_EXPORT void SetReversed(uint32_t ID, bool traversed)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_Reversed = traversed;
	}

	MONO_EXPORT void SetPauseTravel(uint32_t ID, bool stopped)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
			m_obj.m_pArchetype->FindComponent<path_follower>(m_obj.m_PoolDetails)->m_PauseTravel = stopped;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.PathFollower::getaddress(uint)", &MONO_PATHFOLLOWER::GetAddress);
		mono_add_internal_call("CSScript.PathFollower::getpathid(uint)", &MONO_PATHFOLLOWER::GetPathID);
		mono_add_internal_call("CSScript.PathFollower::getdistance(uint)", &MONO_PATHFOLLOWER::GetDistance);
		mono_add_internal_call("CSScript.PathFollower::gettravelspeed(uint)", &MONO_PATHFOLLOWER::GetTravelSpeed);
		mono_add_internal_call("CSScript.PathFollower::getbackandforth(uint)", &MONO_PATHFOLLOWER::GetBackAndForth);
		mono_add_internal_call("CSScript.PathFollower::getreversed(uint)", &MONO_PATHFOLLOWER::GetReversed);
		mono_add_internal_call("CSScript.PathFollower::getpausetravel(uint)", &MONO_PATHFOLLOWER::GetPauseTravel);
		mono_add_internal_call("CSScript.PathFollower::setpathid(uint,int)", &MONO_PATHFOLLOWER::SetPathID);
		mono_add_internal_call("CSScript.PathFollower::setdistance(uint,single)", &MONO_PATHFOLLOWER::SetDistance);
		mono_add_internal_call("CSScript.PathFollower::settravelspeed(uint,single)", &MONO_PATHFOLLOWER::SetTravelSpeed);
		mono_add_internal_call("CSScript.PathFollower::setbackandforth(uint,bool)", &MONO_PATHFOLLOWER::SetBackAndForth);
		mono_add_internal_call("CSScript.PathFollower::setreversed(uint,bool)", &MONO_PATHFOLLOWER::SetReversed);
		mono_add_internal_call("CSScript.PathFollower::setpausetravel(uint,bool)", &MONO_PATHFOLLOWER::SetPauseTravel);
	}
}