#include "ModelParser.h"
#include "GlobalVariables.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
std::map<int, char> charMap;
Vector3f ParseVector(std::string line)
{
	int spacePositions[4] = { -1, -1, -1, line.length() - 1 };
	int nspace = 0;
	for (int i = 0; i < line.length(); i++) {
		if (line.at(i) == ' ') {
			if (spacePositions[nspace] != i) {
				spacePositions[nspace] = i;
				nspace++;
			}
		}
	}
	spacePositions[nspace] = line.length() - 1;

	Vector3f v = { 0, 0, 0 };

	v.x = stof(line.substr(spacePositions[0] + 1, spacePositions[1] - spacePositions[0]));
	v.y = stof(line.substr(spacePositions[1] + 1, spacePositions[2] - spacePositions[1]));
	if (spacePositions[3] > spacePositions[2]) {
		v.z = stof(line.substr(spacePositions[2] + 1, spacePositions[3] - spacePositions[2]));
	}
	return v;

}

Vector3Int* ParseFace(std::string line)
{
	Vector3Int* face = new Vector3Int[3];
	std::vector<std::string> vertices;
	std::stringstream ss(line.substr(2));
	std::string vertex;
	while (std::getline(ss, vertex, ' '))
	{
		vertices.push_back(vertex);
	}
	for (int i = 0; i < 3; i++)
	{
		std::stringstream vss(vertices[i]);
		std::string v;
		int j = 0;
		while (std::getline(vss, v, '/'))
		{
			if (i == 0)
			{
				face[j].x = std::stoi(v);
			}
			else if (i == 1)
			{
				face[j].y = std::stoi(v);
			}
			else if (i == 2)
			{
				face[j].z = std::stoi(v);
			}
			j++;
		}
	}
	return face;
}
void NormalizeVertices(std::list<Vector3f>& vertices) {
	// Find the minimum and maximum values for each coordinate
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const Vector3f& vertex : vertices) {
		minX = std::min(minX, vertex.x);
		minY = std::min(minY, vertex.y);
		minZ = std::min(minZ, vertex.z);
		maxX = std::max(maxX, vertex.x);
		maxY = std::max(maxY, vertex.y);
		maxZ = std::max(maxZ, vertex.z);
	}

	// Calculate the range (difference) for each coordinate
	float rangeX = maxX - minX;
	float rangeY = maxY - minY;
	float rangeZ = maxZ - minZ;

	for (Vector3f& vertex : vertices) {
		vertex.x = ((vertex.x - minX) / rangeX) * 2.0f - 1.0f;
		vertex.y = ((vertex.y - minY) / rangeY) * 2.0f - 1.0f;
		vertex.z = ((vertex.z - minZ) / rangeZ) * 2.0f - 1.0f;
	}
}


Model* ParseObj(std::string filePath)
{
	std::list<Vector3f> vertexs;
	std::list<Vector3f> textures;
	std::list<Vector3f> normals;
	std::list<Vector3Int*> faces;


	std::ifstream file(filePath);
	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("v ") != std::string::npos)
		{
			// Vertexs
			vertexs.push_back(ParseVector(line));

		}
		else if (line.find("vt ") != std::string::npos)
		{
			// Texture coords
			Vector3f v = ParseVector(line);
			textures.push_back(v);

		}
		else if (line.find("vn ") != std::string::npos)
		{
			// Vertex normals
			normals.push_back(ParseVector(line));
		}
		else if (line.find("f ") != std::string::npos)
		{
			// Faces
			faces.push_back(ParseFace(line));
		}
	}
	NormalizeVertices(vertexs);

	Vector3f* vertexArr = new Vector3f[vertexs.size()];
	std::copy(vertexs.begin(), vertexs.end(), vertexArr);

	Vector3f* textureArr = new Vector3f[textures.size()];
	std::copy(textures.begin(), textures.end(), textureArr);

	Vector3f* normalArr = new Vector3f[normals.size()];
	std::copy(normals.begin(), normals.end(), normalArr);

	// Get the number of elements in the list
	int numElements = faces.size();

	// Create a double array of Vector3Int
	Vector3Int** faceArray = new Vector3Int * [numElements];
	for (int i = 0; i < numElements; i++)
	{
		faceArray[i] = new Vector3Int[3];
	}

	// Copy the data from the list to the double array
	int i = 0;
	for (auto& facePtr : faces)
	{
		faceArray[i][0] = *facePtr++;
		faceArray[i][1] = *facePtr++;
		faceArray[i][2] = *facePtr++;

		i++;
	}

	return new Model{ vertexArr, textureArr, normalArr, faceArray, (unsigned int)vertexs.size(), (unsigned int)textures.size(), (unsigned int)normals.size(), (unsigned int)faces.size() };
}


struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char* data;
};

typedef struct Image Image;

#define checkImageWidth 64
#define checkImageHeight 64

