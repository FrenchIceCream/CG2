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
	
	#ifdef BALL
    float dx = input.pos.x - (ConstData.offset.x + 1) * 350;
    float dy = input.pos.y - (700 - (ConstData.offset.y + 1) * 350);	
    if(dx * dx + dy * dy <= 300)
        col =  float4(1.0f, 1.0f, 1.0f, 1.0f);
    else
        col = float4(0.1f, 0.1f, 0.1f, 1.0f);
	#endif

	return col;
}