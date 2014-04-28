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
cbuffer FontColor: register(b5) { 
	float4 fontColor;
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

PSInputType FontVS(VSInputType input)
{
	PSInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);

	output.tex = input.tex;

	return output;
}

float4 FontPS(PSInputType input) : SV_TARGET {
	float4 color;

	color = shaderTexture.Sample(SampleType, input.tex);
	if(color.r == 0.0f)
	{
		color.a = 0.0f;
	}
	else
	{
		color.a = 0.0f;
		color=color*fontColor;
	}
	
	return color;
}