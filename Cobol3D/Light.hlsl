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
	float3 normal : NORMAL;
};

struct PSInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

PSInputType LightVS(VSInputType input)
{
	PSInputType output;
	float4 worldPosition;

	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.tex = input.tex;

	worldPosition = mul(input.position, worldMatrix);
	output.viewDirection = cameraPosition.xyz-worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);
	return output;
}

float4 LightPS(PSInputType input) : SV_TARGET {
	float4 textureColor;
	float3 lightDir; 
	float lightIntesity;
	float4 color = ambientLight;
	float3 reflection;
	float4 specular = float4(0.0f,0.0f,0.0f,0.0f);

	textureColor = shaderTexture.Sample(SampleType, input.tex);

	lightDir = -lightDirection;
	lightIntesity = saturate(dot(input.normal, lightDir));
	if( lightIntesity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntesity);
		color = saturate(color);
		reflection = normalize(2*lightIntesity*input.normal-lightDir);
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }
	//color = saturate(color);
	color = color * textureColor;

	color=saturate(color+specular);
	
	color.w = textureColor.w;
	return color;
}