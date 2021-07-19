Texture2D shaderTexture;
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
    AddressW = Wrap;
    ComparisonFunc = Always;
};

cbuffer MatrixBuffer
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 color : COLOR;
};

PixelInputType ParticleVertexShader(VertexInputType input)
{
	PixelInputType output;
    
	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

    output.color = input.color;
	
	return output;
}

float4 ParticlePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
    float4 finalColor;
	
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	
    finalColor = textureColor * input.color;

    return finalColor;
}

technique11 Pass1
{
	pass P0
	{
        SetVertexShader(CompileShader(vs_5_0, ParticleVertexShader()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, ParticlePixelShader()));
    }
}