#include "Framework/KdFramework.h"

#include "KdLessonShader.h"

//================================================
// 描画準備
//================================================

bool KdLessonShader::Init()
{
    //-------------------------------------
    // 頂点シェーダ
    //-------------------------------------
    {
        // コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdLessonShader_VS.shaderInc"

        // 頂点シェーダー作成
        if (FAILED(KdDirect3D::Instance().WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS_Lit))) {
            assert(0 && "頂点シェーダー作成失敗");
            Release();
            return false;
        }

        // １頂点の詳細な情報
        std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SKININDEX",0, DXGI_FORMAT_R16G16B16A16_UINT,	    0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SKINWEIGHT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

        // 頂点入力レイアウト作成
        if (FAILED(KdDirect3D::Instance().WorkDev()->CreateInputLayout(
            &layout[0],				// 入力エレメント先頭アドレス
            layout.size(),			// 入力エレメント数
            &compiledBuffer[0],		// 頂点バッファのバイナリデータ
            sizeof(compiledBuffer),	// 上記のバッファサイズ
            &m_inputLayout))
            ) {
            assert(0 && "CreateInputLayout失敗");
            Release();
            return false;
        }

    }

    //-------------------------------------
    // ピクセルシェーダ
    //-------------------------------------
    {
#include "KdLessonShader_PS.shaderInc"

        if (FAILED(KdDirect3D::Instance().WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS_Lit))) {
            assert(0 && "ピクセルシェーダー作成失敗");
            Release();
            return false;
        }
    }

	//-------------------------------------
	// 定数バッファ
	//-------------------------------------

	//-------------------------------------
	// 定数バッファ作成
	//-------------------------------------
	m_cb0_Obj.Create();
	m_cb1_Mesh.Create();
	m_cb2_Material.Create();

	// スキンメッシュ対応
	m_cb3_Bone.Create();

	std::shared_ptr<KdTexture> ds = std::make_shared<KdTexture>();
	ds->CreateDepthStencil(1024, 1024);
	D3D11_VIEWPORT vp = {
		0.0f, 0.0f,
		static_cast<float>(ds->GetWidth()),
		static_cast<float>(ds->GetHeight()),
		0.0f, 1.0f };

	m_depthMapFromLightRTPack.CreateRenderTarget(1024, 1024, true, DXGI_FORMAT_R32_FLOAT);
	m_depthMapFromLightRTPack.ClearTexture(kRedColor);

	SetDissolveTexture(*KdAssets::Instance().m_textures.GetData("Asset/Textures/System/WhiteNoise.png"));


	return true;
}

void KdLessonShader::Release()
{
	KdSafeRelease(m_VS_Lit);
	KdSafeRelease(m_VS_GenDepthFromLight);
	KdSafeRelease(m_VS_UnLit);

	KdSafeRelease(m_inputLayout);

	KdSafeRelease(m_PS_Lit);
	KdSafeRelease(m_PS_GenDepthFromLight);
	KdSafeRelease(m_PS_UnLit);

	m_cb0_Obj.Release();
	m_cb1_Mesh.Release();
	m_cb2_Material.Release();
	// スキンメッシュ対応
	m_cb3_Bone.Release();
}

void KdLessonShader::BeginLit()
{
	// 頂点シェーダーのパイプライン変更
	if (KdShaderManager::Instance().SetVertexShader(m_VS_Lit))
	{
		KdShaderManager::Instance().SetInputLayout(m_inputLayout);

		KdShaderManager::Instance().SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::Instance().SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	// ピクセルシェーダーのパイプライン変更
	if (KdShaderManager::Instance().SetPixelShader(m_PS_Lit))
	{
		KdShaderManager::Instance().SetPSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::Instance().SetPSConstantBuffer(2, m_cb2_Material.GetAddress());
	}

	// ボーン情報をセット(スキンメッシュ対応)
	KdShaderManager::Instance().SetVSConstantBuffer(3, m_cb3_Bone.GetAddress());

	// シャドウマップのテクスチャをセット
	KdDirect3D::Instance().WorkDevContext()->PSSetShaderResources(10, 1, m_depthMapFromLightRTPack.m_RTTexture->WorkSRViewAddress());

	// 通常テクスチャ用サンプラーのセット
	KdShaderManager::Instance().ChangeSamplerState(KdSamplerState::Anisotropic_Wrap, 0);

	// 影ぼかし用の比較機能付きサンプラーのセット
	KdShaderManager::Instance().ChangeSamplerState(KdSamplerState::Linear_Clamp_Cmp, 1);
}

void KdLessonShader::EndLit()
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	KdDirect3D::Instance().WorkDevContext()->PSSetShaderResources(10, 1, &pNullSRV);

}

