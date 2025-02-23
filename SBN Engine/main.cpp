#include <iostream>
#include <Windows.h>
#include <math.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"

void input(GLFWwindow *ekran);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);
unsigned int loadCubemap(vector<std::string> faces);
void  ekranýMerkezle(GLFWwindow* window);
void jump();
void editModuGUI(GLFWwindow *ekran);

int GENISLIK  = 1920;
int YÜKSEKLÝK = 1080;
float FOV     = 75.0f;
//float time = 0;

glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
glm::vec3 scaleObject(0.1f, 0.1f, 0.1f);
glm::vec3 Kamerapos(20.0f, 5.0f, 20.0f);

// Kamera
Camera camera(Kamerapos);
double lastX    = GENISLIK  / 2.0f;
double lastY    = YÜKSEKLÝK / 2.0f;
bool firstMouse = true;

// Zamanlama Kamera hýzý için çünkü biz fps'e baðlarsak biri çok hýzlý gider diðeride çok yavaþ. Herkes eþit hýzda gitsin diye..
float deltaTime    = 0.0f;
float lastFrame    = 0.0f;			

float YERCEKIMI = -160.0f;
float ZIPLAMA_GUCU = 35.0f;
float YUKARI_HIZI = 0.0f;
bool havada = false;
bool editModu = true;
bool checkBoxSkyBox = true;
bool checkBoxvSync = true;
bool cikMenuGöster = false;
int skyBox = 0;
int screenTexture = 1;

