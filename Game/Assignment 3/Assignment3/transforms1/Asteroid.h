#ifndef ASTEROID_H
#define ASTEROID_H

#pragma once

#include "GameEntity.h"


using namespace DirectX;

namespace DirectXGame2{
	class Asteroid : public GameEntity
	{
	public:
		Asteroid();
		virtual void Update(DX::StepTimer const& timer);
		virtual void Draw(ID3D11DeviceContext2 *context, XMMATRIX *thexform);

		XMVECTOR getL();
		XMVECTOR getVel();
		bool getShouldBeDel();
		void setL(XMVECTOR);
		void setVel(XMVECTOR);
		void setShouldBeDel(bool);

	private:
		XMVECTOR L; // angular momentum (use as velocity)
		XMVECTOR vel; // linear velocity
		bool shouldBeDel;
	};
}

#endif