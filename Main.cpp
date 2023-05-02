#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_s.h>
#include "Camera.h"
#include "Model.h"

#include <iostream>
#include <irrKlang.h>
using namespace irrklang;
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera, setting the starting point so it looks like you start at the base of the stairs
Camera camera(glm::vec3(-3.5f, 1.0f, 2.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool firstLight, secondLight, thirdLight, fourthLight, monsterStart = false;
int frametime = 1;

// timing
float deltaTime = 2.0f;
float lastFrame = 0.0f;


ISoundEngine* SoundEngine = createIrrKlangDevice();


//We want the animation to start when we are in position. To achieve this, I am using a key to start the animation
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        monsterStart = true;
    }

}
void renderObject(Model aModel, Shader aShader, float posX, float posZ, float posY, float rotX, float rotZ, float rotY, float scaleX, float scaleZ, float scaleY);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "The Strangers", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------

    Shader lightingShader("lightCasterVertexShader.txt", "lightCasterFragmentShader.txt");
    Shader cubeShader("lightCubeVertexShader.txt", "lightCubeFragmentShader.txt");

    //Loop  pulls in each frame of the animation in order
    
    std::vector<Model> animation;
    for (int i = 1; i <= 242; i++)
    {
        Model animationSeries("C:/Users/amand/Desktop/Graphics/LivingRoom/LivingRoom/animation/standup" + std::to_string(i) + ".obj");
        animation.push_back(animationSeries);
    }

    // load models
    // -----------
    Model carpet("floor/Carpet_Flottebo_Mat.obj");
    Model walls("wall/material_0.obj");
    Model ceiling("ceiling/Roof_1001.obj");
    Model light("ceilinglight/FluorescentLight.obj");
    Model staircase("staircase/Scene_-_Root.obj");
    Model table("table/table.obj");
    Model chair("chair/chair.obj");
    Model tvStand("tvstand/TVStand_mat.obj");
    Model tv("tv/DefaultMaterial.obj");
    Model couch("couch/lambert1.obj");
    Model gameboy("toys/1001.obj");
    Model cartrige("toys/uv_checker_material_uv_grid_4096x4096_1.obj");
    Model nerf("toys/01___Default.obj");
    Model washingMachine("laundryroom/lambert2.obj");
    Model dryer("laundryroom/Metal.obj");
    Model table2("table/blinn1.obj");
    Model bopit("toys/bop_it_export_3blinn1.obj");
    Model dagger("toys/Dagger.obj");
    Model desk("computer/Material.004.obj"); 
    Model deskChair("computer/chair.obj");
    Model computer("computer/Material.001.obj");
    Model ashtray("ashtray/material_0.obj");
    Model shelf("shelves/1001.obj");
    Model soap("laundryroom/Slevee_1.obj");
    Model soap2("laundryroom/Sleeve_2.obj");
    Model mask("toys/MAT_Mask_PowerRangers2.obj");
    Model beanBag("chair/A_Bean_Bag.obj");
    Model childChair("chair/plastic.obj");
    Model checkers("toys/initialShadingGroup.obj");
    Model bookcase("bookcase/Material_25.obj");
    Model hulk("toys/material_0.obj");
    Model turtle("toys/mesh.obj");
    Model megazord("toys2/material_0.obj");
    Model radio("toys2/radio_tex.obj");
    Model basketball("toys2/initialShadingGroup.obj");
    Model games("toys2/boardgames.obj");
    Model statue("bookcase/material_0.obj");
    Model books("bookcase/material.obj");
    Model books2("bookcase/books.obj");
    Model spiderman("toys2/spidermanStatue_003_bodySculptdefaultMat.obj");
    Model oldWitch("painting/OldPortraitWitch_01.obj");
    Model animalFight("painting/model_Material_u1_v1.obj");

    SoundEngine->play2D("325363__jacobschroder__horror-suspense.wav", false);

    // render loop
    // -----------

    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
       

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // The point lights are going to have different colors for different objects. We want the computer monitor to be brighter than the surrounding area
        glm::vec3 pointLightColors[] = {
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3(0.05f, 0.05f, 0.05f)
        };

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -3.3f);
        lightingShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05);
        lightingShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

            // point light 1
            lightingShader.setVec3("pointLights[0].position", 0.0f, 1.8f, 3.0f);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
            lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09f);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

        // point light 2
            lightingShader.setVec3("pointLights[1].position", 0.0f, 1.8f, -4.0f);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09f);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032f);

        // point light 3
            lightingShader.setVec3("pointLights[2].position", -4.0f, 1.8f, -4.0f);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09f);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032f);

        // point light 4
            lightingShader.setVec3("pointLights[3].position", -4.0f, 1.8f, 1.0f);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09f);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032f);

            //point light 5
            lightingShader.setVec3("pointLights[4].position", -6.6f, 0.05f, -0.48f);
            lightingShader.setVec3("pointLights[4].ambient", pointLightColors[4].x * 0.1, pointLightColors[4].y * 0.1, pointLightColors[4].z * 0.1);
            lightingShader.setVec3("pointLights[4].diffuse", pointLightColors[4].x, pointLightColors[4].y, pointLightColors[4].z);
            lightingShader.setVec3("pointLights[4].specular", pointLightColors[4].x, pointLightColors[4].y, pointLightColors[4].z);
            lightingShader.setFloat("pointLights[4].constant", 1.0f);
            lightingShader.setFloat("pointLights[4].linear", 0.09f);
            lightingShader.setFloat("pointLights[4].quadratic", 0.032f);

            //point light 6
            lightingShader.setVec3("pointLights[5].position", -6.6f, 0.05f, -0.45f);
            lightingShader.setVec3("pointLights[5].ambient", pointLightColors[5].x * 0.1, pointLightColors[5].y * 0.1, pointLightColors[5].z * 0.1);
            lightingShader.setVec3("pointLights[5].diffuse", pointLightColors[5].x, pointLightColors[5].y, pointLightColors[5].z);
            lightingShader.setVec3("pointLights[5].specular", pointLightColors[5].x, pointLightColors[5].y, pointLightColors[5].z);
            lightingShader.setFloat("pointLights[5].constant", 1.0f);
            lightingShader.setFloat("pointLights[5].linear", 0.09f);
            lightingShader.setFloat("pointLights[5].quadratic", 0.032f);

            //point light 7
            lightingShader.setVec3("pointLights[6].position", 3.0f, 1.8f, -4.0f);
            lightingShader.setVec3("pointLights[6].ambient", pointLightColors[6].x * 0.1, pointLightColors[6].y * 0.1, pointLightColors[6].z * 0.1);
            lightingShader.setVec3("pointLights[6].diffuse", pointLightColors[6].x, pointLightColors[6].y, pointLightColors[6].z);
            lightingShader.setVec3("pointLights[6].specular", pointLightColors[6].x, pointLightColors[6].y, pointLightColors[6].z);
            lightingShader.setFloat("pointLights[6].constant", 1.0f);
            lightingShader.setFloat("pointLights[6].linear", 0.09f);
            lightingShader.setFloat("pointLights[6].quadratic", 0.032f);

            //point light 8
            lightingShader.setVec3("pointLights[7].position", 3.0f, 1.8f, 1.0f);
            lightingShader.setVec3("pointLights[7].ambient", pointLightColors[7].x * 0.1, pointLightColors[7].y * 0.1, pointLightColors[7].z * 0.1);
            lightingShader.setVec3("pointLights[7].diffuse", pointLightColors[7].x, pointLightColors[7].y, pointLightColors[7].z);
            lightingShader.setVec3("pointLights[7].specular", pointLightColors[7].x, pointLightColors[7].y, pointLightColors[7].z);
            lightingShader.setFloat("pointLights[7].constant", 1.0f);
            lightingShader.setFloat("pointLights[7].linear", 0.09f);
            lightingShader.setFloat("pointLights[7].quadratic", 0.032f);
        

        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // don't forget to enable shader before setting uniforms
        lightingShader.use();

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // render the loaded model
        /*When we load each model, each line performs a different function
        Line1: places the model in the 3D space
        Line 2: rotates the object
        Line 3: scales the object
        You will notice that the scales are different depending on the model*/

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, -1.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(0.07f, 0.05f, 0.07f));	
        lightingShader.setMat4("model", model);
        carpet.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.65f, -1.00f, -2.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));	
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);


        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.66f, -1.01f, -1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.67f, -1.02f, 1.0f)); 
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.68f, -1.03f, 2.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);


        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.69f, -1.04f, 3.51f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 3.50f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.8f, -1.0f, 3.49f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));	
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.8f, -1.0f, 3.45f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));	
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.2f, -1.0f, 3.48f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.5f, -1.0f, 3.47f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.5f, -1.0f, 3.46f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));	
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, -1.0f, -5.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 2.5f));	
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

         lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, -4.99f));
        model = glm::scale(model, glm::vec3(2.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.8f, -1.0f, -4.98f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.2f, -1.0f, -4.97f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.5f, -1.0f, -4.96f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-9.5f, -1.0f, -4.95f)); 
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-6.1f, 1.2f, -4.5f));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        lightingShader.setMat4("model", model);
        oldWitch.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.01f, -1.0f, 1.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 2.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.02f, 0.7f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        animalFight.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.03f, -1.0f, -3.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 2.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.04f, -1.0f, -1.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 2.5f));
        lightingShader.setMat4("model", model);
        walls.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 5.5f));
        model = glm::scale(model, glm::vec3(0.7f, 0.5f, 0.7f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 7.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 4.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 4.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 1.0f, 4.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.7f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);


        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 6.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 1.0f, 6.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 7.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, 1.0f, 7.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        ceiling.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        light.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, -4.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        light.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 2.0f, -4.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        light.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 2.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        light.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 2.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        light.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 2.0f, -4.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        light.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.5f, -0.9f, 3.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        lightingShader.setMat4("model", model);
        dagger.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.8f, -0.9f, 2.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        lightingShader.setMat4("model", model);
        mask.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0f, -0.9f, 2.9f));
        model = glm::rotate(model, glm::radians(85.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
        lightingShader.setMat4("model", model);
        staircase.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -0.95f, -1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        table.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.9f, -0.65f, -0.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setMat4("model", model);
        gameboy.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.1f, -0.65f, -0.5f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setMat4("model", model);
        cartrige.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -0.65f, -1.4f)); 
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	
        lightingShader.setMat4("model", model);
        bopit.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, -0.9f, -4.4f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setMat4("model", model);
        hulk.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.8f, -0.8f, -4.4f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setMat4("model", model);
        turtle.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.6f, -0.8f, -3.7f)); 
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setMat4("model", model);
        spiderman.Draw(lightingShader);


        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, -0.75f, -2.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setMat4("model", model);
        megazord.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -0.85f, -0.35f)); 
        model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        lightingShader.setMat4("model", model);
        nerf.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, -0.5f, -1.0f)); 
        model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));	
        lightingShader.setMat4("model", model);
        chair.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, -1.0f, 3.2f)); 
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 1.0f, 0.7f));	
        lightingShader.setMat4("model", model);
        bookcase.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.6f, -0.5f, 3.4f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	
        lightingShader.setMat4("model", model);
        statue.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, 0.2f, 3.17f)); 
        model = glm::rotate(model, glm::radians(145.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));	
        lightingShader.setMat4("model", model);
        books.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.4f, 0.58f, 3.17f)); 
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.15f, 0.14f, 0.15f));
        lightingShader.setMat4("model", model);
        books2.Draw(lightingShader);


        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.7f, -1.0f, 3.3f)); 
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        lightingShader.setMat4("model", model);
        beanBag.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.5f, -0.4f, -4.5f)); 
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        lightingShader.setMat4("model", model);
        washingMachine.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-6.5f, -0.9f, -4.5f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setMat4("model", model);
        games.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.4f, -0.4f, -4.5f)); 
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));	
        lightingShader.setMat4("model", model);
        dryer.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.5f, -0.75f, -4.5f)); 
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setMat4("model", model);
        basketball.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.3f, -1.0f, -4.2f)); 
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setMat4("model", model);
        tvStand.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.3f, -0.02f, -4.2f)); 
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));	
        lightingShader.setMat4("model", model);
        tv.Draw(lightingShader);


        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.4f, -1.8f, 0.0f)); 
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));	
        lightingShader.setMat4("model", model);
        couch.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.5f, -1.0f, 0.0f)); 
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.15f, 0.2f));
        lightingShader.setMat4("model", model);
        table2.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.5f, -0.5f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	
        lightingShader.setMat4("model", model);
        checkers.Draw(lightingShader);


        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.8f, -0.8f, 0.0f)); 
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        childChair.Draw(lightingShader);

        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.3f, -0.8f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        childChair.Draw(lightingShader);


        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.5f, -1.0f, -0.5f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setMat4("model", model);
        desk.Draw(lightingShader);


        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-6.0f, -1.0f, -0.5f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setMat4("model", model);
        deskChair.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.5f, -0.2f, -0.5f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));	
        lightingShader.setMat4("model", model);
        computer.Draw(lightingShader);


        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 1.3f, -4.8f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setMat4("model", model);
        shelf.Draw(lightingShader);



        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.3f, 1.5f, -4.8f)); 
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setMat4("model", model);
        soap.Draw(lightingShader);

        lightingShader.use();
        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.1f, 1.5f, -4.8f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setMat4("model", model);
        soap2.Draw(lightingShader);

        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.3f, 1.5f, -4.8f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setMat4("model", model);
        radio.Draw(lightingShader);

        /*Animation loop to run the animation at 60 FPS
        This loop goes through each frame and plays them in order until it reaches the final frame
        Animation was found at mixamo.com
        There are supposed to be textures, but loading them makes my computer run out of memory*/

        if (monsterStart == true) {
            renderObject(animation.at(frametime), lightingShader, 0.5f, -3.0f, -1.0f, 0.0f, 0.0f, 90.0f, 1.7f, 1.7f, 1.7f);
            frametime += 1;
        }



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void renderObject(Model aModel, Shader aShader, float posX, float posZ, float posY, float rotX, float rotZ, float rotY, float scaleX, float scaleZ, float scaleY) {
    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(posX, posY, posZ));
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
    model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

    aShader.setMat4("model", model);
    aModel.Draw(aShader);
}
