#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "math.h"
#include "ManagerShader.hpp"

#include <iostream>

// 当窗口大小改变的时候回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理窗口里面输入时间的函数
void processInput(GLFWwindow *window);

// 窗口宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
/*
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0\n"
"layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1\n"
"out vec3 ourColor; // 向片段着色器输出一个颜色\n"
"void main()\n"
"{\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    "ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色\n"
"}\n";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
    "FragColor = vec4(ourColor, 1.0);\n"
"}\n";
*/
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
    
/*..封装成了一个类
    // 以下是构架和编译shader程序
    // ------------------------------------
    // 顶点 shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 设置顶点源文件
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    // 检测编译的顶点着色器是否成功
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // 片段 shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 检测片段着色器是否编译成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // 创建一个着色器对象，这个着色器对象可以管理我们创建的各个着色器，将他们按照位置放进渲染管线里面
    int shaderProgram = glCreateProgram();
    // 将我们创建的顶点和片段着色器附加到这个对象上面
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    // 将对象上的着色器按照渲染管线链接起来
    glLinkProgram(shaderProgram);
    // 检查链接的结果是否正确
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        // 获得链接的错误信息
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
*/
    
    Shader shader("/Users/dy/Desktop/OpenGL_Project/OpenGL_Project/GLSL/vertex.glsl","/Users/dy/Desktop/OpenGL_Project/OpenGL_Project/GLSL/fragment.glsl");
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices_tr[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
        0.0f,  0.5f, 0.0f,  // top
    };
    
    float vertices_sq[] ={
        -0.5f,-0.5f,0.0f,
        -0.5f,0.5f,0.0f,
        0.5f,0.5f,0.0f,
        0.5f,-0.5f,0.0f,
    };
    
    float vertices_diamond[] ={
        // 位置             // 颜色
        -0.5f,0.0f,0.0f,   1.0f,0.0f,0.0f,
        0.5f,0.0f,0.0f,    0.0f,1.0f,0.0f,
        0.0f,0.5f,0.0f,    0.0f,0.0f,1.0f,
        0.0f,-0.5f,0.0f,   1.0f,1.0f,1.0f,
    };
    
    unsigned int index[] = {
        0,1,2,
        2,3,0,
    };
    
    unsigned int diamond_index[] ={
        0,1,2,
        0,1,3,
    };
    
    unsigned int VBO, VAO,IBO;
    
    // 创建一个VAO，用来代表当前的VBO
    // 理解：首先绑定VAO是因为，之后的操作都可以算作是对于绑定的VAO上面的，如果先去操作VBO，然后再去绑定VAO，然后结束，此时VAO上面是没有任何的操作的
    glGenVertexArrays(1, &VAO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&IBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_diamond), vertices_diamond, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(diamond_index),diamond_index,GL_STATIC_DRAW);
    
    
    // 第一个参数就是在顶点shader中定义的layout(location =0)
    // 第二个参数是指定顶点属性的大小，比如现在我们的顶点只有x,y,z三个属性值，所以这个值就是3，
    // 第三个参数指的是顶点属性中3个值的类型，现在这个三个值就是float类型，
    // 第四个参数指的是我们是否希望参数被标准化
    // 第五个参数指的是步长,现在我们一个顶点里面有三个数据，所以步长就是3个float，步长也就是一个顶点里面有个值，加了颜色之后就是6个float
    // 第六个参数指的是所需要的数据在顶点缓冲对象VBO里面的偏移量，因为我们现在使用的就是从开头到VBO结尾的数据，所以这个偏移量就是0
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
    
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
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
        
        // draw our first triangle
        
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glUseProgram(shaderProgram);
        shader.use();
        
        // 将片段着色器中的颜色值设置成随着时间变化(因为uniform相对于整个GLSL是全局变量，所以使用是在着色器对象生成之后，不一定非要在片段在shader的后面)
        //float timevalue = glfwGetTime();
        //float Curgreencolor = (sin(timevalue) / 0.2) + 0.5;
        //int ourcoloelocation = glGetUniformLocation(shaderProgram,"ourcolor");
        //glUniform4f(ourcoloelocation,0.0f,Curgreencolor,0.0f,1.0f);
        // 画三角形使用
        //glDrawArrays(GL_TRIANGLES, 0, 4);
        
        // 画矩形使用
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        // glBindVertexArray(0); // no need to unbind it every time
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
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
