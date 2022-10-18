
#include "SceneManager.h"

// static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint sceneWidth, sceneHeight;
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
TextRenderer *Text;

SceneManager::SceneManager()
{
	srand(time(0));
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	sceneWidth = w;
	sceneHeight = h;

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(sceneWidth, sceneHeight, "Trabalho Grau A", nullptr, nullptr);
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
	sceneWidth = w;
	sceneHeight = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, sceneWidth, sceneHeight);
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

	if (parar)
	{
		obsDecrease = 0.0f;
	}
	else if (iniciar)
	{
		obsDecrease = 0.0f;
	}
	else
	{
		// RenderText(shader, "Pontuação", 25.0f, 25.0f, 1.0f, glm::vec3(1.0 , 1.0f, 1.0f));
		obsDecrease = 10.0f;
	}

	if (resetar)
	{
		resetar = false;
		obstacleXtransform = 2000.0f;
		obstacle2Xtransform = 2600.0f;
		obstacle3Xtransform = 3350.0f;
		obsDecrease = 10.0f;
	}
	// Limpa o buffer de cor
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(10);

	/// IMAGEM DE BACKGROUND
	

	// Atualizando a matriz de modelo
	glm::mat4 model = glm::mat4(1); // matriz de modelo: transformações na geometria
	model = glm::translate(model, glm::vec3(sceneWidth / 2, sceneHeight / 2, 0.0f));
	model = glm::scale(model, glm::vec3(sceneWidth, sceneHeight, 1.0f));
	shader.setMat4("model", glm::value_ptr(model));
	// Conectando com a textura a ser usada
	glBindTexture(GL_TEXTURE_2D, textIDs[0]);
	// Passando o offset na textura para o shader
	shader.setVec2("offset", 0.0, 0.0);
	glBindVertexArray(VAOs[0]); // conectando com o buffer de geometria correto
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// personagem
	tamanhoPersonagem += aumentoDeAltura;
	if (tamanhoPersonagem == 150.0f)
	{
		descendo = false;
		if (noAr)
			personagem.setIdFrame(6);
		noAr = false;
		aumentoDeAltura = 0.0f;
		rotacao = 0.0f;
	}
	else if (tamanhoPersonagem == 378.0f)
	{
		rotacao = 0.0f;
		aumentoDeAltura = -12.0f;
		descendo = true;
	}
	else if (tamanhoPersonagem > 320.0f && tamanhoPersonagem < 378.0f && descendo)
	{
		rotacao = 0.0f;
	}
	else if (tamanhoPersonagem > 150.0f && tamanhoPersonagem < 250.0f && descendo)
	{
		rotacao = 330.0f;
	}

	if (noAr)
		personagem.setIdFrame(8);

	personagem.setTranslacao(glm::vec3(70.0f, tamanhoPersonagem, 0.0f));
	personagem.setRotacao(glm::radians(rotacao), glm::vec3(0.0f, 0.0f, 1.0f));
	personagem.setEscala(glm::vec3(100.f, 100.f, 1.0f), false);
	personagem.setTransform();
	float characterx = 70.0f;
	float charactery = tamanhoPersonagem;

	personagem.desenhar();
	if (personagem.GetFrame() < 6)
		personagem.setIdFrame(6);

	// obstacle 1
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(obstacleXtransform, 150.0f, 0.0f));
	model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
	shader.setMat4("model", glm::value_ptr(model));
	// Conectando com a textura a ser usada
	glBindTexture(GL_TEXTURE_2D, textIDs[1]);
	// Passando o offset na textura para o shader
	shader.setVec2("offset", 0.0, 0.0);
	glBindVertexArray(VAOs[1]); // conectando com o buffer de geometria correto
	glDrawArrays(GL_TRIANGLES, 0, 6);
	float obs1x = model[3][0];

	if (obstacleXtransform > 20)
		obstacleXtransform -= obsDecrease;
	else
		obstacleXtransform = 2000.0f;

	// obstacle 2
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(obstacle2Xtransform, 150.0f, 0.0f));
	model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
	shader.setMat4("model", glm::value_ptr(model));
	// Conectando com a textura a ser usada
	glBindTexture(GL_TEXTURE_2D, textIDs[1]);
	// Passando o offset na textura para o shader
	shader.setVec2("offset", 0.0, 0.0);
	glBindVertexArray(VAOs[2]); // conectando com o buffer de geometria correto
	glDrawArrays(GL_TRIANGLES, 0, 6);
	if (obstacle2Xtransform > 20)
		obstacle2Xtransform -= obsDecrease;
	else
		obstacle2Xtransform = obstacleXtransform + 600.0f;

	glBindVertexArray(0);
	float obs2x = model[3][0];

	// obstacle 3
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(obstacle3Xtransform, 150.0f, 0.0f));
	model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
	shader.setMat4("model", glm::value_ptr(model));
	// Conectando com a textura a ser usada
	glBindTexture(GL_TEXTURE_2D, textIDs[1]);
	// Passando o offset na textura para o shader
	shader.setVec2("offset", 0.0, 0.0);
	glBindVertexArray(VAOs[3]); // conectando com o buffer de geometria correto
	glDrawArrays(GL_TRIANGLES, 0, 6);
	if (obstacle3Xtransform > 20)
		obstacle3Xtransform -= obsDecrease;
	else
		obstacle3Xtransform = obstacle2Xtransform + 750.0f;

	glBindVertexArray(0);
	float obs3x = model[3][0];

	if (Collide(characterx, charactery, obs1x, obs2x, obs3x))
	{
		parar = true;
	}
	// Troca os buffers da tela
	Text->RenderText("Teste", 100.0f, 100.0f, 1.0f);
	
	timer.finish();
	double waitingTime = timer.calcWaitingTime(60, timer.getElapsedTimeMs());
	if (waitingTime)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((int)waitingTime));
	}
}

