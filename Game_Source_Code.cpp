/*GOBBLE AND THE GHOSTS */
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <fstream.h>
#include <conio.h>
#include <time.h>
#include<process.h>
#include<string.h>

int gd = DETECT, gm;	//Graph
char ch;				//WASD movement of Gobble
						//Quit 'q'
int b = 0;				//bonus for winning

int option = 1;
//int op=0;				//for the pause menu
int cc = 0;
int t = 0;
int k1 = 0;				//Lives counter

//Ghost Variables
const int n = 12;
int Lefto[n], Topo[n];						//ghost positions
int areao;
void *o1[n], *o2, *ot[n];					//ghost original,scare mode, temp place holders.
int r;										//ghost colour
int s_c[n] = { 0,0,0,0,0,0,0,0,0,0,0,0 };	//check to deactivate ghost
int k[n] = { 1,2,1,1,2,1,1,2,1,1,2,1 };		//check to prevent movement after ghost is dead

int area[3], tl, tt, Left[3], Top[3], left, top, j, temp;   //PacMan variables
int d;														//Delay
void *p[3];													//Pacman pointers
char tch;													//temp character holder for right and left jump
clock_t Time;												//for right and left jump

void *pwr;													//power up pellet
int pwr_area, pwr_left[2], pwr_top[2];
int M[2] = { 0,0 };											//prevent reactivation of power pellet
//Activating Scare Mode
clock_t T, TT;												//track of time
int c = 0;													//no of pellets eaten before regen. counter
int rspn = 250;												//respawning pellets

int trip = 0, ed = 0, ed2 = 0;								//to activate ghost mode, send ghost to dead zone

class highscore												//class for game scores
{
public:
	int hs;
	char pname[30];
	void showrecord()
	{

		setbkcolor(BLACK);
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
		setcolor(GREEN);
		outtextxy(10, 20, "HIGHSCORES");
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
		setcolor(YELLOW);
		outtextxy(10, 70, "NAME");
		outtextxy(100, 70, "SCORE");
		cout << endl;
		puts(pname);
		cout << "             " << hs;

	}
	void accept(int gh, int t, int bo)
	{
		cleardevice();
		flushall();
		setbkcolor(BLACK);
		setcolor(GREEN);
		if (bo == -1)
		{
			if (gh <= 5)
				hs = 210 * gh;
			else if (gh <= 9)
				hs = 315 * gh;
			else
				hs = 420 * gh;
		}
		else
			hs = 8000 * bo - t;

		outtextxy(10, 20, " Your high score is: ");
		gotoxy(23, 2);
		cout << hs;
		outtextxy(10, 50, " Enter your name: ");
		gotoxy(20, 4);
		gets(pname);
	}

};

void SORT(highscore h[], int n)								 //Highscores in descending order
{
	int i, j;
	highscore t;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (h[j].hs < h[j + 1].hs)
			{
				t = h[j];
				h[j] = h[j + 1];
				h[j + 1] = t;
			}

}
void display()
{
	int i = 0, n;
	highscore h[100];
	fstream x("highscore.dat", ios::in | ios::binary);

	while (!x.eof())
	{
		x.read((char*)&h[i], sizeof(h[i]));
		i++;

	}
	n = i;
	SORT(h, n);
	for (i = 0; i < n; i++)
		h[i].showrecord();
	x.close();
}
void create(int gh, int t, int bo)
{
	highscore h;
	fstream x("highscore.dat", ios::app | ios::binary);
	h.accept(gh, t, bo);
	x.write((char*)&h, sizeof(h));
	x.close();
}

void Clear_Input()
{
	while (kbhit())
		getch();

}
void Refresh()									//Refresh all counters
{
	for (int i = 0; i < n; i++)
	{
		s_c[i] = 0;								//check to deactivate ghost
		k[i] = 1;								//check to prevent movement after ghost is dead
	}
	for (i = 0; i < 2; i++)
		M[i] = 0;								//preventing old positions of pellets to remian active

	c = 0;										//respawn pellet at start of new game
	ed = 0;										//restart ghost death counter
	trip = 0;									//start without vulnerable ghosts
	b = 0;										//bonus refresh
}


