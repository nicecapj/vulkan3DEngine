#include "MeshRenderer.h"


MeshRenderer::MeshRenderer(MeshType meshType, Camera* camera)
{
	camera_ = camera;

	switch (meshType)
	{
	case MeshType::Trangile:
	{
		Mesh::setTriData(vertices_, indicies_);
	} break;
	case MeshType::Cube:
	{
		Mesh::setCubeData(vertices_, indicies_);
	} break;
	case MeshType::Sphere:
	{
		Mesh::setSphereData(vertices_, indicies_);
	} break;
	case MeshType::Quad:
	{
		Mesh::setQuadData(vertices_, indicies_);
	} break;
	}

	//VBO
	//���ؽ���� ������Ʈ�� �����, ���ø����̼ǿ� ���ε��ϸ�
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	//���ؽ����� ������Ʈ�� ���� ���ε� �Ҽ� �ִ�. �޸� ���·� ����������, GL_ARRAY_BUFFER���� Ÿ�� ��ð� �ʿ��ϴ�.
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	//���ؽ����� ������Ʈ�� ��������� ���� �����͸� �Ѱܼ�. ���ۿ� �����Ų��.
	//GL_STATIC_DRAW�� �ѹ��� ����(����)�ϰ�, ��� ����ϴ� �����̴�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);

	//position�̳� normal���� �Ӽ��� ������, ��� ����ؾ� �ϴ� ���� �ƴϴ�.
	//�ʿ��� ���� position�� color�� �ʿ������� �̰͸� Ȱ��ȭ����
	glEnableVertexAttribArray(0);	//Vertex ����ü �Ӽ� 0�� ��� Ȱ��ȭ -> position���� ���

	//normalize�ȵ� ���̶� GL_FALSE
	glVertexAttribPointer(0,	//index of vertex array. ���ø����̼ǿ� ������ vao�� ���ε�Ǿ����� ��������, 0���� �� ������ ���
		3,	//x,y,z
		GL_FLOAT,	//type of xyz
		GL_FALSE,	//normalized. 0~1�� �ƴ� ���̸� false�Ѵ�
		sizeof(Vertex),	//stride�� �����̶� ������, ���ۿ��� ��� ������� �پ�鼭 �������� �����ϴ� ������
		(GLvoid*)0);	//���ؽ� struct�ȿ��� ���ؽ��Ӽ��� �ɼ�(position : 0, normal:1? 3? -> 12(pos:4x3))

//debug
//auto off = offsetof(Vertex, Vertex::color);

	glEnableVertexAttribArray(1);	//2��°(1��) ���ؽ��Ӽ� Ȱ��ȭ -> color�� ���
	glVertexAttribPointer(1,	//index of vertex array. ���ø����̼ǿ� ������ vao�� ���ε�Ǿ����� ��������, 1���� �� ������ ���
		3,	//R,G,B
		GL_FLOAT,	//type of RGB
		GL_FALSE,	//normalized
		sizeof(Vertex),	//stride�� �����̶� ������, ���ۿ��� ��� ������� �پ�鼭 �������� �����ϴ� ������
		(GLvoid*)(offsetof(Vertex, Vertex::texCoords)));	//�ɼ��� �˱� �����, offsetof�� �������.

//EBO
	glGenBuffers(1, &ebo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies_.size(), &indicies_[0], GL_STATIC_DRAW);

	//Unbind buffer and vertex array as a precaution
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Draw()
{	
	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), position_);	
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale_);
	glm::mat4 model = transformMatrix * scaleMatrix;


	//set shader
	glUseProgram(program_);

	//model ->  view -> projection matrix�� uniform variables�� ���� ���̴��� ������ ������ �ִ�.
	//glGetUniformLocation()�Լ��� ���̴����α׷� �ȿ� ����� ������ �̸����� ������ �� �ִ�. ex) uniform mat4 model;
	GLuint modelLocation = glGetUniformLocation(program_, "model");
	//glUniformXXX������ �Լ��� ���̴���  ����� ������ ������, ���� ������ �� �ִ�.
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));	//����������, �ѱ浥������ ��, ��ġ���� ����, �ѱ� DATA�� ������

	glm::mat4 view = camera_->GetViewMatrix();
	glm::mat4 proj = camera_->GetProjectMatrix();
	glm::mat4 vp = proj * view;

	GLuint vpLocation = glGetUniformLocation(program_, "vp");	//uniform mat4 view;
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));

	//texture
	glBindTexture(GL_TEXTURE_2D, texture_);

	//draw
	//�ѹ��� �׸� �����͸� ���� �䱸�Ѵ�.(vao_) ���� glDrawElements�� ���� �׸���.
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indicies_.size(), GL_UNSIGNED_INT, 0);

	//marks end of draw function
	glBindVertexArray(0);
	glUseProgram(0);
}

void MeshRenderer::SetPosition(glm::vec3 position)
{
	position_ = position;
}

void MeshRenderer::SetScale(glm::vec3 scale)
{
	scale_ = scale;
}

void MeshRenderer::SetProgram(GLuint program)
{
	program_ = program;
}

void MeshRenderer::SetTexture(GLuint textureID)
{
	texture_ = textureID;
}