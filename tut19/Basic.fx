cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

Texture2DArray shaderTextures;
Texture2D AlphaTextures;

SamplerState SampleType;

PixelInputType TextureVertexShader(VertexInputType input)
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

	return output;
}

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 blendColor;
    float4 alphaValue;

    // Get the pixel color from the first texture.
    color1 = shaderTextures.Sample(SampleType, float3(input.tex, 0.0f));

    // Get the pixel color from the second texture.
    color2 = shaderTextures.Sample(SampleType, float3(input.tex, 1.0f));

    alphaValue = AlphaTextures.Sample(SampleType, input.tex);
    blendColor = (alphaValue * color1) + ((1.0 - alphaValue) * color2);
    
    // Saturate the final color value.
    blendColor = saturate(blendColor);
    return blendColor;
}

technique11 Pass1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, TextureVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, TexturePixelShader()));
	}
}