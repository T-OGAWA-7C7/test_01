

#ifndef __USE_BOOST__
#define __USE_BOOST__
#include <boost/timer/timer.hpp>
#if _DEBUG
#pragma comment (lib, "libboost_timer-vc140-mt-gd-1_63.lib")
#endif
#include <boost/thread.hpp>
#if _DEBUG
#pragma comment (lib, "libboost_thread-vc140-mt-gd-1_63.lib")
#pragma comment (lib, "libboost_date_time-vc140-mt-gd-1_63.lib")
#pragma comment (lib, "libboost_system-vc140-mt-gd-1_63.lib")
#pragma comment (lib, "libboost_chrono-vc140-mt-gd-1_63.lib")
#endif
#endif

#include <ogwFuncGL.h>
#pragma comment(lib, "ogw.lib")

#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

void SetupShader(ogw::GL_Shader* m_pShader);
void SetupVBO(ogw::GL_VBO* m_pVBO);
void SetupTexture(ogw::GL_TEX_CV* m_pTexture);

void DrawSprite(ogw::GL_Shader* m_pShader, GLuint shader, ogw::GL_VBO* m_pVBO, GLuint vbo,
	ogw::GL_TEX_CV* m_pTexture, GLuint id, GLuint sampler, float* atlas,
	float m_fRatio, float scale[2], float pos[2]);

int main()
{
	GLFWwindow* window;
	const char* window_name = "TEST GAME";
	int window_size[2] = {640 , 360};
	float m_fRatio = (float)window_size[0] / (float)window_size[1];

	/* Initialize the library */
	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL3.3を使います。
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(window_size[0], window_size[1], window_name, NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}
	else {
		/* Setup window */
		glfwSetWindowPos(window, 0, 30);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) return -1;

	ogw::GL_Shader* m_pShader = new ogw::GL_Shader;
	SetupShader(m_pShader);
	ogw::GL_VBO* m_pVBO = new ogw::GL_VBO;
	SetupVBO(m_pVBO);
	ogw::GL_TEX_CV* m_pTexture = new ogw::GL_TEX_CV;
	SetupTexture(m_pTexture);

	GLuint sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[4] = { 0.0f,0.0f,0.0f,0.0f };
	glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, color);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glViewport(0, 0, window_size[0], window_size[1]);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float atlas[][4] = {
		{ 0.000000f,0.839114f,0.673063f,1.000000f },
		{ 0.000000f,0.160886f,0.766052f,0.321771f },
		{ 0.695203f,0.839114f,1.000000f,1.000000f },
		{ 0.000000f,0.000000f,0.713653f,0.160886f },
		{ 0.602214f,0.397786f,1.000000f,0.558672f },
		{ 0.602214f,0.628782f,1.000000f,0.789668f },
		{ 0.000000f,0.321771f,0.400738f,0.482657f },//score[6]
		{ 0.366052f,0.644280f,0.457565f,0.805166f },//0[7]
		{ 0.000000f,0.482657f,0.091513f,0.643542f},//1[8]
		{ 0.091513f,0.482657f,0.183026f,0.643542f},//2
		{ 0.183026f,0.482657f,0.274539f,0.643542f},
		{ 0.274539f,0.482657f,0.366052f,0.643542f},
		{ 0.366052f,0.482657f,0.457565f,0.643542f},
		{ 0.000000f,0.644280f,0.091513f,0.805166f},
		{ 0.091513f,0.644280f,0.183026f,0.805166f},
		{ 0.183026f,0.644280f,0.274539f,0.805166f},
		{ 0.274539f,0.644280f,0.366052f,0.805166f},
		
	};


	boost::timer::cpu_timer timer;
	int fps = static_cast<int>(1.0 / 60.0 * 1000000000.0);

	int time_count = 0;
	int score = 0;

	/* Loop until the user closes the window */
	do
	{
		(timer.is_stopped()) ? timer.start() : 0;
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		unsigned int shader = 0;
		unsigned int vbo = 0;
		{
			glUseProgram(m_pShader->pIDs[shader]->id);
			glEnableVertexAttribArray(m_pShader->pIDs[shader]->Attribute[0]);
			glEnableVertexAttribArray(m_pShader->pIDs[shader]->Attribute[1]);
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_pVBO->pVBO[0]->id);
				glVertexAttribPointer(m_pShader->pIDs[shader]->Attribute[0], 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glVertexAttribPointer(m_pShader->pIDs[shader]->Attribute[1], 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 2 * m_pVBO->pVBO[0]->vertexCount));
				glDrawArrays(GL_TRIANGLE_STRIP, 0, m_pVBO->pVBO[0]->vertexCount);
			}
			glDisableVertexAttribArray(m_pShader->pIDs[shader]->Attribute[1]);
			glDisableVertexAttribArray(m_pShader->pIDs[shader]->Attribute[0]);
		}

	
		//スコア表示
		float scale[2] = { 0.6f, 0.6f };
		float pos[2] = { -1.5f, 0.85f };
		DrawSprite(m_pShader, 1, m_pVBO, 1, m_pTexture, 0, sampler, atlas[6], m_fRatio, scale, pos);
		
		//スコア数値表示
		float num;
		(score) ? modf(log10(static_cast<float>(score)), &num) : modf(static_cast<float>(log10(1.0)), &num);
		for (int i = 0; i < static_cast<int>(num)+1; i++) {
			float pos1[2] = { -1.1f + ((num - (float)i) * 0.1f), 0.85f };
			int score_ = static_cast<int>(static_cast<double>(score) / pow(10.0, static_cast<double>(i)));
			DrawSprite(m_pShader, 1, m_pVBO, 1, m_pTexture, 0, sampler, atlas[7 + (score_ % 10)], m_fRatio, scale, pos1);
		}

		score++;
		time_count++;
		time_count = time_count % 60;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		//cout << "TIME : " << (double)timer.elapsed().wall / (double)1000000 << endl;
		while (timer.elapsed().wall < fps);
		timer.stop();
		cout << "TIME : " << (double)timer.elapsed().wall / (double)1000000 << endl;
	} while (!glfwWindowShouldClose(window));

	delete m_pShader;
	delete m_pVBO;
	delete m_pTexture;

	glfwTerminate();
	return 0;
}

