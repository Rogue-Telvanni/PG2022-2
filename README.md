Lista de exercicio 1 referente as atividades 6,7,8,9 e 10

Atividade 6 
para atividade 6 foi utilizado o seguinte metodo, para a criação de de um pentágono pasta utilizar um segmento de tamanho 5 e para um octágono
o tamanho 8 e para um círculo um valor superior a 200 é o ideal para um formato redondo

![Screenshot from 2022-08-31 21-16-55](https://user-images.githubusercontent.com/110510237/187806674-52b30594-4305-4f45-8b57-377aadd5c688.png)

pentágono
![Screenshot from 2022-08-20 21-46-13](https://user-images.githubusercontent.com/110510237/187807112-38a7906a-416c-4574-ac36-3461e2e71e4b.png)

octágono
![Screenshot from 2022-08-20 22-11-23](https://user-images.githubusercontent.com/110510237/187807138-10e8d4de-8866-4765-a4b6-752b98f92378.png)

círculo
![Screenshot from 2022-08-20 21-40-21](https://user-images.githubusercontent.com/110510237/187807180-078b0157-b585-4029-8ada-daed2a4732a5.png)

Pacman
![Screenshot from 2022-08-20 22-14-06](https://user-images.githubusercontent.com/110510237/187807199-de4540e0-b64d-40b7-94b7-ed07a8e8bf74.png)

Atividade 8
para a atividade 8 foi feiita uma mudança dos shaders padrões de aula
![Screenshot from 2022-08-31 21-32-47](https://user-images.githubusercontent.com/110510237/187809538-af62ee71-142b-43fb-bc10-a6e1a89aaaed.png)

e o codigo para a criação do triangulo é o seguinte

vertices
![Screenshot from 2022-08-31 21-35-04](https://user-images.githubusercontent.com/110510237/187809912-7658df2c-3067-47a3-94de-00130cd122bf.png)

buffers e vertex binding
![Screenshot from 2022-08-31 21-35-46](https://user-images.githubusercontent.com/110510237/187809980-d3542c18-1261-4440-98a1-f6fcd54960fe.png)

drawCall
![Screenshot from 2022-08-31 21-42-19](https://user-images.githubusercontent.com/110510237/187810007-c4e0cfc3-6edd-4a37-bd22-b769feba969e.png)

resultado do triangulo
![Screenshot from 2022-08-28 17-50-43](https://user-images.githubusercontent.com/110510237/187810154-a648c910-3afa-463f-a6dc-5c333127deab.png)

Para a Atividade 9 foram Utilizados os mesmos shaders para o exercicio 5
e para a criação dos vertices e a chamada para desenhar a imagem

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

chamada para desenhar
![Screenshot from 2022-08-31 22-01-15](https://user-images.githubusercontent.com/110510237/187810665-ad728bd4-2a97-4820-bac5-0c01add84249.png) 

imagem
![Screenshot from 2022-08-28 15-10-25](https://user-images.githubusercontent.com/110510237/187810702-dadfa278-c7c9-4fce-ad6c-718775195d4d.png)

