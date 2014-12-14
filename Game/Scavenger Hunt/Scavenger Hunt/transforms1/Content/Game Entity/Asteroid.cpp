#include "pch.h"
#include "Asteroid.h"

using namespace DirectXGame2;
using namespace DirectX;
using namespace Windows::Foundation;

Asteroid::Asteroid() : GameEntity(),
shouldBeDel(false)
{
}

void Asteroid::Update(DX::StepTimer const& timer)
{
	ori = XMQuaternionMultiply(ori, L);
}


void Asteroid::setL(XMVECTOR inL)
{
	L = inL;
}

void Asteroid::setVel(XMVECTOR inVel)
{
	vel = inVel;
}

void Asteroid::setShouldBeDel(bool inSBD)
{
	shouldBeDel = inSBD;
}


XMVECTOR Asteroid::getL()
{
	return L;
}

XMVECTOR Asteroid::getVel()
{
	return vel;
}

bool Asteroid::getShouldBeDel()
{
	return shouldBeDel;
}