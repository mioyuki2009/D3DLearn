cbuffer MatrixBuffer
{
    float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
    
    float4x4 lightViewMatrix;
    float4x4 lightProjectionMatrix;
};

cbuffer LightBuffer2
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightPosition;
    float padding;
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

    // Store the position of the vertice as viewed by the camera in a separate variable.
    output.lightViewPosition = mul(input.position, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

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

    return output;
}

Texture2D shaderTexture;
Texture2D depthMapTexture;
Texture2D shadowTexture;

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
    color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
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
                color = float4(1.0f, 1.0f, 1.0f, 1.0f);
            }
        }
    }

    return color;
}

struct VertexInputTypeSoftShadow
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputTypeSoftShadow
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 viewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

PixelInputTypeSoftShadow SoftShadowVertexShader(VertexInputTypeSoftShadow input)
{
    PixelInputTypeSoftShadow output;
    float4 worldPosition;
    
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    // Store the position of the vertice as viewed by the camera in a separate variable.
    output.viewPosition = output.position;

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

    return output;
}

float4 SoftShadowPixelShader(PixelInputTypeSoftShadow input) : SV_TARGET
{
    float4 color;
    float lightIntensity;
    float4 textureColor;
    float2 projectTexCoord;
    float shadowValue;

    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, input.lightPos));
    if (lightIntensity > 0.0f)
    {
        // Determine the light color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);

        // Saturate the light color.
        color = saturate(color);
    }

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

    // Combine the light and texture color.
    color = color * textureColor;
   
    // Calculate the projected texture coordinates to be used with the shadow texture.
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;

    // Sample the shadow value from the shadow texture using the sampler at the projected texture coordinate location.
    shadowValue = shadowTexture.Sample(SampleTypeClamp, projectTexCoord).r;

    // Combine the shadows with the final color.
    color = color * shadowValue;

    return color;
}

// depth pass
technique11 Pass1
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, DepthVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, DepthPixelShader()));
    }
}

cbuffer ScreenSizeBufferW
{
    float screenWidth;
    float3 paddingW;
};

cbuffer ScreenSizeBufferH
{
    float screenHeight;
    float3 paddingH;
};

SamplerState SampleType;

struct VertexInputTypeBlur
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputTypeBlur
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
    float2 texCoord9 : TEXCOORD9;
};


PixelInputTypeBlur HorizontalBlurVertexShader(VertexInputTypeBlur input)
{
    PixelInputTypeBlur output;
    float texelSize;


    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Determine the floating point size of a texel for a screen with this specific width.
    texelSize = 1.0f / screenWidth;

    // Create UV coordinates for the pixel and its four horizontal neighbors on either side.
    output.texCoord1 = input.tex + float2(texelSize * -4.0f, 0.0f);
    output.texCoord2 = input.tex + float2(texelSize * -3.0f, 0.0f);
    output.texCoord3 = input.tex + float2(texelSize * -2.0f, 0.0f);
    output.texCoord4 = input.tex + float2(texelSize * -1.0f, 0.0f);
    output.texCoord5 = input.tex + float2(texelSize * 0.0f, 0.0f);
    output.texCoord6 = input.tex + float2(texelSize * 1.0f, 0.0f);
    output.texCoord7 = input.tex + float2(texelSize * 2.0f, 0.0f);
    output.texCoord8 = input.tex + float2(texelSize * 3.0f, 0.0f);
    output.texCoord9 = input.tex + float2(texelSize * 4.0f, 0.0f);

    return output;

}
float4 HorizontalBlurPixelShader(PixelInputTypeBlur input) : SV_TARGET
{
    float weight0, weight1, weight2, weight3, weight4;
    float normalization;
    float4 color;

    // Create the weights that each neighbor pixel will contribute to the blur.
    weight0 = 1.0f;
    weight1 = 0.9f;
    weight2 = 0.55f;
    weight3 = 0.18f;
    weight4 = 0.1f;

    // Create a normalized value to average the weights out a bit.
    normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

    // Normalize the weights.
    weight0 = weight0 / normalization;
    weight1 = weight1 / normalization;
    weight2 = weight2 / normalization;
    weight3 = weight3 / normalization;
    weight4 = weight4 / normalization;

    // Initialize the color to black.
    color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Add the nine horizontal pixels to the color by the specific weight of each.
    color += shaderTexture.Sample(SampleType, input.texCoord1) * weight4;
    color += shaderTexture.Sample(SampleType, input.texCoord2) * weight3;
    color += shaderTexture.Sample(SampleType, input.texCoord3) * weight2;
    color += shaderTexture.Sample(SampleType, input.texCoord4) * weight1;
    color += shaderTexture.Sample(SampleType, input.texCoord5) * weight0;
    color += shaderTexture.Sample(SampleType, input.texCoord6) * weight1;
    color += shaderTexture.Sample(SampleType, input.texCoord7) * weight2;
    color += shaderTexture.Sample(SampleType, input.texCoord8) * weight3;
    color += shaderTexture.Sample(SampleType, input.texCoord9) * weight4;

    // Set the alpha channel to one.
    color.a = 1.0f;

    return color;
}


