#pragma once

class KdLessonShader
{
public:

    //================================================
    // 初期化・解放
    //================================================

    // 初期化
    bool Init();
    // 解放
    void Release();

    // デストラクタで自動で解放するようにする
    ~KdLessonShader() { Release(); }

    //================================================
    // 描画準備
    //================================================
    // 陰影をつけるオブジェクト等を描画する前後に行う
    void BeginLessonShader();
    void EndLessonShader();

    //================================================
    // 描画関数
    //================================================
    // メッシュ描画
    void DrawMesh(const KdMesh* mesh, const Math::Matrix& mWorld, const std::vector<KdMaterial>& materials,
        const Math::Vector4& col, const Math::Vector3& emissive);

    // モデルデータ描画：アニメーションに非対応
    void DrawModel(const KdModelData& rModel, const Math::Matrix& mWorld = Math::Matrix::Identity,
        const Math::Color& colRate = kWhiteColor, const Math::Vector3& emissive = Math::Vector3::Zero);

private:

    ID3D11VertexShader* m_VS            = nullptr;  // 頂点シェーダー
    ID3D11PixelShader*  m_PS            = nullptr;  // ピクセルシェーダー

    ID3D11InputLayout*  m_inputLayout   = nullptr;  // 頂点入力レイアウト

    
};
