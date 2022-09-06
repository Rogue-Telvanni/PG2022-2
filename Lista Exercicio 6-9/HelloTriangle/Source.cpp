/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 01/08/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <math.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupShader();
int setupGeometry();
GLfloat* draw_circle(GLfloat, GLfloat, GLfloat, GLfloat, GLint);
GLfloat* draw_spiral(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLint);
GLfloat* draw_house();

// tamanhos dos verices para o deseho da casa
const GLuint houseSegments = 300; //segmentos 
const GLuint circleVertices = (houseSegments + 2) * 3; //segmentos * 3 (numero de vertices por ponto)
const GLuint roofSize = 3; // 3 vertices
const GLuint housesize = 3;
const GLuint doorSize = 3;
const GLuint piso = 2;
const GLuint totalsize = circleVertices + roofSize * 3 + housesize * 6 + doorSize * 6 + piso  * 3;
// indeces de cadadesenho
const GLuint roofStarSegmnet = houseSegments + 2;
const GLuint houseFirstSegment = roofStarSegmnet + 3;
const GLuint houseSecondSegment = houseFirstSegment + 3;
const GLuint doorFirsSegment = houseSecondSegment + 3;
const GLuint doorSecondtSegment = doorFirsSegment + 3;
const GLuint pisoSegment = doorSecondtSegment + 3;


// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;
// C�digo fonte do Vertex Shader (em GLSL): ainda hardcoded

//Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

// //shaders for triangle if 3 colors
// const GLchar* vertexShaderSource = "#version 450\n"
// "layout (location = 0) in vec3 aPos;\n"   // the position variable has attribute position 0
// "layout (location = 1) in vec3 aColor;\n" // the color variable has attribute position 1
// "out vec3 ourColor;\n" // output a color to the fragment shader
// "void main()\n"
// "{\n"
// 	"gl_Position = vec4(aPos, 1.0);\n"
//     "ourColor = aColor;\n" // set ourColor to the input color we got from the vertex data
// "}\0";

// //C�difo fonte do Fragment Shader (em GLSL): ainda hardcoded
// const GLchar* fragmentShaderSource = "#version 450\n"
// "out vec4 FragColor;\n"  
// "in vec3 ourColor;\n"
// "void main()\n"
// "{\n"
// "FragColor = vec4(ourColor, 1.0);\n"
// "}\n\0";

int spiralsize;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! - Joao Ricardo Copini", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();
	

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a vari�veis do tipo uniform em GLSL para armazenar esse tipo de info
	// que n�o est� nos buffers
	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	assert(colorLoc > -1);
	glUseProgram(shaderID);
	
	

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(2);
		glPointSize(20);

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		// float timeValue = glfwGetTime();
		// float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		// float redValue = (cos(timeValue) / 2.0f) + 0.5f;
		// float blueValue = (sin(timeValue) / 2.0f) + 0.5f;
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); //enviando cor para variável uniform inputColor
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, spiralsize);

		// // arrays para a casa
		// glDrawArrays(GL_TRIANGLE_FAN, 0, houseSegments + 2);
		// // telhado
		// glUniform4f(colorLoc, 0.0f, 0.2f, 0.4f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_TRIANGLES, roofStarSegmnet, roofSize);
		// glUniform4f(colorLoc, 0.3f, 0.2f, 0.6f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_LINE_LOOP, roofStarSegmnet, roofSize);
		// //casa
		// glUniform4f(colorLoc, 0.1f, 0.4f, 0.1f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_TRIANGLES, houseFirstSegment, housesize);
		// glUniform4f(colorLoc, 0.4f, 0.4f, 0.2f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_LINES, houseFirstSegment, housesize);
		// glUniform4f(colorLoc, 0.1f, 0.4f, 0.1f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_TRIANGLES, houseSecondSegment, housesize);
		// glUniform4f(colorLoc, 0.4f, 0.4f, 0.2f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_LINES, houseSecondSegment, housesize);
		// //porta
		// glUniform4f(colorLoc, 0.4f, 0.6f, 0.3f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_TRIANGLES, doorFirsSegment, doorSize);
		// glUniform4f(colorLoc, 0.7f, 0.4f, 0.4f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_LINE_LOOP, doorFirsSegment, doorSize);
		// glUniform4f(colorLoc, 0.4f, 0.6f, 0.3f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_TRIANGLES, doorSecondtSegment, doorSize);
		// glUniform4f(colorLoc, 0.7f, 0.4f, 0.4f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_LINE_LOOP, doorSecondtSegment, doorSize);
		// //piso
		// glUniform4f(colorLoc, 0.7f, 0.6f, 0.0f, 1.0f); //enviando cor para variável uniform inputColor
		// glDrawArrays(GL_LINE_LOOP, pisoSegment, piso);
		// glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

