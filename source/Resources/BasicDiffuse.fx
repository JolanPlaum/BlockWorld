//---------------------------------------------------
// Global Variables
//---------------------------------------------------
float gPI = 3.14159265358979323846f;
float gLightIntensity = 4.f;
float3 gLightDirection = normalize(float3(0.577f, -2.577f, 1.577f));
float gKd = 4.f / 3.14159265358979323846f;

float4x4 gWorldViewProj : WorldViewProjection;
Texture2D gDiffuseMap : DiffuseMap;

RasterizerState gRasterizerState
{
	CullMode = none;
	FrontCounterClockwise = false; //default
};

SamplerState gSamAnisotropic
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Mirror; // Mirror, Clamp, Border or Wrap
	AddressV = Mirror; // Mirror, Clamp, Border or Wrap
};

//---------------------------------------------------
// Input/Output Structs
//---------------------------------------------------
struct VS_INPUT
{
	float3 Position : POSITION;
	float2 TextureUV : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TextureUV : TEXCOORD;
	float3 Normal : NORMAL;
};

//---------------------------------------------------
// Vertex Shader
//---------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
	output.Normal = normalize(input.Normal);
	output.TextureUV = input.TextureUV;
	return output;
}

//---------------------------------------------------
// Pixel Shader
//---------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	//ambient color
	float3 finalColor = float3(0.025f, 0.025f, 0.025f);

	//Observed area (lambert cosine law)
	float dotProduct = saturate(dot(input.Normal, -gLightDirection));

	//lambert diffuse
	finalColor += gDiffuseMap.Sample(gSamAnisotropic, input.TextureUV).rgb * gKd * dotProduct;

	return float4(finalColor, 1.f);
}

//---------------------------------------------------
// Technique
//---------------------------------------------------
technique11 DefaultTechnique
{
	pass P0
	{
		//SetRasterizerState(gRasterizerState);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}