void DrawPlatform()
{
	setcolor(WHITE);
	int maxx; int j = 3;

	maxx = getmaxx();

	for (int i = 0; i < 5; i++)
	{
		while (j < maxx)
		{
			bar(3, 20 + 95 * i, j++, 25 + 95 * i);
			delay(1);
		}
		j = 3;
	}
}

void DrawBorder()
{
	int maxx = getmaxx();
	int maxy = getmaxy();
	setcolor(WHITE);
	setlinestyle(SOLID_LINE, 1, 3);
	for (int i = maxx / 2, j = maxy / 2; i > 0; i--, j -= 2)
	{
		cleardevice();
		rectangle(i, j, maxx - i, maxy - j);
		delay(5);
	}
	setlinestyle(SOLID_LINE, 1, 1);
}

void Ladder()
{
	setcolor(RED);
	setlinestyle(SOLID_LINE, 1, 3);
	int Area;
	void *l;

	line(5, 5, 5, 96);
	line(25, 5, 25, 96);
	for (int i = 0; i < 5; i++)
		line(3, 10 + 20 * i, 28, 10 + 20 * i);
	Area = imagesize(0, 0, 30, 120);
	l = malloc(Area);
	getimage(0, 0, 30, 120, l);
	putimage(0, 0, l, XOR_PUT);

	DrawBorder();
	DrawPlatform();

	for (i = 0; i < 3; i++)
		putimage(20 + 95 * i * 2, 20 + 95 * i * 2, l, XOR_PUT);
	putimage(590, 20 + 95, l, XOR_PUT);
	putimage(20, 20 + 95 * 3, l, XOR_PUT);

}


//ghost design
void Ghost()
{
	randomize();
	void GhostScareMode();
	for (int i = 0; i < n; i++)
	{
		r = random(3000);

		if (r % 3 == 0)
		{
			setcolor(12);
			setfillstyle(SOLID_FILL, 12);
		}
		else if (r % 3 == 1)
		{
			setcolor(3);
			setfillstyle(SOLID_FILL, 3);
		}
		else if (r % 3 == 2)
		{
			setcolor(13);
			setfillstyle(SOLID_FILL, 13);
		}

		Lefto[i] = 92;
		Topo[i] = 187;

		//Body
		sector(101, 202, 0, 180, 9, 15);

		//Legs
		sector(95, 202, 180, 360, 3, 5);
		sector(101, 202, 180, 360, 3, 5);
		sector(107, 202, 180, 360, 3, 5);

		//Eyes
		setcolor(BLACK);
		setfillstyle(SOLID_FILL, WHITE);
		fillellipse(98, 192, 2, 4);
		fillellipse(103, 192, 2, 4);
		rectangle(98, 190, 99, 192);
		rectangle(103, 190, 104, 192);

		areao = imagesize(Lefto[i], Topo[i], Lefto[i] + 18, Topo[i] + 20);
		o1[i] = malloc(areao);
		getimage(Lefto[i], Topo[i], Lefto[i] + 18, Topo[i] + 20, o1[i]);
		putimage(Lefto[i], Topo[i], o1[i], XOR_PUT);

		if (i == 0)
			GhostScareMode();

		if (i >= 4 && ((i < 8 && i % 2 == 0) || (i > 8 && i % 2 == 1)))
			Lefto[i] = 10 + 50;
		else if (i >= 4)
			Lefto[i] = 610 - 50;
		else
			Lefto[i] = 220 + random(300);

		Topo[i] = 94 + 95 * (i % 4);
		ot[i] = malloc(areao);
		ot[i] = o1[i];
		areao = 0;
	}


}
//Design for ghost in scare mode
void GhostScareMode()
{
	setcolor(BLUE);
	setfillstyle(SOLID_FILL, BLUE);
	//Body
	sector(101, 202, 0, 180, 9, 15);

	//Legs
	sector(95, 202, 180, 360, 3, 5);
	sector(101, 202, 180, 360, 3, 5);
	sector(107, 202, 180, 360, 3, 5);

	//Eyes
	setcolor(WHITE);
	setfillstyle(SOLID_FILL, WHITE);
	bar(97, 192, 99, 194);
	bar(104, 192, 106, 194);

	//Mouth
	bar(97, 199, 99, 200);
	bar(95, 201, 96, 202);
	bar(100, 201, 102, 202);
	bar(103, 199, 105, 200);
	bar(106, 201, 107, 202);

	areao = imagesize(Lefto[0], Topo[0], Lefto[0] + 18, Topo[0] + 20);
	o2 = malloc(areao);
	getimage(Lefto[0], Topo[0], Lefto[0] + 18, Topo[0] + 20, o2);
	putimage(Lefto[0], Topo[0], o2, XOR_PUT);
}

