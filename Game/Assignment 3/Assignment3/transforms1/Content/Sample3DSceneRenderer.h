//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "..\Helpers\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Helpers\StepTimer.h"
#include "..\..\DirectXTK\Inc\Model.h"

#include ".\Content\Game Entity\GameEntity.h"
#include ".\Content\Game Entity\SpaceShip.h"
#include ".\Content\Game Entity\Camera.h"
#include ".\Content\Game Entity\Beam.h"
#include ".\Content\Game Entity\Asteroid.h"
#include ".\Content\Game Entity\PickUP.h"
#include ".\Content\Game Entity\Enemy.h"


using namespace DirectX;

namespace DirectXGame2
{
    // This sample renderer instantiates a basic rendering pipeline.
    class Sample3DSceneRenderer
    {
    public:
        Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();
        void ReleaseDeviceDependentResources();
        void Update(DX::StepTimer const& timer);
        void Render();
		void UpdatePlayer(DX::StepTimer const& timer);
		void UpdateWorld(DX::StepTimer const& timer);
        void StartTracking();
        void TrackingUpdate(float positionX);
        void StopTracking();
        bool IsTracking() { return m_tracking; }
		void CameraSpin(float roll, float pitch, float yaw);
		void CameraMove(float forr, float updown);
		void FireBeam();
		void BurnFuel();
		void SwapCamera();
    private:
        void Rotate(float radians);
		void DrawOne(ID3D11DeviceContext2 *context, XMMATRIX *thexform);
		void CreateAsteroidField();
		void CreateLootBoxes();
		void CreateEnemyBases();
		void CreateCamera(); 
		void CreateShip();
		void CollisionDetection(); 
		void CreateBaseCube();
		void CreateScreenFlash();
		void CreateTargetReticle();
		void ManageScreenFlash(ID3D11DeviceContext2 *context);
		void ManageBeamFire(ID3D11DeviceContext2 *context);
		void ManageTargetReticle(ID3D11DeviceContext2 *context);
		void ManageEnemies(ID3D11DeviceContext2 *context);
		void CheckGameOver();
		
    private:
        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources> m_deviceResources;

        // Direct3D resources for cube geometry.
        Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_inputLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer>        m_vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer>        m_indexBuffer;
        Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_pixelShader;
        Microsoft::WRL::ComPtr<ID3D11Buffer>        m_constantBuffer;

        // System resources for cube geometry.
        ModelViewProjectionConstantBuffer    m_constantBufferData;
        uint32    m_indexCount;

		//for models
		std::unique_ptr<DirectX::Model> healthbox;

        // Variables used with the rendering loop.
        bool    m_loadingComplete;
		bool	m_contextReady;
        float   m_degreesPerSecond;
        bool    m_tracking;

		// Variables for player
		SpaceShip ship;
		Camera cam;

		//keep track of screen size. needed to compute projection matrix to draw models.
		float aspectRatio;
		float fovAngleY;

		Asteroid aField[1500];

		PickUp scrapBoxes[200];
		PickUp fuelBoxes[500];
		PickUp upgradeBoxes[50];

		//all this stuff will be changed later
		Enemy enemies[10];
		XMVECTOR eBasePos;
		XMVECTOR eBaseOri;
		XMVECTOR eBaseL;
		float turnAngle;
		bool isWithinRange = false;
		bool launchOK = false;
		bool startLaunch = false;
		int enemyLaunchCounter = 1000;
		int enemyLaunchCount = 80;
		int currEnemyNum = 0;

		int numast, numScrap, numFuel,numUp;
		float lootRad = 0.5;
		float astRad = 1.0;
		float eRad = 1.0;
		float baseRange = 300;
		float baseRad = 3.0;
		float shipRad = 0.5;
		float screenFlash = 0.0;
		int score = 0;		
		int fuelMax = 1000;
		int fuel = fuelMax;		
		int healthMax = 10;
		int health = healthMax;
		int upgrades = 0;
		int cameraMode = 0;
		bool gameOver = FALSE; 
		int swaptime = 0;

    };
}

