#include <iostream>
#include <SDL.h>
#include <chrono>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <GL/glew.h>
#include <sstream>

using namespace std;

string getShaderCompilationLog(GLuint shader) {
        GLint msgSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgSize);
        if(msgSize < 0) {
                cerr << "shaderiv returned wrong param!" << endl;
                return string("shader returned wrong parameter");
        }
        std::vector<GLchar> log((unsigned long)msgSize);
        glGetShaderInfoLog(shader, msgSize, NULL, &log[0]);
        string msg(log.begin(), log.end());
        return msg;
}

GLuint loadShaderFromFile(GLenum type, const std::string &filePath) {
        using namespace boost::filesystem;
        GLuint shader = glCreateShader(type);
        path file(filePath);
        boost::system::error_code error;
        if (!exists(file, error)) {
                cerr << "Error: " << error.message() << endl;
                return 0;
        }

        boost::filesystem::ifstream fstream(file);
        std::stringstream buffer;
        buffer << fstream.rdbuf();
        std::string result = buffer.str();

        const char *tmp = result.c_str();
        glShaderSource(shader, 1, &tmp, nullptr);

        return shader;
}

GLint compileShader(GLenum shaderType, GLuint shader) {
        glCompileShader(shader);
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
                stringstream ss;
                ss << "Failed to compile ";
                switch (shaderType) {
                        case GL_VERTEX_SHADER:
                                ss << "vertex shader";
                                break;
                        case GL_FRAGMENT_SHADER:
                                ss << "fragment shader";
                                break;
                        default:
                                ss << "unknown shader";
                                break;
                }
                ss << "\n" << getShaderCompilationLog(shader);
                cerr << ss.str() << endl;
        }
        return status;
}

GLuint createShaderProgram(const std::vector<GLuint> &shaders) {
        GLuint program = glCreateProgram();
        for (int i = 0; i < shaders.size(); i++) {
                glAttachShader(program, shaders[0]);
        }
        glLinkProgram(program);
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
                cerr << "Failed to create program" << endl;
        }
        for (int i = 0; i < shaders.size(); i++) {
                glDetachShader(program, shaders[i]);
        }
        glUseProgram(program);
        return program;

}

int initializeShaders() {
        const string fragmentFilePath = "/home/mhl/Sources/Pongl/shaders/fragment.glsl";
        const string vertexFilePath = "/home/mhl/Sources/Pongl/shaders/vertex.glsl";

        GLuint fragmentShader = loadShaderFromFile(GL_FRAGMENT_SHADER, fragmentFilePath);
        GLuint vertexShader = loadShaderFromFile(GL_VERTEX_SHADER, vertexFilePath);

        GLint status = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
        if (status == GL_FALSE) {
                cerr << "Failed to initialize shaders" << endl;
                return -1;
        }
        status = compileShader(GL_VERTEX_SHADER, vertexShader);
        if (status == GL_FALSE) {
                cerr << "Failed to initialize shaders" << endl;
                return -1;
        }

        std::vector<GLuint> shaders;
        shaders.push_back(fragmentShader);
        shaders.push_back(vertexShader);

        status = createShaderProgram(shaders);
        if (status == GL_FALSE) {
                cerr << "Failed to create shader program" << endl;
                return -2;
        }

        return 0;
}

const float vertexPositions[] = {
        0.75f, 0.75f, 0.0f, 1.0f,
        0.75f, -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
};

GLuint positionBufferObject;
GLuint vao;

int initGL() {
        int status = initializeShaders();
        if (status != 0) {
                cerr << "Something went wrong with shaders" << endl;
                return -1;
        }
        // initialize vertex buffer
        glGenBuffers(1, &positionBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        return 0;
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


static void draw() {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);
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
        initGL();

        bool quitting = false;
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
                draw();
                SDL_GL_SwapWindow(mainWindow);

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