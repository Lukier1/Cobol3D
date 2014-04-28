cbuffer PerFrameB : register(b0){
	matrix projMatrix;
	float appTime;
	

};

cbuffer PerSkinnedB : register(b1) {

};

cbuffer PerStaticB : register(b2) {
	matrix worldMatrix;
};

cbuffer PerPassB : register(b3) {
	matrix viewMatrix;
	float3 cameraPosition;
	float padding;
};

cbuffer PerMaterialB : register(b4) {
	float SpecPower;
};
cbuffer LightB : register(b5) { 
	float4 ambientLight;
	float4 diffuseColor;
    float3 lightDirection;
    float  specularPower;
	float4 specularColor;
};
Texture2D shaderTexture;
SamplerState SampleType;

struct VSInputType {
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PSInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PSInputType TextureVS(VSInputType input)
{
	PSInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);

	output.tex = input.tex;

	return output;
}

float4 TexturePS(PSInputType input) : SV_TARGET {
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	textureColor.w = 0.0f;// normalize(input.tex.y);
	return textureColor;
}