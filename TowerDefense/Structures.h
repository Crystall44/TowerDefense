#pragma once
#include <iostream>
#include <time.h>
#include <windows.h>

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
	Enemy(const char n*, int c, short int h, short int d, char p) {
		setName(n) {strcpy(name, n, 30)};
		setCost(c) {cost = c};
		setHp(h) {hp = h};
		setDmg(d) {dmg = d};
		setPct(p) {pct = p};
		place = -1;
	}
	~Enemy();

	void takeDmg(short int damage) {
		hp -= damage
	}

	bool isAlive() {
		return (hp > 0)
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
	Tower(short int h, char l, short int d) {
		setHp(short int h) { hp = h };
		setLvl(short int l) { lvl = l };
		setDmg(short int d) { dmh = d };
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
		std::cout << "Здоровье = " << hp << ", урон = ", << dmg << ", уровень = " << lvl << "\n";
	}
};

//Структура защищающей башни
class TowerDef {
private:
	short int dmg;
	short int lvl;
	short int range;
public:
	TowerDef(short int d, char l, short int r) {
		setLvl(char l) { lvl = l };
		setDmg(short int d) { dmh = d };
		setRange(short int r) { range = r };
	}
	~TowerDef() {
		dmg = 0;
		lvl = 0;
		range = 0;
	}
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
		std::cout << "Уровень = " << lvl << ", урон = ", << dmg << ", дальность = " << range << "\n";
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
		for (int i = 0; i < 8; i++) {
			towers[i].Del;
		}
	}
	~Map();
	void clear(int pos) {
		place[pos][2] = ' ';
	}
	void towerPlace(int num) {
		place[num * 5 + 2][0] = "T";
	}
	void towerBrake(int num) {
		place[num * 5 + 2][0] = "0";
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

//Структура магазина
class Shop {
private:
	int repairCost;
	int upgradeCost;
	int mainUpgradeCost;
public:
	openShop() {
		repairCost = 20;
		upgradeCost = 10;
		mainUpgradeCost = 30;
	}
	void Info(Tower& tower, int& money, TowerDef& deftower, Map& gameMap) {
		int choice;
		do {
			syctem("cls");
			std::cout << "  ---Магазин---  \n1.Главная башня\n2.Вспомогательные башни\n3.Выход\nВаши средства - " << money <<"\n";
			std::sin >> choice;
			switch (choice) {
			case 1:
				tower.Info;
				do {
					std::cout << "\n1.Восполнить здоровье - " << repairCost << "\n2.Улучшить - " << mainUpgradeCost << "\n3.Выход\n";
					std::sin >> choice;
					switch (choice) {
					case 1:
						if (money >= repairCost) {
							tower.getHp();
							repairCost += 10;
						}
						else std::cout << "Недостаточно средств!";
						break;
					case 2:
						if (money >= mainUpgradeCost && tower.getLvl < 10) {
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
					std::sin >> i;
				} while (i < 1 || i > 8);
				TowerDef* deftow = gameMap.getTower(i);
				deftow->Info;
				do {
					std::cout << "\n1.Купить башню - 10\n2.Улучшить - " << upgradeCost << "\n3.Удалить башню\n4.Выход\n";
					std::sin >> choice;
					switch (choice) {
					case 1:
						if (money >= 10 && deftower.getLvl == 0) {
							gameMap.towerPlace[i - 1];
							deftow->Build;
							money -= 10;
						}
						else if (deftow->getLvl != 0) {
							std::cout << "Эта башня уже построена!";
						}
						else std::cout << "Недостаточно средств!";
						break;
					case 2:
						if (money >= upgradeCost && deftow->getLvl < 10) {
							std::cout << "1.Увеличить урон(+2)\n2.Увеличить дальность(+1)\n3.Выход\n";
							do {
								std::sin >> choice;
							} while (choice < 1 && choice > 3);
							if (choice == 1) {
								deftow->upDmg;
							}
							else if (choice == 2) {
								deftow->upRange;
							}
						}
						break;
					case 3:
						std::cout << "Вы уверены?\n1.Удалить башню\n2.Назад\n";
						std::sin >> choice;
						if (choice == 1) {
							deftow->Del;
							gameMap.towerBrake[i - 1];
						}
						break;
					case 4:break;
					default: std::cout << "Неверный ввод!\n";
					}
				} 
			case 3:break;
			default:std::cout << "Неверный ввод!\n";
			}while (choice != 4);
		}
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
		enemys(nullptr);
		enemysCount = 0;
		money = 20;
		enemyMoney = 10;
	}
	~Game() {
		delete[] enemys;
	}
	void getMoney() {
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
		Enemy newEnemy;
		switch (r) {
		case 1:
			newEnemy = Enemy("Zombe", 4, 100, 10, 'Z');
			break;
		case 2:
			newEnemy = Enemy("Skeleton", 2, 30, 20, 'S');
			break;
		case 3:
			newEnemy = Enemey("Angry Zombe", 6, 75, 25, 'A');
			break;
		}
		if (money >= newEnemy.getCost()) {
			Enemy** newenemys = new Enemey * [enemysCount + 1];
			for (int i = 0; i < enemyCount; ++i) {
				newenemys[i] = enemys[i];
			}
			newenemys[enemyCount] = newEnemy;
			delete[] enemys;
			enemys = newenemys;
			enemysCount++;
			enemyMoney -= newEnemy.getCost;
		}
		else delete newEnemy;
	}
	bool Wave(Tower& mainTower, TowerDef* towers) {
		int k = 0; bool f = 1; int i = 0;
		do {
			if (k != 0) {
				for (i; i < k; i++) {
					for (int j = 0; j < 8; j++) {
						if (towers[j].isNear[enemys[i]->getPlace], j * 5 + 2) {
							enemys[i]->takeDmg(towers[j].getDamage);
						}
						if (mainTower.isNear(enemys[i]->getPlace)) {
							enemys[i]->takeDmg(mainTower.getDamage);
						}
						if (enemys[i]->isAlive) {
							if (enemys[i]->getPlace != 38) {
								enemys[i]->move;
							}
							else {
								mainTower.takeDmg(enemys[k]->getDmg);
							}
						}
						else {
							gameMap.clear(enemys[k]->getPlace);
							delete enemys[k];
							i++;
							money += newEnemy.getCost * 2;
						}
					}
				}
			}
			
			if (k < enemysCount) {
				gameMap.placeEnemy(0, enemys[k]->getPct);
				k++;
				enemys[k]->move;
			}
			i -= enemysCount;
			if (i == enemysCount) f = 0;
			if (mainTower.isAlive != true) f = 0;
			Sleep(2000);
		} while (f);
		if (mainTower.isAlive != true) { 
			std::cout << "Вы проиграли!"; 
			return 0;
		}
		else {
			return 1;
		}
	}
};