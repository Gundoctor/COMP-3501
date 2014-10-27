#include "pch.h"
#include "Beam.h"


using namespace DirectXGame2;

using namespace DirectX;
using namespace Windows::Foundation;

Beam::Beam() : GameEntity(),
distance(0),
beamSpeed(3.0)
{
}



void Beam::Update(DX::StepTimer const& timer)
{
	pos = XMVectorAdd(pos, XMVectorScale(forward, beamSpeed));
	distance += 1;
}

void Beam::Draw(ID3D11DeviceContext2 *context, XMMATRIX *thexform)
{

}


void Beam::setForward(XMVECTOR inForward)
{
	forward = inForward;
}

void Beam::setDistance(int inDist)
{
	distance = inDist;
}

XMVECTOR Beam::getForward()
{
	return forward;
}

int Beam::getDistance()
{
	return distance;
}