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
Texture2D shaderTexture[3];
SamplerState SampleType;

struct VSInputType {
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PSInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDirection : TEXCOORD1;
	float fogFactor : FOG;
};

PSInputType BumpMapVS(VSInputType input)
{
	PSInputType output;
	float4 worldPosition;
	float4 campos;
	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.tex = input.tex;

	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	worldPosition = mul(input.position, worldMatrix);
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

	campos = mul(input.position, worldMatrix);
	campos = mul(campos, viewMatrix);
	output.fogFactor = saturate(pow(1.0 / 2.71828, campos.z * 0.2f*campos.z * 0.2f));
	//output.fogFactor = saturate((100.0f - campos.z) / (100.0f - 0.0f));

	return output;
}

float4 BumpMapPS(PSInputType input) : SV_TARGET{
	float4 textureColor;
	float3 lightDir;
	float lightIntesity;
	float4 bumpmap;
	float3 bumpnormal;
	float4 color = ambientLight;
	float3 reflection;
	float4 specularIntensity;
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 fogColor;

	textureColor = shaderTexture[0].Sample(SampleType, input.tex);
	bumpmap = shaderTexture[1].Sample(SampleType, input.tex);
	bumpmap = (bumpmap*2.0f) - 1.0f;
	bumpnormal = (bumpmap.x*input.tangent) + (bumpmap.y*input.binormal) + (bumpmap.z*input.normal);

	bumpnormal = normalize(bumpnormal);

	lightDir = -lightDirection;
	lightIntesity = saturate(dot(bumpnormal, lightDir));
	
	color = saturate(diffuseColor*lightIntesity);
	color = color * textureColor;
	if (lightIntesity > 0.0f)
	{
		specularIntensity = shaderTexture[2].Sample(SampleType, input.tex);
	
		reflection = normalize(2 * lightIntesity * bumpnormal - lightDir);
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
		specular = specular * specularIntensity;

		color = saturate(color + specular);
	}
	// Set the color of the fog to grey.
	fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
	
	// Calculate the final color using the fog effect equation.
	color = input.fogFactor * color + (1.0 - input.fogFactor) * fogColor;
	color.w = 1.0f;
	return color;
}