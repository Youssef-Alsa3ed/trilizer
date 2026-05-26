#include "enginepch.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Camera.h"
#include "Renderer/Model.h"
#include "Renderer/Light.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/IndexBuffer.h"
#include <vector>

#include "Imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace glm;

bool firstMouse = true;
bool hideCursor = false;
int lastX = 400, lastY = 300;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::mat4 projection;
unique_ptr<Shader> lightingshader;
unique_ptr<Shader> singlecolorshader;
unique_ptr<Shader> lightSourceShader;

unique_ptr<Model> cube;
unique_ptr<Model> cube2;

struct Mesh {
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;

    Mesh(const float* vertices, unsigned int vertCount, const unsigned int* indices, unsigned int indexCount) {
        vao.Bind();
        vbo = VertexBuffer(vertices, vertCount * sizeof(float));
        VBL layout;
        layout.Push<float>(3, "position");
        vao.AddBuffer(vbo, layout);
        ibo = IndexBuffer(indices, indexCount);
    }

    void Draw(Shader& shader) {
        vao.Bind();
        ibo.Bind();
        glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
    }
};

struct Plane {
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;
    unsigned int indexCount;

    Plane(float size, int segments) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        float step = size / segments;

        for (int j = 0; j <= segments; j++) {
            for (int i = 0; i <= segments; i++) {
                float x = i * step - size * 0.5f;
                float z = j * step - size * 0.5f;
                float u = (float)i / segments;
                float v = (float)j / segments;
                vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(z);
                vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
                vertices.push_back(u); vertices.push_back(v);
                vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);
                vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(1.0f);
            }
        }

        for (int j = 0; j < segments; j++) {
            for (int i = 0; i < segments; i++) {
                unsigned int v0 = j * (segments + 1) + i;
                unsigned int v1 = v0 + 1;
                unsigned int v2 = v0 + (segments + 1);
                unsigned int v3 = v2 + 1;
                indices.push_back(v0); indices.push_back(v1); indices.push_back(v2);
                indices.push_back(v1); indices.push_back(v3); indices.push_back(v2);
            }
        }

        indexCount = indices.size();
        vao.Bind();
        vbo = VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
        VBL layout;
        layout.Push<float>(3, "position");
        layout.Push<float>(3, "normal");
        layout.Push<float>(2, "texCoord");
        layout.Push<float>(3, "tangent");
        layout.Push<float>(3, "bitangent");
        vao.AddBuffer(vbo, layout);
        ibo = IndexBuffer(indices.data(), indices.size());
    }

    void Draw(Shader& shader) {
        vao.Bind();
        ibo.Bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }
};

unique_ptr<Mesh> lightCube;
unique_ptr<Plane> rockPlane;
unique_ptr<Texture> rockAlbedo;
unique_ptr<Texture> rockNormal;