void PacMan()										//All PacMan icons
{
	left = 190; top = 90; Left[1] = 190; Top[1] = 90;
	Left[0] = 390; Top[0] = 90; Left[2] = 290; Top[2] = 90;

	setcolor(YELLOW);
	circle(200, 100, 10);
	setfillstyle(SOLID_FILL, YELLOW);
	floodfill(200, 100, YELLOW);
	area[1] = imagesize(Left[1], Top[1], Left[1] + 20, Top[1] + 20);
	p[1] = malloc(area[1]);

	setcolor(YELLOW);
	circle(400, 100, 10);
	floodfill(400, 100, YELLOW);
	setcolor(BLACK);
	setfillstyle(SOLID_FILL, BLACK);
	pieslice(397, 100, 330, 360, 15);
	pieslice(397, 100, 0, 30, 15);
	area[0] = imagesize(Left[0], Top[0], Left[0] + 20, Top[0] + 20);
	p[0] = malloc(area[0]);

	setcolor(YELLOW);
	setfillstyle(SOLID_FILL, YELLOW);
	circle(300, 100, 10);
	floodfill(300, 100, YELLOW);
	setfillstyle(SOLID_FILL, BLACK);
	setcolor(BLACK);
	pieslice(303, 100, 150, 210, 15);
	area[3] = imagesize(Left[2], Top[2], Left[2] + 20, Top[2] + 20);
	p[2] = malloc(area[3]);

	d = 20;
	top = 94;
	tl = left;
	tt = top;

}

//Placement of pacman before start of game
void P_Refresh()
{
	for (j = 0; j < 3; j++)
	{
		getimage(Left[j], Top[j], Left[j] + 20, Top[j] + 20, p[j]);
		putimage(Left[j], Top[j], p[j], XOR_PUT);
	}
	j = 1; Top[j] += 4;
	putimage(Left[j], Top[j], p[j], XOR_PUT);

	Time = 0;
}


//pellet
void Create_Pellet()
{
	void Pellet();

	setcolor(WHITE);
	setfillstyle(SOLID_FILL, WHITE);
	pieslice(305, 50, 0, 360, 5);
	pwr_area = imagesize(300, 45, 310, 55);
	pwr = malloc(pwr_area);
	getimage(300, 45, 300 + 10, 45 + 10, pwr);
	putimage(300, 45, pwr, XOR_PUT);
	Pellet();


}

void Pellet()
{
	int c; int j;
	randomize();

	for (int i = 0; i < 2; i++)
	{
		j = random(2);
		c = random(12000);
		pwr_left[i] = 10 + c % 600;
		pwr_top[i] = 45 + 95 * (i + j);
	}
	for (i = 0; i < 2; i++)
		putimage(pwr_left[i], pwr_top[i], pwr, XOR_PUT);

}

