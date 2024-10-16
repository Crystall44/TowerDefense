#pragma once

struct Enemy {
	short int cost;
	short int hp;
	short int dmg;
	char speed;
	char name[30];
	char father;
	short int child;
	char pct;
};

struct BossEnemy {
	short int cost;
	short int hp;
	char speed;
	char pct;
};

struct Tower {
	short int hp;
	char lvl;
};

struct TowerDef {
	short int dmg;
	char lvl;
	char range;
	char speed;
};

struct Map {
	char place[40][3];
	char towerPlace[40];
};

struct Time {

};

struct Shop {
	int cash;
	char towers[8];
};

struct Round {
	short int num;
	short int money;
	bool boss;
	short int spawn[256];

};