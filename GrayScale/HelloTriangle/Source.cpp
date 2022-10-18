/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * ultima atualizacao em 10/10/2022 - joão copini
 *
 */
#include "SceneManager.h"
const GLfloat WIDTH = 1920.0f, HEIGHT = 600.0f;

int main()
{
	SceneManager* scene = new SceneManager;
	scene->initialize(WIDTH, HEIGHT);

	scene->run();

	scene->finish();

	return 0;
}