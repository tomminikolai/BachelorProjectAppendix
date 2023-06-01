#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Sock.h"
#include "model.h"


class msgmon
{
public:
	msgmon()
	{
		msgmonmutex = CreateMutex(NULL, FALSE, NULL);
		msg = 0x00;
	}
	~msgmon()
	{
	}
	void set(char newmsg, int wait=NULL)
	{
		if (WaitForSingleObject(msgmonmutex, wait) == 0)
		{
			msg = newmsg;
			ReleaseMutex(msgmonmutex);
		}
	}
	void get(char* newmsg, int wait = NULL)
	{
		if (WaitForSingleObject(msgmonmutex, wait) == 0)
		{
			*newmsg = msg;
			ReleaseMutex(msgmonmutex);
		}
	}
private:
	char msg;
	HANDLE msgmonmutex;
};

class posmonitor
{
public:
	posmonitor()
	{
		posmonmutex = CreateMutex(NULL, FALSE, NULL);
		for (int i = 0; i < 4; i++)
		{
			possock[i] = 0;
		}
	}
	~posmonitor()
	{

	}
	void set(float* buf, int wait=NULL)
	{
		if (WaitForSingleObject(posmonmutex, wait) == 0)
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				possock[i] = buf[i];
			}
			ReleaseMutex(posmonmutex);
		}
	}
	void get(float* buf, int wait=NULL)
	{
		if (WaitForSingleObject(posmonmutex, wait) == 0)
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				buf[i] = possock[i];
			}
			ReleaseMutex(posmonmutex);
		}
	}
private:
	HANDLE posmonmutex;
	float possock[4];
};

posmonitor robotpos;
msgmon movecmd;

HANDLE getpos;
HANDLE movecontrol;

void getpos_body(LPVOID vptr)
{
	float possock[4];
	Sock_Client sc("192.168.216.156", "9999");
	while (true)
	{
		sc.read((char*)possock, 4 * sizeof(float));
		robotpos.set(possock);
	}
}

void movecontrol_body(LPVOID vptr)
{
	char msg = 0;
	char omsg = 0;
	Sock_Client sc("192.168.216.156", "1234");
	while (true)
	{
		movecmd.get(&msg);
		if (msg != omsg)
		{
			sc.write(&msg, 1);
			omsg = msg;
		}
	}
}



