#include "Earth.h"
#include "../../main.h"

Earth::Earth()
{

}

Earth::~Earth()
{

}

void Earth::Update()
{
	m_deg += 0.3f;
	if (m_deg >= 360) { m_deg -= 360; }

	
	for (auto& obj : Application::Instance().GetObjList())
	{
		if (obj->GetPlatType() == PlanetsType::Sun)
		{
			Math::Matrix sunTrans = Math::Matrix::CreateTranslation(obj->GetPos());

			Math::Matrix rotMat, transMat;
			rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_deg));
			transMat = Math::Matrix::CreateTranslation(m_pos);

			m_mWorld = rotMat * transMat * rotMat * sunTrans;
		}
	}
}

void Earth::Init()
{
	m_type = PlanetsType::Earth;
	m_pos		= { -5.0f, 0.0f , 0};
	m_mWorld	= Math::Matrix::Identity;
	m_deg		= 0;
	m_model.Load("Asset/Data/LessonData/Planets/earth.gltf");
}
