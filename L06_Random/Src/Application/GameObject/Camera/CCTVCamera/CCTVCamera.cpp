#include "CCTVCamera.h"

void CCTVCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	// 注視点
	m_mLocalPos = Math::Matrix::CreateTranslation(0, 6.0f, -10.0f);

	// 一旦行列を確定
	m_mWorld = m_mLocalPos;
}

void CCTVCamera::PostUpdate()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								_targetMat = Math::Matrix::Identity;

	const std::shared_ptr<const KdGameObject>	_spTarget = m_wpTarget.lock();
	if (_spTarget)
	{
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
	}

	// カメラの回転
	UpdateLookAtRotate(_targetMat.Translation());
}

void CCTVCamera::UpdateLookAtRotate(const Math::Vector3& targetPos)
{
	// この状態だと、キャラから見たプレイヤーの行列になる
	Math::Matrix _shadowVP = 
		DirectX::XMMatrixLookAtLH(GetPos(),targetPos,Math::Vector3::Up);

	// カメラから見た行列にするために
	// 逆行列化
	m_mWorld = _shadowVP.Invert();

	// 線形合同法　rand()関数の仕組み

	std::mt19937_64;
}