GLfloat* draw_circle(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat radius, GLint segments)
{
	GLint vertices = segments + 2;
	GLfloat angle = 2.0f * M_PI;

	GLfloat verticesX[vertices];
	GLfloat verticesY[vertices];
	GLfloat verticesZ[vertices];

	// circle center
	verticesX[0] = centerX;
	verticesY[0] = centerY;
	verticesZ[0] = centerZ;

	for(int index = 1; index < vertices; index++){
		
		verticesX[index] = centerX + (radius * cos(index * angle / segments));
		verticesY[index] = centerY + (radius * sin(index * angle / segments));
		verticesZ[index] = centerZ;
	}

	GLfloat* completeArray = new GLfloat[vertices * 3];
	
	for(int i = 0; i < vertices; i++){
		completeArray[i * 3] = verticesX[i];
		completeArray[(i * 3) + 1] = verticesY[i];
		completeArray[(i * 3) + 2] = verticesZ[i];
	}

	return completeArray;
}

GLfloat *draw_house()
{
	GLfloat * array = new GLfloat[totalsize];
	GLfloat angle = 2.0f * M_PI;

	GLfloat centerX = -0.75f;
	GLfloat centerY = 0.75f;
	GLfloat centerZ = 0.0f;
	GLfloat radius = 0.25f;

	GLfloat verticesX[houseSegments + 2];
	GLfloat verticesY[houseSegments + 2];
	GLfloat verticesZ[houseSegments + 2];

	// circle center
	verticesX[0] = centerX;
	verticesY[0] = centerY;
	verticesZ[0] = centerZ;
	
	int index;
	for(index = 1; index < houseSegments + 2; index++){
		
		verticesX[index] = centerX + (radius * cos(index * angle / houseSegments));
		verticesY[index] = centerY + (radius * sin(index * angle / houseSegments));
		verticesZ[index] = centerZ;
	}

	for(int i = 0; i < houseSegments + 2; i++){
		array[i * 3] = verticesX[i];
		array[(i * 3) + 1] = verticesY[i];
		array[(i * 3) + 2] = verticesZ[i];
		cout << "I" << (i * 3) + 2 << endl;
	}

	cout << "index" << index << endl;
	index = index * 3;
	cout << "index" << index << endl;

	// desenha o telhado
	//ponto 1
	array[index++] = 0.0f;
	array[index++] = 0.0f;
	array[index++] = 0.0f;
	//ponto 2
	array[index++] = 0.25f;
	array[index++] = 0.5f;
	array[index++] = 0.0f;
	//ponto 3
	array[index++] = 0.5f;
	array[index++] = 0.0f;
	array[index++] = 0.0f;

	// desenho casa

	//ponto 1
	array[index++] = 0.0f;
	array[index++] = 0.0f;
	array[index++] = 0.0f;
	//ponto 2
	array[index++] = 0.5f;
	array[index++] = 0.0f;
	array[index++] = 0.0f;
	//ponto 3
	array[index++] = 0.0f;
	array[index++] = -0.75f;
	array[index++] = 0.0f;
	
	//ponto 4
	array[index++] = 0.5f;
	array[index++] = -0.75f;
	array[index++] = 0.0f;
	//ponto 5
	array[index++] = 0.0f;
	array[index++] = -0.75f;
	array[index++] = 0.0f;
	//ponto 6
	array[index++] = 0.5f;
	array[index++] = 0.0f;
	array[index++] = 0.0f;

	// desenho porta
	//ponto 1
	array[index++] = 0.125f;
	array[index++] = -0.5f;
	array[index++] = 0.0f;
	//ponto 2
	array[index++] = 0.25f;
	array[index++] = -0.5f;
	array[index++] = 0.0f;
	//ponto 3
	array[index++] = 0.125f;
	array[index++] = -0.75f;
	array[index++] = 0.0f;
	//ponto 4
	array[index++] = 0.25f;
	array[index++] = -0.5f;
	array[index++] = 0.0f;
	//ponto 5
	array[index++] = 0.125f;
	array[index++] = -0.75f;
	array[index++] = 0.0f;
	//ponto 6
	array[index++] = 0.25f;
	array[index++] = -0.75f;
	array[index++] = 0.0f;

	//piso
	//ponto 1
	array[index++] = -1.00f;
	array[index++] = -0.75f;
	array[index++] = 0.0f;
	//ponto 2
	array[index++] = 1.0f;
	array[index++] = -0.75f;
	array[index] = 0.0f;

	// cout << index << endl;
	// cout << totalsize << endl;

	return array;
}

