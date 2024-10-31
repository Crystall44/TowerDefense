#include <iostream>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "head/Structures.h"

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "RUS");
	Game game;
	do {
		game();
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
			printf("1 - Купить башню(40 монет)\n2 - Прокачать башню\n3 - Другое\n4 - Выход");
			do {
				scanf("%hd", &n);
			} while (n < 1 || n > 4);
			
			if (n == 1) {
				printf("Введите номер башни(1 - 8, 0 - выход):");
				do {
					scanf("%hd", &n);
				} while (n < 0 || n > 8);
				if (n != 0) {
					if (shop.cash < 40) printf("У вас недостаточно средств.");
					else if (shop.towers[n - 1] == 0) {
						TowerBuy(&towers[n - 1], &shop, n - 1, &map);
						printf("Башня построена.");
					}
					else printf("Эта башня уже построена.");
					_getch();
				}
			}
			else if (n == 2) {
				printf("Введите номер башни(1 - 8, 0 - выход):");
				do {
					scanf("%hd", &n);
					if (shop.towers[n - 1] == 0) {
						printf("Эта башня ещё не построена.");
						_getch();
						n = 0;
					}
				} while ((n < 0 || n > 9) && shop.towers[n - 1] == 0);
				if (n != 0) {
					int j = 0;
					if (shop.towers[n - 1] == -1) { 
						printf("Эта башня уже прокачана на максимум.");
						_getch();
						n = 0;
					}
					else {
						printf("1 - Увеличить урон\n2 - Увеличить дальность\n3 - Увеличить скорость стрельбы\n0 - Выход");
						do {
							scanf("%d", &j);
						} while (j < 0 || j > 3);
					}
					if (shop.cash < towers[n - 1].lvl * 15) {
						n = 0;
						printf("У вас недостаточно средств.");
						_getch();
					}
					if (j != 0 && n != 0) TowerUpgrade(&towers[n - 1], &shop, n - 1, j);
				}
			}
			else if (n == 3) {
				printf("1 - Восстановить здоровье главной башни(20)\n2 - Удалить башню\n3 - Выход");
				do {
					scanf("%hd", &n);
				} while (n < 1 || n > 3);
				if (n == 1) {
					if (shop.cash < 20) { 
						printf("У вас недостаточно средств.");
						_getch();
					}
					else {
						mainTower.hp += 500;
						shop.cash = shop.cash - 20;
					}
				}
				else if (n == 2) {
					printf("Введите номер башни для удаления(1-8, 0 - отмена)");
					do {
						scanf("%hd", &n);
					} while (n < 0 || n > 8);
					if (n != 0) {
						if (shop.towers[n - 1] == 0) {
							printf("Эта башня ещё не построена.");
							_getch();
						}
						else {
							printf("Вы уверены? Да - 1. Отмена - 0. Эта башня имеет %d уровень. Деньги за постройку и улчения назад вы не получите!", towers[n].lvl);
							int j = 0;
							do {
								scanf("%d", &j);
							} while (j < 0 || j > 1);
							if (j == 1) {
								shop.towers[n - 1] = 0;
								towers[n - 1] = { 0, 0, 0, 0 };
								map.towerPlace[5 * n - 3] = '0';
							}
						}
					}
				}
			}
			n = 1;
			system("cls");
		}
	} while (n != 4);
	return 0;
}

