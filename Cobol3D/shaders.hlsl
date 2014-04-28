

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
};

cbuffer PerMaterialB : register(b4) {
	float SpecPower;
};

struct VSInputType {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PSInputType {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInputType ColorVS(VSInputType input)
{
	PSInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);

	output.color = input.color;

	return output;
}

float3 ColorPS(PSInputType input) : SV_TARGET {
	return input.color;
}