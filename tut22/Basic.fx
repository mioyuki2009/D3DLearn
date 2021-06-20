Texture2D shaderTexture;
Texture2D bumpTexture;
Texture2D speTexture;
SamplerState SampleType;

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
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
	float3 viewDirection : TEXCOORD1;
};

cbuffer LightBuffer
{
	float4 diffuseColor;
	float4 specularColor;
	float specularPower;
	float3 lightDirection;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
};

PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
	float4 worldPosition;
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) worldMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);

	
	// Calculate the tangent vector against the world matrix only and then normalize the final value.
    output.tangent = mul(input.tangent, (float3x3) worldMatrix);
    output.tangent = normalize(output.tangent);

    // Calculate the binormal vector against the world matrix only and then normalize the final value.
    output.binormal = mul(input.binormal, (float3x3) worldMatrix);
    output.binormal = normalize(output.binormal);
	

	// Calculate the position of the vertex in the world.
	worldPosition = mul(input.position, worldMatrix);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

    return output;
}

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float4 bumpMap;
    float3 bumpNormal;
	float4 specularIntensity;
	float3 reflection;
	float4 specular;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	 // Sample the pixel in the bump map.
    bumpMap = bumpTexture.Sample(SampleType, input.tex);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;
	
	// Calculate the normal from the data in the bump map.
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	
	  // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);
	// Invert the light direction for calculations.
    lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(bumpNormal, lightDir));

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    color = saturate(diffuseColor * lightIntensity);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;

	if (lightIntensity > 0.0f)
	{
		// Sample the pixel from the specular map texture.
		specularIntensity = speTexture.Sample(SampleType, input.tex);
		
		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		reflection = lightIntensity * reflect(-lightDir, bumpNormal);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
		
		// Use the specular map to determine the intensity of specular light at this pixel.
		specular = specular * specularIntensity;

		// Add the specular component last to the output color.
		color = saturate(color + specular);
	}

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

PixelInputType TextureVertexShader(VertexInputTypeT input)
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
        SetVertexShader(CompileShader(vs_5_0, LightVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, LightPixelShader()));
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