#include "Moon.h"
#include "../../main.h"

Moon::Moon()
{
}

Moon::~Moon()
{
}

void Moon::Update()
{
	// 角度更新
	{
		m_deg += 3.0f;
		if (m_deg >= 360) { m_deg -= 360; }
	}
	// 行列更新
	{
	
		for (auto& obj : Application::Instance().GetObjList())
		{
			if (obj->GetPlatType() == PlanetsType::Earth)
			{
				Math::Matrix earthTrans = Math::Matrix::CreateTranslation(obj->GetPos());

				Math::Matrix rotMat,transMat;
				rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_deg));
				transMat = Math::Matrix::CreateTranslation(m_pos);
				m_mWorld = rotMat *  transMat * rotMat * earthTrans;
			}
		}
	}
}

void Moon::Init()
{
	m_type = PlanetsType::Moon;
	m_pos = {2.0f,0.0f,0.0f};
	m_mWorld = Math::Matrix::Identity;
	m_deg = 0;
	m_model.Load("Asset/Data/LessonData/Planets/moon.gltf");
}
