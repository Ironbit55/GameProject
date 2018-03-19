#include "MeshSpriteBatch.h"

const int VERTICES_PER_SPRITE = 4;
const int INDICES_PER_SPRITE = 6;

MeshSpriteBatch::MeshSpriteBatch(int maxSprites = 256){
	
	type = GL_TRIANGLES;
	numSpritesMax = maxSprites;

	//4 unique vertices per quad
	//but 6 indices per quad as we can't use triangle strips
	numVertices = numSpritesMax * VERTICES_PER_SPRITE;
	numIndices = numSpritesMax * INDICES_PER_SPRITE;


	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	colours = new Vector4[numVertices];
	//normals = new Vector3[numVertices];
	//tangents = new Vector3[numVertices];
	indices = new unsigned int[numIndices];

	vertexBufferPtr = nullptr;
	colourBufferPtr = nullptr;
	textureCoordBufferptr = nullptr;
	normalBufferPtr = nullptr;
	tangentBufferPtr = nullptr;
	indexBufferPtr = nullptr;

	//BufferData(GL_DYNAMIC_DRAW);
	ImmutableBufferData();
	
	numSpritesFilled = 0;
}

bool MeshSpriteBatch::AddSprite(Matrix4 modelMatrix, Vector4 colour, GLuint spriteTexture) {
	if (spriteTexture != texture || numSpritesFilled == numSpritesMax) {
		return false;
	}

	unsigned int vertexOffset = numSpritesFilled * VERTICES_PER_SPRITE;
	unsigned int indexOffset = numSpritesFilled * INDICES_PER_SPRITE;

	
	//this is slightly faster than the overloaded matrix * vec3 implementation
	vertices[vertexOffset + 0] = modelMatrix.getTransformedVec3(Vector3(-1.0f, -1.0f, 0.0f)); //bottom left
	vertices[vertexOffset + 1] = modelMatrix.getTransformedVec3(Vector3(1.0f, -1.0f, 0.0f)); //bottom right
	vertices[vertexOffset + 2] = modelMatrix.getTransformedVec3(Vector3(-1.0f, 1.0f, 0.0f)); //top left
	vertices[vertexOffset + 3] = modelMatrix.getTransformedVec3(Vector3(1.0f, 1.0f, 0.0f)); //top right

	indices[indexOffset + 0] = vertexOffset + 0;	//bottom left
	indices[indexOffset + 1] = vertexOffset + 1;	//bottom right
	indices[indexOffset + 2] = vertexOffset + 2;	//top left

	indices[indexOffset + 3] = vertexOffset + 1;	//bottom right
	indices[indexOffset + 4] = vertexOffset + 3;	//top right
	indices[indexOffset + 5] = vertexOffset + 2;	//top left

	textureCoords[vertexOffset + 0] = Vector2(0.0f, 1.0f);
	textureCoords[vertexOffset + 1] = Vector2(1.0f, 1.0f);
	textureCoords[vertexOffset + 2] = Vector2(0.0f, 0.0f);
	textureCoords[vertexOffset + 3] = Vector2(1.0f, 0.0f);

	//we map the buffer and stream vertex data directly into it cus we're cool
	//like that


	for (int i = 0; i < 4; i++) {

		colours[vertexOffset + i] = colour;
		//normals[vertexOffset + i] = Vector3(0.0f, 0.0f, -1.0f);
		//tangents[vertexOffset + i] = Vector3(1.0f, 0.0f, 0.0f);
	}

	numSpritesFilled++;
	return true;
}

