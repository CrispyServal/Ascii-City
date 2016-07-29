#include <curses.h>

enum enumDirection
{
	up,
	down,
	left,
	right,
	others
};

enumDirection getDirection()
{
	char chKey;
	noecho();
	chKey = getch();
	if (chKey == 'j')
		return down;
	if (chKey == 'k')
		return up;
	if (chKey == 'h')
		return left;
	if (chKey == 'l')
		return right;
	return others;
}

int main()
{
	initscr();
	enumDirection moveDirection;
	int xPosition = 30;
	int yPosition = 30;
	char characterCh = '@';
	while (true)
	{
		mvaddch(xPosition,yPosition,characterCh);
		moveDirection = getDirection();
		if (moveDirection == up)
			-- xPosition;
		if (moveDirection == down)
			++ xPosition;
		if (moveDirection == left)
			-- yPosition;
		if (moveDirection == right)
			++ yPosition;
	}
	refresh();
	endwin();
	return 0;
}
