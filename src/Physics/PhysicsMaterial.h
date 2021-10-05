// ----- Start of header guard
#ifndef PHYSICSMATERIAL_H
#define PHYSICSMATERIAL_H

class PhysicMaterial
{
	// variables
	float static_friction;
	float dynamic_friction;
	float bounciness;

public:
	//enum of the different physic materials
	enum Materials {
		NONE,
		RUBBER,
		ICE,
		WOOD,
		METAL,
		MUD,
		TESTING
	};

	// material variable
	Materials material;

	//Default constructor
	PhysicMaterial()
		:static_friction{ 0.0f }, dynamic_friction{ 0.0f }, bounciness{ 0.0f }
	{
		SetMaterialHelper(RUBBER);
	}

	//Constructor
	PhysicMaterial(Materials material) { SetMaterialHelper(material); }

	//Get the physics material
	Materials GetMaterial() const { return material; }

	//Set the physics material
	void SetMaterial(Materials material) { SetMaterialHelper(material); }

	float GetStaticFriction() { return static_friction; }

	float GetDynamicFriction() { return dynamic_friction; }

	float GetBounciness() { return bounciness; }

private:
	//Set physics material
	void SetMaterialHelper(Materials material)
	{
		switch (material) {
		case NONE:
			static_friction = 0.0f;
			dynamic_friction = 0.0f;
			bounciness = 0.0f;
			material = NONE;
			break;

		case RUBBER:
			static_friction = 0.9f;
			dynamic_friction = 0.8f;
			bounciness = 0.8f;
			material = RUBBER;
			break;

		case ICE:
			static_friction = 0.0f;
			dynamic_friction = 0.0f;
			bounciness = 0.05f;
			material = ICE;
			break;

		case WOOD:
			static_friction = 0.475f;
			dynamic_friction = 0.475f;
			bounciness = 0.0f;
			material = WOOD;
			break;

		case METAL:
			static_friction = 0.2f;
			dynamic_friction = 0.15f;
			bounciness = 0.0f;
			material = METAL;
			break;

		case MUD:
			static_friction = 0.9f;
			dynamic_friction = 1.0f;
			bounciness = 0.0f;
			material = MUD;
			break;

		case TESTING:
			static_friction = 1.0f;
			dynamic_friction = 1.0f;
			bounciness = 0.0f;
			material = TESTING;
			break;

		default:
			static_friction = 0.0f;
			dynamic_friction = 0.0f;
			bounciness = 0.0f;
			material = NONE;
			break;
		}
	}
	
};

#endif