GLfloat *draw_spiral(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat radius, GLfloat radiusIncrease,GLint rotations)
{
	GLint vertices = 200;
	int totalsize = 100 * rotations * 2;
	GLfloat angle = 2.0f * M_PI;

	GLfloat verticesX[totalsize];
	GLfloat verticesY[totalsize];
	GLfloat verticesZ[totalsize];
	float smothIncrease = radiusIncrease / (vertices / 2);
	std::cout << radiusIncrease << endl;
	std::cout << smothIncrease << endl;

	int index = 0;
	for (int rotation = 0; rotation < rotations * 2; rotation++)
	{
		radius += radiusIncrease;
		for (int i = 0; i < vertices / 2; i++)
		{
			float increase = smothIncrease * (i + 1);
			float increaseRadius = (radius + increase);
			verticesX[index] = centerX + (increaseRadius * cos(index * angle / vertices));
			verticesY[index] = centerY + (increaseRadius * sin(index * angle / vertices));
			verticesZ[index] = centerZ;
			index++;
		}
	}


	GLfloat* completeArray = new GLfloat[totalsize * 3];
	
	for(int i = 0; i < totalsize; i++){
		completeArray[i * 3] = verticesX[i];
		completeArray[(i * 3) + 1] = verticesY[i];
		completeArray[(i * 3) + 2] = verticesZ[i];

		spiralsize += 1;
	}

	return completeArray;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//Esta fun��o est� basntante hardcoded - objetivo � compilar e "buildar" um programa de
// shader simples e �nico neste exemplo de c�digo
// O c�digo fonte do vertex e fragment shader est� nos arrays vertexShaderSource e
// fragmentShader source no ini�io deste arquivo
// A fun��o retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compila��o (exibi��o via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compila��o (exibi��o via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	
	// static star shape
	// GLfloat vetices[] = {
	// 	0.0, 1.0, 0.0, // A 0
	// 	-0.25, 0.5, 0.0, //c 1
	// 	0.25, 0.5, 0.0, //d 2
	// 	-1.0, 0.5, 0.0, //E 3
	// 	1.0, 0.5, 0.0, //B 4
	// 	-0.6, 0.0, 0.0, //b 5
	// 	0.6, 0.0, 0.0, //e 6
	// 	-0.8, -1.0, 0.0, // D 7
	// 	0.8, -1.0, 0.0, // C 8
	// 	0.0, -0.5, 0.0, // a 9
	// };

	// unsigned int indices[] = {
	// 	0,2,4,6,8,9,7,5,3,1
	// 	// 0, 1, 2, // first triangle
	// 	// 1, 3, 5,	 // second triangle
	// 	// 2,4,6,
	// 	// 5,7,9,
	// 	// 9,8,6
	// };

	// // triangles with 3 colors
	// GLfloat vertices[] = {
	// 	//triangulo				cor
	// 	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	// 	0.0f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	// 	0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
	// };
	// int sizes = sizeof(vertices);

	int segments = 5;
	// GLfloat* vertices = draw_circle(0.0f, 0.0f, 0.0f, 0.5f, segments);
	// int sizes = (segments + 2) * 3 * sizeof(GLfloat);
	GLfloat* vertices = draw_spiral(0.0f, 0.0f, 0.0f, 0.05f, 0.05f, segments);
	int sizes = spiralsize * 3 * sizeof(GLfloat);

	//GLfloat* vertices = draw_house();
	//int sizes = totalsize * sizeof(GLfloat);

	GLuint VBO, VAO;
	// int arraySize = (segments + 2) * 3 * sizeof(GLfloat);
	// int spiralsize = 150 * sizeof(GLfloat);
	// std:: cout << arraySize << std::endl; 

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizes, vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)S
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
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	// glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 
	//free(vertices);
	return VAO;
}
