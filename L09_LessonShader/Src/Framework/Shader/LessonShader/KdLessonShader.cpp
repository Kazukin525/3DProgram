#include "Framework/KdFramework.h"

#include "KdLessonShader.h"

//================================================
// 描画準備
//================================================

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影をつけるオブジェクトの描画の直前処理（不透明な物体やキャラクタの板ポリゴン）
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダーのパイプライン変更
// LitShaderで使用するリソースのバッファー設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdLessonShader::Begin()
{
    // 頂点シェーダーのパイプライン変更
    if (KdShaderManager::Instance().SetVertexShader(m_VS))
    {
        KdShaderManager::Instance().SetInputLayout(m_inputLayout);
    }

    // ピクセルシェーダーのパイプライン変更
    if (KdShaderManager::Instance().SetPixelShader(m_PS))
    {
    }
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影ありオブジェクトの描画修了
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdLessonShader::End()
{
}

bool KdLessonShader::Init()
{
    //-------------------------------------
    // 頂点シェーダ
    //-------------------------------------
    {
        // コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdLessonShader_VS.shaderInc"

        // 頂点シェーダー作成
        if (FAILED(KdDirect3D::Instance().WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
            assert(0 && "頂点シェーダー作成失敗");
            Release();
            return false;
        }

        // １頂点の詳細な情報
        std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,   0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,    0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

        if (FAILED(KdDirect3D::Instance().WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
            assert(0 && "ピクセルシェーダー作成失敗");
            Release();
            return false;
        }
    }

    return true;
}

void KdLessonShader::Release()
{
    KdSafeRelease(m_VS);
    KdSafeRelease(m_PS);
    KdSafeRelease(m_inputLayout);
}

void KdLessonShader::DrawMesh(const KdMesh* mesh, const Math::Matrix& mWorld, const std::vector<KdMaterial>& materials,
                              const Math::Vector4& col, const Math::Vector3& emissive)
{
    if (mesh == nullptr) { return; }

    // メッシュの頂点情報転送
    mesh->SetToDevice();

    // 全サブセット
    for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
    {
        // 面が１枚も無い場合はスキップ
        if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

        //-----------------------
        // サブセット描画
        //-----------------------
        mesh->DrawSubset(subi);
    }
}

void KdLessonShader::DrawModel(const KdModelData& rModel, const Math::Matrix& mWorld,
                               const Math::Color& colRate, const Math::Vector3& emissive)
{
    auto& dataNodes = rModel.GetOriginalNodes();

    // 全描画用メッシュノードを描画
    for (auto& nodeIdx : rModel.GetDrawMeshNodeIndices())
    {
        // 描画
        DrawMesh(dataNodes[nodeIdx].m_spMesh.get(), dataNodes[nodeIdx].m_worldTransform * mWorld,
            rModel.GetMaterials(), colRate, emissive);
    }
}
