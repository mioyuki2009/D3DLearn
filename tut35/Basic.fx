cbuffer MatrixBuffer
{
    float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
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

	// First 10% of the depth buffer color red.
    if (depthValue < 0.9f)
    {
        color = float4(1.0, 0.0f, 0.0f, 1.0f);
    }
	
	// The next 0.025% portion of the depth buffer color green.
    if (depthValue > 0.9f)
    {
        color = float4(0.0, 1.0f, 0.0f, 1.0f);
    }

	// The remainder of the depth buffer color blue.
    if (depthValue > 0.925f)
    {
        color = float4(0.0, 0.0f, 1.0f, 1.0f);
    }

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