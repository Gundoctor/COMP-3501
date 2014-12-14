#include "pch.h"
#include "EnemyBase.h"


using namespace DirectXGame2;

using namespace DirectX;
using namespace Windows::Foundation;

EnemyBase::EnemyBase() : GameEntity(), isWithinRange(false), currEnemyNum(0), enemyLaunchCount(80), enemyLaunchCounter(1000), launchOK(false), startLaunch(false), maxEnemyDistance(1000)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update(DX::StepTimer const& timer)
{
	if (isWithinRange)
	{
		for (int i = 0; i < currEnemyNum; i++)
		{
			enemies[i].Update(timer);
		}
	}
}


void EnemyBase::setupBase(XMVECTOR ePos)
{
	pos = ePos;
	ori = XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(0.01*3.14*(rand() % 1000) / 1000.0f, 0.01*3.14*(rand() % 1000) / 1000.0f, 0.01*3.14*(rand() % 1000) / 1000.0f, 1.0f));

	for (int i = 0; i < maxEnemyNum; i++)
	{
		enemies[i].setOri(ori);
		enemies[i].setPos(pos);
		enemies[i].setL(XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(0.01*3.14*(rand() % 1000) / 1000.0f, 0.01*3.14*(rand() % 1000) / 1000.0f, 0.01*3.14*(rand() % 1000) / 1000.0f, 1.0f)));
	}
}


void EnemyBase::manageEnemies(XMVECTOR shipPos)
{
	if (isWithinRange)
	{
		if (currEnemyNum <= maxEnemyNum)
		{
			if (launchOK)
			{
				enemyLaunchCounter = 0;

				enemies[currEnemyNum].setSpeed(0.5);
				++currEnemyNum;

				launchOK = false;
			}
			else if (enemyLaunchCounter < enemyLaunchCount) {
				enemyLaunchCounter++;
			}
			else if (enemyLaunchCounter == enemyLaunchCount && currEnemyNum != maxEnemyNum)
			{
				enemyLaunchCounter++;
				launchOK = true;
			}

			for (int i = 0; i < currEnemyNum; i++)
			{
				if (!enemies[i].getShouldBeDel())
				{
					//lazy way for now
					XMVECTOR shipDir = XMVectorSubtract(shipPos, enemies[i].getPos());
					shipDir = XMVector4Normalize(shipDir);

					enemies[i].setForward(shipDir);
					//lazy way for now

					if (enemies[i].getDistance() > maxEnemyDistance)
					{
						enemies[i].setShouldBeDel(true);
					}
				}
			}
		}
	}
}


Enemy* EnemyBase::getEnemies()
{
	return enemies;
}


bool EnemyBase::getIsWithinRange()
{
	return isWithinRange;
}


void EnemyBase::setIsWithinRange(bool iwr)
{
	isWithinRange = iwr;
}


bool EnemyBase::getLaunchOK()
{
	return launchOK;
}

void EnemyBase::setLaunchOK(bool lok)
{
	launchOK = lok;
}

bool EnemyBase::getStartLaunch()
{
	return startLaunch;
}

void EnemyBase::setStartLaunch(bool sl)
{
	startLaunch = sl;
}

int EnemyBase::getCurrEnemyNum()
{
	return currEnemyNum;
}

void EnemyBase::setCurrEnemyNum(int cen)
{
	currEnemyNum = cen;
}

int EnemyBase::getMaxEnemyNum()
{
	return maxEnemyNum;
}