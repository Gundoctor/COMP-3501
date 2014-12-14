//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved


Texture2D mypic : register(t0);
//Texture2D mypic2 : register(t1);
SamplerState mysampler : register(s0);
//SamplerState mysampler2 : register(s1);


cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
	float4 lightpos;
	float4 eyepos;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
	float3 normal : NORMAL0;
	float4 surfpos : POSITION0;
	float2 texcoord : TEXCOORD0;
};

// 
float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 eyee;
	float3 lighte;

	eyee = float3(eyepos.x, eyepos.y, eyepos.z);
	lighte = float3(lightpos.x, lightpos.y, lightpos.z);

	float3 L, N, V, H;

	N = normalize(input.normal);
	float3 cr;// cr2;


	N = normalize(N);
	V = (eyee - input.surfpos);
	V = normalize(V);
	L = (lighte - input.surfpos);
	L = normalize(L);
	float diffuse = dot(N, L);
	diffuse = saturate(diffuse);
	H = 0.5*(V + L);
	H = normalize(H);

	float spec = dot(N, H);
	spec = saturate(spec);
	spec = pow(spec, 275);


	float amb = 1.0;

	float c = 0.7*diffuse + 0.9*spec + 0.1*amb;
	//c = saturate(c);
	// use texture color instead of per-vertex color:
	//cr = float3(input.color.x, input.color.y, input.color.z);				
	cr = mypic.Sample(mysampler, input.texcoord);
	//cr2 = mypic2.Sample(mysampler, input.texcoord);


	return float4(c*cr, 1.0f);
}
