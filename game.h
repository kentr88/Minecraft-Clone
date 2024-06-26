#pragma once
#include "text.h"
#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <list>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



extern const char *stbi_failure_reason(void);
extern void stbi_image_free(void *ptr);
extern stbi_uc * stbi_load(const char *filename, int *x, int *y, int *comp, int req_comp);
extern void stbi_set_flip_vertically_on_load(int flag);
extern unsigned char *stbi_load(const char *filename, int *x, int *y, int *channels, int desired_channels);
extern void stbi_image_free(void *ptr);

void errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

void checkGLError(const char* location) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": " << error << std::endl;
    }
}

