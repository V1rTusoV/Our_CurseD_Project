#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <ctime>
#include "game.h"
#include <cstdlib> 
#include <Windows.h>
#include <clocale>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    game::GameObject game;
    game.play();

    return 0;
}