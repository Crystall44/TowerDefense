#pragma once

//��������� �����
struct Enemy {
	int cost;
	short int hp;
	short int dmg;
	char speed;
	char name[30];
	char pct;
};

//��������� �����
struct BossEnemy {
	struct Enemy boss;
};

//��������� �����, ��� ������ ����������� ����� ������ ������
struct FatherEnemy {
	struct Enemy father;
	struct Enemy child;
};

//��������� ������� �����
struct Tower {
	short int hp;
	char lvl;
};

//��������� ���������� �����
struct TowerDef {
	short int dmg;
	char lvl;
	char range;
	short int speed;
};

//��������� �����
struct Map {
	char place[40][3];
	char towerPlace[40];
};

//��������� ��������
struct Shop {
	int cash;
	char towers[8];
};

//��������� ������
struct Round {
	short int num;
	int money;
	bool boss;
	short int spawn[256];

};