void MoveCamera(float deltaTime)
{
    if (Input::Get().GetKey(GLFW_KEY_W))
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (Input::Get().GetKey(GLFW_KEY_S))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (Input::Get().GetKey(GLFW_KEY_A))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (Input::Get().GetKey(GLFW_KEY_D))
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

ImGuiIO *io;

void AppInit()
{
    lightingshader = std::make_unique<Shader>("../../Assets/Shaders/lightshader.glsl");
    lightSourceShader = std::make_unique<Shader>("../../Assets/Shaders/lightsource.glsl");
    singlecolorshader = std::make_unique<Shader>("../../Assets/Shaders/outlineshader.glsl");

    cube = std::make_unique<Model>("../../Assets/Models/backpack/backpack.obj", true);
    cube2 = std::make_unique<Model>("../../Assets/Models/companion_cube/scene.gltf", false);
    cube2->transform = glm::translate(cube2->transform, vec3(2.0f));
    Light light = CreateDirectionLight();
    LightManager::Get().Submit(light);
    Light point = CreatePointLight();
    point.position = vec3(point.position + vec3(2.0f, 0.0f, 0.0f));
    point.diffuse = vec3(0.6f, 0.33f, 0.2f) * 3.0f;
    LightManager::Get().Submit(point);

    Light point2 = CreatePointLight();
    point2.position = vec3(0.0f, 2.0f, 0.0f);
    point2.diffuse = vec3(0.2f, 0.4f, 0.8f) * 3.0f;
    LightManager::Get().Submit(point2);

    // #region morelights
    for(int i = 0; i < 3; ++i) {
        Light p = CreatePointLight();
        p.position = vec3(sin(i * 2.0f) * 5.0f, 1.0f, cos(i * 2.0f) * 5.0f);
        p.diffuse = vec3(0.5f + sin(i), 0.5f + cos(i), 0.5f) * 2.0f;
        LightManager::Get().Submit(p);
    }
    // #endregion morelights

    float cubeVerts[] = {
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
    };
    unsigned int cubeIndices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1
    };
    lightCube = std::make_unique<Mesh>(cubeVerts, 24, cubeIndices, 36);

    rockPlane = std::make_unique<Plane>(10.0f, 10);
    rockAlbedo = std::make_unique<Texture>();
    rockAlbedo->LoadAsync("../../Assets/Textures/Quixel/Rock-Albedo.png");
    rockNormal = std::make_unique<Texture>();
    rockNormal->LoadAsync("../../Assets/Textures/Quixel/Rock-Normal.png");

    // Setup Dear ImGui context
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale; // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Application::GetInstance()->GetWindow().GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

    float normalStrength = 0.0f;


ImVec4 clear_color = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
void AppUpdate(float deltaTime)
{

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    LightManager::Get().lights[0].diffuse = vec3(clear_color.x, clear_color.y, clear_color.z);

    static float time = 0.0f;
    time += deltaTime;

    // #region morelights
    for (size_t i = 1; i < LightManager::Get().lights.size(); ++i) {
        float offset = (float)i * 1.3f;
        LightManager::Get().lights[i].position = vec3(
            sinf(time * (0.5f + offset * 0.1f)) * (3.0f + offset),
            1.5f + sinf(time * (0.7f + offset * 0.2f)) * 1.0f,
            cosf(time * (0.5f + offset * 0.1f)) * (3.0f + offset)
        );
    }
    // #endregion morelights


    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.


        ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color


        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
        ImGui::End();
    }

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // enable writing to the stencil buffer
    mat4 projection = glm::perspective(glm::radians(45.0f), Application::GetInstance()->GetWindow().GetAspectRatio(), 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    lightingshader->Use();
    lightingshader->SetMat4("view", camera.GetViewMatrix());
    lightingshader->SetMat4("projection", projection);

    lightingshader->SetMat4("model", model);
    LightManager::Get().Upload(*lightingshader);

    rockAlbedo->Bind(0);
    rockNormal->Bind(1);
    lightingshader->SetInt("material.tex1", 0);
    lightingshader->SetInt("material.tex3", 1);

    {
        ImGui::SliderFloat("Normal Strength", &normalStrength, 0.0f, 10.0f);
    }
    lightingshader->SetFloat("normalStrength", normalStrength);

    mat4 planeModel = glm::translate(mat4(1.0f), vec3(0.0f, -3.0f, 0.0f));
    lightingshader->SetMat4("model", planeModel);
    rockPlane->Draw(*lightingshader);

    lightingshader->SetMat4("model", model);
    cube->Draw(*lightingshader);
    rockNormal->Bind(1);
    cube2->Draw(*lightingshader);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to the stencil buffer
    glDisable(GL_DEPTH_TEST);
    singlecolorshader->Use();
    model = scale(model, vec3(1.025f));
    singlecolorshader->SetMat4("view", camera.GetViewMatrix());
    singlecolorshader->SetMat4("projection", projection);
    singlecolorshader->SetMat4("model", model);
    cube->Draw(*singlecolorshader);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_STENCIL_TEST);
    lightSourceShader->Use();
    lightSourceShader->SetMat4("view", camera.GetViewMatrix());
    lightSourceShader->SetMat4("projection", projection);
    for (size_t i = 1; i < LightManager::Get().lights.size(); i++) {
        auto model = glm::translate(mat4(1.0f), LightManager::Get().lights[i].position);
        model = glm::scale(model, vec3(0.3f));
        lightSourceShader->SetMat4("model", model);
        lightSourceShader->SetVec3("lightColor", LightManager::Get().lights[i].diffuse);
        lightCube->Draw(*lightSourceShader);
    }
    glEnable(GL_STENCIL_TEST);

    MoveCamera(deltaTime);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AppClose()
{
    lightingshader.reset();
    lightSourceShader.reset();
    cube.reset();
    cube2.reset();
    lightCube.reset();
    rockPlane.reset();
    rockAlbedo.reset();
    rockNormal.reset();
}
bool ClientKeyCallBack(KeyPressEvent &e)
{
    return true;
}

bool ClientMouseMoveCallBack(MouseMoveEvent &e)
{
    if (!hideCursor)
    {
        // man i'm such a genius
        firstMouse = true;
        return true;
    }

    float xpos = e.GetX();
    float ypos = e.GetY();
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

    return true;
}

bool ClientMouseButtonCallBack(MouseButtonEvent &e)
{
    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
    {
        hideCursor = !hideCursor;
        if (hideCursor)
            Input::Get().DisableCursor();
        else
            Input::Get().EnableCursor();
    }

    return true;
}

int main()
{
    Application app;
    app.Run();

    return 0;
}
