#pragma once

#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <list>
#include <queue>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>	// for rotate

// for intersectRayPlane
#include <glm/gtx/intersect.hpp>

// include imgui
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/backends/imgui_impl_glfw.h"
#include "../lib/imgui/backends/imgui_impl_opengl3.h"


