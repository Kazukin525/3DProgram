#include "Character.h"
#include "../Terrain/Terrain.h"
#include "../../main.h"

void Character::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial("Asset/Data/LessonData/Character/Hamu.png");
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	}

	// 小さい点の初期化
	m_circlePos = {};

	// テクスチャー
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load("Asset/Textures/PlayLogo.png");
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
	
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// 2D座標を3D座標に(クリックした座標に配置)
		{
			POINT mouse;
			// マウス座標を補正
			{
				GetCursorPos(&mouse);
				ScreenToClient(Application::Instance().GetWindowHandle(), &mouse);
			}

			// レイ情報
			KdCollider::RayInfo rayInfo;
			m_wpCamera.lock()->GenerateRayInfoFromClientPos(mouse, rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

			rayInfo.m_type = KdCollider::Type::TypeGround;

			// レイに当たったオブジェクト情報	(当たったオブジェクトの情報を格納)
			std::list<KdCollider::CollisionResult> retRayList;

			m_wpTerrain.lock()->Intersects(rayInfo, &retRayList);

			// レイリストから一番近いオブジェクトを検出

			float maxOverLap = 0;	// はみ出たレイの長さ
			Math::Vector3 hitPos;	// レイが遮断された座標（当たった座標）
			bool isHit = false;		// 当たっていたらtrue

			// ret -> result
			for (auto& ret : retRayList)
			{
				// 一番近い地点を検出
				if (maxOverLap < ret.m_overlapDistance)
				{
					maxOverLap = ret.m_overlapDistance;
					hitPos = ret.m_hitPos;
					isHit = true;
				}
			}
			if (isHit) // 当たっている場合
			{
				nowPos = hitPos;
			}
		}
	}
	moveVec.Normalize();
	moveVec *= moveSpd;
	nowPos	+= moveVec;

	// キャラクターのワールド行列を創る処理
	m_mWorld = Math::Matrix::CreateTranslation(nowPos);


	// 点の座標返還
	if (m_wpCamera.expired())
	{
		return;
	}

	// 3D座標を2D座標に
	m_wpCamera.lock()->ConvertWorldToScreenDetail(GetPos(), m_circlePos);
}

void Character::DrawLit()
{
	if (!m_spPoly) return;

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Character::DrawSprite()
{
	Math::Rectangle rect = { 0,0,194,62 };
	Math::Color color = { 1,1,1,1 };
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex,m_circlePos.x,m_circlePos.y + 100.0f,&rect,&color);
}
