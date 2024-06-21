#include "../Camera/CameraBase.h"

#include "Character.h"

#include "../../main.h"

void Character::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial("Asset/Data/LessonData/Character/Hamu.png");
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	}
}

void Character::Update()
{
	// キャラクターの移動速度(真似しちゃダメですよ)
	float moveSpd = 0.05f;
	Math::Vector3 nowPos = m_mWorld.Translation();

	Math::Vector3 moveVec = Math::Vector3::Zero;
	if (GetAsyncKeyState('D')) { moveVec.x = 1.0f; }
	if (GetAsyncKeyState('A')) { moveVec.x = -1.0f; }
	if (GetAsyncKeyState('W')) { moveVec.z = 1.0f; }
	if (GetAsyncKeyState('S')) { moveVec.z = -1.0f; }

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		moveSpd = 0.1f;
	}
	else { moveSpd = 0.05f; }

	const std::shared_ptr<CameraBase> _spCamera = m_wpCamera.lock();
	if (_spCamera)
	{
		// 
		moveVec =
			moveVec.TransformNormal(moveVec,_spCamera->GetRotationYMatrix());
	}

	moveVec.Normalize();
	moveVec *= moveSpd;
	nowPos	+= moveVec;

	// キャラクターの回転角度を計算
	UpdateRotate(moveVec);

	// キャラクターのワールド行列を創る処理
	Math::Matrix _rotation = 
		Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_WorldRot.y));
	m_mWorld = _rotation * Math::Matrix::CreateTranslation(nowPos);
}

void Character::DrawLit()
{
	if (!m_spPoly) return;

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Character::UpdateRotate(const Math::Vector3& srcMoveVec)
{
	// 何も入力が無い場合は処理しない
	// ベクトルが0ではない場合は
	if (srcMoveVec.LengthSquared() <= 0.0f) { return; }

	// キャラの正面方向のベクトル(正面のベクトルを取得できる)
	// キャラの前方向ベクトルは Forward　企業はこちら

	// 正面方向
	Math::Vector3 _nowDir		= GetMatrix().Backward();
	_nowDir.Normalize();

	// 移動方向のベクトル
	Math::Vector3 _targetDir	= srcMoveVec;
	_targetDir.Normalize();

	float _nowAng = atan2(_nowDir.x, _nowDir.z);
	_nowAng = DirectX::XMConvertToDegrees(_nowAng);

	float _targetAng = atan2(_targetDir.x, _targetDir.z);
	_targetAng = DirectX::XMConvertToDegrees(_targetAng);

	// 角度の差分を求める
	float _betweenAng = _targetAng - _nowAng;
	
	// 角度を補正する
	if (_betweenAng > 180)
	{
		_betweenAng -= 360;
	}
	else if (_betweenAng < -180)
	{
		_betweenAng += 360;
	}
	
	// clamp -> 最小値 -8.0f この間の数字はそのまま返される 最大値 8.0f
	float _rotateAng = std::clamp(_betweenAng, -8.0f, 8.0f);
	m_WorldRot.y += _rotateAng;
}