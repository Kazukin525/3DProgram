#include "TrackingCamera.h"

void TrackingCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	// 基準点（ターゲット）からどれだけ離れているか
	m_mLocalPos = Math::Matrix::CreateTranslation(0.0f,6.f,-5.f);

	// どれだけ傾けているか
	m_mRotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(45));
}

void TrackingCamera::Update()
{
	// ターゲットの行列
	Math::Matrix _targetMat = Math::Matrix::Identity;
	const std::shared_ptr<KdGameObject> m_spTarget = m_wpTarget.lock();
	if (!m_wpTarget.expired())
	{
		_targetMat = Math::Matrix::CreateTranslation(m_spTarget->GetPos());
	}

	// 行列更新
	m_mWorld = m_mRotation * m_mLocalPos * _targetMat;

	// 親クラスの更新呼び出し
	CameraBase::Update();
}
