#include "PaperbackPCH.h"
#include "IComponent.h"

//#include <rttr/type.h>
//#include <rttr/registration.h>

namespace Paperback
{
	void IComponent::Init() {}
}

//RTTR_REGISTRATION
//{
//	using namespace rttr;
//	using namespace Engine::Component;
//
//	registration::class_<IComponent>("Engine::Component::IComponent")
//		.constructor<>()(rttr::policy::ctor::as_object)
//		.method("Init", &IComponent::Init);
//}