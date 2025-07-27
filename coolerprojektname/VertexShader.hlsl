struct vSOut
{
    float3 color : Color;
    float4 pos : SV_Position;
};

cbuffer cBuf
{
    matrix transform;
};

vSOut main( float3 pos : Position, float3 color : Color )
{
    vSOut vso;
    vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), transform);
    vso.color = color;
    return vso;
}