void KdLessonShader::BeginUnLit()
{
	if (KdShaderManager::Instance().SetVertexShader(m_VS_UnLit))
	{
		KdShaderManager::Instance().SetInputLayout(m_inputLayout);

		KdShaderManager::Instance().SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::Instance().SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	if (KdShaderManager::Instance().SetPixelShader(m_PS_UnLit))
	{
		KdShaderManager::Instance().SetPSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::Instance().SetPSConstantBuffer(2, m_cb2_Material.GetAddress());
	}
}

void KdLessonShader::EndUnLit()
{
}

void KdLessonShader::BeginGenerateDepthMapFromLight()
{
	if (KdShaderManager::Instance().SetVertexShader(m_VS_GenDepthFromLight))
	{
		KdShaderManager::Instance().SetInputLayout(m_inputLayout);

		KdShaderManager::Instance().SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		KdShaderManager::Instance().SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	// ボーン情報をセット(スキンメッシュ対応)
	KdShaderManager::Instance().SetVSConstantBuffer(3, m_cb3_Bone.GetAddress());

	if (KdShaderManager::Instance().SetPixelShader(m_PS_GenDepthFromLight))
	{
		KdShaderManager::Instance().SetPSConstantBuffer(0, m_cb0_Obj.GetAddress());
	}

	m_depthMapFromLightRTPack.ClearTexture(kRedColor);
	m_depthMapFromLightRTChanger.ChangeRenderTarget(m_depthMapFromLightRTPack);

}

void KdLessonShader::EndGenerateDepthMapFromLight()
{
	m_depthMapFromLightRTChanger.UndoRenderTarget();
}

void KdLessonShader::DrawMesh(const KdMesh* mesh, const Math::Matrix& mWorld, const std::vector<KdMaterial>& materials,
                              const Math::Vector4& col, const Math::Vector3& emissive)
{
    if (mesh == nullptr) { return; }

    // メッシュの頂点情報転送
    mesh->SetToDevice();

	// 3Dワールド行列転送
	m_cb1_Mesh.Work().mW = mWorld;	// mWorldが私たちが指定している座標
	m_cb1_Mesh.Write();

    // 全サブセット
    for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
    {
        // 面が１枚も無い場合はスキップ
        if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// マテリアルデータの転送
		const KdMaterial& material = materials[mesh->GetSubsets()[subi].MaterialNo];
		WriteMaterial(material, col, emissive);

        //-----------------------
        // サブセット描画
        //-----------------------
        mesh->DrawSubset(subi);
    }
}

void KdLessonShader::DrawModel(const KdModelData& rModel, const Math::Matrix& mWorld,
                               const Math::Color& colRate, const Math::Vector3& emissive)
{
	// オブジェクト単位の情報転送
	if (m_dirtyCBObj)
	{
		m_cb0_Obj.Write();
	}

	auto& dataNodes = rModel.GetOriginalNodes();

	// 全描画用メッシュノードを描画
	for (auto& nodeIdx : rModel.GetDrawMeshNodeIndices())
	{
		// 描画
		DrawMesh(dataNodes[nodeIdx].m_spMesh.get(), dataNodes[nodeIdx].m_worldTransform * mWorld,
			rModel.GetMaterials(), colRate, emissive);
	}

	// 定数に変更があった場合は自動的に初期状態に戻す
	if (m_dirtyCBObj)
	{
		ResetCBObject();
	}
}

void KdLessonShader::DrawModel(KdModelWork& rModel, const Math::Matrix& mWorld, const Math::Color& colRate, const Math::Vector3& emissive)
{
}

void KdLessonShader::DrawPolygon(const KdPolygon& poly, const Math::Matrix& mWorld, const Math::Color& colRate, const Math::Vector3& emissive)
{
}

void KdLessonShader::DrawVertices(const std::vector<KdPolygon::Vertex>& vertices, const Math::Matrix& mWorld, const Math::Color& colRate)
{
}


// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 描画用マテリアル情報の転送
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// それぞれのマテリアルの影響倍率値とテクスチャを設定
// BaseColor：基本色 / Emissive：自己発光色 / Metalic：金属性(テカテカ) / Roughness：粗さ(材質の色の反映度)
// テクスチャは法線マップ以外は未設定なら白1ピクセルのシステムテクスチャを指定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdLessonShader::WriteMaterial(	const KdMaterial& material, const Math::Vector4& colRate,
									const Math::Vector3& emiRate)
{
	//-----------------------
	// マテリアル情報を定数バッファへ書き込む
	//-----------------------
	m_cb2_Material.Work().BaseColor = material.m_baseColorRate * colRate;
	m_cb2_Material.Work().Emissive = material.m_emissiveRate * emiRate;
	m_cb2_Material.Work().Metallic = material.m_metallicRate;
	m_cb2_Material.Work().Roughness = material.m_roughnessRate;
	m_cb2_Material.Write();

	//-----------------------
	// テクスチャセット
	//-----------------------
	ID3D11ShaderResourceView* srvs[4];

	srvs[0] = material.m_baseColorTex ? material.m_baseColorTex->WorkSRView() : KdDirect3D::Instance().GetWhiteTex()->WorkSRView();
	srvs[1] = material.m_metallicRoughnessTex ? material.m_metallicRoughnessTex->WorkSRView() : KdDirect3D::Instance().GetWhiteTex()->WorkSRView();
	srvs[2] = material.m_emissiveTex ? material.m_emissiveTex->WorkSRView() : KdDirect3D::Instance().GetWhiteTex()->WorkSRView();
	srvs[3] = material.m_normalTex ? material.m_normalTex->WorkSRView() : KdDirect3D::Instance().GetNormalTex()->WorkSRView();

	// セット
	KdDirect3D::Instance().WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);
}

void KdLessonShader::ConvertNormalsFor2D(std::vector<KdPolygon::Vertex>& target, const Math::Matrix& mWorld)
{
	// 平行光の向き
	const Math::Vector3& dirLight_Dir = KdShaderManager::Instance().GetLightCB().DirLight_Dir;

	// どの角度を向いていても表面は常に光の方向を向いている状態：横向きの板ポリが暗くならない対策
	Math::Vector3 normal = Math::Vector3::TransformNormal(-dirLight_Dir, mWorld.Invert());
	Math::Vector3 tangent = (normal != Math::Vector3::Up) ?
		normal.Cross(Math::Vector3::Up) : normal.Cross(Math::Vector3::Right);

	for (size_t i = 0; i < target.size(); ++i)
	{
		target[i].normal = normal;
		target[i].tangent = tangent;
	}
}

void KdLessonShader::ResetCBObject()
{
	m_cb0_Obj.Work() = cbObject();

	m_cb0_Obj.Write();

	m_dirtyCBObj = false;
}