PixelInputTypeBlur VerticalBlurVertexShader(VertexInputTypeBlur input)
{
    PixelInputTypeBlur output;
    float texelSize;


    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Determine the floating point size of a texel for a screen with this specific height.
    texelSize = 1.0f / screenHeight;

    // Create UV coordinates for the pixel and its four vertical neighbors on either side.
    output.texCoord1 = input.tex + float2(0.0f, texelSize * -4.0f);
    output.texCoord2 = input.tex + float2(0.0f, texelSize * -3.0f);
    output.texCoord3 = input.tex + float2(0.0f, texelSize * -2.0f);
    output.texCoord4 = input.tex + float2(0.0f, texelSize * -1.0f);
    output.texCoord5 = input.tex + float2(0.0f, texelSize * 0.0f);
    output.texCoord6 = input.tex + float2(0.0f, texelSize * 1.0f);
    output.texCoord7 = input.tex + float2(0.0f, texelSize * 2.0f);
    output.texCoord8 = input.tex + float2(0.0f, texelSize * 3.0f);
    output.texCoord9 = input.tex + float2(0.0f, texelSize * 4.0f);

    return output;
}

float4 VerticalBlurPixelShader(PixelInputTypeBlur input) : SV_TARGET
{
    float weight0, weight1, weight2, weight3, weight4;
    float normalization;
    float4 color;

    // Create the weights that each neighbor pixel will contribute to the blur.
    weight0 = 1.0f;
    weight1 = 0.9f;
    weight2 = 0.55f;
    weight3 = 0.18f;
    weight4 = 0.1f;

    // Create a normalized value to average the weights out a bit.
    normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

    // Normalize the weights.
    weight0 = weight0 / normalization;
    weight1 = weight1 / normalization;
    weight2 = weight2 / normalization;
    weight3 = weight3 / normalization;
    weight4 = weight4 / normalization;

    // Initialize the color to black.
    color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Add the nine vertical pixels to the color by the specific weight of each.
    color += shaderTexture.Sample(SampleType, input.texCoord1) * weight4;
    color += shaderTexture.Sample(SampleType, input.texCoord2) * weight3;
    color += shaderTexture.Sample(SampleType, input.texCoord3) * weight2;
    color += shaderTexture.Sample(SampleType, input.texCoord4) * weight1;
    color += shaderTexture.Sample(SampleType, input.texCoord5) * weight0;
    color += shaderTexture.Sample(SampleType, input.texCoord6) * weight1;
    color += shaderTexture.Sample(SampleType, input.texCoord7) * weight2;
    color += shaderTexture.Sample(SampleType, input.texCoord8) * weight3;
    color += shaderTexture.Sample(SampleType, input.texCoord9) * weight4;

    // Set the alpha channel to one.
    color.a = 1.0f;

    return color;
}

struct PixelInputTypeT
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};
struct VertexInputTypeT
{
    float4 position : POSITION;
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


    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}

// shadow pass
technique11 Pass2
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, ShadowVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, ShadowPixelShader()));
    }
}

// soft pass
technique11 Pass3
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, SoftShadowVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, SoftShadowPixelShader()));
    }
}

// h Blur
technique11 Pass4
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, HorizontalBlurVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, HorizontalBlurPixelShader()));
    }
}

// v Blur
technique11 Pass5
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VerticalBlurVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, VerticalBlurPixelShader()));
    }
}

// texture
technique11 Pass6
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, TextureVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, TexturePixelShader()));
    }
}