void SetupShader(ogw::GL_Shader* m_pShader)
{
	ogw::GL_Shader::SrcData* srcData = new ogw::GL_Shader::SrcData;
	srcData->src = {
		"#version 150\n"
		"in vec2 posXY;"
		"in vec2 texCoord;"
		"out vec2 texUV;"
		"void main(){"
		"gl_Position = vec4(posXY, 0.0, 1.0);"
		"texUV = texCoord;"
		"}" ,
		"#version 150\n"
		"in vec2 texUV;"
		"out vec4 color;"
		"void main(){"
		"color = vec4(0.1,0.5,0.9,1.0);"
		"color.rgb = color.xyz * (texUV.y / 1.5) + vec3(0.0,0.0,0.2);"
		"}"
	};
	srcData->Attribute = { "posXY", "texCoord" };
	srcData->UniformVS;
	srcData->UniformFS;
	m_pShader->Add(srcData);
	delete srcData;

	//testData.png用
	srcData = new ogw::GL_Shader::SrcData;
	srcData->src = {
		"#version 150\n"
		"in vec2 posXY;"
		"in vec2 texCoord;"
		"uniform float ratio;"
		"uniform float scale[2];"
		"uniform float pos[2];"
		"out vec2 texUV;"
		"void main(){"
		"gl_Position.xyz = vec3(posXY.x * scale[0] + pos[0], posXY.y* scale[1] + pos[1], 0.0);"
		"gl_Position.x = gl_Position.x / ratio;"
		"gl_Position.w = 1.0;"
		"texUV = texCoord;"
		"}" ,
		"#version 150\n"
		"in vec2 texUV;"
		"uniform sampler2D texID;"
		"out vec4 color;"
		"void main(){"
		"color = texture(texID, texUV);"
		"color = vec4(color.r,color.r,color.r,color.r);"
		"}"
	};
	srcData->Attribute = { "posXY", "texCoord" };
	srcData->UniformVS = { "ratio", "scale", "pos" };
	srcData->UniformFS = { "texID" };

	m_pShader->Add(srcData);
	delete srcData;

}

void SetupVBO(ogw::GL_VBO* m_pVBO)
{
	const vector<float> data = { -1.f,1.f,-1.f,-1.f,1.f,1.f,1.f,-1.f,0.0f,0.0f,0.0f,1.f,1.f,0.0f,1.f,1.f };
	m_pVBO->Add(&data, 4, GL_STATIC_DRAW);
	m_pVBO->Add(&data, 4, GL_DYNAMIC_DRAW);
}

void SetupTexture(ogw::GL_TEX_CV* m_pTexture)
{
	m_pTexture->Add("src/testData.png");

}

void DrawSprite(ogw::GL_Shader* m_pShader, GLuint shader, ogw::GL_VBO* m_pVBO, GLuint vbo,
	ogw::GL_TEX_CV* m_pTexture, GLuint id, GLuint sampler, float* atlas,
	float m_fRatio, float scale[2], float pos[2])
{
	
	glUseProgram(m_pShader->pIDs[shader]->id);
	glUniform1f(m_pShader->pIDs[shader]->UniformVS[0], m_fRatio);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(m_pShader->pIDs[shader]->UniformFS[0], 0);

	glBindTexture(GL_TEXTURE_2D, m_pTexture->ID[0]->id);
	glBindSampler(0, sampler);

	glEnableVertexAttribArray(m_pShader->pIDs[shader]->Attribute[0]);
	glEnableVertexAttribArray(m_pShader->pIDs[shader]->Attribute[1]);

	glBindBuffer(GL_ARRAY_BUFFER, m_pVBO->pVBO[vbo]->id);
	float* p = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	float texcoord[8] = { atlas[0],atlas[1],atlas[0],atlas[3],atlas[2],atlas[1],atlas[2],atlas[3] };
	memcpy(p + 8, texcoord, sizeof(GLfloat) * 8);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	float _scale[2] = { (atlas[2] - atlas[0]) * scale[0] , (atlas[3] - atlas[1]) * scale[1] };
	glUniform1fv(m_pShader->pIDs[shader]->UniformVS[1], 2, _scale);
	glUniform1fv(m_pShader->pIDs[shader]->UniformVS[2], 2, pos);

	glVertexAttribPointer(m_pShader->pIDs[shader]->Attribute[0], 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(m_pShader->pIDs[shader]->Attribute[1], 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 2 * m_pVBO->pVBO[vbo]->vertexCount));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_pVBO->pVBO[vbo]->vertexCount);

	glDisableVertexAttribArray(m_pShader->pIDs[shader]->Attribute[1]);
	glDisableVertexAttribArray(m_pShader->pIDs[shader]->Attribute[0]);
	
}