//Function to move smart ghost
void G_Move(int &Lefto, int &Topo, int s, int &s_c, int &k)
{
	if (s_c == 1)
		return;

	putimage(Lefto, Topo, ot[s], XOR_PUT);

	//Scare_Mode (when ghosts are vulnerable )

	if (trip == 1)
		if (left + 10 >= Lefto && left + 10 <= Lefto + 18 && top + 10 >= Topo && top + 10 <= Topo + 20)
		{
			k = 0; Lefto = 10 + 20 * ed; Topo = 440; s_c = 1;
			delay(200); ed++; ed2++;
		}

	if (trip == 0)
		ot[s] = o1[s];
	else
		ot[s] = o2;

	if (s < 4)
	{
		if (trip == 0 && s_c == 0)
		{
			if (Lefto < left - 50 || Lefto < 10)
				k = 1;
			else if (Lefto > left + 50 || Lefto > 610)
				k = 2;
		}
		else if (trip == 1 && s_c == 0)
		{
			if ((Lefto<left + 50 && Lefto>left) || Lefto < 10)
				k = 1;
			else if ((Lefto > left - 50 && Lefto < left) || Lefto > 610)
				k = 2;
		}
	}
	else
	{
		if (Lefto < 10)
			k = 1;
		else if (Lefto > 610)
			k = 2;
	}
	if (k == 1)
		Lefto++;
	else if (k == 2)
		Lefto--;


	putimage(Lefto, Topo, ot[s], XOR_PUT);

}

void Draw_All()
{
	Ghost();
	PacMan();
	Create_Pellet();
}


void Eaten()
{
	TT = clock();
	if (TT >= T + 150 && TT <= T + 200 || (ed2 != 0 && ed2 % 2 == 0))
	{
		trip = 0;
		ed2 = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		if (left + 10 <= pwr_left[i] + 10 && left + 10 >= pwr_left[i] && top + 10 >= pwr_top[i] && top + 10 <= pwr_top[i] + 10 && M[i] == 0)
		{
			putimage(pwr_left[i], pwr_top[i], pwr, XOR_PUT);
			M[i]++;
			trip = 1;
			delay(500);
			c++;
			T = clock();
			ed2 = 0;
		}
	}
	if (c == 2 && TT >= T + rspn)
	{
		Pellet();										//Creating new pellets when old ones are used up
		c = 0;
		for (i = 0; i < 2; i++)
			M[i] = 0;
	}
}

void StraightJump()
{
	int c = 0;
	int G = 200, Vy = 150; int pos1 = top;
	do
	{
		delay(d);
		for (int s = 0; s < n; s++)
			G_Move(Lefto[s], Topo[s], s, s_c[s], k[s]);
		if (c == 0)
			c++;
		else
		{
			c = 0;
			continue;
		}
		Eaten();
		putimage(left, top, p[j], XOR_PUT);
		top -= (int)Vy*0.1;
		Vy -= (int)G*0.1;
		if (top > pos1)
			top = pos1;
		putimage(left, top, p[j], XOR_PUT);
	} while (top < pos1);
	Clear_Input();
}

void JumpLeft()
{
	int G = 250, Vx = 55, Vy = 150; int pos2 = top;
	int c = 0;
	do
	{
		delay(d);
		for (int s = 0; s < n; s++)
			G_Move(Lefto[s], Topo[s], s, s_c[s], k[s]);
		if (c == 0)
			c++;
		else
		{
			c = 0;
			continue;
		}
		Eaten();
		putimage(left, top, p[j], XOR_PUT);
		top -= (int)Vy*0.1;
		Vy -= (int)G*0.1;
		left -= (int)Vx*0.1;
		if (left < 10)
			left = 10;
		if (top > pos2)
			top = pos2;
		putimage(left, top, p[j], XOR_PUT);
	} while (top < pos2);
	Clear_Input();
}

void JumpRight()
{
	int G = 250, Vx = 55, Vy = 150;  int pos = top;
	int c = 0;
	do
	{
		delay(d);
		for (int s = 0; s < n; s++)
			G_Move(Lefto[s], Topo[s], s, s_c[s], k[s]);
		if (c == 0)
			c++;
		else
		{
			c = 0;
			continue;
		}
		Eaten();
		putimage(left, top, p[j], XOR_PUT);
		top -= (int)Vy*0.1;
		Vy -= (int)G*0.1;
		left += (int)Vx*0.1;
		if (left > 610)
			left = 610;
		if (top > pos)
			top = pos;
		putimage(left, top, p[j], XOR_PUT);
	} while (top < pos);
	Clear_Input();
}

