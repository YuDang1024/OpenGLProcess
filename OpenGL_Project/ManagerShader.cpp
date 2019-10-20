//
//  ManagerShader.cpp
//  OpenGL_Project
//
//  Created by DangYu on 2019/9/11.
//  Copyright © 2019 DangYu. All rights reserved.
//

#include "ManagerShader.hpp"


Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    std::string vertextCode;
    std::string fragmentCode;
    std::ifstream vshaderfile;
    std::ifstream fshaderfile;
    
    try {
        //1、获取shader的字符串
        // 确保输入流可以抛出异常
        vshaderfile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fshaderfile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        
        //打开文件
        vshaderfile.open(vertexPath);
        fshaderfile.open(fragmentPath);
        
        std::stringstream vertextstream,fragmentstream;
        
        //读取文件的缓冲内容到数据流中
        vertextstream<<vshaderfile.rdbuf();
        fragmentstream<<fshaderfile.rdbuf();
        
        //将数据流传输到文件中去
        vertextCode = vertextstream.str();
        fragmentCode = fragmentstream.str();
        
        // 关闭文件处理器
        vshaderfile.close();
        fshaderfile.close();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    //2、编译shader
    const char* vshadercode = vertextCode.c_str();
    const char* fshadercode = fragmentCode.c_str();
    
    unsigned int vertextID,fragmentID;
    int successfule;
    char infoLog[1024];
    
    // 编译顶点shader
    vertextID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertextID,1,&vshadercode,NULL);
    glCompileShader(vertextID);
    glGetShaderiv(vertextID, GL_COMPILE_STATUS, &successfule);
    if(!successfule)
    {
        glGetShaderInfoLog(vertextID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // 编译片段shader
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentID,1,&fshadercode,NULL);
    glCompileShader(fragmentID);
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &successfule);
    if(!successfule)
    {
        glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // 创建一个着色器对象，将顶点和片段着色器管理起来
    ID = glCreateProgram();
    glAttachShader(ID,vertextID);
    glAttachShader(ID,fragmentID);
    glLinkProgram(ID);
    
    glGetProgramiv(ID, GL_LINK_STATUS, &successfule);
    if(!successfule)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // 删除shader
    glDeleteShader(vertextID);
    glDeleteShader(fragmentID);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
