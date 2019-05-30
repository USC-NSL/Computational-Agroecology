#include "common/stdafx.h"
#include "common/vectors.hpp"
#include "map_control.h"
#include "model/model.h"
#include "main.h"
#include <chrono>
#include <ctime>
bool render_2d = false;
unsigned int texture;
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
void Render3D(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], camera[6], camera[7], camera[8]);
    map.render();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
}
void Render2D(void)
{
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0);
    glVertex3f(1, -1, -1);
    glTexCoord2f(1, 1);
    glVertex3f(1, 1, -1);
    glTexCoord2f(0, 1);
    glVertex3f(-1, 1, -1);
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}
void Render(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (render_2d == true)
    {
        Render2D();
    }
    else
    {
        Render3D();
    }
}
void writeBuffer2D(void)
{
    Vector3 rgb(1.0, 1.0, 1.0);
    GLuint vboId;
    std::cout << camera[0] << " " << camera[1] << " " << camera[2] << std::endl;
    std::cout << camera[3] << " " << camera[4] << " " << camera[5] << std::endl;
    std::cout << camera[6] << " " << camera[7] << " " << camera[8] << std::endl;
    buffer.clear();
    for (int y = 0; y < scrn_height; y++)
    {
        for (int x = 0; x < scrn_width; x++)
        {
            rgb = map.get_ray_color(x, y, scrn_width, scrn_height,
                                    Vector3((real_t)camera[0], (real_t)camera[1], (real_t)camera[2]),
                                    Vector3((real_t)camera[3], (real_t)camera[4], (real_t)camera[5]),
                                    Vector3((real_t)camera[6], (real_t)camera[7], (real_t)camera[8]));
            buffer.push_back(rgb.x);
            buffer.push_back(rgb.y);
            buffer.push_back(rgb.z);
        }
        if (!(y % 100))
        {
            std::cout << "Rendering pixel " << y << std::endl;
        }
    }
    std::cout << "Writing to buffer.." << std::endl;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 (GLsizei)scrn_width, (GLsizei)scrn_height,
                 0, GL_RGB, GL_FLOAT, &buffer.at(0));
    glDisable(GL_TEXTURE_2D);
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1, 3000.0);
    glMatrixMode(GL_MODELVIEW);
}
void ConfigureScene(void)
{
    map.add_model("input/Corn1.obj", Vector3(0, 0, 0));
    // map.add_model("input/box_small.obj", Vector3(7, 7, 10));
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
    writeBuffer2D();
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
