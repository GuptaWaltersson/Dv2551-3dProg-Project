cbuffer Camera : register(b0)
{
    float4x4 viewprojection;
};

cbuffer camPos : register(b1)
{
    float4 position;
};

struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
};

[maxvertexcount(6)]
void main(point float4 input[1] : POSITION, inout TriangleStream<GeometryShaderOutput> output)
{
    
    const static float SIZE = 0.05f;
    
    float3 frontVec = input[0].xyz - position.xyz;
    float3 tempUp = { 0, 1, 0 };
    float3 rightVec = normalize(cross(frontVec, tempUp));
    float3 upVec = normalize(cross(frontVec, rightVec) * SIZE);
    
    float3 topLeft = input[0].xyz - rightVec * SIZE + upVec * SIZE;
    float3 bottomLeft = input[0].xyz - rightVec * SIZE - upVec * SIZE;
    float3 topRight = input[0].xyz + rightVec * SIZE + upVec * SIZE;
    float3 bottomRight = input[0].xyz + rightVec * SIZE - upVec * SIZE;

    GeometryShaderOutput toAppend;
    toAppend.position = mul(float4(topLeft, 1.0f), viewprojection);
    output.Append(toAppend);
    toAppend.position = mul(float4(bottomRight, 1.0f), viewprojection);
    output.Append(toAppend);
    toAppend.position = mul(float4(bottomLeft, 1.0f), viewprojection);
    output.Append(toAppend);
    output.RestartStrip(); // First triangle done
 
    toAppend.position = mul(float4(topLeft, 1.0f), viewprojection);
    output.Append(toAppend);
    toAppend.position = mul(float4(topRight, 1.0f), viewprojection);
    output.Append(toAppend);
    toAppend.position = mul(float4(bottomRight, 1.0f), viewprojection);
    output.Append(toAppend);
    output.RestartStrip(); // Second Triangle done
    
}