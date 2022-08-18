#pragma pack_matrix(column_major)
#define RootSig "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)," \
				"DescriptorTable(CBV(b0, numDescriptors = 1)), "\
				"DescriptorTable(SRV(t0, numDescriptors = 1)),"\
				"StaticSampler(s0, " \
                             "addressU = TEXTURE_ADDRESS_CLAMP, " \
                             "filter = FILTER_MIN_MAG_MIP_LINEAR )"


cbuffer ConstantBuffer : register(b0)
{
	float4x4 worldViewProjection;
};

Texture2D<float4> albedo : register( t0 );
SamplerState LinearClampSampler : register( s0 );

struct AppData
{
	float3 vertex	: POSITION;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float4 color	: COLOR;
	float2 texCoord : TEXCOORD;
};

struct v2f
{
	float4 vertex	: SV_POSITION;
	float3 normal	: TEXCOORD0;
	float4 tangent	: TANGENT;
	float4 color	: COLOR;
	float2 texCoord : TEXCOORD1;
};

// Basic root signature for just input layout bindings
[RootSignature(RootSig)]
v2f vert(AppData v)
{
	v2f o;
	o.vertex = mul(worldViewProjection, float4(v.vertex, 1));
	o.normal = v.normal;
	o.color  = v.color;
	o.texCoord = v.texCoord;
	
	return o;
}

float4 frag(v2f i) : SV_TARGET
{
	return albedo.Sample(LinearClampSampler, i.texCoord);// * dot(i.normal, float3(0,0.7074,0.7074));
}

