#pragma once
#include <vector>

namespace BJEngine
{
	class GameObject;
	class Particle;

	class ParticleEmitter
	{
	public:
		ParticleEmitter(std::vector<GameObject*>& vectorCopy);
		~ParticleEmitter();

		void Initailize();
		void Update();

	private:
		std::vector<GameObject*> particles;
	};
}

