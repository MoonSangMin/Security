#include <conio.h>
#include <stdio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

int main() {
	char c;
	while (1) {
		if (_kbhit()) {//Ű���尡 ������
			c = _getch();
			if (c == -32) {//char�� -128~127�̰� ����Ű�� 224�̴�.
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