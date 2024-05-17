#include "HamuHamu.h"

HamuHamu::HamuHamu()
{

}

HamuHamu::~HamuHamu()
{

}


void HamuHamu::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Data/LessonData/Character/Hamu.png");
	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
}

void HamuHamu::Update()
{
	// ハム太郎更新
	{
		float			moveSpd = 0.05f;
		Math::Vector3	nowPos = m_mWorld.Translation();

		// ベクトル(方向ベクトル) = 必ず「長さ（力）」が1でなければならない

		// ベクトルとは、矢印である
		Math::Vector3	moveVec = Math::Vector3::Zero;

		//float len = moveVec.Length();	 // ベクトルの長さを返す
		//moveVec.Normalize();			 // ベクトルの長さを1にしてくれる関数

		if (GetAsyncKeyState('W') & 0x8000)
		{
			moveVec.z = 0.5f;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			moveVec.x = -0.5f;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			moveVec.z = -0.5f;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			moveVec.x = 0.5f;
		}

		moveVec.Normalize();
		moveVec *= moveSpd;
		nowPos += moveVec;
		// キャラクターのワールド行列を作成
		m_mWorld = Math::Matrix::CreateTranslation(nowPos);
	};
}

void HamuHamu::DrawLit()
{
	//Math::Matrix _mat = Math::Matrix::CreateTranslation(0, 0, m_zPos);
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
	
}
