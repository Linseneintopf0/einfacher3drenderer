float4 main(float3 color : Color) : SV_TARGET
{
	return float4(color, 0.5f);
}