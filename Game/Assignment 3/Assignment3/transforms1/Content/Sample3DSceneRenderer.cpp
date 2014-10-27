//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "Sample3DSceneRenderer.h"
#include <math.h>

#include "..\Helpers\DirectXHelper.h"

using namespace DirectXGame2;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
m_loadingComplete(false),
m_contextReady(false),
m_degreesPerSecond(45),
m_indexCount(0),
m_tracking(false),
m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
	CreateAsteroidField();
	CreateCamera();
}

void Sample3DSceneRenderer::CreateCamera()
{
	ship.setPos(XMVectorSet(0, 0, 0, 0));
	ship.setOri(XMQuaternionRotationRollPitchYaw(0, 0, 0));
}

void Sample3DSceneRenderer::CameraMove(float ahead, float updown)
{
	/*
	// move "ahead" amount in forward direction
	XMVECTOR aheadv = XMVectorScale(cam.forward, ahead);
	cam.pos = XMVectorAdd(cam.pos, aheadv);
	*/

	ship.CameraMove(ahead, updown);
}

void Sample3DSceneRenderer::CameraSpin(float roll, float pitch, float yaw)
{

	ship.CameraSpin(roll, pitch, yaw);
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
    Size outputSize = m_deviceResources->GetOutputSize();
    float aspectRatio = outputSize.Width / outputSize.Height;
    float fovAngleY = 70.0f * XM_PI / 180.0f;

    // This is a simple example of change that can be made when the app is in
    // portrait or snapped view.
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 1.0f;
    }

    // Note that the OrientationTransform3D matrix is post-multiplied here
    // in order to correctly orient the scene to match the display orientation.
    // This post-multiplication step is required for any draw calls that are
    // made to the swap chain render target. For draw calls to other targets,
    // this transform should not be applied.

    // This sample makes use of a right-handed coordinate system using row-major matrices.
//	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveRH(12, 8, 0.1, 100);
   
	
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
        fovAngleY,
        aspectRatio,
        0.1f,
        1000.0f
        );
		
    XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

    XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

    XMStoreFloat4x4(
        &m_constantBufferData.projection,
        XMMatrixTranspose(perspectiveMatrix) // * orientationMatrix)
        );

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, -0.1f, -58.0f, 1.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 1.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	//XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixV);

}

void Sample3DSceneRenderer::CreateAsteroidField()
{
	numast = 1500;
	XMVECTOR angles;
//	debris = malloc(numast*sizeof(Asteroid));

	for (int i = 0; i < numast; i++)
	{
		angles = XMVectorSet(3.14*(rand() % 1000) / 1000.0f, 3.14*(rand() % 1000) / 1000.0f, 3.14*(rand() % 1000) / 1000.0f, 1.0f);
		aField[i].setPos(XMVectorSet(600 * (rand() % 1000) / 1000.0f, 600 * (rand() % 1000) / 1000.0f, 600 * (rand() % 1000) / 1000.0f, 1.0f));
		aField[i].setOri(XMQuaternionRotationRollPitchYawFromVector(angles));
		angles = XMVectorSet(0.01*3.14*(rand() % 1000) / 1000.0f, 0.01*3.14*(rand() % 1000) / 1000.0f, 0.01*3.14*(rand() % 1000) / 1000.0f, 1.0f);

		aField[i].setL(XMQuaternionRotationRollPitchYawFromVector(angles));
	}
}

