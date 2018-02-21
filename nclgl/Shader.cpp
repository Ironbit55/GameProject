#include "Shader.h"

//I added in some of the capabilities of the shader class we got given in graphics
//1. use default shaders on failure to load shader
//TODO: 2. handle variety of shader types

#define BACKUP_SHADER

#ifdef BACKUP_SHADER
const string defaultVertex =
"#version 150 core\n"
"uniform mat4 modelMatrix;"
"uniform mat4 viewMatrix; "
"uniform mat4 projMatrix; "
"in  vec3 position;"
"in  vec2 texCoord;"
"in  vec4 colour;  "
"out Vertex{		"
"	vec2 texCoord;	"
"	vec4 colour;	"
"} OUT;				"
"void main(void)	{"
"	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);"
"	OUT.texCoord = texCoord;"
"	OUT.colour	 = colour;"
"}";

const string defaultFragment =
"#version 150 core\n"
"in Vertex{"
"	vec2 texCoord;"
"	vec4 colour;"
"} IN;"
"out vec4 gl_FragColor;"
"void main(void)	{"
"	gl_FragColor = vec4(1,0,1,1);"
"}";
#endif

Shader::Shader(string vFile, string fFile, string gFile)	{
	vertexName	= vFile;
	fragName	= fFile;
	geomName	= gFile;

	Reload(false);
}

Shader::~Shader(void)	{
	for(int i = 0; i < 3; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	glDeleteProgram(program);
}

void	Shader::Reload(bool deleteOld) {
	usingBackupShader = false;

	if(deleteOld) {
		for(int i = 0; i < 3; ++i) {
			glDetachShader(program, objects[i]);
			glDeleteShader(objects[i]);
		}
		glDeleteProgram(program);
	}

	program		= glCreateProgram();
	objects[SHADER_VERTEX]		= GenerateShader(vertexName	 ,GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT]	= GenerateShader(fragName,GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY]	= 0;

	if(!geomName.empty()) {
		objects[SHADER_GEOMETRY]	= GenerateShader(geomName,GL_GEOMETRY_SHADER);
		glAttachShader(program,objects[SHADER_GEOMETRY]);
	}

	glAttachShader(program,objects[SHADER_VERTEX]);
	glAttachShader(program,objects[SHADER_FRAGMENT]);

	SetDefaultAttributes();

	LinkProgram();

#ifdef BACKUP_SHADER
	if (loadFailed) {
		LoadDefaultShader();
	}
#endif
}


bool	Shader::LoadShaderFile(string from, string &into)	{
	ifstream	file;
	string		temp;

	cout << "Loading shader text from " << from << endl << endl;

	file.open(from.c_str());
	if(!file.is_open()){
		cout << "File does not exist!" << endl;
		return false;
	}

	while(!file.eof()){
		getline(file,temp);
		into += temp + "\n";
	}

	cout << into << endl << endl;

	file.close();
	cout << "Loaded shader text!" << endl << endl;
	return true;
}

GLuint	Shader::GenerateShader(string from, GLenum type)	{
	cout << "Compiling Shader..." << endl;

	string load;
	if(!LoadShaderFile(from,load)) {
		cout << "Compiling failed!" << endl;
		loadFailed = true;
		return 0;
	}

	GLuint shader = glCreateShader(type);

	const char *chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)	{
		cout << "Compiling failed!" << endl;
		char error[2048];
		//glGetInfoLogARB(shader, sizeof(error), NULL, error);

		glGetShaderInfoLog(shader, 2048, NULL, error);

		cout << error;
		loadFailed = true;
		return 0;
	}
	cout << "Compiling success!" << endl << endl;
	loadFailed = false;
	return shader;
}

bool Shader::LinkProgram()	{
	if(loadFailed) {
		return false;
	}
	glLinkProgram(program); 

	GLint status = 1;
	glGetShaderiv(program, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)	{
		cout << "Linking failed!" << endl;
		char error[8192];
		//glGetInfoLogARB(program, sizeof(error), NULL, error);

		glGetProgramInfoLog(program, 8192, NULL, error);
		cout << error;
		loadFailed = true;
		return false;
	}
	return true;
}


void	Shader::SetDefaultAttributes()	{
	glBindAttribLocation(program, VERTEX_BUFFER,  "position");
	glBindAttribLocation(program, COLOUR_BUFFER,  "colour");
	glBindAttribLocation(program, NORMAL_BUFFER,  "normal");
	glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");
}

/*

If your shader all goes wrong, this function will be called, to create the
emergency 'default' shader. It goes through all of the stages outlined in
the tutorial notes, but instead of reading from a file, it uses some const
strings I defined at the top

*/
void	Shader::LoadDefaultShader() {
	for (int i = 0; i < 3; ++i) {
		if (objects[i]) {
			glDetachShader(program, objects[i]);
			glDeleteShader(objects[i]);
			objects[i] = 0;
		}
	}
	//loadSuccess = true;
	//linkSuccess = true;
	loadFailed = false;
	usingBackupShader = true;

	const char *vertchars = defaultVertex.c_str();
	const char *fragchars = defaultFragment.c_str();

	objects[SHADER_VERTEX] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(objects[SHADER_VERTEX], 1, &vertchars, NULL);
	glCompileShader(objects[SHADER_VERTEX]);

	objects[SHADER_FRAGMENT] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(objects[SHADER_FRAGMENT], 1, &fragchars, NULL);
	glCompileShader(objects[SHADER_FRAGMENT]);

	glAttachShader(program, objects[SHADER_VERTEX]);
	glAttachShader(program, objects[SHADER_FRAGMENT]);

	SetDefaultAttributes();
	LinkProgram();

	cout << "Your shader program has failed! Reverting to default shader..." << endl;
}