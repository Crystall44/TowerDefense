#pragma once
#include <iostream>
#include <time.h>
#include <windows.h>
#include <cstring>
#include <cstdlib>


//��������� �����
class Enemy {
private:
	char name[30];
	int cost;
	short int hp;
	short int dmg;
	char pct;
	int place;
public:
	// ����������� �������������� �����
	Enemy() {
	}
	void NewEnemy(char n[], int c, short int h, short int d, char p) {
		void setName(char n[]);
		void setCost(int c);
		void setHp(int h);
		void setDmg(int d);
		void setPct(char p);
		place = -1;
	}
	~Enemy();
	void setName(char n[]) { strcpy(name, n); }
	void setHp(int h) { hp = h; }
	void setCost(int c) { cost = c; }
	void setDmg(int d) { dmg = d; }
	void setPct(char p) { pct = p; }
	void takeDmg(short int damage) {
		hp -= damage;
	}

	bool isAlive() {
		return (hp > 0);
	}

	char getPct() {
		return pct;
	}

	void move() {
		place++;
	}

	short int getDmg() {
		return dmg;
	}

	int getCost() {
		return cost;
	}
	int getPlace() {
		return place;
	}
};

//��������� �����
class BossEnemy :public Enemy {
};

//��������� �����, ��� ������ ����������� ����� ������� �����
class FatherEnemy :public Enemy {
};

//��������� ������� �����
class Tower {
private:
	short int hp;
	short int lvl;
	short int dmg;
public:
	Tower() {
		hp = 3000;
		lvl = 1;
		dmg = 20;
	}
	~Tower() {
		std::cout << "����� ���������, �� ���������!" << std::endl;
	};
	void takeDmg(short int damage) {
		hp -= damage;
	}
	bool isAlive() {
		return (hp > 0);
	}
	short int getDamage() {
		return dmg;
	}
	short int getLvl() {
		return lvl;
	}
	short int getHp() {
		return hp;
	}
	bool isNear(int enemyPlace) {
		return (enemyPlace >= 36);
	}
	void repair() {
		hp += 500;
		if (hp > 3000) hp = 3000;
	}
	void upDmg() {
		lvl++;
		dmg += 20;
	}
	void Info() {
		std::cout << "�������� = " << hp << ", ���� = " << dmg << ", ������� = " << lvl << "\n";
	}
};

//��������� ���������� �����
class TowerDef {
private:
	short int dmg;
	short int lvl;
	short int range;
public:
	TowerDef() {
		dmg = 0;
		lvl = 0;
		range = 0;
	}
	~TowerDef() {
		dmg = 0;
		lvl = 0;
		range = 0;
	}
	void setLvl(char l) { lvl = l; }
	void setDmg(short int d) { dmg = d; }
	void setRange(short int r) { range = r; }
	short int getDamage() {
		return dmg;
	}
	char getLvl() {
		return lvl;
	}
	bool isNear(int enemyPlace, int towerPlace) {
		return (enemyPlace >= (towerPlace - range) && enemyPlace <= (towerPlace + range));
	}
	void upDmg() {
		lvl++;
		dmg += 2;
	}
	void upRange() {
		lvl++;
		range += 1;
	}
	void Info() {
		std::cout << "������� = " << lvl << ", ���� = " << dmg << ", ��������� = " << range << "\n";
	}
	void Build() {
		lvl = 1;
		range = 1;
		dmg = 2;
	}
	void Del() {
		lvl = 0;
		range = 0;
		dmg = 0;
	}
};

//��������� �����
class Map {
private:
	char place[40][3];
	TowerDef towers[8];
public:
	Map() {
		for (int i = 0; i < 40; i++) {
			place[i][0] = '-';
			place[i][1] = ' ';
			place[i][2] = '-';
		}
		for (int i = 2; i < 40; i += 5) {
			place[i][0] = '0';
		}
		place[39][1] = '9';
	}
	~Map();
	void clear(int pos) {
		place[pos][2] = ' ';
	}
	void towerPlace(int num) {
		place[num * 5 + 2][0] = 'T';
	}
	void towerBrake(int num) {
		place[num * 5 + 2][0] = '0';
	}
	void placeEnemy(int pos, char pct) {
		place[pos][1] = pct;
	}
	void printMap() {
		system("cls");
		for (int i = 0; i < 3; i++) {
			for (int j; j < 40; j++) {
				std::cout << place[j][i];
			}
			std::cout << std::endl;
		}
	}
	TowerDef* getTower(int i) {
		return &towers[i - 1];
	}
};