void JumpUp(int &x, int &y, void *t)
{
	int G = -250, Vy = 100; int pos3 = y - 90;
	do
	{
		delay(d);
		for (int s = 0; s < n; s++)
			G_Move(Lefto[s], Topo[s], s, s_c[s], k[s]);
		putimage(x, y, t, XOR_PUT);
		y -= (int)Vy*0.1;
		Vy -= (int)G*0.1;
		if (y < pos3)
			y = pos3 - 5;
		putimage(x, y, t, XOR_PUT);
	} while (y > pos3);
}
void JumpDown(int &x, int &y, void *t)
{
	int G = 250, Vy = -100; int pos3 = y + 90;
	do
	{
		delay(d);
		for (int s = 0; s < n; s++)
			G_Move(Lefto[s], Topo[s], s, s_c[s], k[s]);
		putimage(x, y, t, XOR_PUT);
		y -= (int)Vy*0.1;
		Vy -= (int)G*0.1;
		if (y > pos3)
			y = pos3 + 5;
		putimage(x, y, t, XOR_PUT);
	} while (y < pos3);

}

void P_Move()
{
	tl = left;
	tt = top;

	switch (ch)
	{
	case 'a':
		putimage(left, top, p[j], XOR_PUT);
		if (j == 1)
			j++;
		else if (j == 2)
			j--;
		else if (j == 0)
			j++;
		tl = left - 9;										
		//tt=top+5;

		//boundry
		if (left < 10)
			left = 10;
		else
			left = tl;
		top = tt;

		putimage(left, top, p[j], XOR_PUT);
		tch = ch;
		for (int s = 0; s < n; s++)
			G_Move(Lefto[s], Topo[s], s, s_c[s], k[s]);
		delay(d);
		Time = clock();
		break;

	case 'd':
		putimage(left, top, p[j], XOR_PUT);
		if (j == 1)
			j--;
		else if (j == 2)
			j--;
		else if (j == 0)
			j++;
		tl = left + 9;										
		//tt=top+5;

		//boundry
		if (left > 610)
			left = 610;
		else
			left = tl;
		top = tt;

		putimage(left, top, p[j], XOR_PUT);
		tch = ch;
		for (s = 0; s < n; s++)
			G_Move(Lefto[s], Topo[s], s, s_c[s], k[s]);
		delay(d - 10);
		Time = clock();
		break;

	case 'w':
	case ' ':
		if (top == 94 + 95 && (left > 585 && left < 605))
			JumpUp(left, top, p[j]);
		else if (top == 94 + 2 * 95 && (left > 205 && left < 225))
			JumpUp(left, top, p[j]);
		else if (top == 94 + 3 * 95 && (left > 15 && left < 35))
			JumpUp(left, top, p[j]);
		else if (tch == 'd'&&clock() < Time + 7)
			JumpRight();
		else if (tch == 'a'&&clock() < Time + 7)
			JumpLeft();
		else
			StraightJump();
		break;


	case 's':
		if (top == 94 && (left > 585 && left < 605))
			JumpDown(left, top, p[j]);
		else if (top == 94 + 95 && (left > 205 && left < 225))
			JumpDown(left, top, p[j]);
		else if (top == 94 + 2 * 95 && (left > 15 && left < 35))
			JumpDown(left, top, p[j]);
		break;
	}
}

