#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    //Load the shader file into a string stream
    std::fstream skyboxVertSrc("Shaders/Skybox.vert");
    std::stringstream skyboxVertBuff;

    //Add the file stream to the string stream
    skyboxVertBuff << skyboxVertSrc.rdbuf();

    //Convert the stream to a character array
    std::string skyboxVertS = skyboxVertBuff.str();
    const char* sky_v = skyboxVertS.c_str();

    //Load the shader file into a string stream
    std::fstream skyboxFragSrc("Shaders/Skybox.frag");
    std::stringstream skyboxFragBuff;

    //Add the file stream to the string stream
    skyboxFragBuff << skyboxFragSrc.rdbuf();

    //Convert the stream to a character array
    std::string skyboxFragS = skyboxFragBuff.str();
    const char* sky_f = skyboxFragS.c_str();

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

    int img_width, //Width of the texture
        img_height, //Height of the texture
        colorChannels; //Number of color channels

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes =
        stbi_load("3D/brickwall.jpg", //Texture path
            &img_width, //Fills out the width
            &img_height, //Fills out the height
            &colorChannels, //Fiills out the colo channels
            0);

    //glViewport(0, 0, 640, 480);

    glfwSetKeyCallback(window, Key_Callback);

    //OpenGL reference to the texture
    GLuint texture;
    //Generate reference
    glGenTextures(1, &texture);
    //Set the current texture we're
    //working
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture);


    //Assign the loaded teexture
    //too the OpenGL reference
    glTexImage2D(GL_TEXTURE_2D,
        0, //Texture 0
        GL_RGB, // Target color format of the texture
        img_width, // Texture width
        img_height,// Texture height
        0,
        GL_RGB,    //Color format of the texturue
        GL_UNSIGNED_BYTE, //Data type of texture
        tex_bytes); // Loaded texture in bytes

    //Generate thhe mipmaps to the current texture
    glGenerateMipmap(GL_TEXTURE_2D);

    //Free uo the loaded bytes
    stbi_image_free(tex_bytes);

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


    int img_width2, //Width of the texture
        img_height2, //Height of the texture
        colorChannels2; //Number of color channels

    stbi_set_flip_vertically_on_load(true);

    unsigned char* normal_bytes =
        stbi_load("3D/brickwall_normal.jpg", //Texture path
            &img_width2, //Fills out the width
            &img_height2, //Fills out the height
            &colorChannels2, //Fiills out the colo channels
            0);
    GLuint norm_tex;
    glGenTextures(1, &norm_tex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //Assign the loaded teexture
    //too the OpenGL reference
    glTexImage2D(GL_TEXTURE_2D,
        0, //Texture 0
        GL_RGB, // Target color format of the texture
        img_width2, // Texture width
        img_height2,// Texture height
        0,
        GL_RGB,    //Color format of the texturue
        GL_UNSIGNED_BYTE, //Data type of texture
        normal_bytes); // Loaded texture in bytes

    //Generate thhe mipmaps to the current texture
    glGenerateMipmap(GL_TEXTURE_2D);

    //Free uo the loaded bytes
    stbi_image_free(normal_bytes);

    //Create a Vertex Shader
    GLuint vertexShaderSkybox = glCreateShader(GL_VERTEX_SHADER);

    //Assign the source to the Vertex Shader
    glShaderSource(vertexShaderSkybox, 1, &sky_v, NULL);

    //Compile the Vertex Shader
    glCompileShader(vertexShaderSkybox);

    //Create a Fragment Shader
    GLuint fragShaderSkybox = glCreateShader(GL_FRAGMENT_SHADER);

    //Assign the source to the Fragment Shader
    glShaderSource(fragShaderSkybox, 1, &sky_f, NULL);

    //Compile the Fragment Shader
    glCompileShader(fragShaderSkybox);

    //Create the Shader Program
    GLuint skyboxShaderProg = glCreateProgram();
    //Attach the compiled Vertex Shader
    glAttachShader(skyboxShaderProg, vertexShaderSkybox);
    //Attach the compiled Fragment Shader
    glAttachShader(skyboxShaderProg, fragShaderSkybox);

    //Finalize the compilation process
    glLinkProgram(skyboxShaderProg);

    glDeleteShader(vertexShaderSkybox);
    glDeleteShader(fragShaderSkybox);

    /*
  7--------6
 /|       /|
4--------5 |
| |      | |
| 3------|-2
|/       |/
0--------1
*/
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER,skyboxVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Skybox/rainbow_rt.png", //RIGHT
        "Skybox/rainbow_lf.png", //LEFT
        "Skybox/rainbow_up.png", //UP
        "Skybox/rainbow_dn.png", //DOWN
        "Skybox/rainbow_ft.png", //Front
        "Skybox/rainbow_bk.png" //Back
    };
    //skybox tex
    unsigned int skyboxTex;
    //Generate our skybox texture
    glGenTextures(1, &skyboxTex);
    //initialize it as a cube map
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    //this is to avoid out skybox from pizelating when its too big / small
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //Instead of ST only- cubemaps rely on RST
    //This is to make sure the texture stretched to the edge
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;

        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data) {

            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );

            stbi_image_free(data);
        }
    }

    stbi_set_flip_vertically_on_load(true);


    std::string path = "3D/chair.obj";
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
    std::vector<GLfloat> fullVertexData;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );
        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[vData1.texcoord_index * 2],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );
        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[vData2.texcoord_index * 2],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );
        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[vData3.texcoord_index * 2],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);


    }

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {

        //Assign the Index data for easy access
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        //Push the X position of the vertex
        fullVertexData.push_back(
            //Multiply the index by 3 to get the base offset
            attributes.vertices[(vData.vertex_index * 3)]
        );

        //Push the Y position of the vertex
        fullVertexData.push_back(
            //Add the base offset to 1 to get Y
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        //Push the Z position of the vertex
        fullVertexData.push_back(
            //Add the base offset to 2 to get Z
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        fullVertexData.push_back(
            //Add the base offset to 2 to get Z
            attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(
            //Add the base offset to 2 to get Z
            attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(
            //Add the base offset to 2 to get Z
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        //Push the U of the Tex Coords
        fullVertexData.push_back(
            //Multiply the index by 3 to get the base offset
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        //Push the V of the Tex Coords
        fullVertexData.push_back(
            //Add the base offset to 1 to get V
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );

        fullVertexData.push_back(
            tangents[i].x
        );
        fullVertexData.push_back(
            tangents[i].y
        );
        fullVertexData.push_back(
            tangents[i].z
        );
        fullVertexData.push_back(
            bitangents[i].x
        );
        fullVertexData.push_back(
            bitangents[i].y
        );
        fullVertexData.push_back(
            bitangents[i].z
        );

       
    }
    //GLfloat UV[]{
    //   0.f, 1.f,
    //   0.f, 0.f,
    //   1.f, 1.f,
    //   1.f, 0.f,
    //   1.f, 1.f,
    //   1.f, 0.f,
    //   0.f, 1.f,
    //   0.f, 0.f
    //};

    //GLfloat vertices[]{
    //    // x    y   z
    //    0.f,   0.5f, 0.f, //0
    //   -0.5f, -0.5f, 0.f, //1
    //    0.5f, -0.5f, 0.f  //2
    //};
    //
    //GLuint indices[]{
    //    0,1,2
    //};

    //initVars
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Generate VAO
    glGenVertexArrays(1, &VAO);

    //Generate VBO
    glGenBuffers(1, &VBO);


    //Bind VAO and VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Position
    glBufferData(
        GL_ARRAY_BUFFER,
        //Size of the whole array in bytes
        sizeof(GLfloat) * fullVertexData.size(),
        //Data of the array
        fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0, //index 0 is the vertex position
        3, //Position is 3 floats (x,y,z)
        GL_FLOAT, // Data type of array
        GL_FALSE,

        //Our vertex data has 5 floats in it
        //(X,Y,Z,U,V)
        14 * sizeof(GLfloat),//size of the vertex data in bytes
        (void*)0
    );

    glEnableVertexAttribArray(0);

    //Since our UV starts at index 3
    //or the 4th of our index data

    //Normalize
    GLintptr normalPtr = 3 * sizeof(float);  

    glVertexAttribPointer(
        1, //index 0 is the vertex position
        3, //Position is 3 floats (x,y,z)
        GL_FLOAT, // Data type of array
        GL_FALSE,

        //Our vertex data has 8 floats in it
        //(X,Y,Z,U,V)
        14 * sizeof(GLfloat),//size of the vertex data in bytes
        (void*)normalPtr
    );

    glEnableVertexAttribArray(1);

    //UV
    GLintptr uvPtr = 6 * sizeof(float);

    glVertexAttribPointer(
        2, //index 0 is the vertex position
        2, //Position is 3 floats (x,,z)
        GL_FLOAT, // Data type of array
        GL_FALSE,

        //Our vertex data has 8 floats in it
        //(X,Y,Z,U,V)
        14 * sizeof(GLfloat),//size of the vertex data in bytes
        (void*)uvPtr
    );
    glEnableVertexAttribArray(2);

    //Tangent
    GLintptr tangentPtr = 8 * sizeof(float);

    glVertexAttribPointer(
        3, //index 3 is the tangent
        3, //Position is 3 floats (x,y,z)
        GL_FLOAT, // Data type of array
        GL_FALSE,

        //Our vertex data has 8 floats in it
        //(X,Y,Z,U,V)
        14 * sizeof(GLfloat),//size of the vertex data in bytes
        (void*)tangentPtr
    );
    glEnableVertexAttribArray(3);

    //Tangent
    GLintptr bitangentPtr = 11 * sizeof(float);

    glVertexAttribPointer(
        4, //index 4 is the bitangent
        3, //Position is 3 floats (x,y,z)
        GL_FLOAT, // Data type of array
        GL_FALSE,

        //Our vertex data has 8 floats in it
        //(X,Y,Z,U,V)
        14 * sizeof(GLfloat),//size of the vertex data in bytes
        (void*)bitangentPtr
    );
    glEnableVertexAttribArray(4);

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
        glm::radians(90.f), //FOV
        window_heigth / window_widith, //Aspect ratio
        0.1f, //ZNear > 0
        100.f // ZFar
    );

    glm::vec3 camaeraPos = glm::vec3(x_mod, 3.f, 10.f);

    //Construct the Position Matrix

    glm::mat4 cameraPositionMatrix =
        glm::translate(glm::mat4(1.0f), //Intialize it as an Identity Matrix
            camaeraPos * -1.f); //Multiply to -1 since we need -P

    //World's Up Direction
    //Normally just 1 in Y
    glm::vec3 WorldUp = glm::vec3(0, 1.f, 0);

    //Camera's Center
    glm::vec3 Center = glm::vec3(0.f,1.f, 0.f);

    //Get Forward
    glm::vec3 F = glm::vec3(Center - camaeraPos);

    //Normalize the Forward
    F = glm::normalize(F);

    glm::vec3 R = glm::normalize(
                //R x F
                glm::cross(F,WorldUp)
    );

    glm::vec3 U = glm::normalize(
        //R x F
        glm::cross(R, F)
    );

    //Construct the Camera Orientation Matrix
    glm::mat4 cameraOrientation = glm::mat4(1.f);

    //Manually assign the Matrix
    //Matrix[Column][Row]
    cameraOrientation[0][0] = R.x;
    cameraOrientation[1][0] = R.y;
    cameraOrientation[2][0] = R.z;

    cameraOrientation[0][1] = U.x;
    cameraOrientation[1][1] = U.y;
    cameraOrientation[2][1] = U.z;

    cameraOrientation[0][2] = -F.x;
    cameraOrientation[1][2] = -F.y;
    cameraOrientation[2][2] = -F.z;

    //Camera View Matrix
    glm::mat4 viewMatrix = cameraOrientation * cameraPositionMatrix;

    //Direction of light
    glm::vec3 lightDirection = glm::vec3(0.5, 0.7, -2);

    //Light Color
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    //Ambient strength
    float ambientStr = 0.1f;

    //Ambient Color
    glm::vec3 ambientColor = lightColor;

    //Spec strength
    float specStr = 10.0f;

    //Spec phong
    float specPhong = 50.f;

    float constant = 1.f;

    float linear = 2.f;

    float exponent = 0.f;

    float brightness = 1.f;
    
    //Enable blending
    glEnable(GL_BLEND);

    //Choose a blending function
    glBlendFunc(GL_SRC_ALPHA, //Foreground layer
        GL_ONE_MINUS_SRC_ALPHA); //Background Layer

    //Cfinal = Cf(Af) + Cb(1-Af)
    //Cfinal = Cf(Af) - Cb(1-Af) glBelndEquation(GL_FUNC_SUBTRACT);

    //Cfinal = Cf(Af) + Cb(Cf)

    //Andres Rigglsen - Visual glBlendFunc and GlBlendEquation Tool

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        glDepthFunc(GL_LEQUAL);

        glUseProgram(skyboxShaderProg);

        //Initialize the SkyBox's view matrix
        glm::mat4 sky_view = glm::mat4(1.0);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShaderProg, "view");

        glUniformMatrix4fv(skyboxViewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(sky_view)); //View Matrix

        unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShaderProg, "projection");

        glUniformMatrix4fv(skyboxProjLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projectionMatrix));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProg);

     
        //Simple animation
        y_mod += 0.01f;

        /*Get the address of the variable x in the shader
        unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
        unsigned int yLoc = glGetUniformLocation(shaderProg, "y");

        //Assign x by using its address
        glUniform1f(xLoc, x_mod);
        glUniform1f(yLoc, y_mod);*/
        z += -2.f;
        


        glm::mat4 transformation_matrix =
            glm::translate(identity_matrix4,
                glm::vec3(0.f,0.f,0.f + z_mod)
            );
        transformation_matrix =
            glm::scale(transformation_matrix,
                glm::vec3(10.f,10.f,10.f)
            );
        transformation_matrix =
            glm::rotate(transformation_matrix,
                glm::radians(-90.f),
                glm::normalize(glm::vec3(0.f,0.f,1.f))
            );
        transformation_matrix =
            glm::rotate(transformation_matrix,
                glm::radians(y_mod),
                glm::normalize(glm::vec3(0.f, 1.f, 0.f))
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

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");

        glUniformMatrix4fv(viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(viewMatrix)); //View Matrix

        glActiveTexture(GL_TEXTURE0);

        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");

        glBindTexture(GL_TEXTURE_2D, texture);

        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightDirection");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightDirection));

        GLuint lightColorAddress = glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProg, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(camaeraPos));

        GLuint specStrAddress = glGetUniformLocation(shaderProg, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProg, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        GLuint constantAddress = glGetUniformLocation(shaderProg, "constant");
        glUniform1f(constantAddress, constant);

        GLuint linearAddress = glGetUniformLocation(shaderProg, "linear");
        glUniform1f(linearAddress, linear);

        GLuint exponentAddress = glGetUniformLocation(shaderProg, "exponent");
        glUniform1f(exponentAddress, exponent);

        GLuint brightnessAddress = glGetUniformLocation(shaderProg, "brightness");
        glUniform1f(brightnessAddress, brightness);

        glUniform1i(tex0Address, 0);

        glActiveTexture(GL_TEXTURE1);

        GLuint tex1Address = glGetUniformLocation(shaderProg, "norm_tex");

        glBindTexture(GL_TEXTURE_2D, norm_tex);

        glUniform1i(tex1Address, 1);

        //Render here

        glBindVertexArray(VAO);

        //Rendering the model
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);
        
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
    glfwTerminate();
    return 0;
}