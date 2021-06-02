cbuffer PerFrameBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputTypeF
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

cbuffer PixelBuffer
{
	float4 pixelColor;
};

struct PixelInputTypeF
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


Texture2D shaderTexture;
SamplerState SampleType;

PixelInputTypeF FontVertexShader(VertexInputTypeF input)
{
	PixelInputTypeF output;


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

float4 FontPixelShader(PixelInputTypeF input) : SV_TARGET
{
	float4 color;
	color = shaderTexture.Sample(SampleType, input.tex);
	if (color.r == 0.0f)
	{
		color.a = 0.0f;
	}
	else
	{
		color.rgb = pixelColor.rgb;
		color.a = 1.0f;
	}
	return color;
}

technique11 Pass1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, FontVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, FontPixelShader()));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
};

PixelInputType LightVertexShader(VertexInputType input)
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

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	return output;
}

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;


	// Sample the texture pixel at this location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));

	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	color = saturate(diffuseColor * lightIntensity);

	// Multiply the texture pixel and the input color to get the final result.
	color = color * textureColor;

	return color;
}



technique11 Pass2
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, LightVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, LightPixelShader()));
	}
}
