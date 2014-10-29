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

#include "GameEntity.h"
#include "SpaceShip.h"
#include "Beam.h"
#include "Asteroid.h"
#include "PickUP.h"


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
    private:
        void Rotate(float radians);
		void DrawOne(ID3D11DeviceContext2 *context, XMMATRIX *thexform);
		void CreateAsteroidField();
		void CreateLootBoxes();
		void CreateCamera(); 
		void CollisionDetection(); 
		void CreateBaseCube();
		void CreateScreenFlash();
		void CreateTargetReticle();
		void ManageScreenFlash(ID3D11DeviceContext2 *context);
		void ManageBeamFire(ID3D11DeviceContext2 *context);
		void ManageTargetReticle(ID3D11DeviceContext2 *context);
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

        // Variables used with the rendering loop.
        bool    m_loadingComplete;
		bool	m_contextReady;
        float   m_degreesPerSecond;
        bool    m_tracking;

		// Variables for player
		SpaceShip ship;

		// Variables for asteroid field
		typedef struct Asteroids {
			XMVECTOR pos; // position
			XMVECTOR ori; // orientation
			XMVECTOR L; // angular momentum (use as velocity)
			XMVECTOR vel; // linear velocity
			bool shouldBeDel;
		};

		Asteroid aField[1500];

		PickUp lootBoxes[300];

		int numast, numBoxes;
		Asteroids debris[1500];
		float lootRad = 0.5;
		float astRad = 1.0;
		float shipRad = 0.5;
		float screenFlash = 0.0;
		int score = 0;

    };
}

