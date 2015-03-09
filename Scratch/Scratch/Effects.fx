cbuffer cbPerObject
{
	float4x4 WVP;
};

struct VS_OUTPUT
{
	float4 Pos:SV_POSITION;
	float4 Color:COLOR;
};


VS_OUTPUT VS( float4 pos : POSITION,float4 color:COLOR )
{
	VS_OUTPUT output;
	output.Pos = mul(pos, WVP);
	output.Color = color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return input.Color;
}