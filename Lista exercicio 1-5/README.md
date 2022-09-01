# PG2022-2
Atividade Semana 1 

Versão x86-32
![image](https://user-images.githubusercontent.com/110510237/183259169-d5fda591-d5e4-46db-9b2d-43d6267e7695.png)

Versão x86-64
![image](https://user-images.githubusercontent.com/110510237/183270259-02ed07db-12f8-450f-ad53-13c29c7412ac.png)

Respostas da lista de exercicio

1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável 
da versão atual que trabalhamos em aula e o que eles processam? 
GLSL é a linguagem de shaders usada pela OPENGL, e os tipos obrigatórios são o vertex shader e o fragment shader.
estes dois shaders são responsaveis pelas cordenadas 3D e 2D dos vertices assim como texturas e cor, e o fragment shader é responsável pela cor
a profundidade Z e o valor alpha das cores pelo RGBA.

2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?
Primitivas são valores para o tipo de estrutura a ser desenhada(GL_POINTS, GL_LINES, GL_TRIANGLES), para armazenarmos um vertices na openGL criamos um
buffer como mostrado a baixo.
  glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer 
um gráfico representando a relação entre eles). 
o VBO é o buffer de memória que contem os valores dos vertices a serem desenhados, o VAO é um array de vertices utilizado para manipular os vertices
que estão no buffer criado pelo VBO e por fim o EBO serve para criarmos um indece indicando a order na qual os vertices devem se ligar
para formar a imagem.

Para o exercício 5 da lista de lista 1 foi utilizado o código dentro desta pasta as imagens de referencia estarão abaixo

versão com poligonos
![Triangulso preenchidos](https://user-images.githubusercontent.com/110510237/187800239-1431499a-eb6e-4b91-9069-df081180ab08.png)

versão com contorno
![triangulos contorno](https://user-images.githubusercontent.com/110510237/187800153-da92c2ef-7380-4f3e-b516-1ff52419cdc1.png)

versão com os pontos
![TriangulosPontos](https://user-images.githubusercontent.com/110510237/187800231-cddd2f86-b2ed-414f-b8a7-cd3fa8637cdb.png)

versão com todos
![triangulos contorno-ponto-preenchido](https://user-images.githubusercontent.com/110510237/187800254-5ebf0023-7caa-4efb-9e67-08b50e963c8b.png)
