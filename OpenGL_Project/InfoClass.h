//
//  InfoClass.hpp
//  OpenGL_Project
//
//  Created by DangYu on 2019/8/10.
//  Copyright © 2019 DangYu. All rights reserved.
//
#pragma once

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#define MAX_SIZE 1024

using namespace std;

class Info
{
private:
    const GLubyte * _name;
    const GLubyte * _biaoshifu;
    const GLubyte * _OpenGLVersion;
    const GLubyte* _glslVersion;
    
public:
    Info();
    ~Info();
    
    // OpenGLd厂商名字
    char* GetOpenGLName();
    // 渲染标识符
    char* GetBiaoshifu();
    // OpenGL版本号
    char* GetNumber();
    // 着色器版本号
    char* GetGLSLVersion();
    
    
};

