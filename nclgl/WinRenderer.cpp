#include "WinRenderer.h"


WinRenderer::WinRenderer(Window& window) : OGLRenderer(), window(window){
	windowHandle = window.GetHandle();
	//need to be initialised with init
	deviceContext = NULL;
	renderContext = NULL;
	
}

WinRenderer::~WinRenderer(){
	wglDeleteContext(renderContext);
}


bool WinRenderer::Init(){
	// Did We Get A Device Context?
	if (!(deviceContext = GetDC(windowHandle))) {
		std::cout << "OGLRenderer::OGLRenderer(): Failed to create window!" << std::endl;
		return false;
	}

	//A pixel format descriptor is a struct that tells the Windows OS what type of front / back buffers we want to create etc
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   //It must be double buffered, it must support OGL(!), and it must allow us to draw to it...
	pfd.iPixelType = PFD_TYPE_RGBA;	//We want our front / back buffer to have 4 channels!
	pfd.cColorBits = 32;				//4 channels of 8 bits each!
	pfd.cDepthBits = 24;				//24 bit depth buffer
	pfd.cStencilBits = 8;				//plus an 8 bit stencil buffer
	pfd.iLayerType = PFD_MAIN_PLANE;


	GLuint		PixelFormat;
	if (!(PixelFormat = ChoosePixelFormat(deviceContext, &pfd))) {	// Did Windows Find A Matching Pixel Format for our PFD?
		std::cout << "OGLRenderer::OGLRenderer(): Failed to choose a pixel format!" << std::endl;
		return false;
	}

	if (!SetPixelFormat(deviceContext, PixelFormat, &pfd)) {		// Are We Able To Set The Pixel Format?
		std::cout << "OGLRenderer::OGLRenderer(): Failed to set a pixel format!" << std::endl;
		return false;
	}

	HGLRC		tempContext;		//We need a temporary OpenGL context to check for OpenGL 3.2 compatibility...stupid!!!
	if (!(tempContext = wglCreateContext(deviceContext))) {	// Are We Able To get the temporary context?
		std::cout << "OGLRenderer::OGLRenderer(): Cannot create a temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return false;
	}

	if (!wglMakeCurrent(deviceContext, tempContext)) {	// Try To Activate The Rendering Context
		std::cout << "OGLRenderer::OGLRenderer(): Cannot set temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return false;
	}

	//Now we have a temporary context, we can find out if we support OGL 3.x
	char* ver = (char*)glGetString(GL_VERSION); // ver must equal "3.2.0" (or greater!)
	int major = ver[0] - '0';		//casts the 'correct' major version integer from our version string
	int minor = ver[2] - '0';		//casts the 'correct' minor version integer from our version string

	if (major < 3) {					//Graphics hardware does not support OGL 3! Erk...
		std::cout << "OGLRenderer::OGLRenderer(): Device does not support OpenGL 3.x!" << std::endl;
		wglDeleteContext(tempContext);
		return false;
	}

	if (major == 3 && minor < 2) {	//Graphics hardware does not support ENOUGH of OGL 3! Erk...
		std::cout << "OGLRenderer::OGLRenderer(): Device does not support OpenGL 3.2!" << std::endl;
		wglDeleteContext(tempContext);
		return false;
	}
	//We do support OGL 3! Let's set it up...

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,	//TODO: Maybe lock this to 3? We might actually get an OpenGL 4.x context...
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifdef OPENGL_DEBUGGING 
		| WGL_CONTEXT_DEBUG_BIT_ARB
#endif		//No deprecated stuff!! DIE DIE DIE glBegin!!!!
		,WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,		//We want everything OpenGL 3.2 provides...
		0					//That's enough attributes...
	};

	//Everywhere else in the Renderers, we use function pointers provided by GLEW...but we can't initialise GLEW yet! So we have to use the 'Wiggle' API
	//to get a pointer to the function that will create our OpenGL 3.2 context...
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	renderContext = wglCreateContextAttribsARB(deviceContext, 0, attribs);

	// Check for the context, and try to make it the current rendering context
	if (!renderContext || !wglMakeCurrent(deviceContext, renderContext)) {
		std::cout << "OGLRenderer::OGLRenderer(): Cannot set OpenGL 3 context!" << std::endl;	//It's all gone wrong!
		wglDeleteContext(renderContext);
		wglDeleteContext(tempContext);
		return false;
	}

	wglDeleteContext(tempContext);	//We don't need the temporary context any more!

	glewExperimental = GL_TRUE;	//This forces GLEW to give us function pointers for everything (gets around GLEW using 'old fashioned' methods
								//for determining whether a OGL context supports a particular function or not

	if (glewInit() != GLEW_OK) {	//Try to initialise GLEW
		std::cout << "OGLRenderer::OGLRenderer(): Cannot initialise GLEW!" << std::endl;	//It's all gone wrong!
		return false;
	}
	//If we get this far, everything's going well!
	


	window.RegisterResizeCallback(BasicResizeFunc);//Tell our window about the new renderer! (Which will in turn resize the renderer window to fit...)		

	glClearColor(0.392f, 0.584f, 0.96f, 1.0f);

	InitDebugRenderer();

	initialised = true;
	return true;
}

void WinRenderer::SwapBuffers(){
	DrawDebug();
	::SwapBuffers(deviceContext);
}
