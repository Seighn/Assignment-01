#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//Modifier for the model's x Position
float x_mod = 0;
float y_mod = 0;
float z_mod = 0;

void Key_Callback(GLFWwindow* window, // the pointer to the window
    int key, // the keycode being pressed
    int scancode, // Physical position of the press on keyboard
    int action, // Either Press / Release
    int mods) //Which modifier keys is held down
{
    //When user pressed D
    if (key == GLFW_KEY_D)
        x_mod += 10.f;
    if (key == GLFW_KEY_W)
        z_mod += 0.5f;
    if (key == GLFW_KEY_A)
        x_mod -= 10.f;
    if (key == GLFW_KEY_S)
        z_mod -= 0.5f;
}

int main(void)
{
    //Load the shader file into a string stream
    std::fstream vertSrc("Shaders/Sample.vert");
    std::stringstream vertBuff;

    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();

    //Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //Load the shader file into a string stream
    std::fstream fragSrc("Shaders/Sample.frag");
    std::stringstream fragBuff;

    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();

    //Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Mathieu Pobre", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    float window_widith = 600;
    float window_heigth = 600;
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //glViewport(0, 0, 640, 480);

    glfwSetKeyCallback(window, Key_Callback);

    //Create a Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Assign the source to the Vertex Shader
    glShaderSource(vertexShader, 1, &v, NULL);

    //Compile the Vertex Shader
    glCompileShader(vertexShader);

    //Create a Fragment Shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    //Assign the source to the Fragment Shader
    glShaderSource(fragShader, 1, &f, NULL);

    //Compile the Fragment Shader
    glCompileShader(fragShader);

    //Create the Shader Program
    GLuint shaderProg = glCreateProgram();
    //Attach the compiled Vertex Shader
    glAttachShader(shaderProg, vertexShader);
    //Attach the compiled Fragment Shader
    glAttachShader(shaderProg, fragShader);

    //Finalize the compilation process
    glLinkProgram(shaderProg);

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );
    std::vector<GLuint> mesh_indices;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[]{
        // x    y   z
        0.f,   0.5f, 0.f, //0
       -0.5f, -0.5f, 0.f, //1
        0.5f, -0.5f, 0.f  //2
    };
    
    GLuint indices[]{
        0,1,2
    };

    //initVars
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Currently editing VAO = null

    glBindVertexArray(VAO);

    //Currently editing VAO = VAO
   
    //Currently editing VBO = null

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Currently editing VBO = VBO

    //VAO <- VBO

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(), //size in bytes
        attributes.vertices.data(), //array
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, //0 pos, 1 nom, 2 tex
        3, //XYZ
        GL_FLOAT,
        GL_FALSE, //IS NORMALIZED
        3 * sizeof(float),
        (void*)0
    );

    //Currently editing VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Currently editing VBO = EBO

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(), 
        GL_STATIC_DRAW
    );


    glEnableVertexAttribArray(0);
    //Currently editing VBO = VBO

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Currently editing VBO = null
   
    //Currently editing VAO = VAO
    glBindVertexArray(0);
    //Currently editing VAO = null

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //MODEL 2
    //GLuint VAO2, VBO2, EBO2;
    
    float x, y, z;

    x, y, z = 0.f;

    //Create 3x3 Matrix
    glm::mat3 identity_matrix3 = glm::mat3(1.0f);
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);
    //glm::mat4 projectionMatrix = glm::ortho(
    //    -2.f, //L
    //    2.f, //R
    //    -2.f, //B
    //    2.f, //T
    //    -1.f, //N
    //    1.f); //F

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f), //FOV
        window_heigth / window_widith, //Aspect ratio
        0.1f, //ZNear > 0
        100.f // ZFar
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Simple animation
        //x_mod += 0.001f;

        /*Get the address of the variable x in the shader
        unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
        unsigned int yLoc = glGetUniformLocation(shaderProg, "y");

        //Assign x by using its address
        glUniform1f(xLoc, x_mod);
        glUniform1f(yLoc, y_mod);*/
        z = -2.f;
        

        glm::mat4 transformation_matrix =
            glm::translate(identity_matrix4,
                glm::vec3(0.f,0.f,z + z_mod)
            );
        transformation_matrix =
            glm::scale(transformation_matrix,
                glm::vec3(1.f + y_mod,1.f + y_mod,1.f)
            );
        transformation_matrix =
            glm::rotate(transformation_matrix,
                glm::radians(x_mod),
                glm::normalize(glm::vec3(0.f,1.f,0.f))
            );

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");

        glUniformMatrix4fv(projectionLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projectionMatrix));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

        glUniformMatrix4fv(transformLoc,
                        1,
                        GL_FALSE,
                        glm::value_ptr(transformation_matrix));

        //Render here
        glUseProgram(shaderProg);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        
        //glBindVertexArray(VAO2);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}