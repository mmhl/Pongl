#include <iostream>
#include <SDL.h>

using namespace std;

int main() {
    cout << "SDL Start" << endl;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Can't initialize SDL" << endl;
        return -1;
    }
    cout << "OK" << endl;

    return 0;
}