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
#include <algorithm>

//Интерфейс поведения - движение врага
class MoveStrategy {
public:
	virtual ~MoveStrategy() = default;
	virtual void Move(int& place, int hp, int maxHp) = 0;
	virtual std::string getSpeedDescription() = 0; 
};

//Обычная скорость движение
class NormalMove : public MoveStrategy {
public:
	void Move(int& place, int hp, int maxHp) override {
		place++;//Движение на 1 клетку
	}
	std::string getSpeedDescription() override {
		return "Обычная скорость. 1 клетка за 1 кадр.";
	}
 };

//Быстрое передвижение
class FastMove : public MoveStrategy {
public:
	void Move(int& place, int hp, int maxHp) override {
		place += 2;//Движение на 2 клетки
	}
	std::string getSpeedDescription() override {
		return "Быстрая скорость. 2 клетки за 1 кадр.";
	}
};

//Динамическое передвижение - ускоряется если здоровье меньше половины
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
	std::string getSpeedDescription() override {
		return "Переменчивая скорость. 1 клетка за 1 кадр, но 2 клетки за кадр если здоровье меньше половины.";
	}
};

//Стоящее на месте поведение врага(будет использованно в будущем)
class StuckMove : public MoveStrategy {
public:
	void Move(int& place, int hp, int maxHp) override {} //На месте(например,оглушение)
	std::string getSpeedDescription() override {
		return "Стоит на месте.";
	}
};

//Медленная скорость - для босса
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
	std::string getSpeedDescription() override {
		return "Медленная скорость. 1 клетка за 2 кадра.";
	}
};

