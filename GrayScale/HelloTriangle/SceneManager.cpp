
#include "SceneManager.h"

// static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
static int dir;

float tamanhoPersonagem = 150.0f;
float aumentoDeAltura = 0.0f;
float rotacao = 0.0f;
float obsDecrease = 0.0f;
bool noAr = false;
bool parar;
bool iniciar = true;
bool resetar;
bool descendo;
float obstacleXtransform = 2000.0f;
float obstacle2Xtransform = 2600.0f;
float obstacle3Xtransform = 3350.0f;

SceneManager::SceneManager()
{
	srand(time(0));
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Trabalho Grau A", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informa��es de vers�o
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Build and compile our shader program
	addShader("../shaders/hello.vs", "../shaders/hello.fs"); // 0
	addShader("../shaders/text.vs", "../shaders/text.fs"); // 1

	// setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE
	//  CONFIGURAÇÃO
	setupScene();

	resized = true; // para entrar no setup da câmera na 1a vez

	setupCamera2D();
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	Shader *shader = new Shader(vFilename.c_str(), fFilename.c_str());
	shaders.push_back(shader);
}

void SceneManager::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_UP)
		{
			dir = NORTE;
		}
		if (key == GLFW_KEY_S)
		{
			dir = SUL;
		}
		if (key == GLFW_KEY_A)
		{
			dir = OESTE;
		}
		if (key == GLFW_KEY_D)
		{
			dir = LESTE;
		}
	}
}

void SceneManager::resize(GLFWwindow *window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}

void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (keys[GLFW_KEY_ENTER])
	{
		keys[GLFW_KEY_ENTER] = false;
		if (parar || iniciar)
		{
			parar = false;
			iniciar = false;
			resetar = true;
		}
	}
	if (keys[GLFW_KEY_UP])
	{
		keys[GLFW_KEY_UP] = false;
		if (parar || iniciar)
			return;

		if (tamanhoPersonagem == 150.0f)
		{
			aumentoDeAltura = 12.0f;
			rotacao = 30.0f;
			noAr = true;
		}
	}
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (resized) // se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	timer.start();

	Shader shader = *shaders[0];
	shader.Use();


	// Limpa o buffer de cor
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(10);

	/// IMAGEM DE BACKGROUND
	

	// Atualizando a matriz de modelo
	glm::mat4 model = glm::mat4(1); // matriz de modelo: transformações na geometria
	model = glm::translate(model, glm::vec3(width / 2, height / 2, 0.0f));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));
	shader.setMat4("model", glm::value_ptr(model));
	// Conectando com a textura a ser usada
	glBindTexture(GL_TEXTURE_2D, textIDs[0]);
	// Passando o offset na textura para o shader
	shader.setVec2("offset", 0.0, 0.0);
	glBindVertexArray(VAOs[0]); // conectando com o buffer de geometria correto
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	timer.finish();
	double waitingTime = timer.calcWaitingTime(60, timer.getElapsedTimeMs());
	if (waitingTime)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((int)waitingTime));
	}
}


void SceneManager::run()
{
	// GAME LOOP
	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Update method(s)
		update();

		// Render scene
		render();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

glm::vec4 normalizaRGB(glm::vec4 byteColor)
{
	glm::vec4 normColor(byteColor.r / 255.0, byteColor.g / 255.0, byteColor.b / 255.0, byteColor.a / 255.0);
	return normColor;
}

void SceneManager::setupScene()
{

	// Habilitar o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	//glEnable(GL_CULL_FACE);

	// Habilitar a transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width, height, nrChannels;
	textIDs.push_back(setupTexture("../textures/desert-100.jpg", width, height, nrChannels));
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaders[0]->ID, "tex_buffer"), 0);
	VAOs[0] = RenderSquareObject(shaders[0], 0.0f, 0.0f);
}

int SceneManager::RenderSquareObject(Shader *shader, float v1, float v2, glm::vec3 color)
{
	GLfloat vertices[] = {
		// posicao       //cor          //tex coord
		-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, // v0  inferior esquerdo
		-0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,  // v1  superior esquerdo
		0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,  // v2  inferior direito
		// posicao       //cor          //tex coord
		-0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, // v1  superior esquerdo
		0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,	 // v3  superior direito
		0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0	 // v2  inferior direito
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// atributo de posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Atributo coord tex
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// glBindTexture(GL_TEXTURE_2D, texture);
	// shader->setVec2("offset", v1, v2);
	shader->setVec4("inputColor", color.r, color.g, color.b, 1.0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	return VAO;
}

void SceneManager::setupCamera2D()
{
	glViewport(0, 0, width, height);

	// Setando o tamanho da janela do mundo
	float xMin = 0.0f, xMax = width, yMin = 0.0f, yMax = height, zNear = -3.0f, zFar = 3.0f;
	projection = glm::ortho(xMin, xMax, yMin, yMax, zNear, zFar);

	// Passando para os shaders que a usam
	shaders[0]->Use();
	GLint projLoc = glGetUniformLocation(shaders[0]->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// shaders[1]->Use();
	// projLoc = glGetUniformLocation(shaders[1]->ID, "projection");
	// glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

int SceneManager::setupTexture(string filePath, int &largura, int &altura, int &nroCanais)
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
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{

		//for grey scale
		// int size = width * height * nrChannels;
		// for(int i = 0; i < size; i += nrChannels)
		// {
		// 	float media = (data[i] + data[i + 1] + data[i + 2])/3;
		// 	data[i] = media;
		// 	data[i + 1] = media;
		// 	data[i + 2] = media;
		// }

		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
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
