#include "BasePlanets.h"

BasePlanets::BasePlanets()
{
}

BasePlanets::~BasePlanets()
{
}

void BasePlanets::Update()
{
}

void BasePlanets::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(m_model, m_mWorld);
}

void BasePlanets::Init()
{
}
