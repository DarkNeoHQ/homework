#include"body.h"
#include <iostream> 
using namespace std;

int main()
{
	SetConsoleTitle(L"                  ������˭��������ս");
	game Game;
	csnake *game1;
	int choose3;
	do
	{
		game1 = new csnake();
		delete game1;
		choose3=Game.start();
	} while (choose3!=2);
	Sleep(2000);
	return 0;
}