void Sample3DSceneRenderer::CollisionDetection() 
{

	int newNumAst = numast;

	XMMATRIX thexform;
	for (int i = 0; i < numast; i++)
	{
		XMFLOAT4 camPos, astPos;
		XMStoreFloat4(&camPos, ship.getPos());
		XMStoreFloat4(&astPos, aField[i].getPos());

		float dx = camPos.x - astPos.x;
		float dy = camPos.y - astPos.y;
		float dz = camPos.z - astPos.z;


		/*
		(l . (o - c))^2 - ||o - c||^2 + r^2
		if > 0, intersects sphere
		if < 0, does not intersect sphere

		l = direction of line (unit vector)
		c = center point
		o = origin of line
		r = radius of sphere
		*/

		Beam *sBeams = ship.getBeams();
		
		for (int j = 0; j < ship.getNumBeam(); j++)
		{
			
			//XMVECTOR oc = XMVectorSubtract(sBeams[j].getPos(), debris[i].pos);
			XMVECTOR oc = XMVectorSubtract(sBeams[j].getPos(), aField[i].getPos());

			XMFLOAT4 dotVec, ocAbs;

			//XMStoreFloat4(&dotVec, XMVector4Dot(XMVector4Normalize(beams[j].forward), oc));
			XMStoreFloat4(&dotVec, XMVector4Dot(XMVector4Normalize(sBeams[j].getForward()), oc));

			XMStoreFloat4(&ocAbs, oc);

			float ocabsF = sqrt(ocAbs.x*ocAbs.x + ocAbs.y*ocAbs.y + ocAbs.z*ocAbs.z);
			float dot = dotVec.x;

			//(l . (o - c)) ^ 2 - || o - c || ^ 2 + r ^ 2
			float val = dot*dot - ocabsF*ocabsF + astRad*astRad;

			if (val >= 0)
			{
				//Is an intersection

				float d = -dot + sqrt(val);
				if (d < astRad)
				{
					//delete the asteroid
					aField[i].setShouldBeDel(true);
				}

				d = -dot - sqrt(val);
				if (d < astRad)
				{
					aField[i].setShouldBeDel(true);
				}
			}
		}

		float length = sqrt(dx*dx + dy*dy + dz*dz);

		if (fabs(length) < astRad + shipRad && !aField[i].getShouldBeDel())
		{
			ship.setIsHit(true);
			ship.setIsHit2(true);
		}
	}

}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
    if (!m_tracking)
    {
        // Convert degrees to radians, then convert seconds to rotation angle
        float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
        double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
        float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

        Rotate(radians);
    }
	UpdateWorld(timer);
	UpdatePlayer(timer);
}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radians)
{
    // Prepare to pass the updated model matrix to the shader
    XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void Sample3DSceneRenderer::DrawOne(ID3D11DeviceContext2 *context, XMMATRIX *thexform) 
{
	/* Draw the object set up in the current context. Use the input transformation matrix.
	(i.e. first send matrix to shader using constant buffer, then draw)
	*/
	
	if (context == NULL)
		return; // don't try anything if no context set

	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(*thexform));

	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
		);
}


void Sample3DSceneRenderer::StartTracking()
{
    m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
    if (m_tracking)
    {
        float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
        Rotate(radians);
    }
}

void Sample3DSceneRenderer::StopTracking()
{
    m_tracking = false;
}

void Sample3DSceneRenderer::UpdatePlayer(DX::StepTimer const& timer)
{
	CollisionDetection();

	ship.Update(timer);

	// remake view matrix, store in constant buffer data
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookToRH(ship.getPos(), ship.getForward(), ship.getUp())));
	// constant buffer data is headed to card on per-object basis, so no need to reset here


}

void Sample3DSceneRenderer::UpdateWorld(DX::StepTimer const& timer)
{
	// asteroids rotate, here; could make them move also, not done now
	for (int i = 0; i < numast; i++)
	{
		aField[i].Update(timer);
	}
}


// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{

	m_contextReady = false;
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

    // Set render targets to the screen.
    ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
    context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

    // Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

    // Each vertex is one instance of the VertexPositionColor struct.
    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    context->IASetVertexBuffers(
        0,
        1,
        m_vertexBuffer.GetAddressOf(),
        &stride,
        &offset
        );

    context->IASetIndexBuffer(
        m_indexBuffer.Get(),
        DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
        0
        );

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->IASetInputLayout(m_inputLayout.Get());

    // Attach our vertex shader.
    context->VSSetShader(
        m_vertexShader.Get(),
        nullptr,
        0
        );

    // Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

    // Attach our pixel shader.
    context->PSSetShader(
        m_pixelShader.Get(),
        nullptr,
        0
        );

	XMMATRIX thexform;
	
	for (int i = 0; i < numast; i++)
	{ // draw every asteroid
		//draw not "destroyed" asteroids
		if (!aField[i].getShouldBeDel())
		{
			thexform = XMMatrixRotationQuaternion(aField[i].getOri());
			thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(aField[i].getPos()));
			DrawOne(context, &thexform);
		}
	}

	ManageTargetReticle(context);
	ManageBeamFire(context);
	ManageScreenFlash(context);


	XMFLOAT4 camPos;
	XMStoreFloat4(&camPos, ship.getPos());
	
	
	m_contextReady = true;
}

//Called from DirectXGame2Main.cpp when spacebar is pressed
void Sample3DSceneRenderer::FireBeam()
{
	ship.FireBeam();
}

void Sample3DSceneRenderer::ManageBeamFire(ID3D11DeviceContext2 *context)
{
	XMMATRIX thexform;

	Beam *sBeams = ship.getBeams();


	for (int i = 0; i < ship.getNumBeam(); i += 2)
	{
		//Right Beam
		thexform = XMMatrixTranslationFromVector(sBeams[i].getPos());
		thexform = XMMatrixMultiply(XMMatrixRotationQuaternion(sBeams[i].getOri()), thexform);

		thexform = XMMatrixMultiply(XMMatrixScaling(0.05, 0.05, 5), thexform);
		DrawOne(context, &thexform);


		//Left Beam
		thexform = XMMatrixRotationQuaternion(sBeams[i + 1].getOri());
		thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(sBeams[i + 1].getPos()));

		thexform = XMMatrixMultiply(XMMatrixScaling(0.05, 0.05, 5), thexform);
		DrawOne(context, &thexform);
	}

}

void Sample3DSceneRenderer::ManageScreenFlash(ID3D11DeviceContext2 *context)
{
	XMMATRIX thexform;

	CreateScreenFlash();

	int stunTimer = ship.getStunTimer();
	int stunTime = ship.getStunTime();
	
	if (ship.getIsHit2())
	{
		stunTimer = 0;
		ship.setIsHit2(false);
	}
	else if (stunTimer < stunTime) {
		stunTimer++;

		thexform = XMMatrixRotationQuaternion(ship.getOri());
		thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(XMVectorAdd(ship.getPos(), XMVectorScale(ship.getForward(), 1.0))));


		thexform = XMMatrixMultiply(XMMatrixScaling(5, 5, 0.05), thexform);
		DrawOne(context, &thexform);
	}
	else if (stunTimer == stunTime)
	{
		stunTimer++;
	}

	ship.setStunTimer(stunTimer);

	CreateBaseCube();
}

