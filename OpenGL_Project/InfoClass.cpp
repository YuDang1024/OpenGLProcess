//
//  InfoClass.cpp
//  OpenGL_Project
//
//  Created by DangYu on 2019/8/10.
//  Copyright © 2019 DangYu. All rights reserved.
//

#include "InfoClass.h"

Info::Info()
{
    _name = glGetString(GL_VENDOR);
    _biaoshifu = glGetString(GL_RENDERER);
    _OpenGLVersion = glGetString(GL_VERSION);
    _glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
}

Info::~Info()
{
    _name = NULL;
    _biaoshifu = NULL;
    _OpenGLVersion = NULL;
    _glslVersion = NULL;
}

char* Info::GetOpenGLName()
{
    char* name = NULL;
    sprintf(name, "%s",_name);
    return name;
}

char* Info::GetBiaoshifu()
{
    char* name = NULL;
    sprintf(name, "%s",_biaoshifu);
    return name;
}
// OpenGL版本号
char* Info::GetNumber()
{
    char* name = NULL;
    sprintf(name, "%s",_OpenGLVersion);
    return name;
}
// 着色器版本号
char* Info::GetGLSLVersion()
{
    char* name = NULL;
    sprintf(name, "%s",_glslVersion);
    return name;
}
