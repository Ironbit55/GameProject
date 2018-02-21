#include "Renderer.h"
#include "RenderObject.h"

#pragma comment(lib, "nclgl.lib")

void main(void) {
	Window w = Window("game test", 800, 600);
	Renderer r(w);

	Mesh*	m	= Mesh::LoadMeshFile("cube.asciimesh");
	Shader* s	= new Shader("basicvert.glsl", "basicFrag.glsl");

	if (s->UsingDefaultShader()) {
		cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
		cout << "Press any key to continue." << endl;
		std::cin.get();
	}



	RenderObject o(m,s);
	o.SetModelMatrix(Matrix4::Translation(Vector3(0, 0, -10)) * Matrix4::Scale(Vector3(1, 1, 1)) * Matrix4::Rotation(45, Vector3(1, 0, 0)));
	o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Scale(Vector3(1,1,1)));
	r.AddRenderObject(o);



	while(w.UpdateWindow()) {
		float msec = w.GetTimer()->GetTimedMS();

		o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Rotation(0.1f * msec,Vector3(0,0,1)));

		r.UpdateScene(msec);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	}
	delete m;
	delete s;
}