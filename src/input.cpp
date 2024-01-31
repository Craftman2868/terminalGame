#include "input.hpp"

#include <termios.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <unistd.h>

static struct termios old, current;

// Initialize new terminal i/o settings
void input_init(bool echo)
{
    tcgetattr(0, &old);         // grab old terminal i/o settings
    current = old;              // make new settings same as old settings
    current.c_lflag &= ~ICANON; // disable buffered i/o
    current.c_iflag &= ~IXON;   // enable Ctrl-S and Ctrl-Q detection

    if (echo)
        current.c_lflag |= ECHO; // set echo mode
    else
        current.c_lflag &= ~ECHO; // set no echo mode

#if IGNORE_SIG
    current.c_lflag &= ~ISIG; // ignore INTR, QUIT, SUSP, or DSUSP signals
#endif

    tcsetattr(0, TCSANOW, &current); // use these new terminal i/o settings now
}

// Initialize new terminal i/o settings
// echo = false
void input_init()
{
    return input_init(false);
}

// Restore old terminal i/o settings
void input_quit()
{
    tcsetattr(0, TCSANOW, &old);
}

int available()
{
    int byteswaiting;

    ioctl(0, FIONREAD, &byteswaiting);

    return byteswaiting;
}

// Check if characters are available in stdin
bool hasch()
{
    return available() > 0;
}

// Read 1 character
char getch()
{
    return getchar();
    // char ch = getchar();
    // printf("%d", ch);
    // return ch;
}

// Wait for a key event
void getkey(Key *res)
{
    // printf("gk");

    res->key = KEY_NULL;

    char ch = getch();

    if (ch == '\033') // escape
    {
        // if (hasch())
        {
            ch = getch();

            if (ch == '[') // CSI
            {
                ch = getch();

                switch (ch)
                {
                case 'A':
                    res->key = KEY_UP;
                    break;
                case 'B':
                    res->key = KEY_DOWN;
                    break;
                case 'C':
                    res->key = KEY_RIGHT;
                    break;
                case 'D':
                    res->key = KEY_LEFT;
                    break;
                case 'F':
                    res->key = KEY_END;
                    break;
                case 'H':
                    res->key = KEY_HOME;
                    break;
                default:
                    ungetc(ch, stdin);
                    break;
                }
            }
            else
            {
                ungetc(ch, stdin);
            }
        }
        // else
        //     res->key = KEY_ESCAPE;
    }

    if (ch == '\177')
        res->key = KEY_BACKSPACE;
    if (ch == '\011')
        res->key = KEY_TAB;
    if (ch == 0)
    {
        res->ch = ' ';
        res->ctrl = true;
    }

    if (res->key)
        ch = 0;

    if (isalpha(ch) && isupper(ch))
    {
        res->shift = true;
    }
    else
    {
        ch = toupper(ch);
        res->shift = false;
    }

    if (ch >= 1 && ch <= 26)
    {
        ch = 64 + ch;
        res->ctrl = true;
    }
    else
        res->ctrl = false;

    res->ch = ch;

    // printf("(%d %d)", res->ch, res->key);
}

bool getkey_noblock(Key *res)
{
    if (hasch())
    {
        getkey(res);
        return true;
    }

    res->key = KEY_NULL;
    res->ch = 0;

    return false;
}