int main()
{
	float time = 0;
    
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
        
		-0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
        
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
        
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        
		-0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
        
		-0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
    
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        
        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f 
	};
    
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
        
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
        
		-1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
        
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
	};
    
	if (!glfwInit()) {
		printf("GLFW COULDNT INITIALIZED!");
		return -1;
	}
    
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	//glfwWindowHint(GLFW_FLOATING, GL_TRUE);
    
	//4. parametreye ( NULL ) yerine ( glfwGetPrimaryMonitor() ) ekle ve fullscreen yap.
	GLFWwindow* ekran = glfwCreateWindow(GENISLIK, YÜKSEKLÝK, u8"SBN Engine(3D) | OpenGL 3.3 | Author: Þaban Burak Nazlým", glfwGetPrimaryMonitor(), nullptr);
    
	ekranýMerkezle(ekran);
    
	glViewport(0, 0, GENISLIK, YÜKSEKLÝK);
    
	glfwMakeContextCurrent(ekran);
    
	// GLFW'a Mouse kontrolü verdik
	glfwSetInputMode(ekran, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(ekran, mouse_callback);
	
	if (glewInit() != GLEW_OK) {
		printf("GLEW COULDNT INITIALIZED!");
	}
    
	GLuint lampVAO, vbo;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
    
	// screen quad VAO
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	
	
    //unsigned int transparentTexture = loadTexture("resources/textures/window.png");
	vector<std::string> faces
	{
		"res/resimler/skybox/right.jpg",
		"res/resimler/skybox/left.jpg",
		"res/resimler/skybox/top.jpg",
		"res/resimler/skybox/bottom.jpg",
		"res/resimler/skybox/front.jpg",
		"res/resimler/skybox/back.jpg"
	};
    
	unsigned int cubemapTexture = loadCubemap(faces);
	unsigned int guiTexture = loadTexture("res/resimler/container2.png");
    
	// KONSOL ÝÞLERÝ
	{
		// CONSOLE'a yazýlar
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
		printf("<<  SBN Engine (3D)  >>   \n\n");
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		printf("\n%s \n", glGetString(GL_VERSION));
		printf("%s \n", glGetString(GL_RENDERER));
	}
    
	// SHADER ÝÞLERÝ
	Shader basShader("vertexShader.glsl", "fragmentShader.glsl");//, "geometryShader.glsl");
	Shader lampShader("lampVertex.glsl", "lampFragment.glsl");
	//Shader stencilShader("StencilVertexShader.glsl", "StencilFragmentShader.glsl");
	Shader screenShader("FrameBufferVertex.glsl", "FrameBufferFragment.glsl");
	Shader skyboxShader("skyboxVertex.glsl", "skyboxFragment.glsl");
    
	screenShader.use();
	screenShader.setInt("screenTexture", screenTexture);
    
	basShader.use();
	basShader.setInt("material.diffuse", 0);
	basShader.setInt("material.specular", 0);
	//basShader.setInt("skybox", cubemapTexture);
    
	// 3D MODEL ÝÞLERÝ
	Model sponza("res/models/sponza/sponza.obj");
    
	// framebuffer configuration
    // -------------------------
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	GLuint textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GENISLIK, YÜKSEKLÝK, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GENISLIK, YÜKSEKLÝK); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
	// GL ENABLE/DÝSABLE FONKSÝYONLARI
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_MULTISAMPLE);
	    //glEnable(GL_STENCIL_TEST);
	    //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}
    
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    
	ImGui_ImplGLFW_InitForOpenGL(ekran, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
    
	// IMGUI WIDGET RENK AYARLARI
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.5f;
		style.FrameRounding = 0.5f;
		style.ScrollbarRounding = 0.1f;
		style.WindowTitleAlign = ImVec2(0.5,0.5);
        
		style.Colors[ImGuiCol_Text] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.00f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.1f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.49f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.21f, 0.21f, 0.21f, 0.49f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.49f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 0.49f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.49f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.42f, 0.42f, 0.42f, 0.49f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.21f, 0.21f, 0.21f, 0.49f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.21f, 0.21f, 0.21f, 0.49f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 0.49f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.21f, 0.21f, 0.21f, 0.49f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.55f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 0.55f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.21f, 0.21f, 0.21f, 0.49f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.68f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_Column] = ImVec4(0, 0, 0, 1); // Separator vb.
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0, 0, 0, 1);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0, 0, 0, 1);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.70f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.00f, 1.00f, 1.00f);//0.90f, 0.70f, 0.00f, 1.00f
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.00f, 1.00f, 1.00f);//1.00f, 0.60f, 0.00f, 1.00f
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}
    
	while (!glfwWindowShouldClose(ekran)) {
		glfwPollEvents();
        
		glfwSetScrollCallback(ekran, scroll_callback);
        
		// Bunu while Loopa koymamýn sebebi imgui ile kontrol etmek için
		skyboxShader.use();
		skyboxShader.setInt("skybox", skyBox);
        
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
        
		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)GENISLIK / (float)YÜKSEKLÝK, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;
        
		basShader.use();
		//basShader.setVec3("light.direction", -1.0f, 0.0f, 1.0f);
		basShader.setVec3("light.position", lightPos);
		basShader.setVec3("viewPos", camera.Position);
        
		// Iþýk özellikleri
		basShader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
		basShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		basShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        
		// Materyal özellikleri
		basShader.setFloat("material.shininess", 64.0f);
		basShader.setMat4("projection", projection);
		basShader.setMat4("view", view);
        
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0, -45, 0));
		model = glm::scale(model, scaleObject);
		basShader.setMat4("model", model);
		sponza.Draw(basShader);
        
		glDisable(GL_CULL_FACE);
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(1.0f)); // a smaller cube
		lampShader.setMat4("model", model);
		lampShader.setFloat("time", time);
		time += 0.2f;
		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
        
		basShader.setFloat("time", glfwGetTime());
        
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
        
		glBindTexture(GL_TEXTURE_2D, guiTexture);
		
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);
		// clear all relevant buffers
		screenShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		screenShader.use();
		screenShader.setInt("screenTexture", 0);
        
		input(ekran);
		if (!editModu) {
			YUKARI_HIZI += YERCEKIMI * deltaTime;
			camera.increasePosition(0, YUKARI_HIZI * deltaTime, 0);
			if (camera.Position.y <= -33) {
				havada = false;
				camera.Position.y = -33;
			}
		}
        
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGLFW_NewFrame();
		ImGui::NewFrame();
        
		if(editModu)
			editModuGUI(ekran);
        
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
		//glStencilMask(0xFF);
		glfwSwapBuffers(ekran);
	}
	
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &skyboxVBO);
    
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGLFW_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(ekran);
    
	// Clean up behind ourselves like good little programmers
    
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glfwTerminate();
}

