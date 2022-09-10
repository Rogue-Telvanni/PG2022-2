Exercicio 1 
código usado para desenhar a geometria



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
		model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(350.0f, 350.f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		shader.setVec4("inputColor",1.0f, 1.0f, 0.0f, 1.0f); //enviando cor para variável uniform inputColor
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// segunda imagem
		model = glm::mat4(1); //matriz de modelo: transformações na geometria		
		model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(350.0f, 350.f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		shader.setVec4("inputColor",1.0f, 0.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
		model = glm::scale(model, glm::vec3(350.0f, 350.f, 1.0f));
		shader.setMat4("model", glm::value_ptr(model));
		drawCircle(&shader, glm::vec3(400.0, 300.0, 0.0));
		
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	
	
Desenho do circulo



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

imagem resultante


![Screenshot from 2022-09-10 13-42-30](https://user-images.githubusercontent.com/110510237/189493388-e22d117c-2a54-4c66-b53d-347bedab5a2c.png)

Código para o exercicio 2 


	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float cory = 0.1f;
		for (float y = 0.0f; y <= 600.0f; y += 100.0f)
		{
			float corx = 0.0f + cory;
			for (float x = 0.0f; x <= 800.0f; x += 100.0f)
			{
				glm::mat4 model = glm::mat4(1); // matriz de modelo: transformações na geometria
				model = glm::translate(model, glm::vec3(x, y, 0.0f));
				model = glm::scale(model, glm::vec3(100.0f, 100.f, 1.0f));
				shader.setMat4("model", glm::value_ptr(model));
				corx += 0.08f;
				shader.setVec4("inputColor", cos(corx), cory, 0.3f, 1.0f); // enviando cor para variável uniform inputColor
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			cory += 0.1f;
		}

		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

vertices usados 

GLfloat vertices[] = {
		// first triangle
		1.0f, 0.0f, 0.0f,  // top right
		1.0f, -1.0f, 0.0f, // bottom right
		0.0f, 0.0f, 0.0f,  // top left
						   // second triangle
		1.0f, -1.0f, 0.0f, // bottom right
		0.0f, -1.0f, 0.0f, // bottom left
		0.0f, 0.0f, 0.0f   // top left
	};
	
	
imagem resultante


![Screenshot from 2022-09-10 15-36-22](https://user-images.githubusercontent.com/110510237/189497247-596e1af9-6ffc-454b-ab73-2c97c1d02501.png)


