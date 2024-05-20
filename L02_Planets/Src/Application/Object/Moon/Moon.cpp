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
		m_dig += 0.3f;
		if (m_dig >= 360) { m_dig -= 360; }
	}
	// 行列更新
	{
	
		for (auto& obj : Application::Instance().GetObjList())
		{
			if (obj->GetPlatType() == PlanetsType::Earth)
			{
				Math::Matrix rotMat,transMat;
				rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_dig));
				transMat = Math::Matrix::CreateTranslation(m_pos);
				m_mWorld = rotMat *  transMat * rotMat * obj->GetMatrix();
			}
		}
	}
}

void Moon::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(m_model, m_mWorld);
}

void Moon::Init()
{
	m_type = PlanetsType::Moon;
	m_pos = {2.0f,0.0f,0.0f};
	m_mWorld = Math::Matrix::Identity;
	m_dig = 0;
	m_model.Load("Asset/Data/LessonData/Planets/moon.gltf");
}
