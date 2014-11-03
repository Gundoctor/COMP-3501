#ifndef SPACESHIP_H
#define SPACESHIP_H

#pragma once

#include "GameEntity.h"
#include "Beam.h"

using namespace DirectX;

namespace DirectXGame2{

	class SpaceShip : public GameEntity
	{
	public:
		SpaceShip();


		virtual void Update(DX::StepTimer const& timer);
		void FireBeam();
		void CameraMove(float, float);
		void CameraSpin(float, float, float);
		XMVECTOR getForward();
		XMVECTOR getUp();
		XMVECTOR getLeft();
		void setIsHit(bool);
		void setIsHit2(bool);
		bool getIsHit();
		bool getIsHit2();

		Beam* getBeams();
		void setBeams(Beam*);

		int getNumBeam();
		void setNumBeam(float);
		float getBeamSpeed();
		int getMaxBeamDistance();

		bool getFireOkay();
		bool getBeamFired();
		void setFireOkay(bool);
		void setBeamFired(bool);

		int getStunTimer();
		void setStunTimer(int);
		int getStunTime();
		void setStunTime(int);


	private:
		void UpdateBeams(DX::StepTimer const& timer);
		void ManageBeamFire();


	private:
		// Variables for player
		XMVECTOR forward;
		XMVECTOR up;
		XMVECTOR left; // player coordinate frame, rebuilt from orientation

		float shipVel;
		bool isHit;
		bool isHit2;
		int stunTimer;
		int stunTime;

		Beam* beams;
		int numBeam;
		bool fireOkay;
		bool beamFired;
		int coolDownTimer;
		int coolDownTime;
		int maxBeamDistance = 75;
	};
}


#endif
