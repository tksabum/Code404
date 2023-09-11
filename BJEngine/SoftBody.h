#pragma once

#ifdef SOFTBODY_EXPORTS
#define SOFTBODY_API __declspec(dllexport)
#else
#define SOFTBODY_API __declspec(dllimport)
#endif

#include <vector>

#include "Component.h"
#include "Vector2D.h"


namespace BJEngine
{
	class PhysicsSystem;

	class Element;
	class Spring;

	class SoftBody : public Component
	{
	public:
		SOFTBODY_API SoftBody();
		SOFTBODY_API virtual ~SoftBody();

		SOFTBODY_API void Update();

		SOFTBODY_API void SetSoftBall(size_t vertex, size_t spring, float radius, float springconstant, float damping);
		SOFTBODY_API void SetSoftBlock(float gap, int column, int row, float springconstant, float damping);
		SOFTBODY_API void Clear();

		SOFTBODY_API void SpringUpdate();
		SOFTBODY_API void ElementPositionUpdate();
		SOFTBODY_API void FixPosition(Math::Vector2D pos);

	private:
		std::vector<Element*> elementList;
		std::vector<Spring*> springList;
		
		friend PhysicsSystem;
	};
}


