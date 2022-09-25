/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 05/09/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB_IMAGE
#include "stb_image.h"

#include <string>

#include "Shader.h"

using namespace std;

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupBackground();
void drawSquareObject(Shader *shader, int texture, float v1, float v2,glm::vec3 color = glm::vec3(1.0, 0.0, 1.0));
GLuint generateTexture(string filePath);
bool Collide(float characterx, float charactery, float obsx1, float obsx2);
bool stop;

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLfloat WIDTH = 800.0f, HEIGHT = 600.0f;
// variaveis locais
float characterHeight;
float heightIncrease;

int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	// Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	// Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	// depois atualize (por exemplo: 4.5 com 4 e 5)
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Cria��o da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Mundo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informa��es de vers�o
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	//Habilitar o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Habilitar a transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	// glViewport(0, 0, width, height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO_Background = setupBackground();

	// Gera��o da textura
	GLuint texID = generateTexture("../textures/desert-100.jpg");
	GLuint texIDRock = generateTexture("../textures/Rock Pile.png");

	// Cria��o da matriz de proje��o paralela ortogr�fica
	glm::mat4 projection = glm::mat4(1); // matriz identidade
	// projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
	projection = glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f);

	glUseProgram(shader.ID);

	shader.setMat4("projection", glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer"), 0);
	float obstacleXtransform = 380.0f;
	float obstacle2Xtransform = 750.0f;
	float obsDecrease = 10.0f;
	characterHeight = 150.0f;
	stop = false;

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();
		if (stop)
		{
			obsDecrease = 0.0f;
		}
		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);

		/// IMAGEM DE BACKGROUND

		//Atualizando a matriz de modelo
		glm::mat4 model = glm::mat4(1); //matriz de modelo: transformações na geometria		
		model = glm::translate(model, glm::vec3(WIDTH/2, HEIGHT/2, 0.0f));
		//model = glm::rotate(model, /*glm::radians(45.0f)*/(float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(WIDTH, HEIGHT, 1.0f));
		//......
		shader.setMat4("model", glm::value_ptr(model));
		// Conectando com a textura a ser usada
		glBindTexture(GL_TEXTURE_2D, texID);
		//Passando o offset na textura para o shader 
		shader.setVec2("offset", 0.0, 0.0);
		// Chamada de desenho - drawcall
		glBindVertexArray(VAO_Background); //conectando com o buffer de geometria correto
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// personagem
		model = glm::mat4(1); // matriz de modelo: transforma��es na geometria
		model = glm::translate(model, glm::vec3(70.0f, characterHeight, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.f, 1.0f));
		characterHeight += heightIncrease;
		if (characterHeight == 150.0f)
			heightIncrease = 0.0f;
		else if (characterHeight == 354.0f)
			heightIncrease = -12.0f;
		shader.setMat4("model", glm::value_ptr(model));
		drawSquareObject(&shader, texIDRock, 0.0f, 0.0f);
		float characterx = model[3][0];
		float charactery = model[3][1];


		// obstacle 1
		model = glm::mat4(1);
		shader.setMat4("model", glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(obstacleXtransform, 150.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));
		
		drawSquareObject(&shader, texIDRock, 0.0f, 0.0f);
		float obs1x = model[3][0];

		if (obstacleXtransform > 20)
			obstacleXtransform -= obsDecrease;
		else
			obstacleXtransform = 800.0f;

		// obstacle 2
		model = glm::mat4(1);
		shader.setMat4("model", glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(obstacle2Xtransform, 150.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));
		drawSquareObject(&shader, texIDRock, 0.0f, 0.0f);
		if (obstacle2Xtransform > 20)
			obstacle2Xtransform -= obsDecrease;
		else
			obstacle2Xtransform = 800.0f;

		glBindVertexArray(0);
		float obs2x = model[3][0];
		if (Collide(characterx, charactery, obs1x, obs2x))
		{
			stop = true;
		}
		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO_Background);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
		{
			if (characterHeight == 150.0f)
			{
				heightIncrease = 12.0f;
			}
		}
		break;
		default:
			break;
		}
	}
}

bool Collide(float characterx, float charactery, float obs1x, float obs2x)
{
	float charX = characterx;
	float charX2 = characterx + 100;

	float charY = charactery;
	float charY2 = charactery + 100;

	cout << "charX " << charX << " charX2 " << charX2 << endl;
	cout << "charY " << charY << " charY2 " << charY2 << endl;

	if (obs1x >= charX && obs1x <= charX2)
	{
		if (charactery >= 150 && charactery <= 250)
		{
			cout << "colisão obs1 em x " << obs1x << "E colisão em Y em "<< charactery << endl;
			return true;
		}
	}
	if (obs2x >= charX && obs2x <= charX2)
	{
		if (charactery >= 150 && charactery <= 250)
		{
			cout << "colisão obs2 em x " << obs2x << "E colisão em Y em "<< charactery << endl;
			return true;
		}
	}

	return false;
}

// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupBackground()
{
	GLfloat vertices[] = {
		// posicao          //cor          //tex coord
		-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, // v0  inferior esquerdo
		-0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,  // v1  superior esquerdo
		0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,  // v2  inferior direito
		// posicao          //cor          //tex coord
		-0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, // v1  superior esquerdo
		0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,	 // v3  superior direito
		0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0	 // v2  inferior direito
	};

	GLuint VBO, VAO;

	// Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);
	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se est� normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero

	// Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Atributo coord tex
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void drawSquareObject(Shader *shader, int texture, float v1, float v2, glm::vec3 color)
{
	// GLfloat vertices[] = {
	// 	// posicao
	// 	-0.5, -0.5, 0.0, // v0  inferior esquerdo
	// 	-0.5, 0.5, 0.0,	 // v1  superior esquerdo
	// 	0.5, -0.5, 0.0,	 // v2  inferior direito
	// 	// posicao
	// 	-0.5, 0.5, 0.0, // v1  superior esquerdo
	// 	0.5, 0.5, 0.0,	// v3  superior direito
	// 	0.5, -0.5, 0.0, // v2  inferior direito
	// };

	GLfloat vertices[] = {
		// posicao       //cor          //tex coord
		-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,   // v0  inferior esquerdo
		-0.5,  0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,   // v1  superior esquerdo
		 0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,   // v2  inferior direito
		// posicao       //cor          //tex coord
		-0.5,  0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,    // v1  superior esquerdo
		 0.5,  0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,	 // v3  superior direito
		 0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0	 // v2  inferior direito
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//atributo de posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Atributo coord tex
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindTexture(GL_TEXTURE_2D, texture);
	shader->setVec2("offset", v1, v2);
	shader->setVec4("inputColor", color.r, color.g, color.b, 1.0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

GLuint generateTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Parametriza o wrapping e o filtering da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	return texID;
}