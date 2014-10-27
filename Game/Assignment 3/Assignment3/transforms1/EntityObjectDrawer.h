#ifndef ENTITYOBJECTDRAWER_H
#define ENTITYOBJECTDRAWER_H

#pragma once

using namespace DirectX;
using namespace Microsoft::WRL;

namespace DirectXGame2{
	class EntityObjectDrawer sealed
	{
	public:
		EntityObjectDrawer();

		void setupEOD(ComPtr<ID3D11InputLayout>,
			ComPtr<ID3D11Buffer>,
			ComPtr<ID3D11Buffer>,
			ComPtr<ID3D11VertexShader>,
			ComPtr<ID3D11PixelShader>,
			ComPtr<ID3D11Buffer>);

	private:
		// Direct3D resources for cube geometry.
		ComPtr<ID3D11InputLayout>   m_inputLayout;
		ComPtr<ID3D11Buffer>        m_vertexBuffer;
		ComPtr<ID3D11Buffer>        m_indexBuffer;
		ComPtr<ID3D11VertexShader>  m_vertexShader;
		ComPtr<ID3D11PixelShader>   m_pixelShader;
		ComPtr<ID3D11Buffer>        m_constantBuffer;
	};

}

#endif