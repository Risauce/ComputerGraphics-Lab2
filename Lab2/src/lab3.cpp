#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "matrix.h"


#include <Shader/shader.h>

using namespace std;

Matrix3 rotation;
Matrix3 scale;
Matrix3 translate;

Matrix3 transform = translate * rotation * scale;

int state = 0;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/*
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        //glfwSetWindowShouldClose(window, true);
        cout << "Changed State" << endl;
        state++;
       

    }
}
*/

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



int main(void) {
    /* Initialize the library */
    GLFWwindow* window;
    if (!glfwInit()) {
        return -1;
    }

    


    /* Create a windowed state window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Lab 3", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwWaitEventsTimeout(0.7);
    glfwSetKeyCallback(window, key_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // tell glfw what to do on resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    

    // init glad
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    Matrix3 first = Matrix3(4.0f, 2, 0, 0, 8, 1, 0, 1, 0);
    Matrix3 second = Matrix3(4, 2, 1, 2, 0, 4, 9, 4, 2);
    Matrix3 scalar = Matrix3(3, 2, 1);

    Matrix3 sum = first * second;

    

    cout << sum.to_string() << endl;


    /* init the triangle drawing */
    // define the vertex coordinates of the triangle
    float triangle[] = {
         0.5f,  0.5f, 1.0, 0.0, 0.0,
         0.5f, -0.5f, 1.0, 1.0, 0.0,
        -0.5f,  0.5f, 0.0, 1.0, 0.0,

         0.5f, -0.5f, 1.0, 1.0, 0.0,
        -0.5f, -0.5f, 0.0, 0.0, 1.0,
        -0.5f,  0.5f, 0.0, 1.0, 0.0,
    };

    // create and bind the vertex buffer object and copy the data to the buffer
    GLuint VBO[1];
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // create and bind the vertex array object and describe data layout
    GLuint VAO[1];
    glGenVertexArrays(1, VAO);
    glBindVertexArray(VAO[0]);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // create the shaders
    Shader shader("../vert.glsl", "../frag.glsl");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        //processInput(window);
        

        /* Render here */

        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader
        shader.use();


        /** Part 2 animate and scene by updating the transformation matrix */

        switch (state)
        {
        case 0: //STATIC
            rotation = Matrix3(0.5f);
            //scale = Matrix3(0.7f, 0.2f, 0);
            //translate = Matrix3(1.0f , -0.5f);
            
            break;
        case 1: //ROTATE AROUND CENTER
            glClear(GL_COLOR_BUFFER_BIT);
            rotation = Matrix3(glfwGetTime());
            scale = Matrix3();
            translate = Matrix3();
            break;
        case 2: //TRANSLATION
            glClear(GL_COLOR_BUFFER_BIT);
            rotation = Matrix3();
            scale = Matrix3();
            translate = Matrix3(0.7f, -0.5f);
            break;
        case 3: //SCALE UP AND DOWN
            glClear(GL_COLOR_BUFFER_BIT);
            rotation = Matrix3();
            scale = Matrix3(0.7f * cos(glfwGetTime()), 0.2f * cos(glfwGetTime()), 0);
            translate = Matrix3();
            break;
        case 4://Everything
            glClear(GL_COLOR_BUFFER_BIT);
            rotation = Matrix3(glfwGetTime());
            scale = Matrix3(0.5f, 0.2f, 0);
            translate = Matrix3(0.5f * cos(glfwGetTime()), 0.2f * sin(glfwGetTime()));
            break;
        default: 
            glClear(GL_COLOR_BUFFER_BIT);
            state = 0;
            rotation = Matrix3();
            scale = Matrix3();
            translate = Matrix3();
            break;
        }

        

        transform = rotation * scale * translate;

        unsigned int transformLoc = glGetUniformLocation(shader.id(), "theMatrix");
        glUniformMatrix3fv(transformLoc, 1, GL_FALSE, transform.values);

        // draw our triangles
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(triangle));

        /* Swap front and back * buffers */
        glfwSwapBuffers(window);

        /* Poll for and * process * events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        state++;
        cout << state << endl;


    }
        
}
