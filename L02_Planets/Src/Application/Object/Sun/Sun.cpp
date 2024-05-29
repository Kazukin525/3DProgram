#include "Sun.h"

Sun::Sun()
{
}

Sun::~Sun()
{
}

void Sun::Update()
{
	// 角度更新
	{
		m_dig++;
		if (m_dig >= 360) { m_dig -= 360; }

	}	
	// 行列更新
	{
		Math::Matrix rotMat, transMat;
		rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_dig));
		transMat = Math::Matrix::CreateTranslation(m_pos);

		m_mWorld = rotMat * transMat;
	}
}

void Sun::Init()
{
	m_type = PlanetsType::Sun;
	m_pos = Math::Vector3::Zero;
	m_mWorld = Math::Matrix::Identity;
	m_dig = 0;
	m_model.Load("Asset/Data/LessonData/Planets/sun.gltf");
}
