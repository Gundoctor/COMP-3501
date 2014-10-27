#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#pragma once

#include "..\Helpers\DeviceResources.h"
#include "..\Helpers\StepTimer.h"
#include "EntityObjectDrawer.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace DirectXGame2{

	class GameEntity
	{
	public:
		GameEntity();

		XMVECTOR getPos();
		XMVECTOR getOri();
		void setPos(XMVECTOR);
		void setOri(XMVECTOR);

	protected:
		virtual void Update(DX::StepTimer const& timer) = 0;
		virtual void Draw(ID3D11DeviceContext2 *context, XMMATRIX *thexform) = 0;

	protected:
		XMVECTOR pos;
		XMVECTOR ori;
		EntityObjectDrawer EOD;


	};
}

#endif