bool SceneManager::Collide(float characterx, float charactery, float obs1x, float obs2x, float obs3x)
{
	float charX = characterx;
	float charX2 = characterx + 100;

	float charY = charactery;
	float charY2 = charactery + 100;

	// cout << "charX " << charX << " charX2 " << charX2 << endl;
	// cout << "charY " << charY << " charY2 " << charY2 << endl;

	if (obs1x >= charX + 10 && obs1x <= charX2 - 15)
	{
		if (charactery >= 150 && charactery <= 250)
		{
			cout << "colisão obs1 em x " << obs1x << "E colisão em Y em " << charactery << endl;
			return true;
		}
	}
	if (obs2x >= charX + 10 && obs2x <= charX2 - 15)
	{
		if (charactery >= 150 && charactery <= 250)
		{
			cout << "colisão obs2 em x " << obs2x << "E colisão em Y em " << charactery << endl;
			return true;
		}
	}
	if (obs3x >= charX && obs2x <= charX2)
	{
		if (charactery >= 150 && charactery <= 250)
		{
			cout << "colisão obs3 em x " << obs3x << "E colisão em Y em " << charactery << endl;
			return true;
		}
	}

	return false;
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
	textIDs.push_back(setupTexture("../textures/Rock Pile.png", width, height, nrChannels));
	textIDs.push_back(setupTexture("../textures/wolfsheet1.png", width, height, nrChannels));
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaders[0]->ID, "tex_buffer"), 0);
	VAOs[0] = RenderSquareObject(shaders[0], 0.0f, 0.0f);
	VAOs[1] = RenderSquareObject(shaders[0], 0.0f, 0.0f);
	VAOs[2] = RenderSquareObject(shaders[0], 0.0f, 0.0f);
	VAOs[3] = RenderSquareObject(shaders[0], 0.0f, 0.0f);
	personagem.inicializar(textIDs[2], glm::vec3(100.0f, 100.f, 1.0f), 12, 10);
	personagem.setShader(shaders[0]);
	personagem.setIdAnim(5);
	Text = new TextRenderer(sceneWidth, sceneHeight, shaders[1]);
    Text->Load("../fonts/BitterPro-Regular.ttf", 24);
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
	glViewport(0, 0, sceneWidth, sceneHeight);

	// Setando o tamanho da janela do mundo
	float xMin = 0.0f, xMax = sceneWidth, yMin = 0.0f, yMax = sceneHeight, zNear = -3.0f, zFar = 3.0f;
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