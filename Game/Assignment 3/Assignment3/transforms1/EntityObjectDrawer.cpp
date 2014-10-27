#include "pch.h"
#include "EntityObjectDrawer.h"


using namespace DirectX;
using namespace DirectXGame2;

EntityObjectDrawer::EntityObjectDrawer()
{

}



void EntityObjectDrawer::setupEOD(
	ComPtr<ID3D11InputLayout> iLayout,
	ComPtr<ID3D11Buffer> vBuffer,
	ComPtr<ID3D11Buffer> iBuffer,
	ComPtr<ID3D11VertexShader> vShader,
	ComPtr<ID3D11PixelShader> pShader,
	ComPtr<ID3D11Buffer> cBuffer)
{

	m_inputLayout = iLayout;
	m_vertexBuffer = vBuffer;
	m_indexBuffer = iBuffer;
	m_vertexShader = vShader;
	m_pixelShader = pShader;
	m_constantBuffer = cBuffer;
}
