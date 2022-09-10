/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 05/09/2022
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

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

void drawPoint(Shader* shader, glm::vec3 coords, glm::vec3 color = glm::vec3(1.0,0.0,1.0), int pointsize = 10);
void drawCircle(Shader* shader, glm::vec3 coords, glm::vec3 color = glm::vec3(1.0, 0.0, 0.0));

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;
float rotation = 0.0f;


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Mundo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();

	// Criação da matriz de projeção paralela ortográfica
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	//projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
	projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	glUseProgram(shader.ID);
	
	shader.setMat4("projection", glm::value_ptr(projection));
	float position_x = 0.0f;
	float increase = 10.0f;
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// primeira imagem
		glm::mat4 model = glm::mat4(1); //matriz de modelo: transformações na geometria		
		model = glm::translate(model, glm::vec3(position_x, 300.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(300.0f, 300.f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		shader.setVec4("inputColor",0.42f, 0.24f, 0.52f, 1.0f); //enviando cor para variável uniform inputColor
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glBindVertexArray(0);
		//650 o tamanho da projeção 800 - 150 o tamanho de metade do triangulo
		position_x += increase;
		if(position_x >= 650.0f)
			increase = -10.0f;
		//150  o tamanho de metade do triangulo já que o o ponto 0 esta o centro do triangulo antes da trasnformação
		else if(position_x <= 150.0f)
			increase = 10.0f;
		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_W:
				rotation = 0.0f;
				break;
			case GLFW_KEY_A:
				rotation = 270.0f;
				break;
			case GLFW_KEY_D:
				rotation = 90.0f;
				break;
			case GLFW_KEY_S:
				rotation = 180.0f;
				break;
			case GLFW_KEY_UP:
				rotation = 0.0f;
				break;
			case GLFW_KEY_LEFT:
				rotation = 270.0f;
				break;
			case GLFW_KEY_RIGHT:
				rotation = 90.0f;
				break;
			case GLFW_KEY_DOWN:
				rotation = 180.0f;
				break;
			default:
				break;
		}
	}
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		-0.5 , -0.5, 0.0,
		 0.5,  -0.5, 0.0,
		 0.0,  0.5 , 0.0,
		 //outro triangulo vai aqui
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}

void drawCircle(Shader* shader, glm::vec3 coords, glm::vec3 color)
{
	GLint segments = 200;
	GLint vertices = segments + 2;
	GLfloat angle = 2.0f * M_PI;

	GLfloat verticesX[vertices];
	GLfloat verticesY[vertices];
	GLfloat verticesZ[vertices];

	// circle center
	verticesX[0] = 0;
	verticesY[0] = 0.5;
	verticesZ[0] = 0;
	GLfloat radius = 0.35f;

	for(int index = 1; index < vertices; index++){
		
		verticesX[index] = 0 + (radius * cos(index * angle / segments));
		verticesY[index] = 0.5f + (radius * sin(index * angle / segments));
		verticesZ[index] = 0;
	}

	GLint size = vertices * 3;
	GLfloat completeArray [size];
	
	for(int i = 0; i < vertices; i++){
		completeArray[i * 3] = verticesX[i];
		completeArray[(i * 3) + 1] = verticesY[i];
		completeArray[(i * 3) + 2] = verticesZ[i];
	}

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(completeArray), completeArray, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	shader->setVec4("inputColor", color.r, color.g, color.b, 1.0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void drawPoint(Shader* shader, glm::vec3 coords, glm::vec3 color, int pointSize)
{
	glPointSize(pointSize);
	
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), glm::value_ptr(coords), GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	shader->setVec4("inputColor", color.r, color.g, color.b, 1.0);
	glDrawArrays(GL_POINTS, 0, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
