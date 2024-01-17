#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;
    float r = 0.3;
    float angle = 0;
    float theta;
    float x;
    float y;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Mathieu Pobre", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Render here
        glBegin(GL_POLYGON);
        for (int sides = 0; sides < 5; sides++) {
            std::cout << angle << '\n';
            theta = angle * (3.14 / 180);
            x = r * cos(theta);
            y = r * sin(theta);
            glVertex2f(x, y);
            std::cout << x << " , " << y << '\n';
            angle += 72;

            //Stops the shape from spinning
            if (sides == 4) {
                angle = 0;
            }
        }
        /*glVertex2f(-0.5f, -0.5f);
        glVertex2f(-0.6f, 0.5f);
        glVertex2f(0.5f, -0.5f);*/
        
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}