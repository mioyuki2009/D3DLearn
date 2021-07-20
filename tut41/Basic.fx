cbuffer MatrixBuffer
{
    float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
    
    float4x4 lightViewMatrix;
    float4x4 lightProjectionMatrix;
    
    float4x4 lightViewMatrix2;
    float4x4 lightProjectionMatrix2;
};

cbuffer LightBuffer2
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 diffuseColor2;
    float3 lightPosition;
    float padding1;
    float3 lightPosition2;
    float padding2;
};

struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};

PixelInputType DepthVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the position value in a second input value for depth value calculations.
    output.depthPosition = output.position;
	
    return output;
}


float4 DepthPixelShader(PixelInputType input) : SV_TARGET
{
    float depthValue;
    float4 color = float4(1.0, 0.0f, 0.0f, 1.0f);

	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    depthValue = input.depthPosition.z / input.depthPosition.w;

    color = float4(depthValue, depthValue, depthValue, 1.0f);

    return color;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////
// TYPEDEFS //
//////////////
struct VertexInputTypeShadow
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputTypeShadow
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
    float4 lightViewPosition2 : TEXCOORD3;
    float3 lightPos2 : TEXCOORD4;
};

PixelInputTypeShadow ShadowVertexShader(VertexInputTypeShadow input)
{
    PixelInputTypeShadow output;
    float4 worldPosition;
    
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = mul(input.position, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

    // Calculate the position of the vertice as viewed by the second light source.
    output.lightViewPosition2 = mul(input.position, worldMatrix);
    output.lightViewPosition2 = mul(output.lightViewPosition2, lightViewMatrix2);
    output.lightViewPosition2 = mul(output.lightViewPosition2, lightProjectionMatrix2);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the light position based on the position of the light and the position of the vertex in the world.
    output.lightPos = lightPosition.xyz - worldPosition.xyz;

    // Normalize the light position vector.
    output.lightPos = normalize(output.lightPos);

    // Determine the second light position based on the position of the light and the position of the vertex in the world.
    output.lightPos2 = lightPosition2.xyz - worldPosition.xyz;

    // Normalize the second light position vector.
    output.lightPos2 = normalize(output.lightPos2);
    
    return output;
}

Texture2D shaderTexture;
Texture2D depthMapTexture;
Texture2D depthMapTexture2;

SamplerState SampleTypeClamp
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
};

SamplerState SampleTypeWrap
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
    AddressW = Wrap;
    ComparisonFunc = Always;
	BorderColor = float4(0, 0, 0, 0);
	MaxAnisotropy = 1;
	MipLODBias = 0;
};


float4 ShadowPixelShader(PixelInputTypeShadow input) : SV_TARGET
{
    float bias;
    float4 color;
    float2 projectTexCoord;
    float depthValue;
    float lightDepthValue;
    float lightIntensity;
    float4 textureColor;
    
    // Set the bias value for fixing the floating point precision issues.
    bias = 0.001f;

    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;
    
    // Calculate the projected texture coordinates.
    projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
    
    // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        // Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
        depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

        // Calculate the depth of the light.
        lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

        // Subtract the bias from the lightDepthValue.
        lightDepthValue = lightDepthValue - bias;

        // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
        if (lightDepthValue < depthValue)
        {
            // Calculate the amount of light on this pixel.
            lightIntensity = saturate(dot(input.normal, input.lightPos));
            if (lightIntensity > 0.0f)
            {
                // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
                color += (diffuseColor * lightIntensity);
            }
        }
    }
    
    // Second light.
    projectTexCoord.x = input.lightViewPosition2.x / input.lightViewPosition2.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.lightViewPosition2.y / input.lightViewPosition2.w / 2.0f + 0.5f;

    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        depthValue = depthMapTexture2.Sample(SampleTypeClamp, projectTexCoord).r;

        lightDepthValue = input.lightViewPosition2.z / input.lightViewPosition2.w;
        lightDepthValue = lightDepthValue - bias;

        if (lightDepthValue < depthValue)
        {
            lightIntensity = saturate(dot(input.normal, input.lightPos2));
            if (lightIntensity > 0.0f)
            {
                color += (diffuseColor2 * lightIntensity);
            }
        }
    }

    // Saturate the final light color.
    color = saturate(color);
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

    // Combine the light and texture color.
    color = color * textureColor;

    return color;
}

technique11 Pass1
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, DepthVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, DepthPixelShader()));
    }
}

technique11 Pass2
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, ShadowVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, ShadowPixelShader()));
    }
}