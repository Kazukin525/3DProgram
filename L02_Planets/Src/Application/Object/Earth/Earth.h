#pragma once
#include "../BasePlanets.h"
class Earth : public BasePlanets
{
public:
	Earth();
	~Earth()	override;

	void Update() override;
	void Init()	  override;
private:
};
