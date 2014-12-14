#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#pragma once

#include "GameEntity.h"
#include "Enemy.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace DirectXGame2{

	class EnemyBase : public GameEntity
	{
	public:
		EnemyBase();
		~EnemyBase();
		virtual void Update(DX::StepTimer const& timer);
		void setupBase(XMVECTOR);

		XMVECTOR getL();
		void setL(XMVECTOR);

		bool getIsWithinRange();
		void setIsWithinRange(bool);
		bool getLaunchOK();
		void setLaunchOK(bool);
		bool getStartLaunch();
		void setStartLaunch(bool);

		int getCurrEnemyNum();
		void setCurrEnemyNum(int);

		int getMaxEnemyNum();

		Enemy* getEnemies();

		void manageEnemies(XMVECTOR);

	private:
		static const int maxEnemyNum = 10;
		Enemy enemies[maxEnemyNum];
		XMVECTOR eBaseL;
		float turnAngle;
		bool isWithinRange;
		bool launchOK;
		bool startLaunch;
		int enemyLaunchCounter;
		int enemyLaunchCount;
		int currEnemyNum;
		int maxEnemyDistance;
	};
}


#endif