//��������� ��������
class Shop {
private:
	int repairCost;
	int upgradeCost;
	int mainUpgradeCost;
public:
	void openShop() {
		repairCost = 20;
		upgradeCost = 10;
		mainUpgradeCost = 30;
	}
	void Info(Tower& tower, int& money, TowerDef& deftower, Map& gameMap) {
		int choice;
		do {
			system("cls");
			std::cout << "  ---�������---  \n1.������� �����\n2.��������������� �����\n3.�����\n���� �������� - " << money << "\n";
			std::cin >> choice;
			switch (choice) {
			case 1:
				tower.Info();
				do {
					std::cout << "\n1.���������� �������� - " << repairCost << "\n2.�������� - " << mainUpgradeCost << "\n3.�����\n";
					std::cin >> choice;
					switch (choice) {
					case 1:
						if (money >= repairCost) {
							tower.getHp();
							repairCost += 10;
						}
						else std::cout << "������������ �������!";
						break;
					case 2:
						if (money >= mainUpgradeCost && tower.getLvl() < 10) {
							tower.upDmg();
							money -= mainUpgradeCost;
							mainUpgradeCost += 20;
						}
						else if (money < mainUpgradeCost) {
							std::cout << "������������ �������!";
						}
						else std::cout << "������� ����� ��������� �� ��������.";
						break;
					case 3:
						break;
					default:
						std::cout << "�������� ����.";
					}
				} while (choice != 3);
				break;
			case 2:
				int i;
				do {
					std::cout << "�������� �����(1-8):";
					std::cin >> i;
				} while (i < 1 || i > 8);
				TowerDef* deftow = gameMap.getTower(i);
				deftow->Info();
				do {
					std::cout << "\n1.������ ����� - 10\n2.�������� - " << upgradeCost << "\n3.������� �����\n4.�����\n";
					std::cin >> choice;
					switch (choice) {
					case 1:
						if (money >= 10 && deftower.getLvl() == 0) {
							gameMap.towerPlace(i - 1);
							deftow->Build();
							money -= 10;
						}
						else if (deftow->getLvl() != 0) {
							std::cout << "��� ����� ��� ���������!";
						}
						else std::cout << "������������ �������!";
						break;
					case 2:
						if (money >= upgradeCost && deftow->getLvl() < 10) {
							std::cout << "1.��������� ����(+2)\n2.��������� ���������(+1)\n3.�����\n";
							do {
								std::cin >> choice;
							} while (choice < 1 && choice > 3);
							if (choice == 1) {
								deftow->upDmg();
							}
							else if (choice == 2) {
								deftow->upRange();
							}
						}
						break;
					case 3:
						std::cout << "�� �������?\n1.������� �����\n2.�����\n";
						std::cin >> choice;
						if (choice == 1) {
							deftow->Del();
							gameMap.towerBrake(i - 1);
						}
						break;
					case 4:break;
					default: std::cout << "�������� ����!\n";
					}
				} while (choice != 4);
			case 3:break;
			default:std::cout << "�������� ����!\n";
			}
		} while (choice != 4);
	}
};

class Game {
private:
	Tower mainTower;
	Map gameMap;
	Shop shop;
	Enemy** enemys;
	int money;
	int enemysCount;
	int enemyMoney;
public:
	Game() {
		enemys = nullptr;
		enemysCount = 0;
		money = 20;
		enemyMoney = 10;
	}
	~Game() {
		delete[] enemys;
	}
	int getMoney() {
		return money;
	}
	void earnMoney(int n) {
		money += n;
	}
	void upenemys() {
		enemyMoney += 10;
	}
	void buyEnemy(Enemy*& enemies, int& enemyCount) {
		int r = rand() % 3;
		Enemy newEn;
		switch (r) {
		case 1:
			newEn.NewEnemy("Zombe", 4, 100, 10, 'Z');
			break;
		case 2:
			newEn.NewEnemy("Skeleton", 2, 30, 20, 'S');
			break;
		case 3:
			newEn.NewEnemy("Angry Zombe", 6, 75, 25, 'A');
			break;
		}
		if (money >= newEn.getCost()) {
			Enemy** newenemys = new Enemy * [enemysCount + 1];
			for (int i = 0; i < enemyCount; ++i) {
				newenemys[i] = enemys[i];
			}
			newenemys[enemyCount] = &newEn;
			delete[] enemys;
			enemys = newenemys;
			enemysCount++;
			enemyMoney -= newEn.getCost();
		}
		else delete newEn;
	}
	bool Wave(Tower& mainTower, TowerDef* towers) {
		int k = 0; bool f = 1; int i = 0;
		do {
			if (k != 0) {
				for (i; i < k; i++) {
					for (int j = 0; j < 8; j++) {
						if (towers[j].isNear(enemys[i]->getPlace(), j * 5 + 2)) {
							enemys[i]->takeDmg(towers[j].getDamage());
						}
						if (mainTower.isNear(enemys[i]->getPlace())) {
							enemys[i]->takeDmg(mainTower.getDamage());
						}
						if (enemys[i]->isAlive()) {
							if (enemys[i]->getPlace() != 38) {
								enemys[i]->move();
							}
							else {
								mainTower.takeDmg(enemys[i]->getDmg());
							}
						}
						else {
							gameMap.clear(enemys[i]->getPlace());
							money += enemys[i]->getCost() * 2;
							delete enemys[i];
							i++;
						}
					}
				}
			}
			
			if (k < enemysCount) {
				gameMap.placeEnemy(0, enemys[k]->getPct());
				k++;
				enemys[k]->move();
			}
			i -= enemysCount;
			if (i == enemysCount) f = 0;
			if (mainTower.isAlive() != true) f = 0;
			Sleep(2000);
		} while (f);
		if (mainTower.isAlive() != true) { 
			std::cout << "�� ���������!"; 
			return 0;
		}
		else {
			return 1;
		}
	}
};