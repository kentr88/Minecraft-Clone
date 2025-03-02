#pragma once
#include "header.h"
#include "camera.h"

/*
Render
has all opengl rendering functions, manages shaders and buffers
instance of this class is created in main.cpp
*/


class Render {
private:
    // file paths
    std::string vertexShaderPath = "src/shaders/shader.vert";
    std::string fragmentShaderPath = "src/shaders/shader.frag";

    GLuint shaderProgram;
    GLuint VAO, VBO;

    glm::mat4 projectionMatrix;

    
	void shaderInit(){
		// 1. load the shader files
		std::string vertexShaderCode;
		std::string fragmentShaderCode;

		try {
			std::ifstream vertexShaderStream(vertexShaderPath, std::ios::in);
			std::ifstream fragmentShaderStream(fragmentShaderPath, std::ios::in);

			if(!vertexShaderStream.is_open()) throw std::runtime_error("Cannot open vertex shader file");
			if(!fragmentShaderStream.is_open()) throw std::runtime_error("Cannot open fragment shader file");

			std::stringstream sstr;
			sstr << vertexShaderStream.rdbuf();
			vertexShaderCode = sstr.str();
			vertexShaderStream.close();

			sstr.str("");
			sstr << fragmentShaderStream.rdbuf();
			fragmentShaderCode = sstr.str();
			fragmentShaderStream.close();
			
		} catch (std::exception e){
			std::cerr << "Error: " << e.what() << std::endl;
		}

		if (vertexShaderCode.empty() || fragmentShaderCode.empty()) {
			std::cerr << "Error: Shader code is empty." << std::endl;
			return;
		}
		

		const char * vertexShaderPtr = vertexShaderCode.c_str();
		const char * fragmentShaderPtr = fragmentShaderCode.c_str();


		// 2. compile the shaders
		int success;
		// vertex
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderPtr, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cerr << "Error: Vertex Shader Compilation Failed\n" << infoLog << std::endl;
		}



		// fragment
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderPtr, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cerr << "Error: Fragment Shader Compilation Failed\n" << infoLog << std::endl;
		}

		// create shader program
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertex);
		glAttachShader(shaderProgram, fragment);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);	// check sucess
		if(!success){
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "Error: Shader Program Linking Failed\n" << infoLog << std::endl;
		}

		// delete shaders - now linked to program, no longer needed
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}


	void createBuffers(){

		// Create Vertex Array Object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Create a Vertex Buffer Object and copy the vertex data to it
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

		// define the vertex attribute pointer
		// for positions - layer 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// for colors - layer 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);


		// Unbind the VAO
		glBindVertexArray(0);
	}

public:
    Render(){}


	bool init(int windowWidth, int windowHeight){
		projectionMatrix = glm::perspective(glm::radians(90.0f), (float) windowWidth / (float) windowHeight, 0.1f, 1000.0f);
		shaderInit();
        createBuffers();
		return true;
	}


    // Render function, called to render the object 
    // format{ x y z r g b,} for 3 points, per triangle
    bool renderData(glm::mat4 viewMatrix, std::vector<float> data){
        if(data.empty()){
            std::cout << "Vertex Data is empty" << std::endl;
            return false;
        }

		glm::mat4 worldMatrix = glm::mat4(1.0f);	// Form World Matrix

		// Set the background color to white
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shader program
		glUseProgram(shaderProgram);


		// Pass matrices to the shader
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(worldMatrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


		// 5. Update Vertex Buffer Object (VBO) - new data
		// use method: glBufferSubData not glMapBuffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);


		// 6. render the object
		glBindVertexArray(VAO);
		//glDrawArraysInstanced
		glDrawArrays(GL_TRIANGLES, 0, data.size() / 6);
		glBindVertexArray(0);

		return true;
		
	}

};