// "Practica 2: Proyecciones, transformaciones (Sanchez Duran Dannae - 422526067)"
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* FRojo = "shaders/FRojo.frag";
static const char* VRojo = "shaders/VRojo.vert";
static const char* FVerde = "shaders/FVerde.frag";
static const char* VVerde = "shaders/VVerde.vert";
static const char* FAzul = "shaders/FAzul.frag";
static const char* VAzul = "shaders/VAzul.vert";
static const char* FCafe = "shaders/FCafe.frag";
static const char* VCafe = "shaders/VCafe.vert";
static const char* FVerdeOscuro = "shaders/FVerde2.frag";
static const char* VVerdeOscuro = "shaders/VVerde2.vert";

float angulo = 0.0f;

//color caf� en RGB : 0.478, 0.255, 0.067

//Pir�mide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12); // Para la matriz es 0,1,2,3 por 4 = 12
	meshList.push_back(obj1);
}

//V�rtices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		//X			Y			Z			R		G		B
		// ------- AQUI EMPIEZA LA LETRA d 
		//T1
		-0.4f,0.5f,0.0f, 0.0f,0.0f,1.0f, // LETRA d en color AZUL rey 
		-0.3f,0.5f,0.0f, 0.0f,0.0f,1.0f,
		-0.4f,1.0f,0.0f, 0.0f,0.0f,1.0f,
		//T2
		-0.3f,0.5f,0.0f, 0.0f,0.0f,1.0f,
		-0.3f,1.0f,0.0f, 0.0f,0.0f,1.0f,
		-0.4f,1.0f,0.0f, 0.0f,0.0f,1.0f,
		//T3
		-0.5f,0.7f,0.0f, 0.0f,0.0f,1.0f, 
		-0.5f,0.8f,0.0f, 0.0f,0.0f,1.0f, 
		-0.4f,0.7f,0.0f, 0.0f,0.0f,1.0f, 
		//T4
		-0.4f,0.7f,0.0f, 0.0f,0.0f,1.0f, 
		-0.4f,0.8f,0.0f, 0.0f,0.0f,1.0f, 
		-0.5f,0.8f,0.0f,0.0f,0.0f,1.0f, 
		//T5
		-0.4f,0.6f,0.0f, 0.0f,0.0f,1.0f, 
		-0.4f,0.5f,0.0f, 0.0f,0.0f,1.0f, 
		-0.5f,0.6f,0.0f, 0.0f,0.0f,1.0f, 
		//T6
		-0.5f,0.5f,0.0f, 0.0f,0.0f,1.0f, 
		-0.5f,0.6f,0.0f, 0.0f,0.0f,1.0f, 
		-0.4f,0.5f,0.0f, 0.0f,0.0f,1.0f, 
		//T7
		-0.6f,0.8f,0.0f, 0.0f,0.0f,1.0f, 
		-0.5f,0.8f,0.0f, 0.0f,0.0f,1.0f, 
		-0.6f,0.5f,0.0f, 0.0f,0.0f,1.0f, 
		//T8
		-0.6f,0.5f,0.0f, 0.0f,0.0f,1.0f, 
		-0.5f,0.5f,0.0f, 0.0f,0.0f,1.0f, 
		-0.5f,0.8f,0.0f, 0.0f,0.0f,1.0f, 

		//--------------------------------AQUI TERMINA LA LETRA d
		// AQUI EMPIEZA LA LETRA S
		//T1
		-0.2f,0.7f,0.0f, 0.5f,0.0f,0.0f, 
		-0.2f,1.0f,0.0f, 0.5f,0.0f,0.0f, // LETRA S EN COLOR ROJO OSCURO
		-0.1f,1.0f,0.0f, 0.5f,0.0f,0.0f, 
		//T2
		-0.2f,0.7f,0.0f, 0.5f,0.0f,0.0f, 
		-0.1f,0.7f,0.0f, 0.5f,0.0f,0.0f, 
		-0.1f,1.0f,0.0f, 0.5f,0.0f,0.0f, 
		//T3
		-0.1f,1.0f,0.0f, 0.5f,0.0f,0.0f, 
		-0.1f,0.9f,0.0f, 0.5f,0.0f,0.0f, 
		0.1f,1.0f,0.0f, 0.5f,0.0f,0.0f, 
		//T4
		-0.1f,0.9f,0.0f, 0.5f,0.0f,0.0f, 
		0.1f,0.9f,0.0f, 0.5f,0.0f,0.0f, 
		0.1f,1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 
		//T5
		-0.1f,0.7f,0.0f, 0.5f,0.0f,0.0f, 
		-0.1f,0.8f,0.0f, 0.5f,0.0f,0.0f, 
		0.0f,0.8f,0.0f, 0.5f,0.0f,0.0f, 
		//T6
		-0.1f,0.7f, 0.0f, 0.5f, 0.0f, 0.0f, 
		0.0f,0.7f,0.0f, 0.5f,0.0f,0.0f, 
		0.0f,0.8f,0.0f, 0.5f,0.0f,0.0f, 
		// T7
		0.0f,0.8f,0.0f, 0.5f,0.0f,0.0f, 
		0.1f,0.8f,0.0f, 0.5f,0.0f,0.0f, 
		0.0f,0.5f,0.0f, 0.5f,0.0f,0.0f, 
		//T8
		0.1f,0.8f,0.0f, 0.5f,0.0f,0.0f, 
		0.0f,0.5f,0.0f, 0.5f,0.0f,0.0f, 
		0.1f,0.5f,0.0f, 0.5f,0.0f,0.0f, 
		//T9
		0.0f,0.6f,0.0f, 0.5f,0.0f,0.0f, 
		-0.2f,0.6f, 0.0f, 0.5f, 0.0f, 0.0f, 
		0.0f,0.5f,0.0f, 0.5f,0.0f,0.0f, 
		//T10
		-0.2f,0.6f,0.0f, 0.5f,0.0f,0.0f, 
		-0.2f,0.5f,0.0f, 0.5f,0.0f,0.0f, 
		0.0f,0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 

		//--------------AQUI TERMINA LA S 
		// 
		// -----------AQUI EMPIEZA LA U-----------
		//T1
		0.2f,0.5f,0.0f, 0.2f,0.0f,0.2f, 
		0.2f,1.0f,0.0f, 0.2f,0.0f,0.2f, // LETRA U EN COLOR VIOLETA 
		0.3f,1.0f,0.0f, 0.2f,0.0f,0.2f, 
		//T2
		0.2f,0.5f,0.0f, 0.2f,0.0f,0.2f, 
		0.3f,0.5f,0.0f, 0.2f,0.0f, 0.2f, 
		0.3f,1.0f,0.0f, 0.2f,0.0f,0.2f, 
		// T3
		0.3f,0.6f,0.0f, 0.2f,0.0f,0.2f, 
		0.3f,0.5f,0.0f, 0.2f,0.0f,0.2f, 
		0.5f,0.6f,0.0f, 0.2f,0.0f, 0.2f,
		//T4
		0.3f,0.5f,0.0f, 0.2f,0.0f,0.2f, 
		0.5f,0.6f,0.0f, 0.2f,0.0f,0.2f, 
		0.5f,0.5f, 0.0f, 0.2f,0.0f, 0.2f, 
		//T5
		0.5f,1.0f,0.0f, 0.2f,0.0f,0.2f, 
		0.5f,0.5f,0.0f, 0.2f,0.0f,0.2f, 
		0.6f,0.5f,0.0f, 0.2f,0.0f,0.2f, 
		//T6
		0.6f, 0.5f, 0.0f, 0.2f,0.0f,0.2f, 
		0.5f, 1.0f, 0.0f, 0.2f,0.0f,0.2f, 
		0.6f, 1.0f, 0.0f, 0.2f,0.0f,0.2f, 
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,432);
	meshColorList.push_back(letras);

	GLfloat vertices_figuras[] = {
		//X			Y			Z			R		G		B
		0.0f,	0.0f,		0.0f,			0.0f,	0.0f,	0.0f, // Triangulo azul
		0.0f,	0.0f,		0.0f,			0.0f,	0.0f,	0.0f,
		0.0f,	0.0f,		0.0f,			0.0f,	0.0f,	0.0f,

		
	};

	GLfloat vertices_cuadrados[] = {
		//X			Y			Z			R		G		B
		-1.1f,	0.8f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.1f,	0.8f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.1f,	-0.8f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadrados = new MeshColor();
	cuadrados->CreateMeshColor(vertices_cuadrados, 36);
	meshColorList.push_back(cuadrados);

}


