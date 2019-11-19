#include "Angel.h"
#include <time.h> 
typedef vec4  color4;
typedef vec4  point4;

point4 cube_points[24] = {
	point4(-0.5, -0.5,  0.5, 1.0),
	point4(-0.5,  0.5,  0.5, 1.0),
	point4(0.5,  0.5,  0.5, 1.0),
	point4(0.5, -0.5,  0.5, 1.0),

	point4(0.5, -0.5,  0.5, 1.0),
	point4(0.5, -0.5,  -0.5, 1.0),
	point4(0.5,  0.5,  -0.5, 1.0),
	point4(0.5,  0.5,  0.5, 1.0),

	point4(0.5, -0.5,  -0.5, 1.0),
	point4(0.5, 0.5,  -0.5, 1.0),
	point4(-0.5, 0.5,  -0.5, 1.0),
	point4(-0.5, -0.5,  -0.5, 1.0),

	point4(-0.5, -0.5,  -0.5, 1.0),
	point4(-0.5, -0.5,  0.5, 1.0),
	point4(-0.5, 0.5,  0.5, 1.0),
	point4(-0.5, 0.5,  -0.5, 1.0),

	point4(-0.5, -0.5,  0.5, 1.0),
	point4(-0.5, -0.5,  -0.5, 1.0),
	point4(0.5, -0.5,  -0.5, 1.0),
	point4(0.5, -0.5,  0.5, 1.0),

	point4(-0.5, 0.5,  0.5, 1.0),
	point4(-0.5, 0.5,  -0.5, 1.0),
	point4(0.5, 0.5,  -0.5, 1.0),
	point4(0.5, 0.5,  0.5, 1.0),
};

