#include "Water.h"

void Water::DrawLesson()
{
	if (!m_spModel) { return; }
	
	KdShaderManager::Instance().m_LessonShader.SetUVOffset(m_UVOffset);	
	KdShaderManager::Instance().m_LessonShader.DrawModel(*m_spModel, m_mWorld);
}

void Water::Update()
{
	m_UVOffset.x += 0.0001f;
	m_UVOffset.y += 0.0001f;

	if (m_UVOffset.x > 1.0f)
	{
		m_UVOffset.x -= 1.0f;
		m_UVOffset.y -= 1.0f;
	}
}

void Water::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/LessonData/Stage/Water/Water.gltf");
	}

	Math::Matrix _transMat = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, 0.3f, 0.0f));
	Math::Matrix _scaleaMat = Math::Matrix::CreateScale(5.0f);

	m_mWorld = _scaleaMat * _transMat;
}
