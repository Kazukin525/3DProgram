// 定数バッファ（メッシュ単位）
// GPUにCPUから情報を渡す

cbuffer cbMesh : register(b1)
{
	// オブジェクト情報
	row_major float4x4 g_mWorld; // ワールド変換行列
};

cbuffer cbMaterial : register(b2)
{
	float4	g_BaseColor;		// ベース色
	float3	g_Emissive;			// 自己発光色
	float	g_Metallic;			// 光沢度
	float	g_Roughness;		// 荒さ
};

// 頂点シェーダから出力するデータ
struct VSOutput
{
	float4 Pos		: SV_Position;
	float2 UV		: TEXCOORD1;
	float4 Color	: TEXCOORD2;
};
