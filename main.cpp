#include <iostream>
#include <SDL.h>
#include <chrono>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <GL/glew.h>
#include <sstream>

using namespace std;


GLuint loadShaderFromFile(GLenum type, const std::string &filepath) {
        using namespace boost::filesystem;
        GLuint shader = glCreateShader(type);
        path file(filepath);
        boost::system::error_code error;

        if (!exists(file, error)) {
                cerr << "Error: " << error.message() << endl;
                return 0;
        }
        boost::filesystem::ifstream fstream(file);
        std::stringstream sstr;
        while (fstream >> sstr.rdbuf()); // Read whole file to sstream

        cout << sstr.str() << endl;
        const char *str = sstr.str().c_str();
        glShaderSource(shader, 1, &str, NULL);

        return shader;
}

static int initGLEW() {
        glewExperimental = GL_TRUE;
        GLenum res = glewInit();
        if (res != 0) {
                cerr << "Unable to initialize GLEW properly" << endl;
                cerr << "\t" << glewGetErrorString(res);
                return -1;
        }
        return 0;

}

static int setGLWinContext(SDL_Window *win) {
        int ret;
        ret = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        ret |= SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        ret |= SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        ret |= SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        ret |= SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        ret |= SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        if (ret != 0) {
                cerr << "Something wrong happened when setting up gl attrs" << endl;
                return -1;
        }
        SDL_GL_CreateContext(win);
        return 0;
}

int main() {
        cout << "SDL Start" << endl;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                cerr << "Can't initialize SDL" << endl;
                return -1;
        }

        SDL_Window *mainWindow = SDL_CreateWindow("Hello SDL World", 10, 10, 800, 600,
                                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        int ret = setGLWinContext(mainWindow);
        if (ret != 0) {
                return -1;
        }
        ret = initGLEW();
        if (ret != 0) {
                return -1;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderClear(renderer);
        if (renderer == nullptr) {
                cerr << "Unable to create renderer" << endl;
        }
        SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);

        glClearColor(255, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(mainWindow);

        GLuint fragmentShader = loadShaderFromFile(GL_FRAGMENT_SHADER, "/home/mhl/Sources/Pongl/shaders/fragment.glsl");
        if (fragmentShader == 0) {
                cerr << "Can't load fragment shader" << endl;
                return -1;
        }

        GLuint vertexShader = loadShaderFromFile(GL_VERTEX_SHADER, "/home/mhl/Sources/Pongl/shaders/vertex.glsl");
        if (vertexShader == 0) {
                cerr << "Can't load vertex shader" << endl;
                return -1;
        }

        bool quitting = false;
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        cout << "Quitting" << endl;
                        quitting = true;
                }
                if (quitting)
                        break;
        }

        cout << "Done" << endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(mainWindow);

        return 0;
}