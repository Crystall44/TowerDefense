#pragma once
#define _CRT_CECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <windows.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>


//��������� �����
class Enemy {
private:
	std::string name;
	int cost;
	short int hp;
	short int dmg;
	char pct;
	int place;
public:
	// ����������� �������������� �����
	Enemy() {
		hp = 0;
		cost = 0;
		dmg = 0;
		pct = ' ';
		place = -1;
	}
	~Enemy() {}
	Enemy(const Enemy& other) { //����������� �����������
		name = other.name;
		cost = other.cost;
		hp = other.hp;
		dmg = other.dmg;
		pct = other.pct;
	}

	Enemy& operator=(const Enemy& other) { //���������� ��������� �����������
		if (this != &other) {
			name = other.name;
			cost = other.cost;
			hp = other.hp;
			dmg = other.dmg;
			pct = other.pct;
		}
		return *this;
	}

	void setName(const std::string& n) { name = n; }
	void setHp(short int h) { hp = h; }
	void setCost(int c) { cost = c; }
	void setDmg(short int d) { dmg = d; }
	void setPct(char p) { pct = p; }
	
	Enemy& operator - (short int damage) {//���������� ���������, ��������� �����
		hp -= damage;
		return *this;
	}

	bool isAlive() { //�������� ��� �� ����
		return (hp > 0);
	}

	char getPct() { 
		return pct;
	}
	short int getHp() {
		return hp;
	}
	void setPlace(int p) { place = p; }

	void Move() {//�������� ����� �� 1 ������
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

//��������� ����� - �������� ����� Enemy
class BossEnemy :public Enemy {
public:
	BossEnemy() : Enemy() {
		setName("Boss");
		setHp(500);
		setCost(20);
		setDmg(50);
		setPct('B');
	}
	~BossEnemy() {};
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
	}
	void takeDmg(short int damage) { //����� �������� ����
		hp -= damage;
	}
	bool isAlive() {//��������,��� ����� ��� �� ���������
		return (hp > 0);
	}
	short int getDamage() {
		return dmg;
	}
	short int* getLvl() {
		return &lvl;
	}
	short int* getHp() {
		return &hp;
	}
	bool isNear(int enemyPlace) {//�������� �� ��,����� �� ���� � ���� ������
		return (enemyPlace >= 35);
	}
	void repair() {//�������(���������� � Shop)
		hp += 500;
		if (hp > 3000) hp = 3000;
	}
	Tower& operator++() {//���������� ���������, ���������� ����� � ������
		lvl++;
		dmg += 20;
		return *this;
	}
	void Info() {//���������� � ������� �����
		std::cout << "�������� = " << hp << ", ���� = " << dmg << ", ������� = " << lvl << "\n";
	}
};


//��������� ���������� �����
class TowerDef {
private:
	short int dmg;
	short int lvl;
	short int range;
	bool boostTower;
public:
	TowerDef() {
		dmg = 0;
		lvl = 0;
		range = 0;
		boostTower = false;
	}
	~TowerDef() {
		dmg = 0;
		lvl = 0;
		range = 0;
	}
	void setLvl(short int l) { lvl = l; }
	void setDmg(short int d) { dmg = d; }
	void setRange(short int r) { range = r; }
	short int getDamage() {
		return dmg;
	}
	short int* getLvl() { //������� �������� ����� ���������
		return &lvl;
	}
	bool isNear(int enemyPlace, int towerPlace) {//�������� ����� �� � ������ ����
		return ((enemyPlace >= ((towerPlace * 5 + 2) - range)) && (enemyPlace <= ((towerPlace * 5 + 2) + range)));
	}
	void upDmg() {//��������� �����
		dmg += 25;
	}
	void upRange() {//��������� ���������
		range += 1;
	}
	void Info() {//���������� � �����
		std::cout << "������� = " << lvl << ", ���� = " << dmg << ", ��������� = " << range << "\n";
	}
	void Build() {//��������� �����
		range = 2;
		dmg = 15;
	}
	void Del() {//������� �����
		lvl = 0;
		range = 0;
		dmg = 0;
		boostTower = false;
	}
	bool isBoosted() {
		return boostTower;
	}
	void boosting() {
		boostTower = true;
	}
	friend void Boost(TowerDef& tower);
};

void Boost(TowerDef& tower) {
	tower.dmg -= 10;
	tower.boostTower = true;
};

//��������� �����
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
	void clear(int pos, int n) {//�������� �������
		place[pos][n] = ' ';
	}
	void towerPlace(int num) {//��������� ����� �� �������
		place[num * 5 + 2][0] = 'T';
	}
	void towerBrake(int num) {//���������� �����
		place[num * 5 + 2][0] = '0';
	}
	void placeEnemy(int pos, char pct) {//��������� ����� �� �������
		place[pos][1] = pct;
	}
	void printMap() {//������� ����� � ���� ������
		system("cls");
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 40; j++) {
				std::cout << place[j][i];
			}
			std::cout << std::endl;
		}
	}
};

