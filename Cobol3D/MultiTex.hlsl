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

Texture2D shaderTexture[3];
SamplerState SampleType;

struct VSInputType {
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PSInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PSInputType MultiTexVS(VSInputType input)
{
	PSInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);

	output.tex = input.tex;

	return output;
}

float4 MultiTexPS(PSInputType input) : SV_TARGET{
	float4 color1,color2,alphaValue;
	float4 blendColor;

	color1 = shaderTexture[0].Sample(SampleType, input.tex);
	color2 = shaderTexture[1].Sample(SampleType, input.tex);
	alphaValue = shaderTexture[2].Sample(SampleType, input.tex);

	blendColor = (alphaValue * color1) + ((1.0 - alphaValue) * color2);
	blendColor = saturate(blendColor);

	return blendColor;
}