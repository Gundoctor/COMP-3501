#include "pch.h"
#include "Enemy.h"


using namespace DirectXGame2;

using namespace DirectX;
using namespace Windows::Foundation;

Enemy::Enemy() : GameEntity(),
distance(0),
speed(0.0),
shouldBeDel(false)
{
}



void Enemy::Update(DX::StepTimer const& timer)
{
	pos = XMVectorAdd(pos, XMVectorScale(forward, speed));
	distance += 1;

	ori = XMQuaternionMultiply(XMQuaternionMultiply(ori, L), L);
	ori = XMQuaternionMultiply(XMQuaternionMultiply(ori, L), L);
	ori = XMQuaternionMultiply(XMQuaternionMultiply(ori, L), L);
	ori = XMQuaternionMultiply(XMQuaternionMultiply(ori, L), L);
	ori = XMQuaternionMultiply(XMQuaternionMultiply(ori, L), L);
	ori = XMQuaternionMultiply(XMQuaternionMultiply(ori, L), L);
}


void Enemy::setForward(XMVECTOR inForward)
{
	forward = inForward;
}

void Enemy::setDistance(int inDist)
{
	distance = inDist;
}

XMVECTOR Enemy::getForward()
{
	return forward;
}

int Enemy::getDistance()
{
	return distance;
}

void Enemy::setL(XMVECTOR inL)
{
	L = inL;
}

XMVECTOR Enemy::getL()
{
	return L;
}

void Enemy::setShouldBeDel(bool inSBD)
{
	shouldBeDel = inSBD;
}

bool Enemy::getShouldBeDel()
{
	return shouldBeDel;
}

void Enemy::setSpeed(float inSpeed)
{
	speed = inSpeed;
}

float Enemy::getSpeed()
{
	return speed;
}