#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
using namespace std;

/* ----------------------------------------------------------------------------
// Authors: Anna Jinneman & William Roberts
// Date completed: 2/3/2020
// Resources used: The Cherno (Youtube Tutorial), Learn OpenGL (Website Tutorial)
// ----------------------------------------------------------------------------
// This program will use OpenGL to create a RGB Triangle from user input.
-----------------------------------------------------------------------------*/


//The structure for the Point we use to organize our data. Simple stuff.
struct point {
    float x;
    float y;

    float r;
    float g;
    float b;

};

//Mainly for debugging, and for outputting data to the screen.
std::ostream& operator<<(std::ostream& stream, const point& v) {
    // std::cout is a std::ostream object, just like stream
    // so use stream as if it were cout and output the components of
    // the vector

    stream << v.x << ", " << v.y << ": " << v.r << ", " << v.g << ", " << v.b << endl;

    return stream;
}

// Compiles the source of the basic shaders below
static unsigned int CompileShader(unsigned int type, const std::string& source) //This compiles the source
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    //Basic Debugger to check what failed to compile
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); //Allocates a generic length char array. This shouldn't ever be called, but works if it is.
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!"  << std::endl;
        std::cout << message << std::endl; //Gives a decent error message. 
        glDeleteShader(id);

        return 0;

    }

    return id;
}

// Run compiler on the vs and fs, links them to the program, then deletes them.
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void normalizeCoord(point* point) //This function updates the values to normalize values (-1 to 1)
{
    point->x = (-1.0f + point->x * (2.0f / (float)SCR_WIDTH)); 
    point->y = (1.0f - point->y * (2.0f / (float)SCR_HEIGHT));
}



/* These functions will convert normal coords to window coords. We didn't end up using these (for obvious reasons)
float w2ndx(float x)
{
    float result = -1.0f + (x * (2.0f / 800.0f));
    return result;
}

float w2ndy(float y)
{
    float result = 1.0f - (y * (2.0f / 600.0f));
    return result;
}
*/


//------------------------------------------------------------------------------------
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // Set up Data initialization:
    // ---------------------------------------

    point a;
    point b;
    point c;


    // User Input Prompt:
    cout << "Enter 3 points (enter a point as x,y:r,g,b): " << endl;

    cout << "Enter point 1: " << endl;
    string userInput1;
    cin >> a.x >> a.y >> a.r >> a.g >> a.b;

    cout << "Enter point 2: " << endl;
    string userInput2;
    cin >> b.x >> b.y >> b.r >> b.g >> b.b;

    cout << "Enter point 3: " << endl;
    string userInput3;
    cin >> c.x >> c.y >> c.r >> c.g >> c.b;

    cout << "You entered: " << endl;
    cout << a << b << c << endl;

    
    normalizeCoord(&a);
    normalizeCoord(&b);
    normalizeCoord(&c);

    
    /*
    a.x = w2ndx(a.x);
    a.y = w2ndy(a.y);

    b.x = w2ndx(b.x);
    b.y = w2ndy(b.y);

    c.x = w2ndx(c.x);
    c.y = w2ndy(c.y);
    */

    cout << "Converted: " << endl;
    cout << a << b << c << endl;

    float positions[] = {
        a.x, a.y, 0.0f, a.r, a.g, a.b, 
        b.x, b.y, 0.0f, b.r, b.g, b.b,
        c.x, c.y, 0.0f, c.r, c.g, c.b

    };

    //Buffer and VertexArray initialization:
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); // This tells OpenGL how to organize the buffer.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float))); // This tells OpenGL how to organize the buffer.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Basic Shaders for the vertexShader and fragmentShader
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "\n"
        "out vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "   ourColor = aColor; \n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0)out vec4 color;\n"
        "in vec3 ourColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(ourColor, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------

        
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}