void CreateShaders()
{	//	como cualquier lista se inicia de 0, en este caso las posiciones van de 0 a 6, guardando en total 7 elementos 
	Shader* shader1 = new Shader(); //shader PIRAMIDE
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1); 

	Shader* shader2 = new Shader();//shader CUBO 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader();// shader COLOR ROJO 
	shader3->CreateFromFiles(VRojo, FRojo);
	shaderList.push_back(*shader3); 

	Shader* shader4 = new Shader();//shader COLOR VERDE
	shader4->CreateFromFiles(VVerde, FVerde);
	shaderList.push_back(*shader4); 

	Shader* shader5 = new Shader(); //shader COLOR AZUL
	shader5->CreateFromFiles(VAzul, FAzul);
	shaderList.push_back(*shader5); 

	Shader* shader6 = new Shader(); //shader COLOR CAFE
	shader6->CreateFromFiles(VCafe, FCafe);
	shaderList.push_back(*shader6); 

	Shader* shader7 = new Shader(); //shader COLOR VERDE OSCURO
	shader7->CreateFromFiles(VVerdeOscuro, FVerdeOscuro);
	shaderList.push_back(*shader7); 
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //�ndice 0 en MeshList
	CrearCubo();//�ndice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, �ndices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensi�n 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensi�n 4x4 en la cual se almacena la multiplicaci�n de las transformaciones geom�tricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con �ndice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		
		//Inicializar matriz de dimensi�n 4x4 que servir� como matriz de modelo para almacenar las transformaciones geom�tricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se env�an al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Para el cubo y la pir�mide se usa el primer set de shaders con �ndice 0 en ShaderList
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		//Inicializar matriz de dimensi�n 4x4 que servir� como matriz de modelo para almacenar las transformaciones geom�tricas

		//Letras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();
		
		//CUBO CUERPO CASA
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//PIRAMIDE DE TECHO 
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.25f, -1.8f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// COPA DE ARBOL (PIRAMIDE)
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.3f, -1.8f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// TRONCO DE ARBOL (CUBO)
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.6f, -1.85f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// COPA DE ARBOL 2 (PIRAMIDE)
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.3f, -1.8f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// TRONCO DE ARBOL 2 (CUBO)
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.6f, -1.85f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// VENTANA CUADRADA
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.26f, -0.1f, -1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.17f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// VENTANA CUADRADA 2
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.26f, -0.1f, -1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.17f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// CUBO DE PUERTA 
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, -1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.19f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		
		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslaci�n
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACI�N //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/