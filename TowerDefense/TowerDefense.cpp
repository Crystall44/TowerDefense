#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "head/Structures.h"
#include <conio.h>

//Начало новой игры
void Restart(Round* round, Shop* shop, Tower* tower) {
	(*round).num = 1;
	(*round).money = 10;
	(*round).boss = false;
	(*shop).cash = 80;
	for (int i = 0; i < 8; i++) (*shop).towers[i] = 0;
	(*tower).lvl = 0;
	(*tower).hp = 3000;
	for (int i = 0; i < 256; i++) (*round).spawn[i] = -1;
}

//Завершение раунда и подсчёт денег
void RoundEnd(Round* round, Shop* shop, Tower* tower) {
	(*shop).cash += (*round).num * 20;
	(*round).num += 1;
	if ((*round).num % 10 == 0) (*round).boss = true;
	(*round).money = (*round).num * 10;
	(*tower).hp += 300;
	for (int i = 0; i < 256; i++) (*round).spawn[i] = -1;
}

//Покупка врага на раунд
void EnemyBuy(Enemy enemy, Round* round, int i, int j) {
	(*round).money = (*round).money - enemy.cost;
	(*round).spawn[i] = j;
}

//Спавн врага
void EnemySpawn(Round* round, Enemy enemy[], int i, Map* map) {
	Enemy spawned;
	spawned = enemy[(*round).spawn[i]];
	(*round).spawn[i] = -1;
	(*map).place[0][1] = spawned.pct;
}

//Спавн босса
void BossSpawn(Round round, BossEnemy boss[], int i, Map* map) {
	BossEnemy spawned;
	spawned = boss[i];
	//Появление босса на карте
	(*map).place[0][1] = spawned.boss.pct;
}

//Функция времени(1 тик, за который каждый враг проходит 1 клетку)
void timeMap(Map* map) {
	Sleep(1000);
	for (int i = 38; i > 0; i = i - 1) (*map).place[i][1] = (*map).place[i - 1][1];
	(*map).place[0][1] = ' ';
}

//Вывод карты и измененного положения всех врагов(ещё не реализованна стрельба самих башен по врагам)
void Changes(Map* map) {
	system("cls");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 40; j++) printf("%c", (*map).place[j][i]);
		printf("\n");
	}
	for (int j = 0; j < 40; j++) printf("%c", (*map).towerPlace[j]);
}

//Покупка новой башни
void TowerBuy(TowerDef* tower, Shop* shop, int i, Map* map) {
	(*shop).cash = (*shop).cash - 40;
	(*shop).towers[i] = 1;
	(*tower).dmg = 20;
	(*tower).lvl = 1;
	(*tower).range = 0.5;
	(*tower).speed = 1;
	(*map).towerPlace[5 * i + 2] = '1';
}

//Прокачка уже купленной башни
void TowerUpgrade(TowerDef *tower, Shop* shop, int i, int j) {
	(*shop).cash = (*shop).cash - (*tower).lvl * 10;
	(*tower).lvl += 1;
	if ((*tower).lvl == 30) (*shop).towers[i] = -1;
	if (j == 1) (*tower).dmg += 20;
	else if (j == 2) (*tower).range += 0.25;
	else (*tower).speed += 1;
}


