#include "ParticleEmitter.h"

BJEngine::ParticleEmitter::ParticleEmitter(std::vector<GameObject*>& vectorCopy)
	: particles(vectorCopy)
{

}

BJEngine::ParticleEmitter::~ParticleEmitter()
{
	for (auto& e : particles)
	{
		delete e;
		e = nullptr;
	}
	particles.clear();
}

void BJEngine::ParticleEmitter::Initailize()
{

}

void BJEngine::ParticleEmitter::Update()
{

}