void NewGame()
{
	void Game();
	void Lives();

	b = 0;
	ed = 0;
	Ladder();
	Draw_All();
	P_Refresh();
	Lives();
	Refresh();

	for (int s = 0; s < n; s++)
		putimage(Lefto[s], Topo[s], o1[s], XOR_PUT);
	Game();

}
void Game()
{
	clock_t start, end;
	int time;
	void Lives();
	void Menu();
	start = clock();
	while (1)
	{
		if (kbhit())
			ch = getch();
		for (int s = 0; s < n; s++)
			G_Move(Lefto[s], Topo[s], s, s_c[s], k[s]);
		P_Move();
		Eaten();
		Lives();
		if (ch == 'q')
		{
			t++;
			break;
		}
		if (ed == 12)
		{
			t--;
			break;
		}
		flushall();
		ch = 't';
		delay(d);

	}
	if (t == 1)
	{
		k1 = 5;
		Lives();
	}
	if (t == -1)
	{
		b = k1;
		k1 = 6;
		Lives();
	}
	else
		b--;
	cc++;
	end = clock();
	time = end - start;
	create(ed, time, b);
	Menu();

}
void Control()
{
	char e;
	void Menu();
	//void Pause();
	if (option == 1)
	{
		clrscr();
		cleardevice();
		option = 100;
		NewGame();

		setbkcolor(BLACK);
		setcolor(GREEN);
		cc++;

		create(ed, t, b);

	}
	else if (option == 2)
	{
		cleardevice();
		setbkcolor(BLACK);
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
		setcolor(GREEN);
		outtextxy(10, 20, "INSTRUCTIONS");
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
		setcolor(YELLOW);
		outtextxy(10, 70, "OBJECTIVE: ");
		setcolor(CYAN);
		outtextxy(10, 85, "Eat all the ghosts to beat the Game ");
		setcolor(YELLOW);
		outtextxy(10, 120, "MOVEMENT: ");
		setcolor(CYAN);
		outtextxy(10, 135, "Mr Gobble (the icon you control) can be controlled using: ");
		setcolor(LIGHTRED);
		outtextxy(10, 150, "W/SPACE - Jump Up, Up the ladder      S - Down the ladder ");
		outtextxy(10, 165, "A       - Move left                   D - Move Right ");
		setcolor(CYAN);
		outtextxy(10, 180, "You can navigate between the platforms only through the ladders. ");
		setcolor(YELLOW);
		outtextxy(10, 205, "PELLETS: ");
		setcolor(CYAN);
		outtextxy(10, 220, "There exists pellets which spawn randomly during gameplay. ");
		outtextxy(10, 235, "When Mr Gobble eats one of these pellets, the ghosts enter a trans like ");
		outtextxy(10, 250, "state and this is the only time that the ghosts can be eaten. ");
		outtextxy(10, 265, "Remember, this mode only exists for a short time so be vigilant! ");
		setcolor(YELLOW);
		outtextxy(10, 290, "WINNING: ");
		setcolor(CYAN);
		outtextxy(10, 305, "You have 5 lives per game. If you are eaten by the ghosts you  ");
		outtextxy(10, 320, "lose a life. The faster you finish the entire game without losing ");
		outtextxy(10, 335, "all your lives, the more is your chance to score really high. ");
		setcolor(YELLOW);
		outtextxy(10, 360, "TIP: ");
		setcolor(LIGHTRED);
		outtextxy(10, 375, "Dont hold the arrow keys for too long... Mr Gobble is heavy and ");
		outtextxy(10, 390, "he won't be able to stop at your command ");
		setcolor(RED);
		outtextxy(10, 410, "You can press 'Q' to terminate early... IF YOU ARE SCARED!!");
		setcolor(GREEN);
		outtextxy(300, 430, "Press any key to go back to menu");
		getch();
		if (option == 2)
			Menu();
	}

	else if (option == 3)
	{
		cleardevice();
		setcolor(YELLOW);
		gotoxy(2, 6);
		display();
		setcolor(GREEN);
		outtextxy(300, 400, "Press any key to go back to menu");
		getch();
		Menu();
	}
	else if (option == 4)
	{
		cleardevice();
		setcolor(RED);
		setbkcolor(BLACK);
		outtextxy(10, 30, " ARE YOU SURE YOU WANT TO EXIT (Y/N)?");
		e = getch();

		if (e == 'n')
		{
			Menu();
		}
		else if (e == 'y')
			exit(0);
	}

}

