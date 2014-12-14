#ifndef ENEMY_H
#define ENEMY_H

#pragma once

#include "GameEntity.h"
#include "pch.h"

using namespace DirectX;

namespace DirectXGame2{

	class Enemy : public GameEntity
	{
	public:
		Enemy();

		virtual void Update(DX::StepTimer const& timer);

		void setForward(XMVECTOR);
		void setDistance(int);
		XMVECTOR getForward();
		int getDistance();
		void setL(XMVECTOR);
		XMVECTOR getL();
		void setShouldBeDel(bool);
		bool getShouldBeDel();

		void setSpeed(float);
		float getSpeed();

	private:
		XMVECTOR forward;
		XMVECTOR L; // angular momentum (use as velocity)
		int distance;
		float speed;
		bool shouldBeDel;
	};
}


#endif
