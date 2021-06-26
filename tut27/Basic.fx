Texture2D shaderTexture;
SamplerState SampleType;

cbuffer MatrixBuffer
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

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
    float3 viewDirection : TEXCOORD1;
};

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
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
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

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
    float3 reflection;
    float4 specular;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	 // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	// Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));
	
    if (lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
		
		// Saturate the ambient and diffuse color.
        color = saturate(color);
		
		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = lightIntensity * reflect(-lightDir, input.normal);
		
		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = specularColor * pow(saturate(dot(reflection, input.viewDirection)), specularPower);
		
    }
	
    color = color * textureColor;

	color = saturate(color + specular);

	return color;
}

struct VertexInputTypeReflection
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

cbuffer ReflectionBuffer
{
	float4x4 reflectionMatrix;
};

struct PixelInputTypeReflection
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 reflectionPosition : TEXCOORD1;
};

PixelInputTypeReflection ReflectionVertexShader(VertexInputTypeReflection input)
{
	PixelInputTypeReflection output;
	float4x4 reflectProjectWorld;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	
	// Create the reflection projection world matrix.
	reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
	reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);
	
	// Calculate the input position against the reflectProjectWorld matrix.
	output.reflectionPosition = mul(input.position, reflectProjectWorld);

	return output;
}

Texture2D reflectionTexture;

float4 ReflectionPixelShader(PixelInputTypeReflection input) : SV_TARGET
{
	float4 textureColor;
	float2 reflectTexCoord;
	float4 reflectionColor;
	float4 color;


	// Sample the texture pixel at this location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Calculate the projected reflection texture coordinates.
	reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
	reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

	// Sample the texture pixel from the reflection texture using the projected texture coordinates.
	reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);

	// Do a linear interpolation between the two textures for a blend effect.
	color = lerp(textureColor, reflectionColor, 0.15f);

	return color;
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
		SetVertexShader(CompileShader(vs_5_0, ReflectionVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ReflectionPixelShader()));
	}
}