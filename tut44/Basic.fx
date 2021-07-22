cbuffer MatrixBuffer
{
    float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
    
    float4x4 viewMatrix2;
    float4x4 projectionMatrix2;
};

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightPosition;
    float padding;
};

Texture2D shaderTexture;
Texture2D projectionTexture;
SamplerState SampleType;

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
    
    float4 viewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
};

PixelInputType ProjectionVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the position of the vertice as viewed by the projection view point in a separate variable.
    output.viewPosition = mul(input.position, worldMatrix);
    output.viewPosition = mul(output.viewPosition, viewMatrix2);
    output.viewPosition = mul(output.viewPosition, projectionMatrix2);

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


float4 ProjectionPixelShader(PixelInputType input) : SV_TARGET
{
    float brightness;
    float4 color = float4(0, 0, 0, 0);
    float lightIntensity;
    float4 textureColor;
    float2 projectTexCoord;
    float4 projectionColor;
    
     // Set brightness of the light.
    brightness = 1.5f;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, input.lightPos));

    if (lightIntensity > 0.0f)
    {
        // Determine the light color based on the diffuse color and the amount of light intensity.
        color = (diffuseColor * lightIntensity) * brightness;
    }

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

     // Calculate the projected texture coordinates.
    projectTexCoord.x = input.viewPosition.x / input.viewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.viewPosition.y / input.viewPosition.w / 2.0f + 0.5f;

    // Determine if the projected coordinates are in the 0 to 1 range.  If it is then this pixel is inside the projected view port.
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        // Sample the color value from the projection texture using the sampler at the projected texture coordinate location.
        projectionColor = projectionTexture.Sample(SampleType, projectTexCoord);
         // Set the output color of this pixel to the projection texture overriding the regular color value.
        color = saturate((color * projectionColor * textureColor) + (ambientColor * textureColor));
    }
    else
    {
        color = ambientColor * textureColor;
    }
    
    return color;
}


technique11 Pass1
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, ProjectionVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, ProjectionPixelShader()));
    }
}