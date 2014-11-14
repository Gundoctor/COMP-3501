#include "pch.h"
#include "SpaceShip.h"


using namespace DirectXGame2;

using namespace DirectX;
using namespace Windows::Foundation;

SpaceShip::SpaceShip() : GameEntity(), shipVel(0), numBeam(0), isHit(false), isHit2(false),
stunTime(10), stunTimer(100), coolDownTime(30), coolDownTimer(100),
fireOkay(true), beamFired(false)
{

}


void SpaceShip::Update(DX::StepTimer const& timer)
{

	XMVECTOR L;

	// canonical coordinate frame
	forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	left = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// rotate frame with current camera quaternion
	forward = XMVector4Transform(forward, XMMatrixRotationQuaternion(ori));
	up = XMVector4Transform(up, XMMatrixRotationQuaternion(ori));
	left = XMVector3Cross(forward, up);

	// tiny extra rotation even when there is no input
	//L = XMQuaternionRotationAxis(cam.forward, 0.00);
	//cam.ori = XMQuaternionMultiply(cam.ori, L);

	// camera advances slowly even without input
	//cam.pos = XMVectorAdd(cam.pos, XMVectorScale(cam.forward, 0.00));

	if (isHit)
	{
		shipVel -= 1;
		isHit = false;
	}
	else if (shipVel < 0)
	{
		shipVel += 0.1;
	}

	pos = XMVectorAdd(pos, XMVectorScale(forward, shipVel));


	ManageBeamFire();
	UpdateBeams(timer);
}

void SpaceShip::UpdateBeams(DX::StepTimer const& timer)
{

	for (int i = 0; i < numBeam; i += 2)
	{
		//Right beam
		beams[i].Update(timer);

		//Left beam
		beams[i + 1].Update(timer);
	}
}


void SpaceShip::CameraMove(float ahead, float updown)
{
	// move "ahead" amount in forward direction
	XMVECTOR aheadv = XMVectorScale(forward, ahead);
	XMVECTOR updownv = XMVectorScale(up, updown);

	pos = XMVectorAdd(pos, aheadv);
	pos = XMVectorAdd(pos, updownv);
}

void SpaceShip::CameraSpin(float roll, float pitch, float yaw)
{
	// make sure camera properties are up to date
	// NB: actually unnecessary, since they are updated every frame anyway; done here for clarity
	forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	left = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	forward = XMVector4Transform(forward, XMMatrixRotationQuaternion(ori));
	up = XMVector4Transform(up, XMMatrixRotationQuaternion(ori));
	left = XMVector3Cross(forward, up);
	// apply camera-relative orientation changes

	XMVECTOR rollq = XMQuaternionRotationAxis(forward, roll*0.01);
	XMVECTOR pitchq = XMQuaternionRotationAxis(left, pitch*0.01);
	XMVECTOR yawq = XMQuaternionRotationAxis(up, yaw*0.01);
	//Roll:
	ori = XMQuaternionMultiply(ori, rollq);
	//Pitch:
	ori = XMQuaternionMultiply(ori, pitchq);
	//Yaw:
	ori = XMQuaternionMultiply(ori, yawq);
}

void SpaceShip::FireBeam()
{
	if (fireOkay)
	{
		beamFired = true;
		fireOkay = false;
	}
}

void SpaceShip::ManageBeamFire() {

	XMVECTOR L;


	if (beamFired)
	{
		coolDownTimer = 0;

		//Add a beam to the beam list;
		numBeam += 2;
		Beam *temp = new Beam[numBeam];

		memcpy(temp, beams, (numBeam - 2)*sizeof(Beam));

		delete[] beams;
		beams = temp;


		//Left Beam
		beams[numBeam - 1].setPos(XMVectorAdd(pos, XMVectorScale(up, -1.0)));
		beams[numBeam - 1].setPos(XMVectorAdd(beams[numBeam - 1].getPos(), XMVectorScale(forward, -0.5)));
		beams[numBeam - 1].setPos(XMVectorAdd(beams[numBeam - 1].getPos(), XMVectorScale(left, -0.22)));
		beams[numBeam - 1].setDistance(0);

		L = XMQuaternionRotationAxis(up, -0.0025);
		beams[numBeam - 1].setOri(XMQuaternionMultiply(ori, L));

		beams[numBeam - 1].setForward(forward);
		beams[numBeam - 1].setForward(XMQuaternionMultiply(forward, L));

		//Right Beam
		beams[numBeam - 2].setPos(XMVectorAdd(pos, XMVectorScale(up, -1.0)));
		beams[numBeam - 2].setPos(XMVectorAdd(beams[numBeam - 2].getPos(), XMVectorScale(forward, -0.5)));
		beams[numBeam - 2].setPos(XMVectorAdd(beams[numBeam - 2].getPos(), XMVectorScale(left, 0.22)));
		beams[numBeam - 2].setDistance(0);

		L = XMQuaternionRotationAxis(up, 0.0025);
		beams[numBeam - 2].setOri(XMQuaternionMultiply(ori, L));

		beams[numBeam - 2].setForward(XMQuaternionMultiply(forward, L));


		beamFired = false;
	}
	else if (coolDownTimer < coolDownTime) {
		coolDownTimer++;
	}
	else if (coolDownTimer == coolDownTime)
	{
		coolDownTimer++;
		fireOkay = true;
	}

	int newNumBeam = numBeam;

	if (numBeam > 0)
	{
		//Move both beams to the end
		for (int j = 0; j < 2; j++)
		{
			if (beams[0].getDistance() > maxBeamDistance)
			{
				for (int i = 0; i < numBeam - 1; i++) {
					Beam tempBeam;

					tempBeam = beams[i];
					beams[i] = beams[i + 1];
					beams[i + 1] = tempBeam;

				}
				newNumBeam -= 1;
			}
		}
	}


	numBeam = newNumBeam;
	Beam *temp = new Beam[numBeam];

	memcpy(temp, beams, numBeam*sizeof(Beam));

	delete[] beams;
	beams = temp;
}


XMVECTOR SpaceShip::getForward()
{
	return forward;
}


XMVECTOR SpaceShip::getUp()
{
	return up;
}


XMVECTOR SpaceShip::getLeft()
{
	return left;
}

void SpaceShip::setIsHit(bool hit)
{
	isHit = hit;
}

void SpaceShip::setIsHit2(bool hit)
{
	isHit2 = hit;
}

bool SpaceShip::getIsHit()
{
	return isHit;
}

bool SpaceShip::getIsHit2()
{
	return isHit2;
}


Beam* SpaceShip::getBeams()
{
	return beams;
}

void SpaceShip::setBeams(Beam* inBeams)
{
	beams = inBeams;
}

int SpaceShip::getNumBeam()
{
	return numBeam;
}

void SpaceShip::setNumBeam(float inNumBeam)
{
	numBeam = inNumBeam;
}

int SpaceShip::getMaxBeamDistance()
{
	return maxBeamDistance;
}

bool SpaceShip::getFireOkay()
{
	return fireOkay;
}

bool SpaceShip::getBeamFired()
{
	return beamFired;
}

void SpaceShip::setFireOkay(bool fOkay)
{
	fireOkay = fOkay;
}

void SpaceShip::setBeamFired(bool bFired)
{
	beamFired = bFired;
}

int SpaceShip::getStunTimer()
{
	return stunTimer;
}

void SpaceShip::setStunTimer(int inST)
{
	stunTimer = inST;
}

int SpaceShip::getStunTime()
{
	return stunTime;
}

void SpaceShip::setStunTime(int inST)
{
	stunTime = inST;
}