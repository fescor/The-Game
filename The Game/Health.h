
#pragma once

struct Health
{
	int hp = 0;

	void changeHP(int i ) {
		if (hp + i   >  5) {
			hp = 5;
			return;
		}
		if (hp  + i <= 0) {
			hp = 0;
			return;
		}
		hp += i;
	}
	int returnHP() {
		return hp;
	}



	Health() {}

	Health(int i) : hp(i) {}
		
	

};