// RGBA colors
color4 vertex_colors[24] = {
	color4(0.0, 1.0, 1.0, 1.0),  // cyan
	color4(0.0, 1.0, 1.0, 1.0),  // cyan
	color4(0.0, 1.0, 1.0, 1.0),  // cyan
	color4(0.0, 1.0, 1.0, 1.0),  // cyan
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 0.0, 1.0, 1.0)   // magenta
};

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;
GLuint vao[8];
//----------------------------------------------------------------------------
GLuint vColor;
int map[6][4];
mat4 model_view_array[8];
mat4 rotatematrix[8];
int face = -1;
double degree_z = -1;
int update_counter = 0;
GLuint buffers[8];
point4 points2[24], points3[24], points4[24],
points5[24], points6[24], points7[24], points8[24];
color4 colors[24], colors1[24], colors2[24], colors3[24], colors4[24], colors5[24],
colors6[24], colors7[24];//to random initialize
int colorcheck[6];
int random_flag = 0;
color4 findColor(int number) {
	switch (number) {
	case 0:
		colorcheck[0] = 1;
		return color4(0.0, 1.0, 1.0, 1.0);
	case 1:
		colorcheck[1] = 1;
		return color4(1.0, 0.0, 0.0, 1.0);
	case 2:
		colorcheck[2] = 1;
		return color4(1.0, 1.0, 0.0, 1.0);
	case 3: 
		colorcheck[3] = 1;
		return color4(0.0, 1.0, 0.0, 1.0);
	case 4:
		colorcheck[4] = 1;
		return color4(0.0, 0.0, 1.0, 1.0);
	case 5:
		colorcheck[5] = 1;
		return color4(1.0, 0.0, 1.0, 1.0);
}
}
void randomizeColor(color4 colors[24]){
	int counter = 0;
	for (int i = 0; i < 6; i++) {
		while (true) {
			int random = rand() % 6;
			if (colorcheck[random] == 0) {
				color4 color = findColor(random);
				for (int i = 0; i < 4; i++) {
					colors[counter++] = color;
				}
				break;
			}
			else
				random = rand() % 6;
		}
	}
	for (int i = 0; i < 6; i++)
		colorcheck[i] = 0;
}
//To draw the cube again after randomized
void draw() {
	srand(time(NULL));
	randomizeColor(colors);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_points), cube_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_points), sizeof(vertex_colors), colors);
	randomizeColor(colors1);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points2) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points2), points2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2), sizeof(vertex_colors), colors1);
	randomizeColor(colors2);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points3) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points3), points3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points3), sizeof(vertex_colors), colors2);
	randomizeColor(colors3);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points4) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points4), points4);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points4), sizeof(vertex_colors), colors3);
	randomizeColor(colors4);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points5) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points5), points5);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points5), sizeof(vertex_colors), colors4);
	randomizeColor(colors5);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points6) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points6), points6);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points6), sizeof(vertex_colors), colors5);
	randomizeColor(colors6);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points7) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points7), points7);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points7), sizeof(vertex_colors), colors6);
	randomizeColor(colors7);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points8) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points8), points8);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points8), sizeof(vertex_colors), colors7);

}
//To initialize the cube in the beginning of the application
void initialize() {
	// Create and initialize buffer objects for each small cube in the rubic cube
	glGenBuffers(1, &buffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_points), cube_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_points), sizeof(vertex_colors), vertex_colors);

	glGenBuffers(1, &buffers[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points2) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points2), points2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points2), sizeof(vertex_colors), vertex_colors);

	glGenBuffers(1, &buffers[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points3) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points3), points3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points3), sizeof(vertex_colors), vertex_colors);

	glGenBuffers(1, &buffers[3]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points4) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points4), points4);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points4), sizeof(vertex_colors), vertex_colors);

	glGenBuffers(1, &buffers[4]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points5) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points5), points5);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points5), sizeof(vertex_colors), vertex_colors);

	glGenBuffers(1, &buffers[5]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points6) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points6), points6);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points6), sizeof(vertex_colors), vertex_colors);

	glGenBuffers(1, &buffers[6]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points7) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points7), points7);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points7), sizeof(vertex_colors), vertex_colors);

	glGenBuffers(1, &buffers[7]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points8) + sizeof(vertex_colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points8), points8);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points8), sizeof(vertex_colors), vertex_colors);
	
	// Create a vertex array object
	glGenVertexArrays(8, vao);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * 24));

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * 24));

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * 24));

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * 24));

	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * 24));

	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * 24));

	glBindVertexArray(vao[6]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[6]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * 24));

	glBindVertexArray(vao[7]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[7]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point4) * 24));

}
// OpenGL initialization
mat4  projection;
void
init()
{
	for (int i = 0; i < 24; i++) {
		colors[i] = vertex_colors[i];
		colors1[i] = vertex_colors[i];
		colors2[i] = vertex_colors[i];
		colors3[i] = vertex_colors[i];
		colors4[i] = vertex_colors[i];
		colors5[i] = vertex_colors[i];
		colors6[i] = vertex_colors[i];
		colors7[i] = vertex_colors[i];
	}
	for (int i = 0; i < 24; i++) {
		points2[i] = Translate(vec3(1.02, 0, 0))*cube_points[i];
		points3[i] = Translate(vec3(1.02, 0, -1.02))*cube_points[i];
		points4[i] = Translate(vec3(1.02, 1.02, 0))*cube_points[i];
		points5[i] = Translate(vec3(1.02, 1.02, -1.02))*cube_points[i];
		points6[i] = Translate(vec3(0, 0, -1.02))*cube_points[i];
		points7[i] = Translate(vec3(0, 1.02, 0))*cube_points[i];
		points8[i] = Translate(vec3(0, 1.02, -1.02))*cube_points[i];
	}
	//initialize vaos and buffers
	initialize();

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	// Set projection matrix

	projection = Ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // Ortho(): user-defined function in mat.h
	//projection = Perspective( 45.0, 5.0, 5, 5.0 );
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	map[0][0] = 6;
	map[0][1] = 3;
	map[0][2] = 0;
	map[0][3] = 1;

	map[1][0] = 7;
	map[1][1] = 6;
	map[1][2] = 5;
	map[1][3] = 0;	
	
	map[2][0] = 3;
	map[2][1] = 4;
	map[2][2] = 1;
	map[2][3] = 2;

	map[3][0] = 4;
	map[3][1] = 7;
	map[3][2] = 2;
	map[3][3] = 5;

	map[4][0] = 0;
	map[4][1] = 1;
	map[4][2] = 5;
	map[4][3] = 2;

	map[5][0] = 7;
	map[5][1] = 4;
	map[5][2] = 6;
	map[5][3] = 3;

	for (int i = 0; i < 8; i++)
		model_view_array[i] = Scale(0.3, 0.3, 0.3);
	face = 0;
}
//----------------------------------------------------------------------------
double degree_counter = 90.0;
int view_counter = 0;
int view_counter_z = 0;
//  Generate tha model-view matrix
const vec3 displacement(0.0, 0.0, 0.0);
mat4  model_view= Scale(0.3, 0.3, 0.3) ;
void
display(void)
{
	mat4 center_minus = Translate(vec3(-0.5, -0.5, +0.5));
	mat4 center = Translate(vec3(0.5, 0.5, -0.5));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < 8; i++) {
			model_view_array[i] *= center * rotatematrix[i] * center_minus;
		}

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4)*24, sizeof(vertex_colors), colors);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4)*24, sizeof(vertex_colors), colors1);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), colors2);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), colors3);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), colors4);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), colors5);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[6]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), colors6);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[7]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), colors7);


	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view_array[0]);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glDrawArrays(GL_QUADS, 0, 24);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view_array[1]);
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glDrawArrays(GL_QUADS, 0, 24);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view_array[2]);
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_QUADS, 0, 24);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view_array[3]);
	glBindVertexArray(vao[3]);
	glDrawArrays(GL_QUADS, 0, 24);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view_array[4]);
	glBindVertexArray(vao[4]);
	glDrawArrays(GL_QUADS, 0, 24);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view_array[5]);
	glBindVertexArray(vao[5]);
	glDrawArrays(GL_QUADS, 0, 24);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view_array[6]);
	glBindVertexArray(vao[6]);
	glDrawArrays(GL_QUADS, 0, 24);
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view_array[7]);
	glBindVertexArray(vao[7]);
	glDrawArrays(GL_QUADS, 0, 24);

	for(int i=0;i<8;i++)
		model_view_array[i] = (Scale(0.3, 0.3, 0.3) * Translate(displacement));

	model_view  = (Scale(0.3, 0.3, 0.3) * Translate(displacement));

	glutSwapBuffers();

}
//----------------------------------------------------------------------------
void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 's':
		face = 0;
		view_counter_z = 5;
		degree_z = 0;
		update_counter = 1;
		break;
	case 'a':
		face = 1;
		view_counter_z = 5;
		degree_z = 0;
		update_counter = 1;
		break;
	case 'd':
		face = 2;
		view_counter_z = 5;
		degree_z = 0;
		update_counter = 1;
		break;
	case 'f':
		face = 3;
		view_counter_z = 5;
		degree_z = 0;
		update_counter = 1;
		break;
	case 'x':
		face = 4;
		view_counter_z = 5;
		degree_z = 0;
		update_counter = 1;
		break;
	case 'w':
		face = 5;
		view_counter_z = 5;
		degree_z = 0;
		update_counter = 1;
		break;
	case 'i':
		printf("To start with randomized faces, mouse right click and choose randomly initialze.\nTo see the cube from different angles, press mouse right button and choose one of the faces from the menu to see.");
		printf("\nTo play and rotate different faces, you can use keyboard because I thought that this is the best way to play. To rotate specific user chosen faces, use the keybord: 'a' rotates left face, 'w' rotates the top face, 'x' rotates the bottom face, 'd' rotates the right face, 's' rotates the front face and 'f' rotates the behind face.");
		printf("\nPlease wait until a rotation animation is completed because if you choose another rotation before the first one finishes, since the faces matrix,which keeps the cube-face information, is calculated at the end of the rotation, will be distorted.");
		printf("\nFor the first turn of each faces, another way to do rotations is by choosing cubes with mouse and doing rotation. You can first see the face you want by rotating whole cube and then while rotating you can choose a face to rotate.");
		printf("\n For example, to turn the above face, first we need to see the above face, so first turn the whole cube by choosing 'top face' and while cube is rotating choose two diagonal faces to rotate that specific face you want to rotate.");
		printf("To see how rotations affected whole cube, we can rotate whole cube by rigth click.");
		break;
	}
}

