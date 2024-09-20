// カメラやライトのデータが使えるようになる
#include "../inc_KdCommon.hlsli"
#include "inc_KdLessonShader.hlsli"

// テクスチャの受け取り	(画像データ)
Texture2D g_baseTex : register(t0);		// ベースカラーテクスチャ

// サンプラ（テクスチャから情報を抜き出す機能）
SamplerState g_ss : register(s0);		// 通常のテクスチャ描画用

// ピクセルシェーダー 1つ1つのピクセルがやってくる
float4 main(VSOutput In) : SV_TARGET
{
	//	テクスチャのUV座標だったらこの色を返す（UVは0～1の値）
	return g_baseTex.Sample(g_ss,In.UV) * g_BaseColor * In.Color;
}
