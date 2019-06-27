#include "common/stdafx.h"
#include "common/vectors.hpp"
#include "map_control.h"
#include "main.h"
#include <chrono>
#include <ctime>
bool render_2d = false;

void ConfigureScene(void)
{
    map.add_model("input/box_big.obj", Vector3(0.0, 0.0, 0.0));
    map.add_model("input/box_small.obj", Vector3(0.5, 0.5, 1.0));
    // map.add_model("input/Heart.obj", Vector3(0, 20, -20));
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'd':
        camera[0] += 1.0;
        break;
    case 'a':
        camera[0] -= 1.0;
        break;
    case 'w':
        camera[1] += 1.0;
        break;
    case 's':
        camera[1] -= 1.0;
        break;
    case 'z':
        camera[2] += 1.0;
        break;
    case 'x':
        camera[2] -= 1.0;
        break;
    case '1':
        camera[3] += 1.0;
        break;
    case '2':
        camera[3] -= 1.0;
        break;
    case '3':
        camera[4] += 1.0;
        break;
    case '4':
        camera[4] -= 1.0;
        break;
    case '5':
        camera[5] += 1.0;
        break;
    case '6':
        camera[5] -= 1.0;
        break;
    case 'r':
        if (render_2d == false)
        {
            map.deleteBuffer3D();
            PhotonMapping();
        }
        render_2d = !render_2d;
        break;
    case 27:
        exit(0);
        break;
    }
    glutSetWindow(mainWindow);
    glutPostRedisplay();
}

void Render(void)
{
    glClearColor(0.5, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (render_2d == true)
    {
        map.render2D();
    }
    else
    {
        map.render3D(camera);
    }
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1, 3000.0);
    glMatrixMode(GL_MODELVIEW);
}

void PhotonMapping(void)
{
    std::chrono::system_clock::time_point start, end;
    std::chrono::duration<double> elapsed_seconds;

    // ray tracing
    std::cout << "Start ray tracing.." << std::endl;
    start = std::chrono::system_clock::now();
    map.photons_emit();
    std::cout << "Photon emitted." << std::endl;
    map.photons_modify();
    map.printResult();
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "End ray tracing..\nElapsed time: " << elapsed_seconds.count() << " s\n";

    // rendering
    std::cout << "Start rendering.." << std::endl;
    start = std::chrono::system_clock::now();
    map.writeBuffer2D(camera, scrn_width, scrn_height);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "End rendering..\nElapsed time: " << elapsed_seconds.count() << " s\n";
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(scrn_pos_x, scrn_pos_y);
    glutInitWindowSize(scrn_width, scrn_height);
    mainWindow = glutCreateWindow("VIZ");
#ifndef __APPLE__
    glewInit();
#endif
    ConfigureScene(); // initializing

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    map.writeBuffer3D(); // 3D buffer

    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