void Sample3DSceneRenderer::ManageTargetReticle(ID3D11DeviceContext2 *context)
{
	CreateTargetReticle();

	XMMATRIX thexform;
	XMVECTOR L, camcpy;

	camcpy = ship.getPos();

	//left bar
	thexform = XMMatrixTranslationFromVector(XMVectorAdd(camcpy, XMVectorScale(ship.getForward(), 6.0)));
	thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(XMVectorScale(ship.getLeft(), -0.33)));
	thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(XMVectorScale(ship.getUp(), -0.98)));
	thexform = XMMatrixMultiply(XMMatrixRotationQuaternion(ship.getOri()), thexform);
	
	thexform = XMMatrixMultiply(XMMatrixScaling(0.05, 0.625, 0.05), thexform);
	DrawOne(context, &thexform);

	//right bar
	thexform = XMMatrixTranslationFromVector(XMVectorAdd(camcpy, XMVectorScale(ship.getForward(), 6.0)));
	thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(XMVectorScale(ship.getLeft(), 0.33)));
	thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(XMVectorScale(ship.getUp(), -0.98)));
	thexform = XMMatrixMultiply(XMMatrixRotationQuaternion(ship.getOri()), thexform);
	
	thexform = XMMatrixMultiply(XMMatrixScaling(0.05, 0.625, 0.05), thexform);
	DrawOne(context, &thexform);

	//top bar
	thexform = XMMatrixTranslationFromVector(XMVectorAdd(camcpy, XMVectorScale(ship.getForward(), 6.0)));
	thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(XMVectorScale(ship.getUp(), -0.65)));
	thexform = XMMatrixMultiply(XMMatrixRotationQuaternion(ship.getOri()), thexform);

	thexform = XMMatrixMultiply(XMMatrixScaling(0.625, 0.05, 0.05), thexform);
	DrawOne(context, &thexform);

	//bottom bar
	thexform = XMMatrixTranslationFromVector(XMVectorAdd(camcpy, XMVectorScale(ship.getForward(), 6.0)));
	thexform = XMMatrixMultiply(thexform, XMMatrixTranslationFromVector(XMVectorScale(ship.getUp(), -1.31)));
	thexform = XMMatrixMultiply(XMMatrixRotationQuaternion(ship.getOri()), thexform);
	
	thexform = XMMatrixMultiply(XMMatrixScaling(0.625, 0.05, 0.05), thexform);
	DrawOne(context, &thexform);

	CreateBaseCube();
}


void Sample3DSceneRenderer::CreateTargetReticle()
{
	// Load mesh vertices. Each vertex has a position and a color.
	static const VertexPositionColor cubeVertices[] =
	{
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_vertexBuffer
		)
		);

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.
	static const unsigned short cubeIndices[] =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	m_indexCount = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&m_indexBuffer
		)
		);




	auto context = m_deviceResources->GetD3DDeviceContext();

	// Set render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);
}


void Sample3DSceneRenderer::CreateBaseCube() {

	// Load mesh vertices. Each vertex has a position and a color.
	static const VertexPositionColor cubeVertices[] =
	{
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) }
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_vertexBuffer
		)
		);

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.
	static const unsigned short cubeIndices[] =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	m_indexCount = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&m_indexBuffer
		)
		);



	auto context = m_deviceResources->GetD3DDeviceContext();

	// Set render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);
}


void Sample3DSceneRenderer::CreateScreenFlash() {

	// Load mesh vertices. Each vertex has a position and a color.
	static const VertexPositionColor cubeVertices[] =
	{
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.5f, 0.0f, 0.0f) }
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_vertexBuffer
		)
		);

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.
	static const unsigned short cubeIndices[] =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	m_indexCount = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&m_indexBuffer
		)
		);


	auto context = m_deviceResources->GetD3DDeviceContext();

	// Set render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
    // Load shaders asynchronously.
    auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
    auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

    // After the vertex shader file is loaded, create the shader and input layout.
    auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateVertexShader(
                &fileData[0],
                fileData.size(),
                nullptr,
                &m_vertexShader
                )
            );

        static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateInputLayout(
                vertexDesc,
                ARRAYSIZE(vertexDesc),
                &fileData[0],
                fileData.size(),
                &m_inputLayout
                )
            );
    });

    // After the pixel shader file is loaded, create the shader and constant buffer.
    auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreatePixelShader(
                &fileData[0],
                fileData.size(),
                nullptr,
                &m_pixelShader
                )
            );

        CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateBuffer(
                &constantBufferDesc,
                nullptr,
                &m_constantBuffer
                )
            );
    });

    // Once both shaders are loaded, create the mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this]() {
		CreateBaseCube();
    });

    // Once the cube is loaded, the object is ready to be rendered.
    createCubeTask.then([this] () {
        m_loadingComplete = true;
    });
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
    m_loadingComplete = false;
    m_vertexShader.Reset();
    m_inputLayout.Reset();
    m_pixelShader.Reset();
    m_constantBuffer.Reset();
    m_vertexBuffer.Reset();
    m_indexBuffer.Reset();
}