//bool MeshSpriteBatch::AddSprite(Matrix4 modelMatrix, Vector4 colour, GLuint spriteTexture) {
//	if (spriteTexture != texture || numSpritesFilled == numSpritesMax) {
//		return false;
//	}
//	if(vertexBufferPtr == nullptr){
//		//can't add sprites if we haven't mapped our buffers duh
//		return false;
//	}
//
//	unsigned int vertexOffset = numSpritesFilled * VERTICES_PER_SPRITE;
//	unsigned int indexOffset = numSpritesFilled * INDICES_PER_SPRITE;
//
//	Vector3 tempVertices[VERTICES_PER_SPRITE];
//	Vector2 tempTextureCoords[VERTICES_PER_SPRITE];
//	Vector4 tempColours[VERTICES_PER_SPRITE];
//	unsigned int tempIndices[INDICES_PER_SPRITE];
//
//	//this is slightly faster than the overloaded matrix * vec3 implementation
//	tempVertices[0] = modelMatrix.getTransformedVec3(Vector3(-1.0f, -1.0f, 0.0f)); //bottom left
//	tempVertices[1] = modelMatrix.getTransformedVec3(Vector3(1.0f, -1.0f, 0.0f)); //bottom right
//	tempVertices[2] = modelMatrix.getTransformedVec3(Vector3(-1.0f, 1.0f, 0.0f)); //top left
//	tempVertices[3] = modelMatrix.getTransformedVec3(Vector3(1.0f, 1.0f, 0.0f)); //top right
//
//	tempTextureCoords[0] = Vector2(0.0f, 1.0f);
//	tempTextureCoords[1] = Vector2(1.0f, 1.0f);
//	tempTextureCoords[2] = Vector2(0.0f, 0.0f);
//	tempTextureCoords[3] = Vector2(1.0f, 0.0f);
//
//	for (int i = 0; i < VERTICES_PER_SPRITE; i++) {
//		tempColours[i] = colour;
//	}
//
//	tempIndices[0] = vertexOffset + 0;	//bottom left
//	tempIndices[1] = vertexOffset + 1;	//bottom right
//	tempIndices[2] = vertexOffset + 2;	//top left
//
//	tempIndices[3] = vertexOffset + 1;	//bottom right
//	tempIndices[4] = vertexOffset + 3;	//top right
//	tempIndices[5] = vertexOffset + 2;	//top left
//
//	//we stream the data directly into the buffers cus we're cool
//	//like that
//
//	memcpy(vertexBufferPtr + (vertexOffset), tempVertices, VERTICES_PER_SPRITE * sizeof(Vector3));
//	memcpy(textureCoordBufferptr + (vertexOffset), tempTextureCoords, VERTICES_PER_SPRITE * sizeof(Vector2));
//	memcpy(colourBufferPtr + (vertexOffset), tempColours, VERTICES_PER_SPRITE * sizeof(Vector4));
//	memcpy(indexBufferPtr + (indexOffset), tempIndices, INDICES_PER_SPRITE * sizeof(GLuint));
//
//	numSpritesFilled++;
//	return true;
//}


void MeshSpriteBatch::Begin(GLuint glTexture) {
	numSpritesFilled = 0;
	texture = glTexture;

	//mapBuffers();
}



void MeshSpriteBatch::Draw(){
	if (numSpritesFilled == 0) {
		return;
	}

	//ovverite the gl buffer data with new sprites data
	//using sub data so we don't have to detroy the buffer


	glBindVertexArray(arrayObject);

	int numUsedVertices = numSpritesFilled * VERTICES_PER_SPRITE;
	int numUsedIndices = numSpritesFilled * INDICES_PER_SPRITE;
	
	//Buffer vertex data

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);

	//apperently orphaning the buffer is a good trick to increase performance
	//however i tested it and got no benefit from it so I dunno
	//maybe opengl got smart and optimises it out itself these days
	//or maybe i should do it per frame not per batch or something
	//glBufferStorage(GL_ARRAY_BUFFER, numUsedVertices*sizeof(Vector3), nullptr, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	//glBufferData(GL_ARRAY_BUFFER, numUsedVertices*sizeof(Vector3), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numUsedVertices*sizeof(Vector3), vertices);

	//Buffer texture data
	if (textureCoords) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numUsedVertices*sizeof(Vector2), textureCoords);
	}

	//buffer colour data
	if (colours) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numUsedVertices*sizeof(Vector4), colours);
	}

	//Buffer normal data
	if (normals) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numUsedVertices*sizeof(Vector3), normals);
	}

	//Buffer tangent data
	if (tangents) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numUsedVertices*sizeof(Vector3), tangents);
	}

	//buffer index data
	if (indices) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numUsedIndices*sizeof(GLuint), indices);
	}



	GL_BREAKPOINT

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpTexture);

	glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);  //draw triangles by indices

	//glBindVertexArray(0);

	//empty batch
	//Begin(0);

	//we don't bother clearing the buffer arrays
	//we'll just overite them with new data
}

