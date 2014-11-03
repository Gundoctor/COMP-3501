#include "pch.h"
#include "PickUp.h"

using namespace DirectXGame2;
using namespace DirectX;
using namespace Windows::Foundation;

PickUp::PickUp() : GameEntity()
{
}

void PickUp::Update(DX::StepTimer const& timer)
{
	ori = XMQuaternionMultiply(ori, L);
}


void PickUp::setL(XMVECTOR inL)
{
	L = inL;
}

void PickUp::setVel(XMVECTOR inVel)
{
	vel = inVel;
}

void PickUp::setShouldBeDel(bool inSBD)
{
	shouldBeDel = inSBD;
}


XMVECTOR PickUp::getL()
{
	return L;
}

XMVECTOR PickUp::getVel()
{
	return vel;
}

bool PickUp::getShouldBeDel()
{
	return shouldBeDel;
}