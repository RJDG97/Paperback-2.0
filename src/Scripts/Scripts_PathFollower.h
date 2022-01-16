#pragma once

#include "Mono.h"

namespace MONO_PATHFOLLOWER
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		auto& m_path_follower = m_obj.m_pArchetype->GetComponent<path_follower>(m_obj.m_PoolDetails);
		return &m_path_follower;
	}

	MONO_EXPORT int GetPathID(void* address)
	{
		if (address)
			return reinterpret_cast<path_follower*>(address)->m_PathID;

		return {};
	}

	MONO_EXPORT float GetDistance(void* address)
	{
		if (address)
			return reinterpret_cast<path_follower*>(address)->m_Distance;

		return {};
	}

	MONO_EXPORT float GetTravelSpeed(void* address)
	{
		if (address)
			return reinterpret_cast<path_follower*>(address)->m_TravelSpeed;

		return {};
	}

	MONO_EXPORT bool GetBackAndForth(void* address)
	{
		if (address)
			return reinterpret_cast<path_follower*>(address)->m_BackAndForth;

		return {};
	}

	MONO_EXPORT bool GetReversed(void* address)
	{
		if (address)
			return reinterpret_cast<path_follower*>(address)->m_Reversed;

		return {};
	}

	MONO_EXPORT bool GetPauseTravel(void* address)
	{
		if (address)
			return reinterpret_cast<path_follower*>(address)->m_PauseTravel;

		return {};
	}

	MONO_EXPORT void SetPathID(void* address, int path_id)
	{
		if (address)
			reinterpret_cast<path_follower*>(address)->m_PathID = path_id;
	}

	MONO_EXPORT void SetDistance(void* address, float distance)
	{
		if (address)
			reinterpret_cast<path_follower*>(address)->m_Distance = distance;
	}

	MONO_EXPORT void SetTravelSpeed(void* address, float travel_speed)
	{
		if (address)
			reinterpret_cast<path_follower*>(address)->m_TravelSpeed = travel_speed;
	}

	MONO_EXPORT void SetBackAndForth(void* address, bool back_and_forth)
	{
		if (address)
			reinterpret_cast<path_follower*>(address)->m_BackAndForth = back_and_forth;
	}

	MONO_EXPORT void SetReversed(void* address, bool traversed)
	{
		if (address)
			reinterpret_cast<path_follower*>(address)->m_Reversed = traversed;
	}

	MONO_EXPORT void SetPauseTravel(void* address, bool stopped)
	{
		if (address)
			reinterpret_cast<path_follower*>(address)->m_PauseTravel = stopped;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.PathFollower::getaddress(uint)", &MONO_PATHFOLLOWER::GetAddress);
		mono_add_internal_call("CSScript.PathFollower::getpathid(void*)", &MONO_PATHFOLLOWER::GetPathID);
		mono_add_internal_call("CSScript.PathFollower::getdistance(void*)", &MONO_PATHFOLLOWER::GetDistance);
		mono_add_internal_call("CSScript.PathFollower::gettravelspeed(void*)", &MONO_PATHFOLLOWER::GetTravelSpeed);
		mono_add_internal_call("CSScript.PathFollower::getbackandforth(void*)", &MONO_PATHFOLLOWER::GetBackAndForth);
		mono_add_internal_call("CSScript.PathFollower::getreversed(void*)", &MONO_PATHFOLLOWER::GetReversed);
		mono_add_internal_call("CSScript.PathFollower::getpausetravel(void*)", &MONO_PATHFOLLOWER::GetPauseTravel);
		mono_add_internal_call("CSScript.PathFollower::setpathid(void*,int)", &MONO_PATHFOLLOWER::SetPathID);
		mono_add_internal_call("CSScript.PathFollower::setdistance(void*,single)", &MONO_PATHFOLLOWER::SetDistance);
		mono_add_internal_call("CSScript.PathFollower::settravelspeed(void*,single)", &MONO_PATHFOLLOWER::SetTravelSpeed);
		mono_add_internal_call("CSScript.PathFollower::setbackandforth(void*,bool)", &MONO_PATHFOLLOWER::SetBackAndForth);
		mono_add_internal_call("CSScript.PathFollower::setreversed(void*,bool)", &MONO_PATHFOLLOWER::SetReversed);
		mono_add_internal_call("CSScript.PathFollower::setpausetravel(void*,bool)", &MONO_PATHFOLLOWER::SetPauseTravel);
	}
}