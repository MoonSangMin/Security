#include <conio.h>
#include <stdio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

int main() {
	char c;
	while (1) {
		if (_kbhit()) {//키보드가 눌리면
			c = _getch();
			if (c == -32) {//char는 -128~127이고 방향키는 224이다.
				c = _getch();
				switch(c)
				{
				case LEFT:
					printf("left\n");
					break;
				case RIGHT:
					printf("right\n");
					break;
				case UP:
					printf("up\n");
					break;
				case DOWN:
					printf("down\n");
					break;
				}
			}
		}
	}
	return 0;
}