#pragma once

#include <termios.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/poll.h>

#define IGNORE_SIG true  // if you want to ignore the stop signals like ctrl-c or ctrl-d

enum Keys {
    KEY_NULL,
    KEY_ESCAPE,
    KEY_TAB,
    KEY_UP,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_BACKSPACE,
    KEY_HOME,
    KEY_END,
    // KEY_DELETE,
    // KEY_INSERT,
} typedef Keys;

struct Key {
    char ch;
    Keys key;
    bool shift;
    bool ctrl;
} typedef Key;

// Initialize new terminal i/o settings
void input_init(bool echo);

// Initialize new terminal i/o settings
// echo = false
void input_init();

// Restore old terminal i/o settings
void input_quit();

// Returns the number of characters available in stdin
int available();

// Check if characters are available in stdin
bool hasch();

// Read 1 character
char getch();

// Wait for a key event
void getkey(Key *key);
