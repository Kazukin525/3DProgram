#include "TPSCamera.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	// 基準点（ターゲット）からどれだけ離れているか
	m_mLocalPos = Math::Matrix::CreateTranslation(0.0f, 1.5f, -10.0f);

	// マウスカーソルを画面中央に固定する
	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void TPSCamera::Update()
{
	// ターゲットの行列
	Math::Matrix _targetMat = Math::Matrix::Identity;
	const std::shared_ptr<KdGameObject> m_spTarget = m_wpTarget.lock();
	if (!m_wpTarget.expired())
	{
		_targetMat = Math::Matrix::CreateTranslation(m_spTarget->GetPos());
	}

	// 行列更新
	UpdateRotateByMouse();
	m_mRotation = GetRotationMatrix();
	m_mWorld = m_mLocalPos * m_mRotation* _targetMat;

	// 親クラスの更新呼び出し
	CameraBase::Update();
}