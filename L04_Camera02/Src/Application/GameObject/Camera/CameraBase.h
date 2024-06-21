#pragma once

class Terrain;
class CameraBase : public KdGameObject
{
public:
	CameraBase() {}
	virtual ~CameraBase()	override {}

	void Init()				override;
	
	void Update()			override;

	void PreDraw()			override;

	void SetTarget(const std::shared_ptr<KdGameObject>& target) { m_wpTarget = target; }
	
	const Math::Matrix GetRotationMatrix() const
	{
		// 各軸の回転行列を作成してくれる優れモノ
		// Yaw = Y軸の回転 Pitch = X軸回転 Roll = Z軸回転
		return Math::Matrix::CreateFromYawPitchRoll
		(
			DirectX::XMConvertToRadians(m_DegAng.y),
			DirectX::XMConvertToRadians(m_DegAng.x),
			DirectX::XMConvertToRadians(m_DegAng.z)
		);
	}

	const Math::Matrix GetRotationYMatrix()
	{
		return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_DegAng.y));
	}

private:
	// カメラの回転用角度
	Math::Vector3				m_DegAng = Math::Vector3::Zero;

protected:
	void UpdateRotateByMouse();

	std::shared_ptr<KdCamera>	m_spCamera = nullptr;
	std::weak_ptr<KdGameObject> m_wpTarget;

	Math::Matrix				m_mLocalPos = Math::Matrix::Identity;
	Math::Matrix				m_mRotation = Math::Matrix::Identity;

	// カメラ回転用マウス座標
	POINT						m_FixMousePos = {};
};
