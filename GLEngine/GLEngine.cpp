// GLEngine.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <GL/eglew.h>
#include <GLFW/glfw3.h>
#include <gl/glew.h>
#include "Mesh.h"

#include "LightRenderer.h"
#include "MeshRenderer.h"
#include "LitMeshRenderer.h"
#include "Camera.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "TextRenderer.h"

#include "VulkanContext.h"

void RenderScene()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(1.0, 0.0, 0.0, 1.0);

    //light->Draw();
    //mesh->Draw();
    //litMesh->Draw();
    //label->Draw();
}

void InitScene()
{

}

static void ErrorFunction(int id, const char* desc)
{
    std::cout << desc << std::endl;
}

//-------------------------------------------------------------------------------------
int main()
{
    glfwSetErrorCallback(&::ErrorFunction);

    glfwInit();
	//https://www.glfw.org/docs/latest/window_guide.html
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	//GLFW_CLIENT_API : opengl용 api사용하는지
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		//GLFW_RESIZABLE : 윈도우 사이즈변경 가능한지

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Vulkan Renderer", nullptr, nullptr);

    //opengl 컨텍스트 설정 - Vulkan은 opengl과 호환되는 render api가 아님.
    //glfwMakeContextCurrent(window);

    glewInit();

    VulkanContext::getInstance()->initVulkan(window);


    //InitScene();

    while (!glfwWindowShouldClose(window))
    {
		VulkanContext::getInstance()->DrawBegin();

		//RenderScene();

		VulkanContext::getInstance()->DrawEnd();		
        //glfwSwapBuffers(window);

        glfwPollEvents();
    }
    
	VulkanContext::getInstance()->CleanUp();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

