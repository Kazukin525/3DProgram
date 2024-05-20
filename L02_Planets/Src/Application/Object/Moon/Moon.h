#pragma once
#include "../BasePlanets.h"
class Moon : public BasePlanets
{
public:
	Moon();
	~Moon()	override;

	void Update() override;
	void DrawLit()override;
	void Init()	  override;
private:
};
