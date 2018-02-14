#include "HeightMap.h"

HeightMap::HeightMap(std::string name, const float rawWidth, const float rawHeight, const float HeightMapX, const float HeightMapY, const float HeightMapZ, const float HeightMapTexX, const float HeightMapTexZ) {
	std::ifstream file(name.c_str(), ios::binary);
	if (!file) {
		return;
	}
	numVertices = rawWidth * rawHeight;
	numIndices = (rawWidth - 1)*(rawHeight - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];
	unsigned char * data = new unsigned char[numVertices];
	file.read((char *)data, numVertices * sizeof(unsigned char));
	file.close();

	for (int x = 0; x < rawWidth; ++x) {
		for (int z = 0; z < rawHeight; ++z) {
			int offset = (x * rawWidth) + z;

			vertices[offset] = Vector3(x * HeightMapX, data[offset] * HeightMapY, z * HeightMapZ);

			textureCoords[offset] = Vector2(x * HeightMapTexX, z * HeightMapTexZ);
		}
	}
	delete data;
	numIndices = 0;

	for (int x = 0; x < rawWidth - 1; ++x) {
		for (int z = 0; z < rawHeight - 1; ++z) {
			int a = (x * (rawWidth)) + z;
			int b = ((x + 1) * (rawWidth)) + z;
			int c = ((x + 1) * (rawWidth)) + (z + 1);
			int d = (x * (rawWidth)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}
	GenerateNormals();
	GenerateTangents();

	BufferData();
}