int main(void)
{
	getpos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)getpos_body, nullptr, 0, NULL);
	movecontrol = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)movecontrol_body, nullptr, 0, NULL);
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	const int width = 1920;
	const int height = 1080;
	window = glfwCreateWindow(width, height, "Ikke Hello World!!", glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "couldnt init glew" << std::endl;
	}
	glfwSwapInterval(1);
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0, 1.0f);
		glm::mat4 proj = glm::perspective(-glm::radians<float>(70), 16.0f / 9.0f, 0.1f, 10000.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 0));
		Texture texture("res/textures/star.png");
		model m1("res/platformv3.obj", glm::vec3(0, 0, 0), "res/shaders/m1.shader", glm::vec3(-4, -66.5, -48), glm::vec3(0, 0, 0), glm::vec3(-glm::pi<float>()/2, -glm::pi<float>() / 2, 0));
		model m2("res/square.obj", glm::vec3(150, 0, 0), "res/shaders/m1.shader", glm::vec3(0, 0, 0));	
		model m3("res/groundplane.obj", glm::vec3(0, 0, 0), "res/shaders/m1.shader", glm::vec3(0, 2, 0));
		std::vector<model*> models;
		models.push_back(&m1);
		models.push_back(&m2);
		models.push_back(&m3);
		Renderer renderer;
		glm::vec3 transa(0, 0, 0);
		float ay_r = 0;
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();
		bool has_square = false;
		bool draw_square = true;
		glm::vec3 pos(0, 1725, 0);
		glm::vec3 rot(0, 0, 0);
		bool running = true;
		ImGuiIO keys;
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		std::chrono::steady_clock::time_point start;
		std::chrono::steady_clock::time_point end;
		std::chrono::duration<double> dur;
		double jumptime = 0.5;
		bool jumping = false;
		bool menu = false;
		bool first = true;
		bool esc_pressed = false;
		float omouse[2] = { 0, 0 };
		float angle_add = 0.01;
		float x_add = 1;
		float y_add = 1;
		float z_add = 0.1;
		float possock[4];
		glm::vec3 lightpos(0, -200, 100);
		glm::vec3 viewpos(0, 250, -400);
		float* pixels = (float*)malloc(width * height * 4 * sizeof(float));
		unsigned int passes = 0;
		unsigned int lines = 0;
		float fspeed = 500;
		constexpr float angular_speed = glm::pi<float>();
		std::chrono::steady_clock::time_point ot = std::chrono::high_resolution_clock::now();
		while (!glfwWindowShouldClose(window) && running)
		{
			std::chrono::duration<double> ts = std::chrono::high_resolution_clock::now() - ot;
			ot = std::chrono::high_resolution_clock::now();
			double ts_d = ts.count();
			glClearDepth(1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);
			renderer.clear(); 
			robotpos.get(possock);
			view = glm::mat4(1.0f);
			view = glm::rotate(view, rot.x, glm::vec3(1, 0, 0));
			view = glm::rotate(view, rot.y, glm::vec3(0, 1, 0));
			viewpos.y = pos.y;
			view = glm::translate(view, viewpos);//glm::vec3(possock[1] * 50, 3, -possock[3] * 50));
			ImGui_ImplGlfwGL3_NewFrame();
			models.at(0)->move_pos(glm::vec3(-possock[1] * 1000, 0, possock[3] * 1000), glm::vec3(0, -possock[0], 0));
			for (unsigned int i = 0; i < models.size(); i++)
			{
				models.at(i)->draw(proj, view, lightpos);
			}
			{
				ImGui::Checkbox("Draw square?", &draw_square);
				ImGui::SliderFloat3("location", &transa[0], -50, 50);
				ImGui::SliderFloat3("lighpos", &lightpos[0], -400, 400);
				//ImGui::SliderAngle("angle", &ay_r, -180.0f, 180.0f);
				keys = ImGui::GetIO();
				ImGui::Text("Application average (%.1f FPS)", keys.Framerate);
				if (!menu)
				{
					if (first)
					{
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						first = false;
					}
					rot.x += -0.001 * (keys.MousePos.y - omouse[0]);
					rot.y += -0.001 * (keys.MousePos.x - omouse[1]);
					omouse[0] = keys.MousePos.y;
					omouse[1] = keys.MousePos.x;
				}
				else
				{
					if (first)
					{
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
						first = false;
					}
				}
				if (keys.KeysDown[GLFW_KEY_ESCAPE] && !esc_pressed)
				{
					esc_pressed = true;
					menu = !menu;
					first = true;
				}
				else if (esc_pressed)
				{
					esc_pressed = false;
				}
				if (keys.KeysDown[GLFW_KEY_W])
				{
					movecmd.set(0x01);
					transa.z -= glm::cos(ay_r) * fspeed * ts_d;
					transa.x -= glm::sin(ay_r) * fspeed * ts_d;
				}
				else if (keys.KeysDown[GLFW_KEY_S])
				{
					movecmd.set(0x02);
					transa.z += glm::cos(ay_r) * fspeed * ts_d;
					transa.x += glm::sin(ay_r) * fspeed * ts_d;
				}
				else if (keys.KeysDown[GLFW_KEY_A])
				{
					movecmd.set(0x03);
					ay_r -= angular_speed * ts_d;
				}
				else if (keys.KeysDown[GLFW_KEY_D])
				{
					movecmd.set(0x04);
					ay_r += angular_speed * ts_d;
				}
				else
				{
					movecmd.set(0x00);
				}
				if (keys.KeysDown[GLFW_KEY_SPACE]&&(!jumping))
				{
					start = std::chrono::high_resolution_clock::now();
					jumping = true;
				}
				if (jumping)
				{
					dur = std::chrono::high_resolution_clock::now() - start;
					pos.y = (-640) * (dur.count() * dur.count()) + 320 * dur.count() + 50;
					if (dur.count() >= jumptime)
					{
						jumping = false;
						pos.y = 50;
					}
				}
				if ((passes == 25) && (lines < 0))
				{
					std::ofstream imsave;
					imsave.open("image.csv", std::ios_base::app);
					/*std::string to_write4(std::to_string(viewpos.x - transa.x));
					imsave.write(to_write4.c_str(), to_write4.length());
					imsave.write(",", 1);

					std::string to_write5(std::to_string(-(viewpos.y - transa.y)));
					imsave.write(to_write5.c_str(), to_write5.length());
					imsave.write(",", 1);

					std::string to_write6(std::to_string(viewpos.z - transa.z));
					imsave.write(to_write6.c_str(), to_write6.length());
					imsave.write(",", 1);*/
					if (draw_square)
					{
						imsave.write("1,", 2);
					}
					else
					{
						imsave.write("0,", 2);
					}
					//std::cout << viewpos.x - transa.x << ", " << -(viewpos.y + transa.y) << ", " << viewpos.z - transa.z << std::endl;
					glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, pixels);
					std::cout << "Saving: l. " << lines << std::endl;
					for (unsigned int i = 0; i < width * height; i++)
					{
						std::string to_write(std::to_string((unsigned char)(pixels[i * 4] * 255)));
						imsave.write(to_write.c_str(), to_write.length());
						imsave.write(",", 1);
						std::string to_write2(std::to_string((unsigned char)(pixels[i * 4 + 1] * 255)));
						imsave.write(to_write2.c_str(), to_write2.length());
						imsave.write(",", 1);
						std::string to_write3(std::to_string((unsigned char)(pixels[i * 4 + 2] * 255)));
						imsave.write(to_write3.c_str(), to_write3.length());
						if (i+1 != width * height)
						{
							imsave.write(",", 1);
						}
					}
					imsave.write("\n", 1);
					std::cout << "Saved: l. " << lines << std::endl;
					imsave.close();
					passes = 0;
					lines++;
					draw_square = !draw_square;
				}
				if (keys.KeysDown[GLFW_KEY_Q])
				{
					running = false;
					movecmd.set(0x05);
				}
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			

			glfwSwapBuffers(window);
			glfwPollEvents();
			passes++;
		}
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}