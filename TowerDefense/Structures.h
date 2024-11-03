#pragma once
#define _CRT_CECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <windows.h>
#include <cstring>
#include <cstdlib>


//Структура врага
class Enemy {
private:
	char name[30];
	int cost;
	short int hp;
	short int dmg;
	char pct;
	int place;
public:
	// Конструктор инициализирует врага
	Enemy() {
		hp = 0;
		cost = 0;
		dmg = 0;
		pct = ' ';
		place = -1;
	}
	~Enemy() {};
	void setName(const char n[]) { strcpy(name, n); }
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

//Структура босса
class BossEnemy :public Enemy {
};

//Структура врага, при смерти вызывающего более слабого врага
class FatherEnemy :public Enemy {
};

//Структура главной башни
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
		std::cout << "Башня разрушена, вы проиграли!" << std::endl;
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
		std::cout << "Здоровье = " << hp << ", урон = " << dmg << ", уровень = " << lvl << "\n";
	}
};

//Структура защищающей башни
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
		std::cout << "Уровень = " << lvl << ", урон = " << dmg << ", дальность = " << range << "\n";
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

//Структура карты
class Map {
private:
	char place[40][3];
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
	~Map() {};
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
};

//Структура магазина
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
	void Info(Tower& tower, int& money, TowerDef deftowers[], Map& gameMap) {
		int choice;
		do {
			system("cls");
			std::cout << "  ---Магазин---  \n1.Главная башня\n2.Вспомогательные башни\n3.Выход\nВаши средства - " << money << "\n";
			std::cin >> choice;
			switch (choice) {
			case 1:
				tower.Info();
				do {
					std::cout << "\n1.Восполнить здоровье - " << repairCost << "\n2.Улучшить - " << mainUpgradeCost << "\n3.Выход\n";
					std::cin >> choice;
					switch (choice) {
					case 1:
						if (money >= repairCost) {
							tower.getHp();
							repairCost += 10;
						}
						else std::cout << "Недостаточно средств!";
						break;
					case 2:
						if (money >= mainUpgradeCost && tower.getLvl() < 10) {
							tower.upDmg();
							money -= mainUpgradeCost;
							mainUpgradeCost += 20;
						}
						else if (money < mainUpgradeCost) {
							std::cout << "Недостаточно средств!";
						}
						else std::cout << "Главная башня прокачана на максимум.";
						break;
					case 3:
						break;
					default:
						std::cout << "Неверный ввод.";
					}
				} while (choice != 3);
				break;
			case 2:
				int i;
				do {
					std::cout << "Выберете башню(1-8):";
					std::cin >> i;
				} while (i < 1 || i > 8);
				TowerDef& deftow = deftowers[i - 1];
				deftow.Info();
				do {
					std::cout << "\n1.Купить башню - 10\n2.Улучшить - " << upgradeCost << "\n3.Удалить башню\n4.Выход\n";
					std::cin >> choice;
					switch (choice) {
					case 1:
						if (money >= 10 && deftow.getLvl() == 0) {
							gameMap.towerPlace(i - 1);
							deftow.Build();
							money -= 10;
						}
						else if (deftow.getLvl() != 0) {
							std::cout << "Эта башня уже построена!";
						}
						else std::cout << "Недостаточно средств!";
						break;
					case 2:
						if (money >= upgradeCost && deftow.getLvl() < 10) {
							std::cout << "1.Увеличить урон(+2)\n2.Увеличить дальность(+1)\n3.Выход\n";
							do {
								std::cin >> choice;
							} while (choice < 1 && choice > 3);
							if (choice == 1) {
								deftow.upDmg();
								money -= upgradeCost;
							}
							else if (choice == 2) {
								deftow.upRange();
								money -= upgradeCost;
							}
						}
						break;
					case 3:
						std::cout << "Вы уверены?\n1.Удалить башню\n2.Назад\n";
						std::cin >> choice;
						if (choice == 1) {
							deftow.Del();
							gameMap.towerBrake(i - 1);
						}
						break;
					case 4:break;
					default: std::cout << "Неверный ввод!\n";
					}
				} while (choice != 4);
			case 3:break;
			default:std::cout << "Неверный ввод!\n";
			}
		} while (choice != 4);
	}
};

class Game {
public:
	int money;
	int enemysCount;
	int enemyMoney;
	Enemy** enemys;
public:
	Game() {
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
	void buyEnemy() {
		int r = rand() % 3;
		Enemy* newEnemy = new Enemy;
		switch (r) {
		case 1:
			newEnemy->setName("Zombe");
			newEnemy->setCost(4);
			newEnemy->setHp(100);
			newEnemy->setDmg(10);
			newEnemy->setPct('Z');
			break;
		case 2:
			newEnemy->setName("Skeleton");
			newEnemy->setCost(2);
			newEnemy->setHp(30);
			newEnemy->setDmg(20);
			newEnemy->setPct('S');
			break;
		case 3:
			newEnemy->setName("Angry Zombe");
			newEnemy->setCost(6);
			newEnemy->setHp(75);
			newEnemy->setDmg(25);
			newEnemy->setPct('A');
			break;
		}
		if (enemyMoney >= newEnemy->getCost()) {
			Enemy** newenemys = new Enemy * [enemysCount + 1];
			for (int i = 0; i < enemysCount; ++i) {
				newenemys[i] = enemys[i];
			}
			newenemys[enemysCount] = newEnemy;
			delete[] enemys;
			enemys = newenemys;
			enemysCount++;
			enemyMoney -= newEnemy->getCost();
		}
		else delete newEnemy;
	}
	bool Wave(Tower& mainTower, TowerDef* towers, Map& gameMap) {
		int k = 0; bool f = 1; int i = 0; int chek = enemysCount;
		do {
			if (k < chek) {
				gameMap.placeEnemy(0, enemys[k]->getPct());
				k++;
				enemys[k]->move();
			}
			for (i = 0; i < enemysCount; i++) {
				for (int j = 0; j < 8; j++) {
					for (i = 0; i < enemysCount; i++) {
						if (enemys[i]->isAlive()) {
							if (towers[j].isNear(enemys[i]->getPlace(), j)) {
								enemys[i]->takeDmg(towers[j].getDamage());
							}
						}
						if (enemys[i]->isAlive()) {
							if (mainTower.isNear(enemys[i]->getPlace())) {
								enemys[i]->takeDmg(mainTower.getDamage());
							}
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
							enemysCount--;
						}
					}
				}
			}
			gameMap.printMap();
			Sleep(2000);
			if (enemysCount == 0) f = 0;
			if (mainTower.isAlive() != true) f = 0;
		} while (f);
		if (mainTower.isAlive() != true) { 
			std::cout << "Вы проиграли!"; 
			return 0;
		}
		else {
			earnMoney();
			upenemys();
			return 1;
		}
	}
};