#include "PaperbackPCH.h"
#include "ISystem.h"

namespace Paperback
{
	void ISystem::RemoveEntity(EntityID id)
	{
		// Assert if does not exist

		auto deleted = list.erase(id);

		//// If invalid ID
		//if (deleted < 1)
		//	// some assert
	}

	void ISystem::RegisterEntity(EntityID id)
	{
		auto it = list.insert(id);

		//// ID already exists
		//if (it.second == false)
		//	// Some assert
	}

	void ISystem::SetSignature(Signature sig)
	{
		requirements = sig;
	}
}