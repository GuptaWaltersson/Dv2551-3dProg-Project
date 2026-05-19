cbuffer cameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 projection;
}

struct VSInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 uv : UV;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float2 uv : UV;
};

PSInput main(VSInput input)
{
    PSInput output;

    float4 worldPos =
        float4(
            input.position,
            1);

    output.position =
        mul(worldPos, view);

    output.position =
        mul(
            output.position,
            projection);

    output.color = input.color;
    
    output.uv = input.uv;

    return output;
}