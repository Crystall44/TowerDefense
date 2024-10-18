#pragma once

//Структура врага
struct Enemy {
	int cost;
	short int hp;
	short int dmg;
	char speed;
	char name[30];
	char pct;
};

//Структура босса
struct BossEnemy {
	struct Enemy boss;
};

//Структура врага, при смерти вызывающего более слабых врагов
struct FatherEnemy {
	struct Enemy father;
	struct Enemy child;
};

//Структура главной башни
struct Tower {
	short int hp;
	char lvl;
};

//Структура защищающей башни
struct TowerDef {
	short int dmg;
	char lvl;
	char range;
	short int speed;
};

//Структура карты
struct Map {
	char place[40][3];
	char towerPlace[40];
};

//Структура магазина
struct Shop {
	int cash;
	char towers[8];
};

//Структура раунда
struct Round {
	short int num;
	int money;
	bool boss;
	short int spawn[256];

};