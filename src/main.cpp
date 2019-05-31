#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

#include <GL/glew.h>
#include "render/Window.hpp"
#include "render/opengl/GLCompiler.hpp"
#include "render/opengl/GLProgram.hpp"
#include <SDL.h>
#include <SDL_opengl.h>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

/*
 * http:*nehe.gamedev.net/article/replacement_for_gluperspective/21002/
 *
 * Replaces gluPerspective. Sets the frustum to perspective mode.
 * fovY     - Field of vision in degrees in the y direction
 * aspect   - Aspect ratio of the viewport
 * zNear    - The near clipping distance
 * zFar     - The far clipping distance
 */

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {

    GLdouble fW, fH;

    fH = tan(fovY / 360 * M_PI) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);

}

void Display_InitGL() {

    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH);

    /* Set the background red */
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

    /* Depth buffer setup */
    glClearDepth(1.0f);

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST);

    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL);

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/* function to reset our viewport after a window resize */
int Display_SetViewport(int width, int height) {

    /* Height / width ration */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if (height == 0) {
        height = 1;
    }

    ratio = (GLfloat) width / (GLfloat) height;

    /* Setup our viewport. */
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION);
    glLoadIdentity();

    /* Set our perspective */
    perspectiveGL(45.0f, ratio, 0.1f, 100.0f);

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW);

    /* Reset The View */
    glLoadIdentity();

    return 1;
}

void Display_Render() {

    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor4f(1, 1, 1, 1);

    /* Move Left 1.5 Units And Out of The Screen 6.0 */
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, 6.0f);

    glBegin( GL_TRIANGLES); /* Drawing Using Triangles */
    glVertex3f(0.0f, 1.0f, 0.0f); /* Top */
    glVertex3f(-1.0f, -1.0f, 0.0f); /* Bottom Left */
    glVertex3f(1.0f, -1.0f, 0.0f); /* Bottom Right */
    glEnd(); /* Finished Drawing The Triangle */

    /* Move Right 3 Units */
    glTranslatef(3.0f, 0.0f, 0.0f);

    glBegin( GL_QUADS); /* Draw A Quad */
    glVertex3f(-1.0f, 1.0f, 0.0f); /* Top Left */
    glVertex3f(1.0f, 1.0f, 0.0f); /* Top Right */
    glVertex3f(1.0f, -1.0f, 0.0f); /* Bottom Right */
    glVertex3f(-1.0f, -1.0f, 0.0f); /* Bottom Left */
    glEnd(); /* Done Drawing The Quad */
}

int main(int argc, char *argv[]) {
    using namespace zero::render;
    using namespace std;

    WindowConfig window_config{WindowFlags::WINDOW_MAXIMIZED,
                               512,
                               512,
                               WindowMode::WINDOW_WINDOWED,
                               GraphicsAPI::GRAPHICS_OPENGL,
                               RefreshRate::REFRESH_IMMEDIATE,
                               "Sample Window",
                               ""};
    Window window(window_config);

    // Initialize window
    window.Initialize();

    GLCompiler compiler;

    // Initialize Shaders
    string vertex_source = "void main() { gl_Position = gl_Vertex; }";
    string fragment_source = "uniform vec4 color; void main() { gl_FragColor = vec4(1,1,0,1); }";
    GLCompiler::ShaderStage vertex_stage{ IShader::Type::TYPE_VERTEX_SHADER, "Vertex Shader", vertex_source };
    GLCompiler::ShaderStage fragment_stage{ IShader::Type::TYPE_FRAGMENT_SHADER, "Fragment Shader", fragment_source };
    bool vertex_res = compiler.InitializeShader(vertex_stage);
    bool fragment_res = compiler.InitializeShader(fragment_stage);

    // Create Program
    Material material;
    material.shaders_.vertex_shader_ = "Vertex Shader";
    material.shaders_.fragment_shader_ = "Fragment Shader";
    auto program = compiler.CreateProgram(material);

    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EventType::SDL_QUIT) quit = true;
        }

        // Draw with program
        program->Use();
        Display_InitGL();
        Display_SetViewport(SCREEN_WIDTH, SCREEN_HEIGHT);
        Display_Render();
        program->Finish();

        // Swap
        window.SwapBuffers();
    }

    return 0;
}