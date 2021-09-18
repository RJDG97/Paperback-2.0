#pragma once
#include <rttr/registration>


//TO BE REMOVED | USED FOR TESTING PURPOSES
struct testcomponent
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "testcomponent"
	};


	int props1;
	int props2;
	float props3;
};





namespace RR_SampleComponent
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<testcomponent>("Samplecomponent")
			.constructor<>()(rttr::policy::ctor::as_object)
			.property("prop#1", &testcomponent::props1)
			.property("prop#2", &testcomponent::props2)
			.property("prop#3", &testcomponent::props3);
	}
}


