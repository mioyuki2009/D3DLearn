Texture2D shaderTexture;
Texture2D colorTexture;
Texture2D normalTexture;
Texture2D refractionTexture;
SamplerState SampleType;

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
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 refractionPosition : TEXCOORD1;
};


PixelInputType GlassVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4x4 viewProjectWorld;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	
	// Create the view projection world matrix for refraction.
	viewProjectWorld = mul(viewMatrix, projectionMatrix);
	viewProjectWorld = mul(worldMatrix, viewProjectWorld);
	
	// Calculate the input position against the viewProjectWorld matrix.
	output.refractionPosition = mul(input.position, viewProjectWorld);

	return output;
}

cbuffer GlassBuffer
{
	float refractionScale;
	float3 padding;
};

float4 GlassPixelShader(PixelInputType input) : SV_TARGET
{
	float2 refractTexCoord;
	float4 normalMap;
	float3 normal;
	float4 refractionColor;
	float4 textureColor;
	float4 color;
	// Calculate the projected refraction texture coordinates.
	refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
	refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;

	// Sample the normal from the normal map texture.
	normalMap = normalTexture.Sample(SampleType, input.tex);

	// Expand the range of the normal from (0,1) to (-1,+1).
	normal = (normalMap.xyz * 2.0f) - 1.0f;

	// Re-position the texture coordinate sampling position by the normal map value to simulate light distortion through glass.
	refractTexCoord = refractTexCoord + (normal.xy * refractionScale);

	// Sample the texture pixel from the refraction texture using the perturbed texture coordinates.
	refractionColor = refractionTexture.Sample(SampleType, refractTexCoord);

	// Sample the texture pixel from the glass color texture.
	textureColor = colorTexture.Sample(SampleType, input.tex);

	// Evenly combine the glass color and refraction value for the final color.
	color = lerp(refractionColor, textureColor, 0.5f);

	return color;
}


struct VertexInputTypeT
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputTypeT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInputTypeT TextureVertexShader(VertexInputTypeT input)
{
	PixelInputTypeT output;


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

float4 TexturePixelShader(PixelInputTypeT input) : SV_TARGET
{
	float4 textureColor;
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	return textureColor;
}

technique11 Pass1
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, GlassVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, GlassPixelShader()));
    }
}

technique11 Pass2
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, TextureVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, TexturePixelShader()));
	}
}