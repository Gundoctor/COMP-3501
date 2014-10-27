#ifndef BEAM_H
#define BEAM_H


#pragma once


#include "GameEntity.h"

using namespace DirectX;

namespace DirectXGame2{

	class Beam : public GameEntity
	{
	public:
		Beam();
		virtual void Update(DX::StepTimer const& timer);
		virtual void Draw(ID3D11DeviceContext2 *context, XMMATRIX *thexform);

		void setForward(XMVECTOR);
		void setDistance(int);
		XMVECTOR getForward();
		int getDistance();

	private:
		XMVECTOR forward;
		int distance;
		float beamSpeed;

	};
}
#endif