//Структура врага
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
	// Конструктор инициализирует врага
	Enemy() {
		cost = 0;
		hp = 0;
		dmg = 0;
		pct = ' ';
		place = -1;
	}
	~Enemy() {}
	Enemy(const char* n, int c, int h, int d, char p, std::unique_ptr<MoveStrategy> strategy) :
		name(n), hp(h), maxHp(h), cost(c), dmg(d), pct(p), place(0), moveStrategy(std::move(strategy)) {}
	Enemy(const Enemy& other) { //Конструктор копирования
		name = other.name;
		cost = other.cost;
		hp = other.hp;
		dmg = other.dmg;
		pct = other.pct;
	}

	Enemy& operator=(const Enemy& other) { //Перегрузка оператора копирования
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
	
	Enemy& operator - (short int damage) {//Перегрузка оператора, получение урона
		hp -= damage;
		return *this;
	}

	bool isAlive() { //Проверка жив ли враг
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
	short int getMaxHp() {
		return maxHp;
	}
	void setPlace(int p) { place = p; }

	void setMoveStrategy(std::unique_ptr<MoveStrategy> newStrategy) { //Задаётся стратегия движения
		moveStrategy = std::move(newStrategy);
	}

	virtual void Move() {//Движение врага
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

	virtual std::string getSpeedDescription() {
		if (moveStrategy) {
			return moveStrategy->getSpeedDescription();
		}
		return nullptr;
	}

	virtual std::string getDescription() {
		if (pct == 'Z') {
			return "Зомби - обычный враг, который имеет большое количество здоровья. \nНе представляет особой угрозы, но может быть щитом для других врагов, что делает его довольно опасным.\n Имеет малый урон и обычную скорость.\n";
		}
		else if (pct == 'S') {
			return "Скелет - обычный враг, который имеет мало здоровья, появляется часто.\nВ одиночку не представляет никакой угрозы.\nНо могут создавать неприятности, ведь может появится сразу много скелетов за счёт их малой редкости.\n";
		}
		else if (pct == 'A') {
			return "Злой зомби - училенный зомби, с меньшим количеством здоровья, но с большим уроном.\nПредставляет сам по себе опасность, ведь как только его здоровье падает до половины - начинает двигаться в 2 раза быстрее.\n";
		}
		else {
			return "Информации об этом враге нет.";
		}
	}
};

//Структура босса - дочерный класс Enemy
class BossEnemy :public Enemy {
public:
	BossEnemy() : Enemy() {
		setName("Boss");
		setHp(500);
		setCost(20);
		setDmg(50);
		setPct('B');
		setMoveStrategy(std::make_unique<SlowMove>());
	}
	~BossEnemy() {};

	std::string getDescription() override {
		return "Босс - выходит на каждую 5 волну, и только 1 босс на карте. Очень медленный, но очень опасный враг.\nОгромное количество здоровья и урона, до него игра кажеться совсем лёгкой,не так ли?\n";
	}
};

//Класс быстрого врага
class FastEnemy : public Enemy {
public:
	FastEnemy() : Enemy() {
		setName("FastEnemy");
		setHp(50);
		setCost(10);
		setDmg(20);
		setPct('F');
		setMoveStrategy(std::make_unique<FastMove>());
	}

	~FastEnemy() {}

	std::string getDescription() override {
		return "Быстрый враг - соответствует названию, быстрый и опасный враг.\nДвигается очень быстро, наносит много урона, но у него совсем мало здоровья.\n";
	}
};

// Класс для управления врагами
class EnemyManager {
private:
	std::vector<std::unique_ptr<Enemy>> enemys;
public:
	//Добавить врага
	void addEnemy(std::unique_ptr<Enemy> enemy) {
		enemys.push_back(std::move(enemy));
	}

	void showAll(bool f) {
		for (auto& enemy : enemys) {
			std::cout << "Имя: " << enemy->getName() << "\n";
			if (f) {
				std::cout << "Здоровье: " << enemy->getHp() << "/" << enemy->getMaxHp() << "\n";
			}
			else std::cout << "Здоровье: " << enemy->getMaxHp() << "\n";
			std::cout << "Урон: " << enemy->getDmg() << "\n";
			std::cout << "Скорость: " << enemy->getSpeedDescription() << "\n";
			std::cout << "Появляется на карте в таком виде: " << enemy->getPct() << "\n";
			if (f) {
				std::cout << "Стоимость(редкость): " << enemy->getCost() << "\n";
				std::cout << "Позиция: " << enemy->getPlace() << "\n";
				std::cout << "Описание: " << enemy->getDescription() << "\n\n";
			}
			else std::cout << "\n";
		}
	}

	//Сортировка по здоровью
	void sortByHp() {
		std::sort(enemys.begin(), enemys.end(), [](const std::unique_ptr<Enemy>& a, const std::unique_ptr<Enemy>& b) {
			return a->getHp() < b->getHp(); // Сортировка по здоровью
			});
	}

	// Сортировка по стоимости
	void sortByCost() {
		std::sort(enemys.begin(), enemys.end(), [](const std::unique_ptr<Enemy>& a, const std::unique_ptr<Enemy>& b) {
			return a->getCost() < b->getCost(); // Сортировка по стоимости
			});
	}

	// Сортировка по урону
	void sortByDmg() {
		std::sort(enemys.begin(), enemys.end(), [](const std::unique_ptr<Enemy>& a, const std::unique_ptr<Enemy>& b) {
			return a->getDmg() < b->getDmg(); // Сортировка по урону
			});
	}

	//Поиск врага по имени
	void findEnemyByName(const std::string& name, bool details) {
		bool f = false;
		for (auto& enemy : enemys) {
			if (enemy->getName() == name) {
				f = true;
				std::cout << "Враг с таким именем найден!\n";
				std::cout << "Имя: " << enemy->getName() << "\n";
				if (details) {
					std::cout << "Здоровье: " << enemy->getHp() << "/" << enemy->getMaxHp() << "\n";
				} else std::cout << "Здоровье: " << enemy->getMaxHp() << "\n";
				std::cout << "Урон: " << enemy->getDmg() << "\n";
				std::cout << "Скорость: " << enemy->getSpeedDescription() << "\n";
				std::cout << "Появляется на карте в таком виде: " << enemy->getPct() << "\n";
				if (details) {
					std::cout << "Стоимость(редкость): " << enemy->getCost() << "\n";
					std::cout << "Позиция: " << enemy->getPlace() << "\n";
					std::cout << "Описание: " << enemy->getDescription() << "\n";
				}
			}
		}
		if (!f) {
			std::cout << "Враг с таким именем не найден.\n";
		}
	}

	//Поиск врага по картинке
	void findEnemyByPct(char pct, bool details) {
		bool f = false;
		for (auto& enemy : enemys) {
			if (enemy->getPct() == pct) {
				f = true;
				std::cout << "Враг с такой картинкой найден!\n";
				std::cout << "Имя: " << enemy->getName() << "\n";
				if (details) {
					std::cout << "Здоровье: " << enemy->getHp() << "/" << enemy->getMaxHp() << "\n";
				}
				else std::cout << "Здоровье: " << enemy->getMaxHp() << "\n";
				std::cout << "Урон: " << enemy->getDmg() << "\n";
				std::cout << "Скорость: " << enemy->getSpeedDescription() << "\n";
				std::cout << "Появляется на карте в таком виде: " << enemy->getPct() << "\n";
				if (details) {
					std::cout << "Стоимость(редкость): " << enemy->getCost() << "\n";
					std::cout << "Позиция: " << enemy->getPlace() << "\n";
					std::cout << "Описание: " << enemy->getDescription() << "\n";
				}
			}
		}
		if (!f) {
			std::cout << "Враг с такой картинкой не найден.\n";
		}
	}
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
	}
	void takeDmg(short int damage) { //Башня получает урон
		hp -= damage;
	}
	bool isAlive() {//Проверка,что башня ещё не разрушена
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
	bool isNear(int enemyPlace) {//Проверка на то,рядом ли враг с этой башней
		return (enemyPlace >= 35);
	}
	void repair() {//Починка(вызывается в Shop)
		hp += 500;
		if (hp > 3000) hp = 3000;
	}
	Tower& operator++() {//Перегрузка оператора, увеличение урона и уровня
		lvl++;
		dmg += 20;
		return *this;
	}
	void Info() {//Информация о главной башне
		std::cout << "Здоровье = " << hp << ", урон = " << dmg << ", уровень = " << lvl << "\n";
	}
};

//Абстрактный класс
class AbstractTower {
public:
	virtual void SpecialAblity() = 0;  //Виртуальная функция
	virtual ~AbstractTower() {}
};

//Класс защищающей башни
class TowerDef : public AbstractTower {
protected:
	short int dmg;
	short int lvl;
	short int range;
	bool boostTower;
	int attackSpeed;
	int type;
public:
	TowerDef(short int dmg1, short int lvl1, short int range1, bool boostTower1) {
		dmg = dmg1;
		lvl = lvl1;
		range = range1;
		boostTower = boostTower1;
	}
	virtual ~TowerDef() { //Виртуальный деструктор
		dmg = 0;
		lvl = 0;
		range = 0;
		boostTower = false;
	}


	TowerDef& operator=(const TowerDef& other) { //Оператор копирования
		if (this != &other) {
			lvl = other.lvl;
			dmg = other.dmg;
			range = other.range;
		}
		return *this;
	}
	virtual void clone(const TowerDef& other) { //Глубокое копирование
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
	virtual int getType() {
		return type;
	}
	virtual void setType(int t) { type = t; }
	virtual short int* getLvl() { //Возврат значения через указатель
		return &lvl;
	}
	virtual bool isNear(int enemyPlace, int towerPlace) {//Проверка рядом ли с башней враг
		return ((enemyPlace >= ((towerPlace * 5 + 2) - range)) && (enemyPlace <= ((towerPlace * 5 + 2) + range)));
	}
	virtual void upDmg() {//Повышение урона
		dmg += 25;
	}
	virtual void upRange() {//Повышение дальности
		range += 1;
	}
	virtual void Info() {//Информация о башне
		std::cout << "Уровень = " << lvl << ", урон = " << dmg << ", дальность = " << range << "\n";
		if (type == 1) std::cout << "Базовая башня.\n";
	}
	virtual void Build() {//Построить башню
		range = 2;
		dmg = 15;
		lvl = 1;
	}
	virtual void Del() {//Удалить башню
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

	virtual void SpecialAblity() override { //Перегрузка виртуальной функции абстрактного класса
		std::cout << "\nБазовая башня. Специальных возможностей нет.\n";
	}
};

//Класс башни - снайпера
class SniperTower : public TowerDef {
public:
	//Конструктор,вызывающий конструктор базового класса
	SniperTower(short int dmg1, short int lvl1, short int range1, bool boostTower1) 
		: TowerDef(dmg1, lvl1, range1, boostTower1) {}
	~SniperTower() override {}

	void SpecialAblity() override { //Перегрузка оператора абстрактного класса
		std::cout << "\nСпециальная способность - большой урон и дальность\n";
	}
	void Info() override { //перегрузка оператора базового класса
		TowerDef::Info();
		std::cout << "Тип башни - Снайперская башня(увеличенный радиус и урон)\n";
	}
	void BuildSpecial() {
		dmg = 20;
		range = 5;
		lvl = 1;
		boostTower = false;
	}
};

//Класс бишни скорострельности
class RapidFireTower : public TowerDef {
private:
	int attackSpeed;
public:
	RapidFireTower(short int dmg1, short int lvl1, short int range1, bool boostTower1, int attackSpeed2) 
		: TowerDef(dmg1, lvl1, range1, boostTower1), attackSpeed(attackSpeed2) {}
	~RapidFireTower() override {}

	//Запрет конструктора копирования,который создан автоматически
	RapidFireTower(const RapidFireTower& other) = delete;
	void clone(const TowerDef& other) override { //Глубокое копирование
		const RapidFireTower* rapidtower = dynamic_cast<const RapidFireTower*>(&other);
		if (rapidtower) {
			TowerDef::clone(other);
		}
	}
	void SpecialAblity() override {
		std::cout << "\nСпециальная способность - малый урон и большая скорострельность\n";
	}
	void Info() override {
		std::cout << "Тип башни: Башня скорострельности\n";
	}
	void BuildSpecial() {
		dmg = 10;
		range = 2;
		lvl = 1;
		boostTower = true;
		attackSpeed = 2;
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
	void clear(int pos, int n) {//Очистить позицию
		place[pos][n] = ' ';
	}
	void towerPlace(int num, int n) {//Поставить башню на позицию
		if (n == 1) place[num * 5 + 2][0] = 'T';
		else if (n == 2) place[num * 5 + 2][0] = 'L';
		else place[num * 5 + 2][0] = 'R';
	}
	void towerBrake(int num) {//Разрушение башни
		place[num * 5 + 2][0] = '0';
	}
	void placeEnemy(int pos, char pct) {//Поставить врага на позицию
		place[pos][1] = pct;
	}
	void printMap() {//Вывести карту в поле вывода
		system("cls");
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 40; j++) {
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
	void openShop() {//Открыть магазин(установление цен)
		repairCost = 20;
		upgradeCost = 10;
		mainUpgradeCost = 30;
	}
	void Info(Tower& tower, int& money, TowerDef *deftowers[], Map& gameMap, int& booster) {//Диалог с пользователем и вызов функций по его выбору
		int choice;
		do {
			system("cls");
			std::cout << "  ---Магазин---  \n1.Главная башня\n2.Вспомогательные башни\n3.Выход\nВаши средства - " << money << " (Бустеров = " << booster << ")\n";
			std::cin >> choice;
			switch (choice) {
			case 1: {
				tower.Info();
				int choice2;
				do {//Главная башня
					std::cout << "\n1.Восполнить здоровье - " << repairCost << "\n2.Улучшить - " << mainUpgradeCost << "\n3.Выход\n";
					std::cin >> choice2;
					switch (choice2) {
					case 1://Восполнение здоровья
						if (money >= repairCost) {
							tower.setHp(tower.getHp() + 500);
							repairCost += 10;
							if (tower.getHp() > 3000) tower.setHp(3000);
						}
						else {
							std::cout << "Недостаточно средств!";
							system("pause");
						}
						break;
					case 2://Улучшение
						if (money >= mainUpgradeCost && tower.getLvl() < 10) {
							++tower;
							money -= mainUpgradeCost;
							mainUpgradeCost += 20;
							tower.setLvl(tower.getLvl() + 1);
						}
						else if (money < mainUpgradeCost) {
							std::cout << "Недостаточно средств!";
							system("pause");
						}
						else {
							std::cout << "Главная башня прокачана на максимум.";
							system("pause");
						}
						break;
					case 3:
						break;
					default:
						std::cout << "Неверный ввод.";
						system("pause");
					}
				} while (choice2 != 3);
				break; }
			case 2: {//Вспомогательные башни
				int choice2;
				int i;
				do {
					std::cout << "Выберете башню(1-8):";
					std::cin >> i;
				} while (i < 1 || i > 8);
				TowerDef* deftow = deftowers[i - 1];
				do {
					TowerDef* usual;
					usual = new TowerDef(0, 0, 0, false);
					SniperTower* sniper;
					sniper = new SniperTower(0, 0, 0, false);
					RapidFireTower* rapid;
					rapid = new RapidFireTower(10, 2, 1, true, 2);
					if (deftow->getType() == 1) {
						deftow->Info();
					}
					else if (deftow->getType() == 2) {
						sniper->clone(*deftow);
						sniper->Info();
					}
					else if(deftow->getType() == 3) { 
						rapid->clone(*deftow);
						deftow->Info();
						rapid->Info(); 
					}
					std::cout << "\n1.Купить башню - 10\n2.Улучшить - " << upgradeCost << "\n3.Удалить башню\n4.Выход\n";
					std::cin >> choice2;
					switch (choice2) {
					case 1://Покупка башни
						std::cout << "\nВыберите тип башни:\n1 - Обычная(10)";
						usual->SpecialAblity();
						std::cout << "2 - Дальняя(25)";
						sniper->SpecialAblity();
						std::cout << "3 - Скорострельная(25)";
						rapid->SpecialAblity();
						int choice22;
						do {
							std::cin >> choice22;
						} while (choice22 < 1 || choice22 > 3);
						if (choice22 == 1) {
							if (money >= 10 && *deftow->getLvl() == 0) {
								gameMap.towerPlace(i - 1, 1);
								deftow->Build();
								deftow->setType(1);
								money -= 10;
							}
							else if (*deftow->getLvl()) {
								std::cout << "Эта башня уже построена!";
								system("pause");
							}
							else {
								std::cout << "Недостаточно средств!";
								system("pause");
							}
						}
						else {
							if (money >= 25 && *deftow->getLvl() == 0) {
								if (choice22 == 2) {
									gameMap.towerPlace(i - 1, 2);
									sniper->BuildSpecial();
									*deftow = *sniper;
									deftow->setType(2);
								}
								else {
									rapid = new RapidFireTower(10, 2, 1, true, 2);
									gameMap.towerPlace(i - 1, 3);
									rapid->BuildSpecial();
									deftow->clone(*rapid);
									deftow->setType(3);
								}
								money -= 25;
							}
							else if (*deftow->getLvl() != 0) {
								std::cout << "Эта башня уже построена!";
								system("pause");
							}
							else {
								std::cout << "Недостаточно средств!";
								system("pause");
							}
						}
						break;
					case 2://Улучшение башни
						if (money >= upgradeCost && (int)*deftow->getLvl() < 10) {
							std::cout << "1.Увеличить урон(+2)\n2.Увеличить дальность(+1)\n3.Усиление(3x скорострелность башни)\n4.Выход\n";
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
									std::cout << "У вас нет бустеров!\n";
								}
							}
						}
						break;
					case 3://Удаление башни
						std::cout << "Вы уверены?\n1.Удалить башню\n2.Назад\n";
						std::cin >> choice;
						if (choice == 1) {
							deftow->Del();
							gameMap.towerBrake(i - 1);
						}
						break;
					case 4:break;
					default: {std::cout << "Неверный ввод!\n";
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
			default: {std::cout << "Неверный ввод!\n"; }
			}
			} while (choice != 3);
		}
};
//Класс самой игры, который отвечает за проведение волн и логику
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
	void earnMoney(int n) {//Заработок средств
		money += n;
	}
	void upenemys() {//Повышение количества врагов
		enemyMoney += 20;
	}
	void buyEnemy(int wave) {//Рандомайзер и покупка врага(не пользователем, компьютером)
		if (wave % 5 == 0 && enemysCount == 0) {//Босс
			std::unique_ptr<BossEnemy> boss = std::make_unique<BossEnemy>();
			if (enemyMoney >= boss->getCost()) {
				enemyMoney -= boss->getCost();
				boss->setMoveStrategy(std::make_unique<SlowMove>());
				enemys.push_back(std::move(boss));
				enemysCount++;
				enemys[enemysCount - 1]->setPlace(-1 * enemysCount);
			}
		}
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>(); //Использование смарт указателей
		int r = rand() % 5;//Обычный враг
		r++;
		FastEnemy fastEnemy = FastEnemy();
		switch(r) {
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
			newEnemy->setName(fastEnemy.getName());
			newEnemy->setCost(fastEnemy.getCost());
			newEnemy->setHp(fastEnemy.getHp());
			newEnemy->setDmg(fastEnemy.getDmg());
			newEnemy->setPct(fastEnemy.getPct());
			newEnemy->setMoveStrategy(std::make_unique<FastMove>());
		case 5:
			newEnemy->setName("Fly");
			newEnemy->setCost(1);
			newEnemy->setHp(5);
			newEnemy->setDmg(10);
			newEnemy->setPct('*');
			newEnemy->setMoveStrategy(std::make_unique<NormalMove>());
			break;
		}
		if (enemyMoney >= newEnemy->getCost()) {
			enemyMoney -= newEnemy->getCost();
			enemys.push_back(std::move(newEnemy));
			enemysCount++;
			enemys[enemysCount - 1]->setPlace(-1 * enemysCount);
		}
	}
	bool Wave(Tower& mainTower, TowerDef* towers[], Map& gameMap) {//Проведение волны
		int k = 0; bool f = true; int i = 0; int chek = enemysCount;
		do {
			for (int j = 0; j < 8; j++) {//После башни пытаются выстрелить по одному из врагов
				if (towers[j]->isBoosted()) {
					for (int p = 0; p < 3; p++) {
						for (i = 0; i < enemysCount; i++) {
							if (enemys[i]->isAlive()) {
								if (towers[j]->isNear(enemys[i]->getPlace(), j) && (towers[j]->getLvl() != 0) && enemys[i]->getPlace() >= 0) {
									*enemys[i] - towers[j]->getDamage(); //Один из примеров использования перегрузки
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
			for (i = 0; i < enemysCount; i++) {//Если есть живые враги то они двигаются или наносят урон
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
				else {//Смерть одного из врагов
					gameMap.clear(enemys[i]->getPlace(), 1);
					money += enemys[i]->getCost() * 2;
					enemys.erase(enemys.begin() + i);
					enemysCount--;
					i--;
				}
			}
			gameMap.printMap();//Вывод карты и цикл, пока все враги живы или не уничтожена главная башня
			Sleep(800);
			if (enemysCount == 0) f = 0;
			if (mainTower.isAlive() != true) f = 0;
		} while (f);
		enemysCount = 0;
		if (mainTower.isAlive() != true) { 
			std::cout << "Вы проиграли!"; 
			return 0;
		}
		else {
			upenemys();
			return 1;
		}
	}
};