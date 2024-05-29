#pragma once
enum class PlanetsType
{
	Earth,
	Moon,
	Sun
};
class BasePlanets : public KdGameObject
{
public:
	BasePlanets();
	virtual ~BasePlanets() override;

	virtual void Update()override;
	void		 DrawLit()override;
	virtual void Init()override;

	PlanetsType GetPlatType() { return m_type; }
protected:
	PlanetsType m_type;
	KdModelData m_model;
	Math::Vector3 m_pos;
	float m_deg = 0.0f;
};
