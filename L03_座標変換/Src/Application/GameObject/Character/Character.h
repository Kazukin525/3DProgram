#pragma once

class Terrain;
class Character : public KdGameObject
{
public:
	Character() {}
	virtual ~Character()	override {}

	void Init()				override;
	void Update()			override;
	void DrawLit()			override;
	void DrawSprite()		override;

	void SetCamera(std::shared_ptr<KdCamera> _camera)  { m_wpCamera  = _camera; }
	void SetTerrain(std::shared_ptr<Terrain> _terrain) { m_wpTerrain = _terrain; }

private:
	std::shared_ptr<KdSquarePolygon>	m_spPoly = nullptr;

	std::shared_ptr<KdTexture>			m_tex;

	std::weak_ptr<KdCamera>				m_wpCamera;

	std::weak_ptr<Terrain>				m_wpTerrain;

	Math::Vector3						m_circlePos;
};