//void MeshSpriteBatch::Draw() {
//	if (numSpritesFilled == 0) {
//		return;
//	}
//	
//	//we need to flush / unmap our buffer before we can use them
//
//
//	
//	
//
//	//buffers should already contain our data but
//	//we need to flush / unmap our buffer before we can use them
//	flushBuffers();
//
//	glBindVertexArray(arrayObject);
//	int numUsedVertices = numSpritesFilled * VERTICES_PER_SPRITE;
//	int numUsedIndices = numSpritesFilled * INDICES_PER_SPRITE;
//	//Buffer vertex data
//
//
//	GL_BREAKPOINT
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture);
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, bumpTexture);
//
//	glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);  //draw triangles by indices
//
//														   // check for errors
//	GLenum error = glGetError();
//	
//	if (error != GL_NO_ERROR) {
//		//std::cerr << gluErrorString(error) << std::endl;
//	}
//
//	//glBindVertexArray(0);
//
//
//	//we don't bother clearing the buffer arrays
//	//we'll just overite them with new data
//}

void MeshSpriteBatch::flushBuffers() {

	if(vertexBufferPtr == nullptr){
		//cout << "UNECESSARY FLUSH";
		return;
	}

	

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	//unmap texture coord buffer
	if (textureCoords) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	//unmap colour buffer
	if (colours) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	//unmap normal buffer
	if (normals) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	//unmap tangent buffer
	if (tangents) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	//unmap index buffer
	if (indices) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
	
	vertexBufferPtr = nullptr;
	colourBufferPtr = nullptr;
	textureCoordBufferptr = nullptr;
	normalBufferPtr = nullptr;
	tangentBufferPtr = nullptr;
	indexBufferPtr = nullptr;
}

void MeshSpriteBatch::mapBuffers() {

	if(vertexBufferPtr != nullptr){
		//cout << "UNECCESARY BEGIN";
		return;
	}
	

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	vertexBufferPtr = reinterpret_cast<Vector3*> (glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

	//map texture coord buffer
	if (textureCoords) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		textureCoordBufferptr = reinterpret_cast<Vector2*> (glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	}

	//map colour buffer
	if (colours) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		colourBufferPtr = reinterpret_cast<Vector4*> (glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	}

	//map normal buffer
	if (normals) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		normalBufferPtr = reinterpret_cast<Vector3*> (glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	}

	//map tangent buffer
	if (tangents) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		tangentBufferPtr = reinterpret_cast<Vector3*> (glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	}

	//map index buffer
	if (indices) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		indexBufferPtr = reinterpret_cast<unsigned int*> (glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	}

}

void MeshSpriteBatch::ImmutableBufferData(){
	glBindVertexArray(arrayObject);

	//Buffer vertex data
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferStorage(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), vertices, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_BUFFER);

	//Buffer texture data
	if (textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferStorage(GL_ARRAY_BUFFER, numVertices*sizeof(Vector2), textureCoords, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	//buffer colour data
	if (colours) {
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferStorage(GL_ARRAY_BUFFER, numVertices*sizeof(Vector4), colours, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}

	//Buffer normal data
	if (normals) {
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferStorage(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), normals, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}

	//Buffer tangent data
	if (tangents) {
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferStorage(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), tangents, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENT_BUFFER);
	}

	//buffer index data
	if (indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), indices, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	}

	glBindVertexArray(0);
}
