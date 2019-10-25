#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "math.h"
#include "ManagerShader.hpp"
#include "../include/SOIL/SOIL.h"
#include "../include/SOIL/stb_image_aug.h"


#include <iostream>

// 当窗口大小改变的时候回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理窗口里面输入时间的函数
static void Key_callback(GLFWwindow *window,int key, int scancode, int action, int mods);
void ReverseImage(int width, int height,unsigned char * data,int chanel);

// 窗口宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
static float percent = 0.0;

// 因为SOIL获取的图像数据是反过来的，所以需要将数据反转一下
void ReverseImage(int width, int height,unsigned char * data,int chanel)
{
    int i,j;
    for (j=0; j*2<height; j++) {
        int index1 = j * width * chanel;
        int index2 = (height-1-j) * width * chanel;
        for (i = width * chanel; i>0; --i) {
            unsigned char temp = data[index1];
            data[index1] = data[index2];
            data[index2] = temp;
            index2++;
            index1++;
        }
    }
}

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
    
    Shader shader("../GLSL/vertex.glsl","../GLSL/fragment.glsl");
    
    // 加载纹理部分
    int width1,height1,Channels1;
    unsigned char* data1 = stbi_load("../assert/img_cheryl.jpg",&width1,&height1,&Channels1,0);
    
    
    unsigned int texture[2];
    // 第一个参数我们需要告诉OpenGL生成纹理的数量，并将其储存在unsigned int的数组中，因为现在我们只生成一个纹理，所以就不同数组了，使用一个单独的值
    glGenTextures(2,texture);
    // 绑定纹理数组，让之后的纹理操作都在这个纹理上
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置过滤方式
    // 纹理缩小的时候用临近过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // 纹理放大的时候用线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 使用前面图像的data数据生成纹理
    if (data1) {
        ReverseImage(width1, height1, data1, Channels1);
        /*
         参数解析：
         1、指定了要将纹理生成在哪一个纹理对象上
         2、为纹理设置多级渐进纹理的级别，如果希望手动设置的话，此处可以设为0，表示只设置第一级别的纹理，如果要设置其他级别的纹理，需要改动此参数
         3、告诉OpenGL我们希望把纹理设置成哪一种储存模式
         4、5、纹理原图像的宽高
         6、文档上写的是历史遗留问题，设置为0就行
         7、告诉OpenGL原图的格式
         8、告诉OpenGL原图的类型，因为我们获取的是char(byte)类型的数据，所以此处传GL_UNSIGNED_BYTE
         9、图像的数据
         */
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width1,height1,0,GL_RGB,GL_UNSIGNED_BYTE,data1);
        glGenerateMipmap(GL_TEXTURE_2D);
        //glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width2,height2,0,GL_RGB,GL_UNSIGNED_BYTE,data2);
        // 为当前绑定的纹理生成所有级别的渐进纹理
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout<<"使用SOIL加载图像数据失败"<<std::endl;
    }
    
    // 加载第二张纹理数据
    int width2,height2,Channels2;
    unsigned char* data2 = stbi_load("../assert/stones.jpg",&width2,&height2,&Channels2,0);
    glBindTexture(GL_TEXTURE_2D,texture[1]);
    if (data2) {
        ReverseImage(width2, height2, data2, Channels2);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width2,height2,0,GL_RGB,GL_UNSIGNED_BYTE,data2);
        // 为当前绑定的纹理生成所有级别的渐进纹理
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout<<"使用SOIL加载图像数据失败"<<std::endl;
    }
    
    
    // 生成纹理之后，释放刚才的图像数据
    stbi_image_free(data1);
    stbi_image_free(data2);
    
    
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
        // 位置             // 颜色          //纹理坐标
        0.5f,0.5f,0.0f,     1.0f,0.0f,0.0f,  1.0f,1.0f,
        0.5f,-0.5f,0.0f,    0.0f,1.0f,0.0f,  1.0f,0.0f,
        -0.5f,-0.5f,0.0f,   0.0f,0.0f,1.0f,  0.0f,0.0f,
        -0.5f,0.5f,0.0f,    1.0f,1.0f,1.0f,  0.0f,1.0f,
    };
    
    unsigned int index[] = {
        0,1,2,
        2,3,0,
    };
    
    unsigned int diamond_index[] ={
        0,2,3,
        0,2,1,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
    
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
        //Key_callbac(window);
        glfwSetKeyCallback(window, Key_callback);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw our first triangle
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture[1]);
        
        shader.use();
        shader.setInt("texture1", 1);
        shader.setInt("texture2", 0);
        shader.setFloat("percent", percent);
        
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glUseProgram(shaderProgram);
        
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
static void Key_callback(GLFWwindow *window,int key, int scancode, int action, int mods)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        percent += 0.1f;
        if (percent > 1.0f) {
            percent = 1.0f;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        
        percent -= 0.1f;
        if (percent < 0.0f) {
            percent = 0.0f;
        }
    }
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
