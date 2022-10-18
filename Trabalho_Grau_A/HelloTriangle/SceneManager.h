#define SCENE_IMPLEMENTAION

#include "Shader.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "stb_image.h"
#include "Sprite.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

//freetype
#include "ft2build.h"
#include FT_FREETYPE_H

// C++
#include <vector>
#include<ctime>
#include <iostream>
#include <string>
#include <assert.h>
#include <chrono>
#include <thread>
#include <map>



enum direcoes{PARADO, NORTE, SUL, LESTE, OESTE};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	void update();
	void render();
	void run();
	void finish();


	// scene
	void setupScene();
	void setupCamera2D();
	bool Collide(float characterx, float charactery, float obs1x, float obs2x, float obs3x);
	int setupTexture(string filename, int& largura, int& altura, int& nroCanais); //apenas mostra como criar uma textura
    int RenderSquareObject(Shader *shader, float v1, float v2, glm::vec3 color = glm::vec3(1.0, 0.0, 1.0));// cria um objeto quadrado e retor
	// void generateCharacters(FT_Face face, FT_Library ft);
	// void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
private:

	//GFLW window
	GLFWwindow *window;

	//Array de programas de shader
	vector <Shader*> shaders;

	//Transformations - Model Matrix
	glm::mat4 model;

	//2D Camera - Projection matrix
	glm::mat4 projection;

	Sprite personagem;
	glm::vec2 characterPos; //Posição no mundo
	Timer timer;
    int VAOs[4];
	vector<GLint>textIDs;
};