GLubyte checkImage[checkImageWidth][checkImageHeight][3];
void makeCheckImage(void) {
	int i, j, c;
	for (i = 0; i < checkImageWidth; i++) {
		for (j = 0; j < checkImageHeight; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
		}
	}
}
int ImageLoad(char* filename, Image* image) {
	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
	unsigned short int bpp; // number of bits per pixel (must be 24)
	char temp; // temporary color storage for bgr-rgb conversion.
	// make sure the file is there.
	FILE* file;
	errno_t err = fopen_s(&file, filename, "rb");
	if (err != 0) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);
	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	//printf("Width of %s: %lu\n", filename, image->sizeX);
	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	//printf("Height of %s: %lu\n", filename, image->sizeY);
	// calculate the size (assuming 24 bits or 3 bytes per pixel).
	size = image->sizeX * image->sizeY * 3;
	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}
	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);
		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
	// read the data.
	image->data = (char*)malloc(size);
	if (image->data == nullptr) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	// we're done.
	return 1;
}

Image* loadTexture(char* filePath) {
	Image* image1;
	// allocate space for texture
	image1 = (Image*)malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	if (!ImageLoad(filePath, image1)) {
		exit(1);
	}
	return image1;
}
void initTextures(GLuint* tid, std::string texturePath) {
	int w;
	int h;
	int comp;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load((char*)texturePath.c_str(), &w, &h, &comp, STBI_rgb_alpha);
	if (image == nullptr) {
		printf("Image was not loaded\n");
		exit(0);
	}
	glGenTextures(1, tid);
	glBindTexture(GL_TEXTURE_2D, tid[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);
}

void paintCharacter(Font* font, unsigned char* image, int wi, int hi, int wf, int hf, int w, int xd, int yd) {
	for (int y = hi; y < hf + hi; y++) {
		for (int x = wi; x < wf + wi; x++) {
			int pixelIndex = ((yd + (y - hi)) * w + (xd + x - wi)) * 4;
			if (font->font[y][x] > 0) {
				image[pixelIndex] = 255;
				image[pixelIndex + 1] = 255;
				image[pixelIndex + 2] = 255;
				image[pixelIndex + 3] = 255;
			}
			else if (font->font[y + 1][x + 1] == 255 || font->font[y][x + 1] == 255 || font->font[y + 1][x] == 255) {
				bool isNeighbour = false;
				for (int i = 10; i > 0; i--) {
					for (int j = 10; j > 0; j--) {
						if (font->font[y + i][x + j] >0) {
							isNeighbour = true;
						}
					}
				}
				if (isNeighbour) {
					image[pixelIndex] = 0;
					image[pixelIndex + 1] = 0;
					image[pixelIndex + 2] = 0;
					image[pixelIndex + 3] = 255;
				}
				
			}
		}
	}
}

void loadText(std::string text, GLuint* tid, Font* font, int startingX, int startingY) {
	// load char with keys
	int x = startingX;
	int y = startingY;
	int w = 1080;
	int h = 1080;
	unsigned char* image = new unsigned char[w * h * 4];
	for (int i = 0; i < w * h * 4; i++) {
		image[i] = 1;
	}

	for (int i = 0; i < text.size(); i++) {
		char c = text.at(i);
		// if character is \n, change line
		if (c == '\n') {
			y += font->hchar+10;
			x = startingX;
		}
		else if (c == ' ') {
			x += font->wchar+10;
		}
		else {
			// search id of character
			int id = (c - 0) - 33;
			int hi = (id / (font->w/font->wchar)) * font->hchar;
			int wi = (id % (font->w / font->wchar)) * font->wchar;
			paintCharacter(font, image, wi, hi, font->wchar, font->hchar, w, x, y);
			x += font->wchar + 10;
		}
	}

	unsigned char* flippedImage = new unsigned char[w * h * 4];

	// Flip the 'image' array by swapping rows
	for (int y = 0; y < h; y++) {
		int flippedY = h - 1 - y;
		memcpy(&flippedImage[flippedY * w * 4], &image[y * w * 4], w * 4);
	}

	glGenTextures(1, tid);
	glBindTexture(GL_TEXTURE_2D, tid[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, flippedImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void loadFont(std::string fontPath, Font* font, int charSize) {
	int w;
	int h;
	int comp;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* image = stbi_load((char*)fontPath.c_str(), &w, &h, &comp, STBI_rgb_alpha);
	font->font = new unsigned char* [h];
	for (int i = 0; i < h; i++) {
		font->font[i] = new unsigned char[w];
	}
	font->w = w;
	font->h = h;
	font->hchar = charSize;
	font->wchar = charSize;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int pixelIndex = (y * w + x) * 4;
			unsigned char red = image[pixelIndex + 3];
			font->font[y][x] = red;
		}
	}
	
}

Model* cubeMesh(Vector3f vo, Vector3f vf, float widthRatio, float heightRatio) {
	Model* cube = new Model;
	Vector3f vertices[8] = {
		{vo.x, vo.y, vo.z},     // 1
		{vo.x, vf.y, vo.z},     // 4
		{vo.x, vf.y, vf.z},     // 3
		{vo.x, vo.y, vf.z},     // 2
		{vf.x, vo.y, vf.z},     // 7
		{vf.x, vf.y, vf.z},     // 8
		{vf.x, vf.y, vo.z},     // 5
		{vf.x, vo.y, vo.z}      // 6
	};

	// Calculate face normals
	Vector3f normals[6] = {
		{ 0.0f,  0.0f, 1.0f},
		{1.0f,  0.0f,  0.0f},
		{ 0.0f,  0.0f,  -1.0f},
		{-1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f}
	};

	// Calculate texture coordinates
	Vector3f textures[4] = {

		{0.0f,heightRatio,0 },
		{widthRatio, heightRatio,0},
		{widthRatio,0, 0.0f},
		{0.0f,0, 0.0f}
	};

	Vector3Int** faceArray = new Vector3Int * [12];
	for (int i = 0; i < 12; i++)
	{
		faceArray[i] = new Vector3Int[3];
	}

	// Face 1
	faceArray[0][0] = Vector3Int(1, 2, 3);
	faceArray[0][1] = Vector3Int(1, 4, 3);
	faceArray[0][2] = Vector3Int(2, 2, 2);

	// Face 2
	faceArray[1][0] = Vector3Int(1, 3, 4);
	faceArray[1][1] = Vector3Int(1, 3, 2);
	faceArray[1][2] = Vector3Int(2, 2, 2);

	// Face 3
	faceArray[2][0] = Vector3Int(4, 3, 6);
	faceArray[2][1] = Vector3Int(1, 4, 3);
	faceArray[2][2] = Vector3Int(3, 3, 3);

	// Face 4
	faceArray[3][0] = Vector3Int(4, 6, 5);
	faceArray[3][1] = Vector3Int(1, 3, 2);
	faceArray[3][2] = Vector3Int(3, 3, 3);

	// Face 5
	faceArray[4][0] = Vector3Int(8, 1, 4);
	faceArray[4][1] = Vector3Int(1, 4, 3);
	faceArray[4][2] = Vector3Int(6, 6, 6);

	// Face 6
	faceArray[5][0] = Vector3Int(8, 4, 5);
	faceArray[5][1] = Vector3Int(1, 3, 2);
	faceArray[5][2] = Vector3Int(6, 6, 6);

	// Face 7
	faceArray[6][0] = Vector3Int(5, 6, 8);
	faceArray[6][1] = Vector3Int(1, 4, 2);
	faceArray[6][2] = Vector3Int(4, 4, 4);

	// Face 8
	faceArray[7][0] = Vector3Int(6, 7, 8);
	faceArray[7][1] = Vector3Int(4, 3, 2);
	faceArray[7][2] = Vector3Int(4, 4, 4);

	// Face 9
	faceArray[8][0] = Vector3Int(8, 7, 2);
	faceArray[8][1] = Vector3Int(1, 4, 3);
	faceArray[8][2] = Vector3Int(1, 1, 1);

	// Face 10
	faceArray[9][0] = Vector3Int(8, 2, 1);
	faceArray[9][1] = Vector3Int(1, 3, 2);
	faceArray[9][2] = Vector3Int(1, 1, 1);

	// Face 11
	faceArray[10][0] = Vector3Int(2, 7, 6);
	faceArray[10][1] = Vector3Int(1, 4, 3);
	faceArray[10][2] = Vector3Int(5, 5, 5);

	// Face 12
	faceArray[11][0] = Vector3Int(2, 6, 3);
	faceArray[11][1] = Vector3Int(1, 3, 2);
	faceArray[11][2] = Vector3Int(5, 5, 5);

	cube->vertexs = new Vector3f[8];
	std::copy(vertices, vertices + 8, cube->vertexs);
	// Copy texture coordinates
	cube->textures = new Vector3f[4];
	std::copy(textures, textures + 4, cube->textures);

	// Copy face normals
	cube->normals = new Vector3f[6];
	std::copy(normals, normals + 6, cube->normals);

	// Create face array and copy its data
	cube->faces = new Vector3Int * [12];
	for (int i = 0; i < 12; i++) {
		cube->faces[i] = new Vector3Int[3];
		std::copy(faceArray[i], faceArray[i] + 3, cube->faces[i]);
	}



	// Set other attributes
	cube->n_vertices = 8;
	cube->n_textures = 4;
	cube->n_normals = 6;
	cube->n_faces = 12;

	return cube;
}



Model* getMesh(MESH mesh, Vector3f vo, Vector3f vf, float widthRatio, float heightRatio) {
	Model* model = cubeMesh(vo, vf, widthRatio, heightRatio);
	//Model* model = ParseObj("res/models/cube.obj");
	return model;
}