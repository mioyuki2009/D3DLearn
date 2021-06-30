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
    float paddingCameraBuffer;
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

//Water

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
	float4 refractionPosition : TEXCOORD2;
};

PixelInputTypeReflection ReflectionVertexShader(VertexInputTypeReflection input)
{
	PixelInputTypeReflection output;
	float4x4 reflectProjectWorld;
	float4x4 viewProjectWorld;

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

	// Create the view projection world matrix for refraction.
	viewProjectWorld = mul(viewMatrix, projectionMatrix);
	viewProjectWorld = mul(worldMatrix, viewProjectWorld);

	// Calculate the input position against the viewProjectWorld matrix.
	output.refractionPosition = mul(input.position, viewProjectWorld);


	return output;
}

Texture2D reflectionTexture;
Texture2D refractionTexture;
Texture2D normalTexture;

cbuffer WaterBuffer
{
	float waterTranslation;
	float reflectRefractScale;
	float2 paddingWaterBuffer;
};

float4 ReflectionPixelShader(PixelInputTypeReflection input) : SV_TARGET
{
	float2 reflectTexCoord;
	float2 refractTexCoord;
	float4 normalMap;
	float3 normal;
	float4 reflectionColor;
	float4 refractionColor;
	float4 color;

	// Move the position the water normal is sampled from to simulate moving water.	
	input.tex.y += waterTranslation;

	// Calculate the projected reflection texture coordinates.
	reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
	reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

	// Calculate the projected refraction texture coordinates.
	refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
	refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;

	// Sample the normal from the normal map texture.
	normalMap = normalTexture.Sample(SampleType, input.tex);

	// Expand the range of the normal from (0,1) to (-1,+1).
	normal = (normalMap.xyz * 2.0f) - 1.0f;

	// Re-position the texture coordinate sampling position by the normal map value to simulate the rippling wave effect.
	reflectTexCoord = reflectTexCoord + (normal.xy * reflectRefractScale);
	refractTexCoord = refractTexCoord + (normal.xy * reflectRefractScale);

	// Sample the texture pixels from the textures using the updated texture coordinates.
	reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);
	refractionColor = refractionTexture.Sample(SampleType, refractTexCoord);

	// Combine the reflection and refraction results for the final color.
	color = lerp(reflectionColor, refractionColor, 0.6f);

	return color;
}

//refraction
cbuffer ClipPlaneBuffer
{
	float4 clipPlane;
};

struct VertexInputTypeRefraction
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputTypeRefraction
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float clip : SV_ClipDistance0;
};

PixelInputTypeRefraction RefractionVertexShader(VertexInputTypeRefraction input)
{
	PixelInputTypeRefraction output;


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
	
	// Set the clipping plane.
	output.clip = dot(mul(input.position, worldMatrix), clipPlane);

	return output;
}

float4 RefractionPixelShader(PixelInputTypeRefraction input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;


	// Sample the texture pixel at this location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		color += (diffuseColor * lightIntensity);
	}

	// Saturate the final light color.
	color = saturate(color);

	// Multiply the texture pixel and the input color to get the final result.
	color = color * textureColor;

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

technique11 Pass3
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, RefractionVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, RefractionPixelShader()));
	}
}