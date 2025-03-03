struct ConstantData
{
	float4 offset;
};

cbuffer ConstBuf: register(b0)
{
	ConstantData ConstData;
};


struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
	output.pos = input.pos + ConstData.offset;
	output.col = input.col;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = float4(1.f, 1.f, 1.f, 1.f);
#ifdef LEFT
	col = float4(0.f, 0.f, 1.f, 1.f);
#elif RIGHT
	col = float4(1.f, 0.f, 0.f, 1.f);
#endif
	return col;
}