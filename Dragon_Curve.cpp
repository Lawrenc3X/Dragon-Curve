#include <ncurses.h>
#include <math.h>
#include <vector>
#include <array>
#include <string.h>
#include <unistd.h>

#define ITERATIONS 16
#define SLEEP 1000

typedef struct dragon
{
    std::vector<char> curve;
    std::array<int, 2> position;
    int index;
    int direction;

    std::vector<int> directions;

    dragon(int x, int y)
    {
        curve.push_back('R');
        position = (std::array<int, 2>) {x, y};
        index = 0;
        direction = 1;
    }

    void reset(int x, int y)
    {
        curve.clear();
        curve.push_back('R');
        position = (std::array<int, 2>) {x, y};
        index = 0;
        direction = 0;
    }
    
    void iterate()
    {
        std::vector<char> appendix;
        appendix.push_back('R');

        for (int i = curve.size() - 1; i >= 0; i--)
        {
            char c = curve[i];

            if (c == 'L')
            {
                appendix.push_back('R');
            } else
            {
                appendix.push_back('L');
            }
        }

        curve.insert(curve.end(), appendix.begin(), appendix.end() );
    }

    void drawSegment() // update position also
    {
        if (direction < 0)
        {
            direction += 4;
        }
        direction %= 4;

        chtype c;
        c = inch();
        switch (direction)
        {
            case 0:                
                if (c == ACS_LTEE)
                {
                    addch(ACS_LLCORNER);
                } else if (c == ACS_RTEE)
                {
                    addch(ACS_LRCORNER);
                } else if (c == ACS_PLUS)
                {
                    // do nothing
                } else
                {
                    addch(ACS_BTEE);
                }
                move(position[1] - 1, position[0]);

                chtype c2;
                c2 = inch();
                if (    c2 == ACS_ULCORNER || 
                        c2 == ACS_URCORNER || 
                        c2 == ACS_LLCORNER || 
                        c2 == ACS_LRCORNER || 
                        c2 == ACS_TTEE || 
                        c2 == ACS_BTEE || 
                        c2 == ACS_LTEE || 
                        c2 == ACS_RTEE)
                {
                    addch(ACS_PLUS);
                } else
                {
                    addch(ACS_TTEE);
                }
                move(position[1] - 1, position[0]);
                break;

            case 1:
                if (c == ACS_BTEE)
                {
                    addch(ACS_LLCORNER);
                } else if (c == ACS_TTEE)
                {
                    addch(ACS_ULCORNER);
                } else if (c == ACS_PLUS)
                {
                    // do nothing
                } else
                {
                    addch(ACS_LTEE);
                }
                move(position[1], position[0] + 1);

                addch(ACS_HLINE);

                c2 = inch();
                if (    c2 == ACS_ULCORNER || 
                        c2 == ACS_URCORNER || 
                        c2 == ACS_LLCORNER || 
                        c2 == ACS_LRCORNER || 
                        c2 == ACS_TTEE || 
                        c2 == ACS_BTEE || 
                        c2 == ACS_LTEE || 
                        c2 == ACS_RTEE)
                {
                    addch(ACS_PLUS);
                } else
                {
                    addch(ACS_RTEE);
                }
                move(position[1], position[0] + 2);
                break;

            case 2:
                if (c == ACS_LTEE)
                {
                    addch(ACS_ULCORNER);
                } else if (c == ACS_RTEE)
                {
                    addch(ACS_URCORNER);
                } else if (c == ACS_PLUS)
                {
                    // do nothing
                } else
                {
                    addch(ACS_TTEE);
                }
                move(position[1] + 1, position[0]);

                c2 = inch();
                if (    c2 == ACS_ULCORNER || 
                        c2 == ACS_URCORNER || 
                        c2 == ACS_LLCORNER || 
                        c2 == ACS_LRCORNER || 
                        c2 == ACS_TTEE || 
                        c2 == ACS_BTEE || 
                        c2 == ACS_LTEE || 
                        c2 == ACS_RTEE)
                {
                    addch(ACS_PLUS);
                } else
                {
                    addch(ACS_BTEE);
                }
                move(position[1] + 1, position[0]);
                break;

            case 3:
                if (c == ACS_BTEE)
                {
                    addch(ACS_LRCORNER);
                } else if (c == ACS_TTEE)
                {
                    addch(ACS_URCORNER);
                } else if (c == ACS_PLUS)
                {
                    // do nothing
                } else
                {
                    addch(ACS_RTEE);
                }
                move(position[1], position[0] - 1);

                addch(ACS_HLINE);
                move(position[1], position[0] - 2);

                c2 = inch();
                if (    c2 == ACS_ULCORNER || 
                        c2 == ACS_URCORNER || 
                        c2 == ACS_LLCORNER || 
                        c2 == ACS_LRCORNER || 
                        c2 == ACS_TTEE || 
                        c2 == ACS_BTEE || 
                        c2 == ACS_LTEE || 
                        c2 == ACS_RTEE)
                {
                    addch(ACS_PLUS);
                } else
                {
                    addch(ACS_LTEE);
                }
                move(position[1], position[0] - 2);
                break;
        }

        directions.push_back(direction);

        refresh(); // to update cursor position

        int x, y;
        getyx(curscr, y, x);
        position = (std::array<int, 2>) {x, y};        
    }

    void draw()
    {
        move(position[1], position[0]);
        
        if (index == 0)
        {
            drawSegment();
        }

        for (int i = index; i < curve.size(); i ++)
        {
            // usleep(SLEEP);

            char c = curve[i];
            if (c == 'L')
            {
                direction -= 1;
            } else
            {
                direction += 1;
            }
            
            drawSegment();
        }
    }

    void printCurve()
    {
        move(0, 0);
        for (char c: curve)
        {
            printw("%c", c);
        }
    }

    void printDirections()
    {
        move(60, 0);
        for(int i: directions)
        {
            printw("%d", i);
        }
    }
} dragon;

int main()
{
    initscr();
    noecho();
    keypad(stdscr, true);
    
    dragon curve(350, 350);
    for (int i = 0; i < ITERATIONS; i++)
    {
        curve.iterate();
    }
    curve.draw();
//     curve.printCurve();
//     curve.printDirections();
    refresh();
    
    getch();
    endwin();
    return 0;
}
