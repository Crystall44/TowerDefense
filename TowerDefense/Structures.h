#pragma once

struct Enemy {
	short int cost;
	short int hp;
	short int dmg;
	char speed;
};

struct FatherEnemy {
	short int cost;
	short int hp;
	char count;
	char speed;
	Enemy child;
};

struct BossEnemy {
	short int cost;
	short int hp;
	char speed;
};

struct Tower {
	short int hp;
	char lvl;
};

struct TowerDef {
	short int dmg;
	char lvl;
	char range;
};

struct Time {

};

struct Shop {
	int cash;
	char towers[8];
	chat t;
};

struct Round {
	short int num;
	short int money;
};