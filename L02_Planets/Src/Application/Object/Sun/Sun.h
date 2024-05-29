#pragma once
#include "../BasePlanets.h"
class Sun : public BasePlanets
{
public:
	Sun();
	~Sun()	override;

	void Update() override;
	void Init()	  override;
private:
};