//��������� ��������
class Shop {
private:
	int repairCost;
	int upgradeCost;
	int mainUpgradeCost;
public:
	void openShop() {//������� �������(������������ ���)
		repairCost = 20;
		upgradeCost = 10;
		mainUpgradeCost = 30;
	}
	void Info(Tower& tower, int& money, TowerDef deftowers[], Map& gameMap, int& booster) {//������ � ������������� � ����� ������� �� ��� ������
		int choice;
		do {
			system("cls");
			std::cout << "  ---�������---  \n1.������� �����\n2.��������������� �����\n3.�����\n���� �������� - " << money << " (�������� = " << booster << ")\n";
			std::cin >> choice;
			switch (choice) {
			case 1: {
				tower.Info();
				int choice2;
				short int* mainTowerLvl = tower.getLvl();
				short int* mainTowerHp = tower.getHp();
				do {//������� �����
					std::cout << "\n1.���������� �������� - " << repairCost << "\n2.�������� - " << mainUpgradeCost << "\n3.�����\n";
					std::cin >> choice2;
					switch (choice2) {
					case 1://����������� ��������
						if (money >= repairCost) {
							*mainTowerHp += 500;
							repairCost += 10;
							if (*mainTowerHp > 3000) *mainTowerHp = 3000;
						}
						else { 
							std::cout << "������������ �������!";
							system("pause");
						}
						break;
					case 2://���������
						if (money >= mainUpgradeCost && *mainTowerLvl < 10) {
							++tower;
							money -= mainUpgradeCost;
							mainUpgradeCost += 20;
							*mainTowerLvl++;
						}
						else if (money < mainUpgradeCost) {
							std::cout << "������������ �������!";
							system("pause");
						}
						else { 
							std::cout << "������� ����� ��������� �� ��������.";
							system("pause");
						}
						break;
					case 3:
						break;
					default:
						std::cout << "�������� ����.";
						system("pause");
					}
				} while (choice2 != 3);
				break; }
			case 2:{//��������������� �����
				int choice2;
				int i;
				do {
					std::cout << "�������� �����(1-8):";
					std::cin >> i;
				} while (i < 1 || i > 8);
				TowerDef& deftow = deftowers[i - 1];
				deftow.Info();
				short int* deftowerLvl = deftowers[i - 1].getLvl();
				do {
					std::cout << "\n1.������ ����� - 10\n2.�������� - " << upgradeCost << "\n3.������� �����\n4.�����\n";
					std::cin >> choice2;
					switch (choice2) {
					case 1://������� �����
						if (money >= 10 && *deftowerLvl == 0) {
							gameMap.towerPlace(i - 1);
							deftow.Build();
							money -= 10;
							*deftowerLvl++;
						}
						else if (*deftowerLvl != 0) {
							std::cout << "��� ����� ��� ���������!";
							system("pause");
						}
						else {
							std::cout << "������������ �������!";
							system("pause");
						}
						break;
					case 2://��������� �����
						if (money >= upgradeCost && *deftowerLvl < 10) {
							std::cout << "1.��������� ����(+2)\n2.��������� ���������(+1)\n3.��������(3x ��������������� �����)\n4.�����\n";
							do {
								std::cin >> choice;
							} while (choice < 1 && choice > 4);
							if (choice == 1) {
								deftow.upDmg();
								money -= upgradeCost;
								*deftowerLvl++;
							}
							else if (choice == 2) {
								deftow.upRange();
								money -= upgradeCost;
								*deftowerLvl++;
							} 
							else if (choice == 3) {
								if (deftow.isBoosted() == false) {
									deftow.boosting();
									booster--;
								}
								else {
									std::cout << "� ��� ��� ��������!\n";
								}
							}
						}
						break;
					case 3://�������� �����
						std::cout << "�� �������?\n1.������� �����\n2.�����\n";
						std::cin >> choice;
						if (choice == 1) {
							deftow.Del();
							gameMap.towerBrake(i - 1);
						}
						break;
					case 4:break;
					default: {std::cout << "�������� ����!\n";
						system("pause"); }
					}
				} while (choice2 != 4);
			}
			case 3: {
				break;
			}
			default: {std::cout << "�������� ����!\n"; }
			}
		} while (choice != 3);
	}
};
//����� ����� ����, ������� �������� �� ���������� ���� � ������
class Game {
private:
	int money;
	int enemysCount;
	int enemyMoney;
	std::vector<std::unique_ptr<Enemy>> enemys;
public:
	Game() {
		enemysCount = 0;
		money = 20;
		enemyMoney = 20;
	}
	int getMoney() {
		return money;
	}
	void setMoney(int mon) {
		money = mon;
	}
	int getEnemyMoney() {
		return enemyMoney;
	}
	void earnMoney(int n) {//��������� �������
		money += n;
	}
	void upenemys() {//��������� ���������� ������
		enemyMoney += 20;
	}
	void buyEnemy(int wave) {//����������� � ������� �����(�� �������������, �����������)
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>(); //������������� ����� ����������
		if (wave % 5 == 0 && enemysCount == 0) {//����
			std::unique_ptr<BossEnemy> boss = std::make_unique<BossEnemy>();
			if (enemyMoney >= boss->getCost()) {
				enemyMoney -= boss->getCost();
				enemys.push_back(std::move(boss));
				enemysCount++;
				enemys[enemysCount - 1]->setPlace(-1 * enemysCount);
			}
		}
		int r = rand() % 3;//������� ����
		r++;
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
			enemyMoney -= newEnemy->getCost();
			enemys.push_back(std::move(newEnemy));
			enemysCount++;
			enemys[enemysCount - 1]->setPlace(-1 * enemysCount);
		}
	}
	bool Wave(Tower& mainTower, TowerDef* towers, Map& gameMap) {//���������� �����
		int k = 0; bool f = true; int i = 0; int chek = enemysCount;
		do {
			for (int j = 0; j < 8; j++) {//����� ����� �������� ���������� �� ������ �� ������
				if (towers[j].isBoosted()) {
					for (int p = 0; p < 3; p++) {
						for (i = 0; i < enemysCount; i++) {
							if (enemys[i]->isAlive()) {
								if (towers[j].isNear(enemys[i]->getPlace(), j) && (towers[j].getLvl() != 0) && enemys[i]->getPlace() >= 0) {
									*enemys[i] - towers[j].getDamage(); //���� �� �������� ������������� ����������
									break;
								}
							}
						}
					}
				}
				else {
					for (i = 0; i < enemysCount; i++) {
						if (enemys[i]->isAlive()) {
							if (towers[j].isNear(enemys[i]->getPlace(), j) && (towers[j].getLvl() != 0) && enemys[i]->getPlace() >= 0) {
								*enemys[i] - towers[j].getDamage();
								break;
							}
						}
					}
				}
			}
			for (i = 0; i < enemysCount; i++) {
				if (enemys[i]->isAlive() && (enemys[i] != nullptr)) {
					if (mainTower.isNear(enemys[i]->getPlace()) && enemys[i]->getPlace() >= 0) {
						*enemys[i] - mainTower.getDamage();
						break;
					}
				}
			}
			for (i = 0; i < enemysCount; i++) {//���� ���� ����� ����� �� ��� ��������� ��� ������� ����
				if (enemys[i]->isAlive()) {
					if (enemys[i]->getPlace() != 38 && enemys[i]->getPlace() >= 0) {
						gameMap.clear(enemys[i]->getPlace(), 1);
						enemys[i]->Move();
						gameMap.placeEnemy(enemys[i]->getPlace(), enemys[i]->getPct());
					}
					else if (enemys[i]->getPlace() >= 0) {
						mainTower.takeDmg(enemys[i]->getDmg());
					}
					else { 
						enemys[i]->Move();
						if (enemys[i]->getPlace() == 0) gameMap.placeEnemy(0, enemys[i]->getPct());
					}

				}
				else {//������ ������ �� ������
					gameMap.clear(enemys[i]->getPlace(), 1);
					money += enemys[i]->getCost() * 2;
					enemys.erase(enemys.begin() + i);
					enemysCount--;
					i--;
				}
			}
			gameMap.printMap();//����� ����� � ����, ���� ��� ����� ���� ��� �� ���������� ������� �����
			Sleep(750);
			if (enemysCount == 0) f = 0;
			if (mainTower.isAlive() != true) f = 0;
		} while (f);
		enemysCount = 0;
		if (mainTower.isAlive() != true) { 
			std::cout << "�� ���������!"; 
			return 0;
		}
		else {
			upenemys();
			return 1;
		}
	}
};