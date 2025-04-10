/*
Práctica 8: Iluminación 2 
Alumna: Dannae Sanchez Duran - 422526067
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture octaTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

//CREACION DEL AUTO
Model AUTOCARCASA;
Model ESPEJO_NOEDIT;
Model CAPOTEBUENO;
Model LLANTADERECHA_ATRAS;
Model LLANTADERECHAFRONTAL;
Model LLANTAIZQ_ATRAS;
Model LLANTAIZQ_FRONTAL;

//CREACION DE LA LAMPARA
Model LAMPARA;

//NUEVO MODELO PARA AGREGARLE UNA LUZ DEL EJERCICIO 3-PRACTICA 8
Model HACHA;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Variable global para guardar el estado de la luz 
bool luz_encendida = false;
bool luz = false;
static double Decision = 0.0;
static double Dec = 0.0;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		-1.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		-1.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		-1.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		-1.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, -1.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	// ------------------------------- AQUI SE MANDA A LLAMAR A LOS MODELOS ---------------
	AUTOCARCASA = Model();
	AUTOCARCASA.LoadModel("MODEL_CARRO_SEPARADO/AUTOCARCASA.obj");

	ESPEJO_NOEDIT = Model();
	ESPEJO_NOEDIT.LoadModel("MODEL_CARRO_SEPARADO/ESPEJO_NOEDIT.obj");

	CAPOTEBUENO = Model();
	CAPOTEBUENO.LoadModel("MODEL_CARRO_SEPARADO/CAPOTE_DISENO_USAR.obj");

	LLANTADERECHA_ATRAS = Model();
	LLANTADERECHA_ATRAS.LoadModel("MODEL_CARRO_SEPARADO/LLANTA_COM_DERE_ATRAS.obj");

	LLANTADERECHAFRONTAL = Model();
	LLANTADERECHAFRONTAL.LoadModel("MODEL_CARRO_SEPARADO/LLANTA_COM_DERE_ATRAS.obj");

	LLANTAIZQ_ATRAS = Model();
	LLANTAIZQ_ATRAS.LoadModel("MODEL_CARRO_SEPARADO/LLANTA_COM_DERE_ATRAS.obj");

	LLANTAIZQ_FRONTAL = Model();
	LLANTAIZQ_FRONTAL.LoadModel("MODEL_CARRO_SEPARADO/LLANTA_COM_DERE_ATRAS.obj");

	// --------------------------------------------------------------------------------

	//--------------------- CREACION DE LA LAMPARA
	LAMPARA = Model();
	LAMPARA.LoadModel("LAMPARA/lamparatext.obj");
	//---------------------------------------------------

	//-------------------- CREACION DEL OBJETO EXTRA (HACHA)
	HACHA = Model();
	HACHA.LoadModel("MODELO_STEVENU/guadana.obj");
	//---------------------------------------------------

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	octaTexture = Texture("Textures/octaedro.png");
	octaTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual
	//LUZ PARA EL NUEVO MODELO AGREGADO
	pointLights[0] = PointLight(0.5f, 0.5f, 0.5f, //Color blanco 
		0.5f, 1.0f, //intensidad ambiental y difusa 
		-50.0f, -1.0f, 50.0f, //posicion en el escenario (x,y,z)
		0.3f, 0.01f, 0.01f); //atenuacion (const, linear, quadratic)
	pointLightCount++;

	//LUZ PARA LA LAMPARA
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		-30.0f, -1.0f, 65.0f, //POSICION
		0.3f, 0.01f, 0.01f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, //--------- NO SE OCUPO ------------
		0.0f, 2.0f, // Ilumina un punto
		0.0f, 0.0f, 0.0f, // Origen
		0.0f, -1.0f, 0.0f, // Apunta
		1.0f, 0.0f, 0.0f, // Con, Lin, Exp
		10.0f); // Abertura de luz
	spotLightCount++;

	//luz trasera
	spotLights[1] = SpotLight(
		0.0f, 1.0f, 0.0f, // verde 
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	// Luz delantera
	spotLights[2] = SpotLight(
		1.0f, 0.0f, 0.0f, // roJO
		1.0f, 1.0f, // Intensidad de iluminación
		0.0f, 0.0f, 0.0f, // Posición de la fuente
		-0.1f, 0.0f, 0.0f, // Dirección del haz de luz
		0.001f, 0.001f, 0.001f, // Factores de atenuación (constante, lineal, exponencial)
		20.0f); // Ángulo de apertura del foco
	spotLightCount++;

	// luz capo
	spotLights[3] = SpotLight(
		0.5f, 0.5f, 0.0f, // Color AMARILLO
		1.0f, 1.0f, // Intensidad de iluminación
		0.0f, 0.8f, 0.1f, // Posición de la fuente
		-0.1f, 0.0f, 0.0f, // Dirección del haz de luz
		0.001f, 0.001f, 0.001f, // Factores de atenuación (constante, lineal, exponencial)
		20.0f); // Ángulo de apertura del foco
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
	/*		glm::vec3 lowerLight = camera.getCameraPosition();
		/*lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/

		//información al shader de fuentes de iluminación
		/*shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount-1);*/

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//AUTO CUERPO ----------------- EJERCICIO 2-----------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion1(), 0.0f,0.0f)); // SE MUEVE CON Z
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion0(), 0.0f, 0.0f)); //SE MUEVE CON F
		// AQUI SE MUEVE LA LUZ CON EL CARRO
		glm::vec3 Faro = glm::vec3(-8.0f + mainWindow.getarticulacion1() + mainWindow.getarticulacion0(), 3.4f, 1.2f);
		spotLights[2].SetFlash(Faro, glm::vec3(-1.0f, 0.0f, 0.0f));

		glm::vec3 Trasero = glm::vec3(12.0f + mainWindow.getarticulacion1() + mainWindow.getarticulacion0(), 4.2f, 1.2f);
		spotLights[1].SetFlash(Trasero, glm::vec3(1.0f, 0.0f, 0.0f));

		Dec = Decision;

		Decision = mainWindow.getarticulacion1() + mainWindow.getarticulacion0();

		if (Decision > Dec) {
			Dec = -5;  // Si Decision es mayor que des, se resta des de Decision
		}
		else if (Decision < Dec) {
			Dec = 5;  // Si Decision es menor que des, se resta Decision de des
		}
		// Actualizar el estado de la luz basado en la señal de articulacion3
		if (Dec == -5) {
			luz = true;
		}
		else if (Dec == 5) {
			luz = false;
		}
		// Aplicar el estado actual de la luz
		if (luz) {

			spotLights[2].SetFlash(Faro, glm::vec3(0.0f, 0.0f, 0.0f));
			spotLights[1].SetFlash(Trasero, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else {
			spotLights[2].SetFlash(Faro, glm::vec3(-1.0f, 0.0f, 0.0f));
			spotLights[1].SetFlash(Trasero, glm::vec3(0.0f, 0.0f, 0.0f));
		}
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AUTOCARCASA.RenderModel();

		//ESPEJO
		model = modelaux;						//	Z,		Y,  X
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.1f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ESPEJO_NOEDIT.RenderModel();

		//------- ***************** JERARQUIA DEL AUTO **************------------------------
			//COFRE --------- EJERCICIO 1
		model = modelaux;						//	Z,		Y,  X
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(1.0f, 0.0f, 0.0f)); //SE MUEVE CON T
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		// Define la posición base del capó
		glm::vec3 capoPosition = glm::vec3(0.3f, 18.0f, 2.0f);

		// Obtenemos las articulaciones 4 y 5 (deberías tener métodos en `mainWindow` para obtener estos valores)
		float desplazamientoX = mainWindow.getarticulacion1() + mainWindow.getarticulacion0();  // Combina las dos articulaciones para el desplazamiento en X

		// Ahora actualizas la posición de la luz sumando el desplazamiento al valor de X
		capoPosition.x += desplazamientoX;  // La luz se mueve en el eje X con base en las articulaciones 4 y 5

		// Dirección original de la luz (suponiendo que la luz originalmente apunta en la dirección X negativa)
		glm::vec3 direccionLuzOriginal = glm::vec3(-1.0f, 0.0f, 0.0f);

		// Rota la dirección de la luz con la misma rotación aplicada al modelo
		glm::mat4 rotacionLuz = glm::rotate(glm::mat4(1.0f), glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 direccionLuzRotada = glm::vec3(rotacionLuz * glm::vec4(direccionLuzOriginal, 0.0f));

		// Ahora aplicamos la nueva posición y dirección de la luz
		spotLights[3].SetFlash(capoPosition, direccionLuzRotada);  // La luz se mueve en X y rota con el capó

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CAPOTEBUENO.RenderModel();
		

		// LLANTA DERECHA ATRAS
		model = modelaux;					//	Z,		y,		x
		model = glm::translate(model, glm::vec3(-1.8f, -0.4f, -2.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(1.0f, 0.0f, 0.0f)); //SE MUEVE CON K
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLANTADERECHA_ATRAS.RenderModel();

		//LLANTA DERECHA FRONTAL YA QUEDO
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.8f, -0.4f, 3.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f)); //SE MUEVE CON J 
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLANTADERECHAFRONTAL.RenderModel();

		// LLANTA IZQUIERda adelante YA QUEDO POSICIONADA
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.8f, -0.4f, 3.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(1.0f, 0.0f, 0.0f)); //SE MUEVE CON H 
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLANTAIZQ_ATRAS.RenderModel();

		// LLANTA IZQUIERDA ATRAS
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.8f, -0.4f, -2.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f)); //SE MUEVE CON G
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LLANTAIZQ_FRONTAL.RenderModel();
		//------------------------ AQUI TERMINA EL AUTO -------------

		
		//LAMPARA ---------- EJERCICIO 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -1.0f, 50.0f)); // X simétrico, Z ajustado
		// Actualizar el estado de la luz basado en la señal de articulacion3
		if (mainWindow.getLamparaBoton() == 1) {
			luz_encendida = true;
		}
		else if (mainWindow.getLamparaBoton() == 0) {
			luz_encendida = false;
		}

		// Aplicar el estado actual de la luz
		if (luz_encendida) {
			// La luz está encendida
			shaderList[0].SetPointLights(&pointLights[1], 1);

		}
		else {
			// La luz está apagada
			shaderList[0].SetPointLights(&pointLights[1], 0);

		}
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LAMPARA.RenderModel();

		// HACHA 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 50.0f)); // X simétrico, Z ajustado

		// Actualizar el estado de la luz basado en la señal de articulacion3
		if (mainWindow.getluz_hacha() == 1) {
			luz_encendida = true;
		}
		else if (mainWindow.getluz_hacha() == 0) {
			luz_encendida = false;
		}

		// Aplicar el estado actual de la luz
		if (luz_encendida) {
			// La luz está encendida
			shaderList[0].SetPointLights(&pointLights[0], 1);

		}
		else {
			// La luz está apagada
			shaderList[0].SetPointLights(&pointLights[0], 0);

		}

		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HACHA.RenderModel();
		

		//// HELICOPTERO
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f + mainWindow.getadelante_heli() + mainWindow.getatras_heli(), 50.0f, 80.0f));
		//glm::vec3 Luz_Helicoptero = glm::vec3(0.0f + mainWindow.getadelante_heli() + mainWindow.getatras_heli(), 50.0f, 80.0f);
		//spotLights[3].SetFlash(Luz_Helicoptero, glm::vec3(0.0f, -1.5f, 0.0f)); //ATRAS ES CON LA TECLA M Y ADELANTE TECLA N
		//modelaux = model;
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Blackhawk_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//GEOMETRIA CON TRANSLUCIDES O TRANSPARENCIA VAN HASTA ABAJO 
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
