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
	m_dig += 0.1f;
	if (m_dig >= 360) { m_dig -= 360; }

	Math::Matrix rotMat, transMat;
	rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_dig));
	transMat = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = rotMat * transMat * rotMat;
}

void Earth::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(m_model, m_mWorld);
}

void Earth::Init()
{
	m_type = PlanetsType::Earth;
	m_pos		= { -5.0f, 0.0f , 0};
	m_mWorld	= Math::Matrix::Identity;
	m_dig		= 0;
	m_model.Load("Asset/Data/LessonData/Planets/earth.gltf");
}
