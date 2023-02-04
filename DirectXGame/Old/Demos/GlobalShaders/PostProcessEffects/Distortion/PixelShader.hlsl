Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

cbuffer Constant : register(b0)
{
	float distortion_level;
};

float2 distort(float2 p)
{
	float theta = atan2(p.y, p.x);
	float radius = length(p);

	radius = pow(radius, distortion_level);

	p.x = radius * cos(theta);
	p.y = radius * sin(theta);

	//Move range in [-1,1]
	return 0.5 * (p + 1.0);
}

float4 main(PS_INPUT input) : SV_TARGET
{
	//Move range in [-1,1]
	float2 xy = 2.0 * input.texcoord.xy - 1.0;
	float2 uv;

	//Length of point texture coordinate
	float d = length(xy);


	if(d < 1.0)
	{
		//Inside circle
		uv = distort(xy);
	}
	else
	{
		//Outside circle
		uv = input.texcoord.xy;
	}

	return Texture.Sample(TextureSampler, uv);
}