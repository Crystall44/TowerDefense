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

//��������� ��������� - �������� �����
class MoveStrategy {
public:
	virtual ~MoveStrategy() = default;
	virtual void Move(int& place, int hp, int maxHp) = 0;
};

//������� �������� ��������
class NormalMove : public MoveStrategy {
public:
	void Move(int& place, int hp, int maxHp) override {
		place++;//�������� �� 1 ������
	}
 };

//������� ������������
class FastMove : public MoveStrategy {
public:
	void Move(int& place, int hp, int maxHp) override {
		place += 2;//�������� �� 2 ������
	}
};

//������������ ������������ - ���������� ���� �������� ������ ��������
class AdaptiveMove : public MoveStrategy {
public:
	void Move(int& place, int hp, int maxHp) override {
		if (hp > (maxHp/2)) {
			place++;
		}
		else {
			place += 2;
		}
	}
};

//������� �� ����� ��������� �����(����� ������������� � �������)
class StuckMove : public MoveStrategy {
public:
	void Move(int& place, int hp, int maxHp) override {} //�� �����(��������,���������)
};

//��������� �������� - ��� �����
class SlowMove : public MoveStrategy {
private:
	int f;
public:
	SlowMove() {
		f = 0;
	}
	void Move(int& place, int hp, int maxHp) override {
		if (f == 0) {
			f++;
		}
		else {
			f = 0;
			place++;
		}
	} 
};

//��������� �����
class Enemy{
protected:
	std::string name;
	int cost;
	short int hp;
	short int maxHp;
	short int dmg;
	char pct;
	int place;
	std::unique_ptr<MoveStrategy> moveStrategy;
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
	void setHp(short int h) { 
		hp = h;
		maxHp = hp;
	}
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
	std::string getName() {
		return name;
	}
	short int getHp() {
		return hp;
	}
	void setPlace(int p) { place = p; }

	void setMoveStrategy(std::unique_ptr<MoveStrategy> newStrategy) { //������� ��������� ��������
		moveStrategy = std::move(newStrategy);
	}