void input(GLFWwindow *ekran) {
    
	//		 ----------Klavye----------
	if (glfwGetKey(ekran, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(ekran, true);
    
	if (glfwGetKey(ekran, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(ekran, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
	if (glfwGetKey(ekran, GLFW_KEY_UP) == GLFW_PRESS)
		editModu = true;
	if (glfwGetKey(ekran, GLFW_KEY_DOWN) == GLFW_PRESS)
		editModu = false;
    
	if (glfwGetKey(ekran, GLFW_KEY_SPACE) == GLFW_PRESS)
		if (editModu) {
        camera.Position.y += 1.0f;
    }
    else if (!editModu) {
        if (!havada) {
            jump();
            havada = true;
        }
    }
	if (glfwGetKey(ekran, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		if (editModu) {
        camera.Position.y -= 0.5f;
    }
	if (glfwGetKey(ekran, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		if (editModu) {
        camera.Position.y -= 0.5f;
    }
    
	if (glfwGetKey(ekran, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(ekran, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(ekran, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(ekran, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
    
	//		----------Fare----------
	if (glfwGetMouseButton(ekran, 2))
	{
		glfwSetInputMode(ekran, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(ekran, nullptr);
	}
    
	if (glfwGetMouseButton(ekran, 1))
	{
		glfwSetInputMode(ekran, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(ekran, GENISLIK / 2, YÜKSEKLÝK / 2);
		glfwSetCursorPosCallback(ekran, mouse_callback);
	}
    
}

void editModuGUI(GLFWwindow* ekran) {
	{
        
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Motor"))
			{
				if (ImGui::MenuItem("Sahneyi Kaydet"))
				{
					MessageBox(NULL, "Motor da sahne kaydetme özelliði henüz bulunmamaktadýr.", "Bilgilendirme",
                               MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
				}
                
				ImGui::Separator();
                
				if (ImGui::MenuItem("Kapat"))
				{
					glfwSetWindowShouldClose(ekran, true);
				}
                
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
        
		ImGui::Begin("EDIT MODU", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse); // | ImGuiWindowFlags_NoCollapse <-- Resize ve ekran kapatmýyor...
		ImGui::SliderFloat3("ISIK KONTROL", &lightPos.x, -120.0f, 120.0f);
		ImGui::SliderFloat3(u8"OBJE BÜYÜTME", &scaleObject.x, 0.1f, 50.0f);
		ImGui::SliderFloat("HIZ", &SPEED, 30.0f, 100.0f);
		ImGui::SliderFloat("ZIPLAMA", &ZIPLAMA_GUCU, 35.0f, 500.0f);
		ImGui::SliderFloat(u8"YERÇEKIMI", &YERCEKIMI, -160, 160);
		ImGui::Separator();
		if (ImGui::Checkbox(u8"GÖKYÜZÜ (AÇ/KAPAT)", &checkBoxSkyBox)) {
			if (checkBoxSkyBox) {
				skyBox = 0;
			}
			if (!checkBoxSkyBox) {
				skyBox = 1;
			}
		}
		if (ImGui::Checkbox(u8"vSync (AÇ/KAPAT)", &checkBoxvSync)) {
			if (checkBoxvSync) {
				glfwSwapInterval(1);
			}
			if (!checkBoxvSync) {
				glfwSwapInterval(0);
			}
		}
		ImGui::Separator();
		ImGui::Text(u8"Performans: %.3f ms/fps (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void mouse_callback(GLFWwindow* ekran, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
    
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
    
	lastX = xpos;
	lastY = ypos;
    
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* ekran, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
    
	int width, height, nrComponents;
    
	stbi_set_flip_vertically_on_load(true); // Resimler ters gösteriyorsa bu düzeltir.
    
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
        
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
        
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
    
	return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                         );
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
	return textureID;
}

void  ekranýMerkezle(GLFWwindow* window) {
	// Get window position and size
	int window_x, window_y;
	glfwGetWindowPos(window, &window_x, &window_y);
    
	int window_width, window_height;
	glfwGetWindowSize(window, &window_width, &window_height);
    
	// Halve the window size and use it to adjust the window position to the center of the window
	window_width *= 0.5;
	window_height *= 0.5;
    
	window_x += window_width;
	window_y += window_height;
    
	// Get the list of monitors
	int monitors_length;
	GLFWmonitor **monitors = glfwGetMonitors(&monitors_length);
    
	if (monitors == NULL) {
		// Got no monitors back
		return;
	}
    
	// Figure out which monitor the window is in
	GLFWmonitor *owner = NULL;
	int owner_x, owner_y, owner_width, owner_height;
    
	for (int i = 0; i < monitors_length; i++) {
		// Get the monitor position
		int monitor_x, monitor_y;
		glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);
        
		// Get the monitor size from its video mode
		int monitor_width, monitor_height;
		GLFWvidmode *monitor_vidmode = (GLFWvidmode*)glfwGetVideoMode(monitors[i]);
        
		if (monitor_vidmode == NULL) {
			// Video mode is required for width and height, so skip this monitor
			continue;
            
		}
		else {
			monitor_width  = monitor_vidmode->width;
			monitor_height = monitor_vidmode->height;
		}
        
		// Set the owner to this monitor if the center of the window is within its bounding box
		if ((window_x > monitor_x && window_x < (monitor_x + monitor_width)) && (window_y > monitor_y && window_y < (monitor_y + monitor_height))) {
			owner = monitors[i];
            
			owner_x = monitor_x;
			owner_y = monitor_y;
            
			owner_width = monitor_width;
			owner_height = monitor_height;
		}
	}
    
	if (owner != NULL) 
	{
		// Set the window position to the center of the owner monitor
		glfwSetWindowPos(window, owner_x + (owner_width * 0.5) - window_width, owner_y + (owner_height * 0.5) - window_height);
	}
    
}

void jump() {
	YUKARI_HIZI = ZIPLAMA_GUCU;
}
