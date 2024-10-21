#pragma once
class Water : public KdGameObject
{
public:
	Water()				{}
	~Water()override	{}

	void DrawLesson()override;
	void Update()override;
	void Init()		override;

private:

	Math::Vector2					m_UVOffset;

	std::shared_ptr<KdModelWork>	m_spModel;
};
