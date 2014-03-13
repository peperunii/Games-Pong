/********************************************
*											*
*    PONG software. Author Petar Bijev      *
*											*
*********************************************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

/*
Config file:
name: config.cfg
example content:

arraySizeX				30
arraySizeY				16
StartingBallDir 		4
ShowScore		 		1
PauseAfterGOAL	 		1
PauseInMSafterGoal		300
Vreditel		 		 1
DoubleClickSpeed		 1
IncreaseSpeed_afterHit   1  // increase speed by ten. Sleep(in ms) between frames  = 1000/Speed;
Vreditel_slowSpeed		 1  // two times slower
*/
int main()
{
	FILE *fp;
	int CONFIG_FILE_ELEMENTS = 11;
	int SHOW_SCORE = 0;
	int INCREASE_SPEED_AFTER_HIT_WITH = 5;
	int PAUSE_AFTER_GOAL = 0;
	int INSERT_VREDITEL = 0;
	int PAUSE_IN_MS_AFTERGOAL = 0;
	int DOUBLE_CLICK_SPEED = 0;
	int INCREASE_SPEED_AFTER_HIT = 0;
	int VREDITEL_SLOW_SPEED = 0;
	int i, j;
	int oncePassed = 0;
	int Speed = 0;
	int Vreditel_speed_control = 1; 
	int Win = 0;
	int originalSpeed = 0;
	int game_score_P1 = 0;
	int game_score_P2 = 0;
	int calculus = 0;
	int calculus2 = 0;
	int CurrentDir = 0;
	int posP1[4];
	int passHui = 0;
	int posP2[4];
	int Key = 0;
	int GoalFrames = 0;
	int c = 0;
	int BallPos[2];
	int PC_as_P2 = 0;
	int difficult = 0;
	int PC_difficult = 0;
	int arraySizeX = 40;
	int arraySizeY = 24;
	int snakeSize = 3;
	int P1_dir, P2_dir;
	int VreditelPos[4];
	int VreditelDirection;
	char unused[30];
	char *arr = NULL;

	int BallDir = 1; /* 6 options : 1 - right, 2 - up_right, 3 -up_left, 4 -left, 5 - down_left, 6 -down_right */
	COORD pos = { 0, 0 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos_results = { 44, 2 };
	COORD pos_results2 = { 44, 3 };
	HANDLE output_results = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos_middle = { 28, 3 };
	HANDLE output_middle = GetStdHandle(STD_OUTPUT_HANDLE);

	fp = fopen("config.cfg", "rt");
	if (fp == NULL) 
	{
		printf("Nema config file..."); 
		_getch(); 
		return 0;
	}
	for (i = 0; i < CONFIG_FILE_ELEMENTS; i++)
	{
		if (i == 0) fscanf(fp, "%s%d", unused, &arraySizeX);
		if (i == 1) fscanf(fp, "%s%d", unused, &arraySizeY);
		if (i == 2) fscanf(fp, "%s%d", unused, &BallDir);
		if (i == 3) fscanf(fp, "%s%d", unused, &SHOW_SCORE);
		if (i == 4) fscanf(fp, "%s%d", unused, &PAUSE_AFTER_GOAL);
		if (i == 5) fscanf(fp, "%s%d", unused, &PAUSE_IN_MS_AFTERGOAL);
		if (i == 6) fscanf(fp, "%s%d", unused, &INSERT_VREDITEL);
		if (i == 7) fscanf(fp, "%s%d", unused, &DOUBLE_CLICK_SPEED);
		if (i == 8) fscanf(fp, "%s%d", unused, &INCREASE_SPEED_AFTER_HIT);
		if (i == 9) fscanf(fp, "%s%d", unused, &INCREASE_SPEED_AFTER_HIT_WITH);
		if (i == 10) fscanf(fp, "%s%d", unused, &VREDITEL_SLOW_SPEED);
	}
	fclose(fp);
	
	arr = malloc(arraySizeX * arraySizeY * sizeof(char));
	if (arr == NULL)
	{
		printf("cannot allocate memory\n"); _getch();
	}

	while (1)
	{ 
		// new game cycle
		srand(time(NULL));
		pos_results.X = arraySizeX + 4;
		pos_results2.X = arraySizeX + 4;
		for (i = 0; i < arraySizeX * arraySizeY; i++) arr[i] = ' ';

		if (INSERT_VREDITEL)
		{
			VreditelPos[0] = arraySizeX / 2;
			VreditelPos[1] = arraySizeY / 2;
			VreditelPos[2] = arraySizeX / 2;
			VreditelPos[3] = arraySizeY / 2 + 1;
			VreditelDirection = 0;
		}
		CurrentDir = 0;
		BallDir = 1;
		P1_dir = 0;
		P2_dir = 0;
		Win = 0;
		game_score_P1 = 0;
		game_score_P2 = 0;
		posP1[0] = 1;
		posP1[1] = 6;
		posP1[2] = 1;
		posP1[3] = 7;

		posP2[0] = 22;
		posP2[1] = 6;
		posP2[2] = 22;
		posP2[3] = 7;

		BallPos[0] = 7;
		BallPos[1] = 6;
		for (i = 0; i < arraySizeX * arraySizeY; i++)
		{
			arr[i] = ' ';
			if ((i % arraySizeX == 0) && (i < 3 * arraySizeX || i>((arraySizeY - 4) * arraySizeX))) { arr[i] = '|'; }
			if ((i % arraySizeX == arraySizeX - 1) && (i < 3 * arraySizeX || ((i>((arraySizeY - 3)* arraySizeX)) && i < (arraySizeY - 1) * arraySizeX))) arr[i] = '|';
			if (i >(arraySizeX * (arraySizeY - 1)) - 1) arr[i] = '-';
			if (i < arraySizeX) arr[i] = '-';
		}
		while (1)
		{
			system("cls");
			printf("During the game- press ESC to pause.. or DIE !\nSpeed level: (1-300)\n");
			fflush(stdin);
			scanf("%d", &Speed);
			if (Speed > 300) Speed = 300;
			if (!(Speed <= 0)) break; /* challenging levels up to speed = 30*/
		}
		originalSpeed = Speed;

		posP1[0] = 1;
		posP1[1] = arraySizeY/2;
		posP1[2] = 1;
		posP1[3] = posP1[1] + 1;

		posP2[0] = arraySizeX -2;
		posP2[1] = arraySizeY / 2;
		posP2[2] = arraySizeX - 2;
		posP2[3] = posP2[1] + 1;

		BallPos[0] = arraySizeX / 2;
		BallPos[1] = arraySizeY / 2;

		while (1)
		{
			printf("Wanna play with computer?\n1: Yes\n2:no\n");
			fflush(stdin);
			scanf("%d", &PC_as_P2);
			if (PC_as_P2 == 2) PC_as_P2 = 0;
			if (PC_as_P2 == 1 || PC_as_P2 == 0) break;
			system("cls");
		}
		system("cls");
		fflush(stdin);

		while (1)
		{
			printf("ComputerLevel?\n0: Elementary\n1: Easy\n2:Hard\n");
			fflush(stdin);
			scanf("%d", &PC_difficult);
			if (PC_difficult == 0 || PC_difficult == 1 || PC_difficult == 2) break;
			system("cls");
		}
		system("cls");
		fflush(stdin);

		while (1)
		{
			printf("Difficult level with additional (almost random) obstacles?\n1: Yes\n2:no\n");
			fflush(stdin);
			scanf("%d", &difficult);
			if (difficult == 2) difficult = 0;
			if (difficult == 1 || difficult == 0) break;
			system("cls");
		}
		system("cls");
		fflush(stdin);

		if (difficult)
		{
			for (i = 0; i < arraySizeX * arraySizeY; i++)
			{
				if ((i == (3 * arraySizeX / 2)) || 
					(i == (5 * arraySizeX / 2)) || 
					i == ((arraySizeY - 1)*arraySizeX - arraySizeX / 2) || 
					i == ((arraySizeY - 3)*arraySizeX - arraySizeX / 2)
					)
					arr[i] = '|';
				if (i == ((arraySizeY/2 -1)*arraySizeX + (arraySizeX/2))) arr[i] = '|';
				if (i == ((arraySizeX + 2) * 5) || i == (arraySizeX - 3) * (arraySizeY - 3)) arr[i] = '|';
				if (i == ((arraySizeX - 2) * 6) || i == ((arraySizeX + 3) * (arraySizeY - 6) + 11)) arr[i] = '|';
			}
		}
		if (PC_difficult == 2)
		{
			for (i = 0; i < arraySizeX * arraySizeY; i++)
			{
			//	if (i == arraySizeX * 5 - 1) arr[i] = '|';
				if (i == (arraySizeX * 4 - arraySizeX)) arr[i] = '|';
				if (i == arraySizeX *(arraySizeY - 4))arr[i] = '|';
				if (i == (arraySizeX *(arraySizeY - 4) + arraySizeX - 1))arr[i] = '|';
			}
		}

		while (1)
		{
			oncePassed = 0;
			if (Win) Speed = originalSpeed;
			/* Clear the screen and print the matrix with Pong */
			pos.X = 0;
			pos.Y = 0;
			SetConsoleCursorPosition(output, pos);
			CurrentDir = BallDir;
			Win = 0;
			Key = 0;

			if (VREDITEL_SLOW_SPEED) 
			{
				if (Vreditel_speed_control) Vreditel_speed_control = 0;
				else Vreditel_speed_control = 1;
			}

			if (PC_difficult == 2 && INSERT_VREDITEL && Vreditel_speed_control)
			{
				if (VreditelPos[1] < 5) VreditelDirection = 1;
				if (VreditelPos[3] > arraySizeY - 5) VreditelDirection = 0;

				switch (VreditelDirection)
				{
					case 0:
						arr[VreditelPos[1] * arraySizeX + VreditelPos[0]] = ' ';
						arr[VreditelPos[3] * arraySizeX + VreditelPos[2]] = ' ';
						VreditelPos[1] -= 1;
						VreditelPos[3] -= 1;
						break;
					case 1:
						arr[VreditelPos[1] * arraySizeX + VreditelPos[0]] = ' ';
						arr[VreditelPos[3] * arraySizeX + VreditelPos[2]] = ' ';
						VreditelPos[1] += 1;
						VreditelPos[3] += 1;
						break;
				}
				arr[VreditelPos[1] * arraySizeX + VreditelPos[0]] = '|';
				arr[VreditelPos[3] * arraySizeX + VreditelPos[2]] = '|';
			}

			Sleep(1000 / Speed);
			while (1)
			{
				if (_kbhit())
				{
zamunda:			Key = _getch();
				}
				if (Key == 72 || Key == 'w' || Key == 's' || Key == 80)
				{
					if (Key == 72)
					{
						oncePassed = 1;
doubleUp:				P1_dir = 0; //up_userP1
						if (posP1[1] - 1 > 1)
						{
							arr[posP1[3] * arraySizeX + posP1[2]] = ' ';
							posP1[1] = posP1[1] - 1;
							posP1[3] = posP1[3] - 1;
						}
						if (DOUBLE_CLICK_SPEED && oncePassed) { oncePassed = 0; goto doubleUp; }
					}
					if (Key == 80)
					{
						oncePassed = 1;
doubleDown:				P1_dir = 1; //down_userP1
						if (posP1[3] + 1 < (arraySizeY - 2))
						{
							arr[posP1[1] * arraySizeX + posP1[0]] = ' ';
							posP1[1] = posP1[1] + 1;
							posP1[3] = posP1[3] + 1;
						}
						if (DOUBLE_CLICK_SPEED && oncePassed) { oncePassed = 0; goto doubleDown; }
					}
					if (Key == 'w')
					{
						if (!PC_as_P2)
						{
							P2_dir = 0; //up_userP2
							if (posP2[1] - 1 > 2)
							{
								arr[posP2[3] * arraySizeX + posP2[2]] = ' ';
								posP2[1] = posP2[1] - 1;
								posP2[3] = posP2[3] - 1;

							}
						}
					}
					if (Key == 's')
					{
						if (!PC_as_P2)
						{
							P2_dir = 1; //down_userP2
							if (posP2[3] + 1 < (arraySizeY - 2))
							{
								arr[posP2[1] * arraySizeX + posP2[0]] = ' ';
								posP2[1] = posP2[1] + 1;
								posP2[3] = posP2[3] + 1;
							}
						}
					}
				}
				else if (Key == 27)
				{
					Key = 0;
					while (1)
					{
						fflush(stdin);
						if (_kbhit())
						{
							Key = _getch();
						}
						if (Key == 27) break;
					}
				}
				if (_kbhit())
					goto zamunda;
				else break;
			}
			fflush(stdin);


			/* MoveBall */
			// check dir, move the ball. if hit a wall-change the dir. BallDir = 1; /* 6 options : 1 - right, 2 - up_right, 6 -up_left, 4 -left, 5 - down_left, 3 -down_right */
directions:
			if (passHui && INCREASE_SPEED_AFTER_HIT) Speed += INCREASE_SPEED_AFTER_HIT_WITH;
			if (BallDir == 1) //right
			{
				if (BallPos[0] + 1 != (arraySizeX - 2) && BallPos[0] - 1 != 1){
					if (arr[BallPos[1] * arraySizeX + BallPos[0] + 1] == '|')
					{
						BallDir = 4; goto directions;
					}
				}
				else
				{
					if (!passHui)
					{
						if (arr[(BallPos[1])*arraySizeX + BallPos[0] + 1] == '|')
						{
							passHui = 1;
							BallDir = 4 + rand() % 3;  goto directions;
						}
					}
				}
				arr[BallPos[1] * arraySizeX + BallPos[0]] = ' ';
				BallPos[0] += 1;
			}
			if (BallDir == 2) // up right
			{
				if (BallPos[0] + 1 != (arraySizeX - 2) && BallPos[0] - 1 != 1)
				{
					if (arr[(BallPos[1] - 1) * arraySizeX + BallPos[0] + 1] == '|') // go to up left
					{
						BallDir = 6; goto directions;
					}
				}
				else
				{
					if (!passHui)
					{
						if (arr[(BallPos[1] - 1)*arraySizeX + BallPos[0] + 1] == '|')
						{
							passHui = 1;
							BallDir = 4 + rand() % 3;  goto directions;
						}
					}
				}
				if (arr[(BallPos[1] - 1) * arraySizeX + BallPos[0] + 1] == '-') // go to down right
				{
					BallDir = 3; goto directions;
				}
				arr[BallPos[1] * arraySizeX + BallPos[0]] = ' ';
				BallPos[0] += 1;
				BallPos[1] -= 1;
			}
			if (BallDir == 3) // down-right
			{
				if (BallPos[0] + 1 != (arraySizeX - 2) && BallPos[0] - 1 != 1)
				{
					if (arr[(BallPos[1] + 1) * arraySizeX + BallPos[0] + 1] == '|')
					{
						BallDir = 5; goto directions;
					}
				}
				else
				{
					if (!passHui)
					{
						if (arr[(BallPos[1] + 1)*arraySizeX + BallPos[0] + 1] == '|')
						{
							passHui = 1;
							BallDir = 4 + rand() % 3; goto directions;
						}
					}
				}
				if (arr[(BallPos[1] + 1) * arraySizeX + BallPos[0] + 1] == '-')
				{
					BallDir = 2; goto directions;
				}
				arr[BallPos[1] * arraySizeX + BallPos[0]] = ' ';
				BallPos[0] += 1;
				BallPos[1] += 1;
			}

			if (BallDir == 4) // left
			{
				if (BallPos[0] + 1 != (arraySizeX - 2) && BallPos[0] - 1 != 1)
				{
					if (arr[BallPos[1] * arraySizeX + BallPos[0] - 1] == '|')
					{
						BallDir = 1; goto directions;
					}
				}
				else
				{
					if (!passHui)
					{
						if (arr[(BallPos[1])*arraySizeX + BallPos[0] - 1] == '|')
						{
							passHui = 1;
							BallDir = 1 + rand() % 3; goto directions;
						}
					}
				}
				arr[BallPos[1] * arraySizeX + BallPos[0]] = ' ';
				BallPos[0] -= 1;
			}
			if (BallDir == 5) // down-left
			{
				if (BallPos[0] + 1 != (arraySizeX - 2) && BallPos[0] - 1 != 1)
				{
					if (arr[(BallPos[1] + 1) * arraySizeX + BallPos[0] - 1] == '|')
					{
						BallDir = 3; goto directions;
					}
				}
				else
				{
					if (!passHui)
					{
						if (arr[(BallPos[1] + 1)*arraySizeX + BallPos[0] - 1] == '|')
						{
							passHui = 1;
							BallDir = 1 + rand() % 3; goto directions;
						}
					}
				}
				if (arr[(BallPos[1] + 1) * arraySizeX + BallPos[0] - 1] == '-')
				{
					BallDir = 6; goto directions;
				}
				arr[BallPos[1] * arraySizeX + BallPos[0]] = ' ';
				BallPos[0] -= 1;
				BallPos[1] += 1;
			}
			if (BallDir == 6) // up left
			{
				if (BallPos[0] + 1 != (arraySizeX - 2) && BallPos[0] - 1 != 1)
				{
					if (arr[(BallPos[1] - 1) * arraySizeX + BallPos[0] - 1] == '|')
					{
						BallDir = 2; goto directions;
					}
				}
				else
				{
					if (!passHui)
					{
						if (arr[(BallPos[1] - 1)*arraySizeX + BallPos[0] - 1] == '|')
						{
							passHui = 1;
							BallDir = 1 + rand() % 3; goto directions;
						}
					}
				}
				if (arr[(BallPos[1] - 1) * arraySizeX + BallPos[0] - 1] == '-')
				{
					BallDir = 5; goto directions;
				}
				arr[BallPos[1] * arraySizeX + BallPos[0]] = ' ';
				BallPos[0] -= 1;
				BallPos[1] -= 1;
			}
			passHui = 0;

			/* Computer logic if P2 is PC */

			// if the ball is in P2's part of the field move to the center. else follow the ball
			if (PC_as_P2)
			{
				if (PC_difficult == 0)  //elementary
				{
					if (BallPos[0] > (arraySizeX / 2 + 4))
					{
						if (BallPos[1] > posP2[1])
						{
							if (posP2[3] + 1 < (arraySizeY - 2))
							{
								arr[posP2[1] * arraySizeX + posP2[0]] = ' ';
								posP2[1] = posP2[1] + 1;
								posP2[3] = posP2[3] + 1;
							}
						}
						if (BallPos[1] < posP2[1])
						{
							if (posP2[1] + 1 > 2)
							{
								arr[posP2[3] * arraySizeX + posP2[2]] = ' ';
								posP2[1] = posP2[1] - 1;
								posP2[3] = posP2[3] - 1;
							}
						}
					}
				}
				if (PC_difficult == 1)
				{
					if (BallPos[0] > arraySizeX / 2)
					{
						if (BallPos[1] > posP2[1])
						{
							if (posP2[3] + 1 < (arraySizeY - 2))
							{
								arr[posP2[1] * arraySizeX + posP2[0]] = ' ';
								posP2[1] = posP2[1] + 1;
								posP2[3] = posP2[3] + 1;
							}
						}
						if (BallPos[1] < posP2[1])
						{
							if (posP2[1] + 1 > 2)
							{
								arr[posP2[3] * arraySizeX + posP2[2]] = ' ';
								posP2[1] = posP2[1] - 1;
								posP2[3] = posP2[3] - 1;
							}
						}
					}
					else //if the ball is in the oposite field.. move to center
					{
						if (posP2[1] < arraySizeY / 2)  //move down
						{
							arr[posP2[1] * arraySizeX + posP2[0]] = ' ';
							posP2[1] = posP2[1] + 1;
							posP2[3] = posP2[3] + 1;
						}
						if (posP2[1] > arraySizeY / 2)  //move up
						{
							arr[posP2[3] * arraySizeX + posP2[2]] = ' ';
							posP2[1] = posP2[1] - 1;
							posP2[3] = posP2[3] - 1;
						}
					}
				}
				else // PC_difficult == 2
				{
					if (BallPos[0] >= arraySizeX / 2)
					{
						// check the three options for ball directions
						// right
						if (BallDir == 1)
						{
							if (BallPos[1] > posP2[1])
							{
								if (posP2[3] + 1 < (arraySizeY - 2))
								{
									arr[posP2[1] * arraySizeX + posP2[0]] = ' ';
									posP2[1] = posP2[1] + 1;
									posP2[3] = posP2[3] + 1;
								}
							}
							if (BallPos[1] < posP2[1])
							{
								if (posP2[1] + 1 > 2)
								{
									arr[posP2[3] * arraySizeX + posP2[2]] = ' ';
									posP2[1] = posP2[1] - 1;
									posP2[3] = posP2[3] - 1;
								}
							}
						}
						// up-right
						if (BallDir == 2)
						{
							if (BallPos[1] > posP2[1])
							{
								calculus = arraySizeX - 2 - BallPos[0];
								if (BallPos[1] - calculus > posP2[3])
								{
									if (posP2[3] + 1 < (arraySizeY - 2))
									{
										//move down
										arr[posP2[1] * arraySizeX + posP2[0]] = ' ';
										posP2[1] = posP2[1] + 1;
										posP2[3] = posP2[3] + 1;
									}
								}
								if (BallPos[1] - calculus < posP2[1])
								{
									if (posP2[1] + 1 > 3)
									{
										arr[posP2[3] * arraySizeX + posP2[2]] = ' ';
										posP2[1] = posP2[1] - 1;
										posP2[3] = posP2[3] - 1;
									}
								}
							}
						}
						// down-right
						if (BallDir == 3)
						{
							if (BallPos[1] <= posP2[1])
							{
								calculus = arraySizeX - 2 - BallPos[0];
								if (BallPos[1] + calculus >= posP2[1])
								{
									if (posP2[3] + 1 < (arraySizeY - 2))
									{
										//move down
										arr[posP2[1] * arraySizeX + posP2[0]] = ' ';
										posP2[1] = posP2[1] + 1;
										posP2[3] = posP2[3] + 1;
									}
								}
								if (BallPos[1] + calculus < posP2[1])
								{
									if (posP2[1] + 1 > 2)
									{
										arr[posP2[3] * arraySizeX + posP2[2]] = ' ';
										posP2[1] = posP2[1] - 1;
										posP2[3] = posP2[3] - 1;
									}
								}
							}
						}
					}
					else //if the ball is in the oposite field.. move to center
					{
						if (posP2[1] < arraySizeY / 2)  //move down
						{
							arr[posP2[1] * arraySizeX + posP2[0]] = ' ';
							posP2[1] = posP2[1] + 1;
							posP2[3] = posP2[3] + 1;
						}
						if (posP2[1] > arraySizeY / 2)  //move up
						{
							arr[posP2[3] * arraySizeX + posP2[2]] = ' ';
							posP2[1] = posP2[1] - 1;
							posP2[3] = posP2[3] - 1;
						}
					}
				}
			}

			/* check coordinates - wining solution */
			if (BallPos[0] == (arraySizeX - 1) && (BallPos[1] < (arraySizeY - 3) && BallPos[1] > 2))
			{
				game_score_P1++;
				BallDir = 4 + rand() % 3;
				if (BallDir > 6) BallDir = 6;
				Win = 1;
			}
			if (BallPos[0] == 0 && (BallPos[1] < (arraySizeY - 3) && BallPos[1] > 2))
			{
				game_score_P2++;
				BallDir = 1 + rand() % 3;
				if (BallDir > 3) BallDir = 3;
				Win = 1;
			}
			/* If win - move the ball to the center.*/
			if (Win)
			{
				if(PAUSE_AFTER_GOAL) Sleep(PAUSE_IN_MS_AFTERGOAL);
				BallPos[0] = arraySizeX / 2;
				BallPos[1] = arraySizeY / 2;
			}

			/* put players in the array  */
			arr[posP1[1] * arraySizeX + posP1[0]] = '|';
			arr[posP1[3] * arraySizeX + posP1[2]] = '|';
			arr[posP2[1] * arraySizeX + posP2[0]] = '|';
			arr[posP2[3] * arraySizeX + posP2[2]] = '|';

			/* put the ball in the array */
			arr[BallPos[1] * arraySizeX + BallPos[0]] = '*';

			for (i = 0; i < arraySizeY; i++)
			{
				for (j = 0; j < arraySizeX; j++)
				{
					printf("%c", arr[i * arraySizeX + j]);
				}
				printf("\n");
			}
			/* print the results*/
			if (SHOW_SCORE)
			{
				SetConsoleCursorPosition(output_results, pos_results);
				printf("%P1    P2 ", game_score_P1, game_score_P2);
				SetConsoleCursorPosition(output_results, pos_results2);
				printf("%d  |  %d ", game_score_P1, game_score_P2);
			}

			if (Win) 
			{
				GoalFrames = 15; //Sleep(300);
			}
			if (GoalFrames > 0)
			{
				pos_middle.X = arraySizeX / 2 - 3;
				pos_middle.Y = arraySizeY + 1;
				SetConsoleCursorPosition(output_middle, pos_middle);
				printf("G O A L  !!!");
				GoalFrames--;
			}
			else 
			{
				GoalFrames = 0;
				SetConsoleCursorPosition(output_middle, pos_middle);
				printf("               ");
			}
		}
	}

	_getch();
	return 0;
}