//----------------------------------------------------------------------------
void updateCubeIndex() {
	if (face == 0) {
		int temp  = map[0][0];
		map[0][0] = map[0][1];
		map[0][1] = map[0][3];
		map[0][3] = map[0][2];
		map[0][2] = temp;

		map[1][1] = map[0][0];
		map[1][3] = map[0][2];

		map[2][0] = map[0][1];
		map[2][2] = map[0][3];

		map[4][0] = map[0][2];
		map[4][1] = map[0][3];

		map[5][2] = map[0][0];
		map[5][3] = map[0][1];
	}
	else if (face == 1) {
		int temp  = map[1][0];
		map[1][0] = map[1][1];
		map[1][1] = map[1][3];
		map[1][3] = map[1][2];
		map[1][2] = temp;

		map[3][1] = map[1][0];
		map[3][3] = map[1][2];

		map[0][0] = map[1][1];
		map[0][2] = map[1][3];

		map[4][0] = map[1][3];
		map[4][2] = map[1][2];

		map[5][0] = map[1][0];
		map[5][2] = map[1][1];
	}
	else if (face == 3) {
		int temp  = map[3][0];
		map[3][0] = map[3][1];
		map[3][1] = map[3][3];
		map[3][3] = map[3][2];
		map[3][2] = temp;

		map[1][0] = map[3][1];
		map[1][2] = map[3][3];

		map[5][0] = map[3][1];
		map[5][1] = map[3][0];

		map[2][1] = map[3][0];
		map[2][3] = map[3][2];

		map[4][2] = map[3][3];
		map[4][3] = map[3][2];
	}

	else if (face == 2) {
		int temp  = map[2][0];
		map[2][0] = map[2][1];
		map[2][1] = map[2][3];
		map[2][3] = map[2][2];
		map[2][2] = temp;

		map[0][1] = map[2][0];
		map[0][3] = map[2][2];

		map[3][0] = map[2][1];
		map[3][2] = map[2][3];

		map[4][1] = map[2][2];
		map[4][3] = map[2][3];

		map[5][1] = map[2][1];
		map[5][3] = map[2][0];
	}

	else if (face == 5) {
		int temp  = map[5][0];
		map[5][0] = map[5][1];
		map[5][1] = map[5][3];
		map[5][3] = map[5][2];
		map[5][2] = temp;

		map[3][0] = map[5][1];
		map[3][1] = map[5][0];

		map[0][0] = map[5][2];
		map[0][1] = map[5][3];

		map[1][0] = map[5][0];
		map[1][1] = map[5][2];

		map[2][1] = map[5][1];
		map[2][0] = map[5][3];
	}
	
	else if (face == 4) {
		int temp  = map[4][0];
		map[4][0] = map[4][1];
		map[4][1] = map[4][3];
		map[4][3] = map[4][2];
		map[4][2] = temp;

		map[1][2] = map[4][2];
		map[1][3] = map[4][0];

		map[0][2] = map[4][0];
		map[0][3] = map[4][1];

		map[2][2] = map[4][1];
		map[2][3] = map[4][3];

		map[3][2] = map[4][3];
		map[3][3] = map[4][2];
	}
}
int color_counter;
int face_color[8];
//To decide on the rotate face by picking with mouse
//The user chooses two cubes with mouse and this function finds which face to rotate
void findRotateFace() {
	int counter = 0;
	int cube1 =-1, cube2 =-1;
	for (int i = 0; i < 8; i++) {
		if (face_color[i] == 1) {
			if (cube1 == -1)
				cube1 = i;
			else
				cube2 = i;
		}
	}
	printf("\nFirst Cube Number: %d, Second Cube Number: %d", cube1, cube2);
	int face_inside = -1;
	for (int i = 0; i < 6; i++) {
		for (int k = 0; k < 4; k++) {
			if (map[i][k] == cube1) {
				for (int m = 0; m < 4; m++) {
					if (map[i][m] == cube2) {
						face_inside = i;
						break;
					}
				}
			}
			if (face_inside != -1)
				break;
		}	
	}
	printf("\nRotation Face: %d", face_inside);
	for (int i = 0; i < 8; i++)
		face_color[i] = 0;
	face = face_inside;
	view_counter_z = 5;
	degree_z = 0;
	update_counter = 1;
}
void
mouse(int button, int state, int x, int y)
{
		if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			color4 backcolor[24], backcolor1[24], backcolor2[24], backcolor3[24],
				backcolor4[24], backcolor5[24], backcolor6[24], backcolor7[24];
			for (int i = 0; i < 24; i++) {
				backcolor[i]  = color4(0.2, 0, 0, 1);
				backcolor1[i] = color4(0, 0.2, 0, 1);
				backcolor2[i] = color4(0, 0, 0.2, 1);
				backcolor3[i] = color4(0.2, 0.2, 0, 1);
				backcolor4[i] = color4(0, 0.2, 0.2, 1);
				backcolor5[i] = color4(0.2, 0, 0.2, 1);
				backcolor6[i] = color4(0.2, 0.2, 0.2, 1);
				backcolor7[i] = color4(0, 0, 0, 1);
			}
			glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4)*24, sizeof(vertex_colors), backcolor);
			glBindVertexArray(vao[0]);
			glDrawArrays(GL_QUADS, 0, 24);

			glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), backcolor1);
			glBindVertexArray(vao[1]);
			glDrawArrays(GL_QUADS, 0, 24);

			glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), backcolor2);
			glBindVertexArray(vao[2]);
			glDrawArrays(GL_QUADS, 0, 24);

			glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), backcolor3);
			glBindVertexArray(vao[3]);
			glDrawArrays(GL_QUADS, 0, 24);

			glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), backcolor4);
			glBindVertexArray(vao[4]);
			glDrawArrays(GL_QUADS, 0, 24);

			glBindBuffer(GL_ARRAY_BUFFER, buffers[5]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), backcolor5);
			glBindVertexArray(vao[5]);
			glDrawArrays(GL_QUADS, 0, 24);

			glBindBuffer(GL_ARRAY_BUFFER, buffers[6]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), backcolor6);
			glBindVertexArray(vao[6]);
			glDrawArrays(GL_QUADS, 0, 24);

			glBindBuffer(GL_ARRAY_BUFFER, buffers[7]);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * 24, sizeof(vertex_colors), backcolor7);
			glBindVertexArray(vao[7]);
			glDrawArrays(GL_QUADS, 0, 24);
			
			glFlush();

			y = glutGet(GLUT_WINDOW_HEIGHT) - y;

			unsigned char pixel[4];
			glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		
			if (pixel[0] == 51 && pixel[1] == 0 && pixel[2] == 0) {
				printf("Zeroth cube");
				color_counter++;
				face_color[0] = 1;
			}
			else if (pixel[0] == 0 && pixel[1] == 51 && pixel[2] == 0) {
				printf("First cube");
				color_counter++;
				face_color[1] = 1;
			}
			else if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 51) {
				printf("Second cube");
				color_counter++;
				face_color[2] = 1;
			}
			else if (pixel[0] == 51 && pixel[1] == 51 && pixel[2] == 0) {
				printf("Third cube");
				color_counter++;
				face_color[3] = 1;
			}
			else if (pixel[0] == 0 && pixel[1] == 51 && pixel[2] == 51) {
				printf("Fourth cube");
				color_counter++;
				face_color[4] = 1;
			}
			else if (pixel[0] == 51 && pixel[1] == 0 && pixel[2] == 51) {
				printf("Fifth cube");
				color_counter++;
				face_color[5] = 1;
			}
			else if (pixel[0] == 51 && pixel[1] == 51 && pixel[2] == 51) {
				printf("Sixth cube");
				color_counter++;
				face_color[6] = 1;
			}
			else if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0) {
				printf("Seventh cube");
				color_counter++;
				face_color[7] = 1;
			}
			//else std::cout << "None" << std::endl;

			/*view_counter_z = 5;
			degree_z = 0;
			update_counter = 1;*/
			
			//std::cout << "\nR: " << (int)pixel[0] << std::endl;
			//std::cout << "G: " << (int)pixel[1] << std::endl;
			//std::cout << "B: " << (int)pixel[2] << std::endl;
			//std::cout << std::endl;

			if (color_counter == 2) {
				findRotateFace();
				color_counter = 0;
			}
		
		}
}
//----------------------------------------------------------------------------
void
idle(void)
{
	mat4 center_minus = Translate(vec3(-0.5, -0.5, +0.5));
	mat4 center = Translate(vec3(0.5, 0.5, -0.5));
	if (degree_counter <= 360 && view_counter == 1) {
		degree_counter += 0.09;
		for (int i = 0; i < 8; i++)
			rotatematrix[i] = RotateY(0.09) * rotatematrix[i] ;
		
	}
	else if (degree_counter >= 0.0 && view_counter == 2) {
		degree_counter -= 0.09;
		for (int i = 0; i < 8; i++)
			rotatematrix[i] =  RotateY(-0.09) * rotatematrix[i];
	}
	else if (degree_counter <= 360.0 && view_counter == 3) {
		degree_counter += 0.09;
		for (int i = 0; i < 8; i++)
			rotatematrix[i] = RotateX(0.09) * rotatematrix[i];
	}
	else if (degree_counter >= 0.0 && view_counter == 4) {
		degree_counter -= 0.09;
		for (int i = 0; i < 8; i++)
			rotatematrix[i] = RotateX(-0.09) * rotatematrix[i];
	}
	else if (degree_z < 90 && view_counter_z == 5) {
		degree_z += 0.09;
		for (int i = 0; i < 4; i++) {
			if(face==0)
				rotatematrix[map[face][i]] = RotateZ(0.09) * rotatematrix[map[face][i]];
			else if (face == 3)
				rotatematrix[map[face][i]] = RotateZ(-0.09) * rotatematrix[map[face][i]];
			else if (face==1)
				rotatematrix[map[face][i]] = RotateX(-0.09) * rotatematrix[map[face][i]];
			else if (face == 2)
				rotatematrix[map[face][i]] = RotateX(0.09) * rotatematrix[map[face][i]];
			else if (face==5)
				rotatematrix[map[face][i]] = RotateY(0.09) * rotatematrix[map[face][i]];
			else if (face == 4)
				rotatematrix[map[face][i]] = RotateY(-0.09) * rotatematrix[map[face][i]];
		}
	}
	else if( degree_z>=90 && update_counter!=0){
		//for (int i = 0; i < 4; i++)
			//printf("before face: %d Index: %d Value: %d\n", face, i, map[face][i]);
		updateCubeIndex();
		//for (int i = 0; i < 4; i++) 
			//printf("after face: %d Index: %d Value: %d\n",face, i, map[face][i]);
		update_counter = 0;
	}
	
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void viewMenu(int id)
{
	switch (id)
	{
	case 1: /* rotate left 90 degrees to see the left face */
		degree_counter = 0;
		view_counter = 1;
		break;
	case 2: /* rotate left 90 degrees to see the right face */
		degree_counter = 360;
		view_counter = 2;
		break;
	case 3: /* rotate left 90 degrees to see the top face */
		degree_counter = 0;
		view_counter = 3;
		break;
	case 4: /* rotate left 90 degrees to see the bottom face */
		degree_counter = 360;
		view_counter = 4;
		break;
	case 5:
		draw();
		break;
}
}
void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Rubik's Cube");

	printf("Supported GLSL version is %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("%s\n%s\n%s\n",
		glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
		glGetString(GL_VERSION),    // e.g. 3.2 INTEL-8.0.61
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);

	int view_Menu = glutCreateMenu(viewMenu);
	glutAddMenuEntry("Left Face", 1);
	glutAddMenuEntry("Right Face", 2);
	glutAddMenuEntry("Top Face", 3);
	glutAddMenuEntry("Bottom Face", 4);
	glutAddMenuEntry("Randomly Initialize", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}