void Lives()
{
	int t;
	static int l = 51;
	int s = 0;
	setcolor(WHITE);
	if (k1 == 0)
		for (int i = 0; i < 5; i++)
			line(610 + 5 * i, 30, 610 + 5 * i, 50);

	for (s; s < n; s++)
	{
		if (Lefto[s] <= left + 8 && Lefto[s] >= left - 8 && l > 50 && Topo[s] >= top - 8 && Topo[s] <= top + 8)
		{

			setcolor(BLACK);
			line(630 - 5 * k1, 30, 630 - 5 * k1, 50);
			k1++;
			delay(100);
			for (int i = 0; i < 6; i++)
			{
				putimage(left, top, p[j], XOR_PUT);
				if (ch == 'd')
					left += 5;
				else
					left -= 5;
				delay(100);
				putimage(left, top, p[j], XOR_PUT);
				delay(100);
				flushall();
				Clear_Input();
			}
		}
		if (k1 == 5 || k1 == 6)
		{
			cleardevice();
			setcolor(GREEN);
			settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
			if (k1 == 5)
				outtextxy(150, 200, "GAME  OVER");
			if (k1 == 6)
				outtextxy(150, 200, " YOU  WIN !! ");
			delay(300);
			settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
			ch = 'q';
		}


		putimage(left, top, p[j], XOR_PUT);
		putimage(left, top, p[j], XOR_PUT);

	}
}



void Menu()
{
	char chh;
	ch = 't';
	t = 0;
	k1 = 0;
	option = 1;

	do
	{
		delay(20);
		cleardevice();
		setbkcolor(BLACK);
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
		setcolor(GREEN);
		outtextxy(250, 100, "MENU");
		settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

		if (option == 1)          //use alt 0175 for inserting ¯
		{
			setcolor(RED);
			outtextxy(250, 130, "¯ 1.PLAY");
			setcolor(LIGHTBLUE);
			outtextxy(250, 150, "2.INSTRUCTIONS");
			outtextxy(250, 170, "3.HIGH SCORES");
			outtextxy(250, 190, "4.QUIT");
		}
		else if (option == 2)
		{
			setcolor(LIGHTBLUE);
			outtextxy(250, 130, "1.PLAY");
			setcolor(RED);
			outtextxy(250, 150, "¯ 2.INSTRUCTIONS");
			setcolor(LIGHTBLUE);
			outtextxy(250, 170, "3.HIGH SCORES");
			outtextxy(250, 190, "4.QUIT");
		}
		else if (option == 3)
		{
			setcolor(LIGHTBLUE);
			outtextxy(250, 130, "1.PLAY");
			outtextxy(250, 150, "2.INSTRUCTIONS");
			setcolor(RED);
			outtextxy(250, 170, "¯ 3.HIGH SCORES");
			setcolor(LIGHTBLUE);
			outtextxy(250, 190, "4.QUIT");
		}
		else if (option == 4)
		{
			setcolor(LIGHTBLUE);
			outtextxy(250, 130, "1.PLAY");
			outtextxy(250, 150, "2.INSTRUCTIONS");
			outtextxy(250, 170, "3.HIGH SCORES");
			setcolor(RED);
			outtextxy(250, 190, "¯ 4.QUIT");
		}
		setcolor(GREEN);
		outtextxy(280, 400, "Use Arrow keys to scroll and ENTER to select");
		chh = getch();

		if (chh == 72)						//scroll up
			option--;
		else if (chh == 80)					//scroll down
			option++;
		if (option == 5)
			option = 1;
		else if (option == 0)
			option = 4;
	} while (chh != 13);					//enter for select

	Control();

}

void main()
{
	//Initiating graphic functions
	initgraph(&gd, &gm, "C:\\TC\\BGI");

	setcolor(YELLOW);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
	outtextxy(80, 100, "MR. GOBBLE");
	setcolor(LIGHTRED);
	outtextxy(30, 200, "& THE GHOSTS");
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	setcolor(GREEN);
	outtextxy(400, 400, "Press any key to continue");
	getch();

	Menu();

	closegraph();
}