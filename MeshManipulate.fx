//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    float4 DiffuseMtrl;
    float4 DiffuseLight;
    float3 LightVecW;
    float gTime;
    float4 AmbientMtrl;
    float4 AmbientLight;
    float4 SpecularMtrl;
    float4 SpecularLight;
    float SpecularPower;
    float3 EyePosW;
}
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 Col : COLOR;
    float2 Tex : TEXCOORD0;
};


struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : NORMAL;
	float4 Col : COLOR;
    float3 PosW : POSITION;
    float2 TexC : TEXCOORD0;
};


//------------------------------------------------------------------------------------
// Vertex Shader - Implements Gouraud Shading using Diffuse lighting only
// Useful Functions dot(a,b)
//------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{


    VS_OUTPUT output = (VS_OUTPUT)0;

    output.Pos = mul(input.Pos, World);
    output.PosW = (output.Pos).xyz;
    // Convert from local space to world space
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    // W component of vector is 0 as vectors cannot be translated
    float3 normalW = normalize(mul(float4(input.NormalL, 0.0f), World).xyz);
    output.Norm = normalW;
    output.TexC = input.Tex;
    output.TexC.xy += 0.5f * sin(output.TexC.x) * sin(3.0f * gTime);


    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader!!!! Was a Float
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{


    input.Norm = normalize(input.Norm);
    // Compute the vector from the vertex to the eye position.
    // output.Pos is currently the position in world space
    float3 toEye = normalize(EyePosW - input.PosW.xyz);

    // Compute the reflection vector.
    float3 r = reflect(-LightVecW, input.Norm);

    // Compute Colour
    float diffuseAmount = max(dot(LightVecW, input.Norm), 0.0f);
    float3 diffuse = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;

    // Calculate Diffuse and Ambient Lighting
    float3 ambient = (AmbientMtrl * AmbientLight).rgb;


    // Determine how much (if any) specular light makes it
    // into the eye.
    float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
    // Compute the ambient, diffuse, and specular terms separately.
    float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;

    float4 textureColour = txDiffuse.Sample(samLinear, input.TexC);
    clip(textureColour.a - 0.05f);

    // Sum all the terms together and copy over the diffuse alpha.
    input.Col.rgb = ((textureColour + (diffuse + ambient)) + specular);
    input.Col.a = DiffuseMtrl.a;
    return input.Col;
}
