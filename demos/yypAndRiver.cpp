#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include <mutex>

enum enumDirection
{
	up = 0,
	down = 1,
	left = 2,
	right = 3,
	others = -1
};

class Direction
{
private:
	enumDirection _dr;
public:
	const enumDirection getDirection()
	{
		return _dr;
	}
	void getInput()
	{
		//while ( true )
		//{
			noecho();
			char chKey;
			cbreak();
			chKey = getch();
			//mvprintw(3,0,"got %c", chKey);
			if (chKey == 'j')
				_dr = down;
			else if (chKey == 'k')
				_dr = up;
			else if (chKey == 'h')
				_dr = left;
			else if (chKey == 'l')
				_dr = right;
			else
				_dr = others;
		//}
	}
};

class Character
{
private:
	int _xPos;
	int _yPos;
	char _ch;
public:
	Character(int xPos = LINES - 3, int yPos = COLS / 2, char ch = 'Y'):
		_xPos(xPos),
		_yPos(yPos),
		_ch(ch)
	{
	}
	int getX()
    {
        return _xPos;
    }
    
    int getY()
    {
        return _yPos;
    }
	//use this only once
	void initShow()
	{
		mvaddch(_xPos,_yPos,_ch);
	}
	void update(enumDirection moveDirection)
	{
		if (moveDirection == up)
		{
			mvaddch(_xPos,_yPos,' ');
			-- _xPos;
		}
		else if (moveDirection == down)
		{
			mvaddch(_xPos,_yPos,' ');
			++ _xPos;
		}
		else if (moveDirection == left)
		{
			mvaddch(_xPos,_yPos,' ');
			-- _yPos;
		}
		else if (moveDirection == right)
		{
			mvaddch(_xPos,_yPos,' ');
			++ _yPos;
		}
		mvaddch(_xPos,_yPos,_ch);
	}
};

class Water
{
private:
    int _initX;
    int _initY;
    int _heightX;
    int _widthY;
    WINDOW * _waterWin;
    bool _isClear;
public:
    Water()
    :
    _initX(10),
    _initY(5),
    _heightX(10),
    _widthY(COLS - 2 * _initY),
    _isClear(false)
    {
        
        /* init window */
        _waterWin = newwin(_heightX, _widthY, _initX, _initY );
        wrefresh(_waterWin);
    }
    bool isInWater(int x, int y)
    {
        if ( x >= _initX && x < _initX + _heightX && y >= _initY && y <= _widthY + 4 )
            return true;
        return false;
    }
    void clearWater()
    {
        for ( int i = 0; i <= _heightX; ++ i )
            for ( int j = 0; j <= _widthY; ++ j )
            {
                mvwaddch(_waterWin,i,j,' ');
            }
        _isClear = true;
    }
    /* a thread func */
    void update()
    {
        /*use a window */
        
        start_color();
        init_pair( 1, COLOR_BLUE, COLOR_CYAN);
        init_pair( 2, COLOR_CYAN, COLOR_BLACK );
        init_pair( 3, COLOR_WHITE, COLOR_BLACK );
        while( ! _isClear )
        {
            //blue
            int maxStep = 3;
            for (int step = 0; step < maxStep; ++ step)
            {
                wattron(_waterWin,COLOR_PAIR(1));
                for ( int i = 0; i <= _heightX; ++ i )
                    for ( int j = 0; j <= _widthY; ++ j )
                    {
                        if ( (i+j) % 3 == step )
                            mvwaddch(_waterWin,i,j,'~');
                        else
                            mvwaddch(_waterWin,i,j,' ');
                    }
                wattroff(_waterWin,COLOR_PAIR(1));
                wrefresh(_waterWin);
                if ( _isClear )
                    break;
                sleep(1);
            }
        }
}

};


void die()
{
    //clear screen
    init_pair( 1, COLOR_RED, COLOR_RED);
    attron(COLOR_PAIR(1));
    for (int i = 0; i <= LINES; ++ i )
        for ( int j = 0; j <= COLS; ++j )
        {
            mvaddch(i,j,'O');
        }
    attroff(COLOR_PAIR(1));
    mvprintw(LINES / 2, COLS / 2 - 10,"YOU DIE!");
    mvprintw(LINES / 2 + 1, COLS / 2 - 10,"PRESS ANY KEY TO LEAVE!");
    getch();
}


int main()
{
	initscr();
	/*hide curser*/
	curs_set(0);
	Character yyp;
	yyp.initShow();
	refresh();
	/*multi thread*/
	Direction _direction;
    Water water;
	std::thread t1(&Water::update,&water);
	while (true)
	{
        _direction.getInput();
        //mvprintw(0,0,"dr: %d",_direction.getDirection());
		enumDirection dr = _direction.getDirection();
		//mvprintw(0,0,"             ");
		yyp.update(dr);
        refresh();
        if ( water.isInWater(yyp.getX(),yyp.getY()) )
        {
            water.clearWater();
            t1.join();
            die();
            break;
        }
		sleep(0.2);
	}
	getch();
	endwin();
	return 0;
}
