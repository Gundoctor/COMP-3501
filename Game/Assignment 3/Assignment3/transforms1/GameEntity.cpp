#include "pch.h"
#include "GameEntity.h"

using namespace DirectXGame2;

GameEntity::GameEntity()
{


}


XMVECTOR GameEntity::getPos()
{
	return pos;
}

XMVECTOR GameEntity::getOri()
{
	return ori;
}

void GameEntity::setPos(XMVECTOR inPos)
{
	pos = inPos;
}

void GameEntity::setOri(XMVECTOR inOri)
{
	ori = inOri;
}