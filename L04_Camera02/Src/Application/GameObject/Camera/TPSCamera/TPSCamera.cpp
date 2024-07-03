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
	Math::Matrix _spTargetMat = Math::Matrix::Identity;
	const std::shared_ptr<KdGameObject> _spTarget = m_wpTarget.lock();
	if (!m_wpTarget.expired())
	{
		_spTargetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
	}

	// 行列更新
	UpdateRotateByMouse();
	m_mRotation = GetRotationMatrix();
	m_mWorld = m_mLocalPos * m_mRotation* _spTargetMat;

	// ↓めり込み防止の為の座標補正計算↓
	// ①当たり判定(レイ判定)用の情報を作成
	KdCollider::RayInfo _rayInfo;

	// レイの発射位置
	_rayInfo.m_pos = GetPos();

	// レイの発射方向
	_rayInfo.m_dir = Math::Vector3::Down;

	// レイの長さ
	_rayInfo.m_range = 1000.0f;

	// レイの当たり判定をしたいタイプを設定
	_rayInfo.m_type = KdCollider::Type::TypeGround;

	if (_spTarget)
	{
		Math::Vector3 _targetPos = _spTarget->GetPos();
		
		// キャラクターの足元に座標があるので
		// キャラクターの中央に飛ぶようにする
		_targetPos.y += 0.1f;
		
		_rayInfo.m_dir		= _targetPos - GetPos();
		_rayInfo.m_range	= _rayInfo.m_dir.Length();
		_rayInfo.m_dir.Normalize();
	}

	// ②HIT判定対象オブジェクトに総当たり
	
	for (std::weak_ptr<KdGameObject> wpGameObj : m_wpHitObjectList)
	{
		std::shared_ptr<KdGameObject> _spGameObj = wpGameObj.lock();

		if (_spGameObj)
		{
			// 結果を格納するリスト
			std::list<KdCollider::CollisionResult> _retRayList;

			// 当たり判定を行う
			_spGameObj->Intersects(_rayInfo, &_retRayList);

			// ③結果を利用して座標を保管する
			// レイに当たったリストから一番近いオブジェクトを検出
			float			_maxOverLap = 0.0f;
			Math::Vector3	_hitPos		= {};
			bool			_isHit		= false;

			// 当たっている場合は何かが入っているため
			for (auto& ret : _retRayList)
			{
				//レイを遮断し、オーバーした長さが
				// 一番長いものを探す
				if (_maxOverLap < ret.m_overlapDistance)
				{
					_maxOverLap = ret.m_overlapDistance;
					_hitPos		= ret.m_hitPos;
					_isHit		= true;
				}
			}

			// 何かしらの障害物に当たっている場合
			if (_isHit)
			{
				// 値の決め打ちはよろしくない
				// 地面の当たった部分より上にして地面の中身が見えないようにする
				Math::Vector3 _hoseiPos = _hitPos;
				
				// レイがヒットした場所から少し前に進める
				_hoseiPos += _rayInfo.m_dir * 0.4f;
				
				// 当たった座標をセット
				SetPos(_hitPos);
			}
		}
	}

	// 親クラスの更新呼び出し
	CameraBase::Update();
}