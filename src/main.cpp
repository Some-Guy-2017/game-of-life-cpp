#include <iostream>
#include <unistd.h>
#include <time.h>
#include "init.hpp"
#include <stdio.h>
using std::cout;
using std::endl;

#include <bitset>

double time_diff(clock_t);

int main() {

	bool running = true;

	while (running) {
        for (int i=0;i<tpf;i++) {
            if (game->run()) running = false;
        }
	}

    delete game;
}