	virtual void Move() {//�������� �����
		moveStrategy->Move(place, hp, maxHp);
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

//����� �������� �����
class FastEnemy : public Enemy {
public:
	FastEnemy() : Enemy() {
		setName("FastEnemy");
		setHp(50);
		setCost(10);
		setDmg(20);
		setPct('F');
		maxHp = hp;
		setPlace(-2);
	}

	~FastEnemy() {}
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
	short int getLvl() {
		return lvl;
	}
	short int getHp() {
		return hp;
	}
	void setHp(short int hp1) {
		hp = hp1;
	}
	void setLvl(short int lvl1) {
		lvl = lvl1;
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

//����������� �����
class AbstractTower {
public:
	virtual void SpecialAblity() = 0;  //����������� �������
	virtual ~AbstractTower() {}
};

//����� ���������� �����
class TowerDef : public AbstractTower {
protected:
	short int dmg;
	short int lvl;
	short int range;
	bool boostTower;
	int attackSpeed;
public:
	TowerDef(short int dmg1, short int lvl1, short int range1, bool boostTower1) {
		dmg = dmg1;
		lvl = lvl1;
		range = range1;
		boostTower = boostTower1;
	}
	virtual ~TowerDef() { //����������� ����������
		dmg = 0;
		lvl = 0;
		range = 0;
		boostTower = false;
	}


	TowerDef& operator=(const TowerDef& other) { //�������� �����������
		if (this != &other) {
			lvl = other.lvl;
			dmg = other.dmg;
			range = other.range;
		}
		return *this;
	}
	virtual void clone(const TowerDef& other) { //�������� �����������
		lvl = other.lvl;
		dmg = other.dmg;
		range = other.range;
		boostTower = other.boostTower;
		attackSpeed = other.attackSpeed;
	}

	virtual void setLvl(short int l) { lvl = l; }
	virtual void setDmg(short int d) { dmg = d; }
	virtual void setRange(short int r) { range = r; }
	virtual short int getDamage() {
		return dmg;
	}
	virtual short int* getLvl() { //������� �������� ����� ���������
		return &lvl;
	}
	virtual bool isNear(int enemyPlace, int towerPlace) {//�������� ����� �� � ������ ����
		return ((enemyPlace >= ((towerPlace * 5 + 2) - range)) && (enemyPlace <= ((towerPlace * 5 + 2) + range)));
	}
	virtual void upDmg() {//��������� �����
		dmg += 25;
	}
	virtual void upRange() {//��������� ���������
		range += 1;
	}
	virtual void Info() {//���������� � �����
		std::cout << "������� = " << lvl << ", ���� = " << dmg << ", ��������� = " << range << "\n";
	}
	virtual void Build() {//��������� �����
		range = 2;
		dmg = 15;
		lvl = 1;
	}
	virtual void Del() {//������� �����
		lvl = 0;
		range = 0;
		dmg = 0;
		boostTower = false;
	}
	virtual bool isBoosted() {
		return boostTower;
	}
	virtual void boosting() {
		boostTower = true;
	}
	virtual void Boost(TowerDef& tower) {
		tower.dmg -= 10;
		tower.boostTower = true;
	};

	virtual void SpecialAblity() override { //���������� ����������� ������� ������������ ������
		std::cout << "\n������� �����. ����������� ������������ ���.\n";
	}
};

//����� ����� - ��������
class SniperTower : public TowerDef {
public:
	//�����������,���������� ����������� �������� ������
	SniperTower(short int dmg1, short int lvl1, short int range1, bool boostTower1) 
		: TowerDef(dmg1, lvl1, range1, boostTower1) {}
	~SniperTower() override {}

	void SpecialAblity() override { //���������� ��������� ������������ ������
		std::cout << "\n����������� ����������� - ������� ���� � ���������\n";
	}
	void Info() override { //���������� ��������� �������� ������
		TowerDef::Info();
		std::cout << "��� ����� - ����������� �����(����������� ������ � ����)\n";
	}
	void BuildSpecial() {
		dmg = 20;
		range = 5;
		lvl = 1;
		boostTower = false;
	}
};

//����� ����� ����������������
class RapidFireTower : public TowerDef {
private:
	int attackSpeed;
public:
	RapidFireTower(short int dmg1, short int lvl1, short int range1, bool boostTower1, int attackSpeed2) 
		: TowerDef(dmg1, lvl1, range1, boostTower1), attackSpeed(attackSpeed2) {}
	~RapidFireTower() override {}

	//������ ������������ �����������,������� ������ �������������
	RapidFireTower(const RapidFireTower& other) = delete;
	void clone(const TowerDef& other) override { //�������� �����������
		const RapidFireTower* rapidtower = dynamic_cast<const RapidFireTower*>(&other);
		if (rapidtower) {
			TowerDef::clone(other);
		}
	}
	void SpecialAblity() override {
		std::cout << "\n����������� ����������� - ����� ���� � ������� ����������������\n";
	}
	void Info() override {
		std::cout << "��� �����: ����� ����������������\n";
	}
	void BuildSpecial() {
		dmg = 10;
		range = 2;
		lvl = 1;
		boostTower = true;
		attackSpeed = 2;
	}
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
	void towerPlace(int num, int n) {//��������� ����� �� �������
		if (n == 1) place[num * 5 + 2][0] = 'T';
		else if (n == 2) place[num * 5 + 2][0] = 'L';
		else place[num * 5 + 2][0] = 'R';
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
	void Info(Tower& tower, int& money, TowerDef *deftowers[], Map& gameMap, int& booster) {//������ � ������������� � ����� ������� �� ��� ������
		int choice;
		do {
			system("cls");
			std::cout << "  ---�������---  \n1.������� �����\n2.��������������� �����\n3.�����\n���� �������� - " << money << " (�������� = " << booster << ")\n";
			std::cin >> choice;
			switch (choice) {
			case 1: {
				tower.Info();
				int choice2;
				do {//������� �����
					std::cout << "\n1.���������� �������� - " << repairCost << "\n2.�������� - " << mainUpgradeCost << "\n3.�����\n";
					std::cin >> choice2;
					switch (choice2) {
					case 1://����������� ��������
						if (money >= repairCost) {
							tower.setHp(tower.getHp() + 500);
							repairCost += 10;
							if (tower.getHp() > 3000) tower.setHp(3000);
						}
						else {
							std::cout << "������������ �������!";
							system("pause");
						}
						break;
					case 2://���������
						if (money >= mainUpgradeCost && tower.getLvl() < 10) {
							++tower;
							money -= mainUpgradeCost;
							mainUpgradeCost += 20;
							tower.setLvl(tower.getLvl() + 1);
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
			case 2: {//��������������� �����
				int choice2;
				int i;
				do {
					std::cout << "�������� �����(1-8):";
					std::cin >> i;
				} while (i < 1 || i > 8);
				TowerDef* deftow = deftowers[i - 1];
				deftow->Info();
				do {
					TowerDef* usual;
					usual = new TowerDef(0, 0, 0, false);
					SniperTower* sniper;
					sniper = new SniperTower(0, 0, 0, false);
					RapidFireTower* rapid;
					rapid = new RapidFireTower(10, 2, 1, true, 2);
					std::cout << "\n1.������ ����� - 10\n2.�������� - " << upgradeCost << "\n3.������� �����\n4.�����\n";
					std::cin >> choice2;
					switch (choice2) {
					case 1://������� �����
						std::cout << "\n�������� ��� �����:\n1 - �������(10)";
						usual->SpecialAblity();
						std::cout << "2 - �������(25)";
						sniper->SpecialAblity();
						std::cout << "3 - ��������������(25)";
						rapid->SpecialAblity();
						int choice22;
						do {
							std::cin >> choice22;
						} while (choice22 < 1 || choice22 > 3);
						if (choice22 == 1) {
							if (money >= 10 && *deftow->getLvl() == 0) {
								gameMap.towerPlace(i - 1, 1);
								deftow->Build();
								money -= 10;
							}
							else if (*deftow->getLvl()) {
								std::cout << "��� ����� ��� ���������!";
								system("pause");
							}
							else {
								std::cout << "������������ �������!";
								system("pause");
							}
						}
						else {
							if (money >= 25 && *deftow->getLvl() == 0) {
								if (choice22 == 2) {
									gameMap.towerPlace(i - 1, 2);
									sniper->BuildSpecial();
									*deftow = *sniper;
								}
								else {
									gameMap.towerPlace(i - 1, 3);
									rapid->BuildSpecial();
									deftow->clone(*rapid);
								}
								money -= 25;
							}
							else if (*deftow->getLvl() != 0) {
								std::cout << "��� ����� ��� ���������!";
								system("pause");
							}
							else {
								std::cout << "������������ �������!";
								system("pause");
							}
						}
						break;
					case 2://��������� �����
						if (money >= upgradeCost && (int)*deftow->getLvl() < 10) {
							std::cout << "1.��������� ����(+2)\n2.��������� ���������(+1)\n3.��������(3x ��������������� �����)\n4.�����\n";
							do {
								std::cin >> choice;
							} while (choice < 1 && choice > 4);
							if (choice == 1) {
								deftow->upDmg();
								money -= upgradeCost;
								deftow->setLvl((int)*deftow->getLvl() + 1);
							}
							else if (choice == 2) {
								deftow->upRange();
								money -= upgradeCost;
								deftow->setLvl((int)*deftow->getLvl() + 1);
							}
							else if (choice == 3) {
								if (deftow->isBoosted() == false) {
									deftow->boosting();
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
							deftow->Del();
							gameMap.towerBrake(i - 1);
						}
						break;
					case 4:break;
					default: {std::cout << "�������� ����!\n";
						system("pause"); }
					}
					delete usual;
					delete sniper;
					delete rapid;
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
		if (wave % 5 == 0 && enemysCount == 0) {//����
			std::unique_ptr<BossEnemy> boss = std::make_unique<BossEnemy>();
			if (enemyMoney >= boss->getCost()) {
				enemyMoney -= boss->getCost();
				boss->setMoveStrategy(std::make_unique<SlowMove>());
				enemys.push_back(std::move(boss));
				enemysCount++;
				enemys[enemysCount - 1]->setPlace(-1 * enemysCount);
			}
		}
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>(); //������������� ����� ����������
		int r = rand() % 4;//������� ����
		r++;
		switch (r) {
		case 1:
			newEnemy->setName("Zombe");
			newEnemy->setCost(4);
			newEnemy->setHp(100);
			newEnemy->setDmg(10);
			newEnemy->setPct('Z');
			newEnemy->setMoveStrategy(std::make_unique<NormalMove>());
			break;
		case 2:
			newEnemy->setName("Skeleton");
			newEnemy->setCost(2);
			newEnemy->setHp(30);
			newEnemy->setDmg(20);
			newEnemy->setPct('S');
			newEnemy->setMoveStrategy(std::make_unique<NormalMove>());
			break;
		case 3:
			newEnemy->setName("Angry Zombe");
			newEnemy->setCost(6);
			newEnemy->setHp(75);
			newEnemy->setDmg(25);
			newEnemy->setPct('A');
			newEnemy->setMoveStrategy(std::make_unique<AdaptiveMove>());
			break;
		case 4:
			FastEnemy fastEnemy = FastEnemy();
			newEnemy->setName(fastEnemy.getName());
			newEnemy->setCost(fastEnemy.getCost());
			newEnemy->setHp(fastEnemy.getHp());
			newEnemy->setDmg(fastEnemy.getDmg());
			newEnemy->setPct(fastEnemy.getPct());
			newEnemy->setMoveStrategy(std::make_unique<FastMove>());
		}
		if (enemyMoney >= newEnemy->getCost()) {
			enemyMoney -= newEnemy->getCost();
			enemys.push_back(std::move(newEnemy));
			enemysCount++;
			enemys[enemysCount - 1]->setPlace(-1 * enemysCount);
		}
	}
	bool Wave(Tower& mainTower, TowerDef* towers[], Map& gameMap) {//���������� �����
		int k = 0; bool f = true; int i = 0; int chek = enemysCount;
		do {
			for (int j = 0; j < 8; j++) {//����� ����� �������� ���������� �� ������ �� ������
				if (towers[j]->isBoosted()) {
					for (int p = 0; p < 3; p++) {
						for (i = 0; i < enemysCount; i++) {
							if (enemys[i]->isAlive()) {
								if (towers[j]->isNear(enemys[i]->getPlace(), j) && (towers[j]->getLvl() != 0) && enemys[i]->getPlace() >= 0) {
									*enemys[i] - towers[j]->getDamage(); //���� �� �������� ������������� ����������
									break;
								}
							}
						}
					}
				}
				else {
					for (i = 0; i < enemysCount; i++) {
						if (enemys[i]->isAlive()) {
							if (towers[j]->isNear(enemys[i]->getPlace(), j) && (towers[j]->getLvl() != 0) && enemys[i]->getPlace() >= 0) {
								*enemys[i] - towers[j]->getDamage();
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
						if (enemys[i]->getPlace() > 38) enemys[i]->setPlace(38);
						gameMap.placeEnemy(enemys[i]->getPlace(), enemys[i]->getPct());
					}
					else if (enemys[i]->getPlace() == 38) {
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
			Sleep(800);
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