#include "Demo.h"

Demo::Demo() {

}

Demo::~Demo() {
}

void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 3, 8), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredCube();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("texture-wood-compress.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// Table : Front Position
		-1.0, -1.0, 1.0,  0.0, 0.0,    // 0
		-0.6, -1.0, 1.0,  0.2, 0.0,    // 1
		-0.6,  0.6, 1.0,  0.2, 1.0,    // 2
		-1.0,  0.6, 1.0,  0.0, 1.0,    // 3
		 0.6, -1.0, 1.0,  0.8, 0.0,    // 4
		 1.0, -1.0, 1.0,  1.0, 0.0,    // 5
		 1.0,  0.6, 1.0,  1.0, 1.0,    // 6
		 0.6,  0.6, 1.0,  0.8, 1.0,    // 7
		 1.0,  1.0, 1.0,  1.0, 0.2,    // 8
		-1.0,  1.0, 1.0,  0.0, 0.2,    // 9

		// Table : Right Position
		1.0, -1.0,  1.0, 0.0, 0.0,   // 10
		1.0, -1.0,  0.6, 0.2, 0.0,   // 11
		1.0,  0.6,  0.6, 0.2, 1.0,   // 12
		1.0,  0.6,  1.0, 0.0, 1.0,   // 13
		1.0, -1.0, -0.6, 0.8, 0.0,   // 14
		1.0, -1.0, -1.0, 1.0, 0.0,   // 15
		1.0,  0.6, -1.0, 1.0, 1.0,   // 16
		1.0,  0.6, -0.6, 0.8, 1.0,   // 17
		1.0,  1.0, -1.0, 1.0, 1.0,   // 18
		1.0,  1.0,  1.0, 0.0, 1.0,   // 19

		// Table : Back Position
		 1.0, -1.0, -1.0, 0.0, 0.0,   // 20
		 0.6, -1.0, -1.0, 0.2, 0.0,   // 21
		 0.6,  0.6, -1.0, 0.2, 1.0,   // 22
		 1.0,  0.6, -1.0, 0.0, 1.0,   // 23
		-0.6, -1.0, -1.0, 0.8, 0.0,   // 24
		-1.0, -1.0, -1.0, 1.0, 0.0,   // 25
		-1.0,  0.6, -1.0, 1.0, 1.0,   // 26
		-0.6,  0.6, -1.0, 0.8, 1.0,   // 27
		-1.0,  1.0, -1.0, 1.0, 1.0,   // 28
		 1.0,  1.0, -1.0, 0.0, 1.0,   // 29

		// Table : Left Position
		-1.0, -1.0, -1.0, 0.0, 0.0,   // 30
		-1.0, -1.0, -0.6, 0.2, 0.0,   // 31
		-1.0,  0.6, -0.6, 0.2, 1.0,   // 32
		-1.0,  0.6, -1.0, 0.0, 1.0,   // 33
		-1.0, -1.0,  0.6, 0.8, 0.0,   // 34
		-1.0, -1.0,  1.0, 1.0, 0.0,   // 35
		-1.0,  0.6,  1.0, 1.0, 1.0,   // 36
		-1.0,  0.6,  0.6, 0.8, 1.0,   // 37
		-1.0,  1.0,  1.0, 1.0, 1.0,   // 38
		-1.0,  1.0, -1.0, 0.0, 1.0,   // 39

		// Table : Upper Position
		-1.0, 1.0,  1.0, 0.0, 0.0,   // 40
		 1.0, 1.0,  1.0, 1.0, 0.0,   // 41
		 1.0, 1.0, -1.0, 1.0, 1.0,   // 42
		-1.0, 1.0, -1.0, 0.0, 1.0    // 43
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,         // Front Left
		4,  5,  6,  4,  6,  7,         // Front Right
		3,  6,  8,  3,  8,  9,         // Front Top
		10,  11,  12,  10,  12,  13,   // Right-Left
		14,  15,  16,  14,  16,  17,   // Right-Right
		13,  16,  18,  13,  18,  19,   // Right-Top
		22,  21,  20,  23,  22,  20,   // Back-Left
		26,  25,  24,  27,  26,  24,   // Back-Right
		28,  26,  23,  29,  28,  23,   // Back-Top
		30,  31,  32,  30,  32,  33,   // Left-Left
		34,  35,  36,  34,  36,  37,   // Left-Right
		33,  36,  38,  33,  38,  39,   // Left-Top
		40,  41,  42,  40,  42,  43    // Upper
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 78, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Texture Mapping: Draw textured cube and plane", 800, 600, false, false);
}