int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "RUS");
	Enemy enemy[] = {{4, 100, 10, 1, "Zombe", 'Z'}, {2, 30, 20, 1, "Skeleton", 'X'}, {6, 75, 25, 1, "AngryZombe", 'A'}};
	Enemy roundEnemys[] = { {} };
	int enemyskount;
 	BossEnemy boss[] = { {} };
	TowerDef towers[] = { {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };
	Round round;
	Shop shop;
	Tower mainTower;
	Map map;
	int i;
	//Пустая карта и начало игры
	for (i = 0; i < 40; i++) map.place[i][0] = '-';
	for (i = 0; i < 40; i++) map.place[i][2] = '-';
	map.place[39][1] = '|';
	for (i = 0; i < 39; i++) map.place[i][1] = ' ';
	for (i = 0; i < 40; i++) {
		if ((i + 3) % 5 == 0) map.towerPlace[i] = '0';
		else map.towerPlace[i] = ' ';
	}
	short int k = 0;
	short int n = 0;
	Restart(&round, &shop, &mainTower);
	do {
		printf("1 - Начать новую волну\n2 - Магазин\n3 - Выход из игры");
		do {
			scanf("%hd", &n);
		} while (n < 1 || n > 3);
		if (n == 1) {
			//Новая волна
			Changes(&map);
			if (round.boss == true) {
				round.money = round.money - boss[(round.num / 10)].boss.cost;
				BossSpawn(round, boss, round.num / 10, &map);
				Changes(&map);
				timeMap(&map);
				Changes(&map);
				timeMap(&map);
				Changes(&map);
				timeMap(&map);
				Changes(&map);
			}
			enemyskount = 0;
			while (round.money > 1 && enemyskount < 256) {
				do {
					i = rand() % 3;
				} while (enemy[i].cost > round.money);
				EnemyBuy(enemy[i], &round, enemyskount, i);
				enemyskount++;
			}
			i = 0;
			while(round.spawn[i] != -1) {
				EnemySpawn(&round, enemy, i, &map);
				Changes(&map);
				timeMap(&map);
				Changes(&map);
				timeMap(&map);
				Changes(&map);
				i++;
			}
			while (enemyskount > 0 && map.place[38][1] == ' ') {
				timeMap(&map);
				Changes(&map);
			}
		}
		else if (n == 2) {
			system("cls");
			printf("Количество монет = %d\n", shop.cash);
			printf("1 - Купить башню(40 монет)\n2 - Прокачать башню\n3 - Другое\n4 - выход");
			do {
				scanf("%hd", &n);
			} while (n < 1 || n > 3);
			
			if (n == 1) {
				printf("Введите номер башни(1 - 8):");
				do {
					scanf("%hd", &n);
				} while (n < 1 || n > 8);
				if (shop.cash < 40) printf("У вас недостаточно средств.");
				else if (shop.towers[n] == 0)TowerBuy(&towers[n], &shop, n, &map);
				else printf("Эта башня уже построена.");
				getchar();
			}
			else if (n == 2) {
				printf("Введите номер башни(1 - 8, 0 - выход):");
				do {
					scanf("%hd", &n);
					if (shop.towers[n] == 0) {
						printf("Эта башня ещё не построена.");
						n = 0;
					}
				} while ((n < 0 || n > 9) && shop.towers[n] == 0);
				if (n != 0) {
					int j = 0;
					if (shop.towers[n] == -1) { 
						printf("Эта башня уже прокачана на максимум.");
						getchar();
						n = 0;
					}
					else {
						printf("1 - Увеличить урон\n2 - Увеличить дальность\n3 - Увеличить скорость стрельбы\n0 - Выход");
						do {
							scanf("%d", &j);
						} while (j < 0 || j > 3);
					}
					if (shop.cash < towers[n].lvl * 15) {
						n = 0;
						printf("У вас недостаточно средств.");
						getchar();
					}
					if (j != 0 && n != 0) TowerUpgrade(&towers[n], &shop, n, j);
				}
			}
			else if (n == 3) {
				printf("1 - Восстановить здоровье главной башни(20)\n2 - Удалить башню\n3 - Выход");
				do {
					scanf("%hd", &n);
				} while (n < 1 || n > 3);
				if (n == 1) {
					if (shop.cash < 200) { 
						printf("У вас недостаточно средств.");
						_getch();
					}
					else {
						mainTower.hp += 500;
						shop.cash = shop.cash - 200;
					}
				}
				else if (n == 2) {
					printf("Введите номер башни для удаления(1-8, 0 - отмена)");
					do {
						scanf("%hd", &n);
					} while (n < 0 || n > 8);
					if (n != 0) {
						if (shop.towers[n] == 0) {
							printf("Эта башня ещё не построена.");
							getchar();
						}
						else {
							printf("Вы уверены? Да - 1. Отмена - 0. Эта башня имеет %d уровень. Деньги за постройку и улчения назад вы не получите!", towers[n].lvl);
							int j = 0;
							do {
								scanf("%d", &j);
							} while (j < 0 || j > 1);
							if (j == 1) {
								shop.towers[n] = 0;
								towers[n] = { 0, 0, 0, 0 };
							}
						}
					}
				}
			}
			n = 1;
			system("cls");
		}
	} while (n != 3);
	return 0;
}

