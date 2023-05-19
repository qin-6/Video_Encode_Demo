#include <glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <stdlib.h>
#include <unistd.h>
#include <filesystem.h>
#include <shader_m.h>
#include <camera.h>
#include "cloudxr_streaming_server.h"
#include <dlfcn.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sys/time.h>
#include <signal.h>
#include <execinfo.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include "avcenc.h"

typedef void *GLeglImageOES;
typedef void (*PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)(GLenum target, GLeglImageOES image);
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void initStreaming();
static void logout(const char* msg);

unsigned int interval = 30; //ms
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


static void sig(int sig)
{
    int nptrs;
    void *buffer[1024];
    char **strings;
    char map[64];
    memset(map, 0, 64);
    sprintf(map, "cat /proc/%d/maps", getpid());

    nptrs=backtrace(buffer,100);
    strings=backtrace_symbols(buffer,nptrs);
    for(int i=0; i<nptrs; ++i)
    {
        printf("backtrace:%s\n", strings[i]);
    }
    free(strings);
    exit(-1);
}

void SaveEncodedStreamLocal(char* buf, uint32_t size, std::string name, std::fstream &testFile, unsigned int maxSize)
{	
    static unsigned int sizeTotal = 0;
    if(sizeTotal > maxSize)
    {
        return;
    }
    if(!testFile.good())
	{	
		std::stringstream ss;  
		ss << "/home/" <<  name << ".avc";
		std::string path = ss.str();
		testFile.open(path, std::ios::out|std::ios::binary);
    }
	testFile.write(buf, size);
    sizeTotal += size;
    if(sizeTotal > maxSize)
    {
        testFile.close();
    }
}

int main()
{
    std::fstream file;
    void *lib_module;
    GLFWwindow* window;
    VA_ENC_API vaEncApi;
    unsigned int frameCount = 0;
    unsigned int width = 512;
    unsigned int height = 512;


    signal(SIGSEGV, sig);
    signal(SIGABRT, sig);

    lib_module = dlopen("libavcenc.so", RTLD_LAZY);
    if (!lib_module) {
        printf("dlopen lib failed ... %s\n", dlerror());
        return 0;
    }

    VaEncApiInstance(lib_module, vaEncApi);
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API,  GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    //GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
   
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // RenderMain();
    // build and compile our shader zprogram
    // ------------------------------------
    
    Shader ourShader("../resources/shaders/4.1.texture.vs", "../resources/shaders/4.1.texture.fs"); 

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left      
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    unsigned int texture;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int texture_width, texture_height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

    unsigned char *data = stbi_load("../resources/textures/container.jpg", &texture_width, &texture_height, &nrChannels, 0);
   // unsigned char *data = stbi_load(FileSystem::getPath("/resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width,texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 1" << std::endl;
    }
    unsigned int texOut;
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glGenTextures(1, &texOut);
    glBindTexture(GL_TEXTURE_2D, texOut); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);  
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texOut, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    stbi_image_free(data);
    glEGLImageTargetTexture2DOES =(PFNGLEGLIMAGETARGETTEXTURE2DOESPROC) glfwGetProcAddress("glEGLImageTargetTexture2DOES");
    if(glEGLImageTargetTexture2DOES == NULL)
    {
        exit(1);
    }

    float counter = 0;
    struct timeval start_time;
    struct timeval end_time;

    //Init encoder
    VA_ENC_INIT_PARAMS initParams;
    VA_ENC_PIC_PARAMS  picParams;
    VA_ENC_BITSTREAM  bitStream;
    initParams.bitrate = 200000;
    initParams.gop_length = 60;
    initParams.intra_frame_period = 30;
    initParams.width = width;
    initParams.height = height;
    initParams.device = (void*)glEGLImageTargetTexture2DOES;

    vaEncApi.VaEncInitializeEncoder(&initParams);
    while (!glfwWindowShouldClose(window))
    {

        
        counter += 0.1;
        processInput(window);
        
        ourShader.use();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 model      = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::rotate(model, glm::radians(counter), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)indices);
        glFinish();
        gettimeofday(&start_time, NULL);
        vaEncApi.VaEncInputFrame((void *)framebuffer);
        picParams.frameNumber = frameCount;
        vaEncApi.VaEncEncodePicture(&picParams);
        VA_ENC_BITSTREAM bitStream;
        vaEncApi.VaEncLockBitstream(&bitStream);
        gettimeofday(&end_time, NULL);
        unsigned int consum = end_time.tv_usec - start_time.tv_usec;
        printf("encode time = %d\n", consum);
        SaveEncodedStreamLocal((char *)bitStream.buf, bitStream.size, "video", file, 1024 * 200);
        vaEncApi.VaEncUnLockBitstream();
        frameCount ++;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
 
        ourShader.use();
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view2       = camera.GetViewMatrix();
        glm::mat4 model2      = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, -2.0f));
        model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));
        model2 = glm::rotate(model2, glm::radians(counter), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
        ourShader.setMat4("projection", projection2);
        ourShader.setMat4("view", view2);
        ourShader.setMat4("model", model2);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)indices);
        
        glfwSwapBuffers(window);
        glfwPollEvents(); 
        if(counter > 360.0)
        {
            counter = 0;
        }
        
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    //glViewport(0, 0, width, height);
}
