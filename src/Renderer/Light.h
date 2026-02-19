#include <glm/glm.hpp>
#include "Shader.h"
#include <memory>
enum LightType {
    Directional = 0,
    Point = 1,
    Spot = 2
};

struct Light {
    glm::vec3 position{0.0f};
    glm::vec3 direction{0.0f};
    glm::vec3 ambient{0.1f};
    glm::vec3 diffuse{0.1f};
    glm::vec3 specular{1.0f};

    float constant{1.0f};
    float linear{0.0f};
    float quadratic{0.0f};

    float cutOff{0.0f};
    float outerCutOff{0.0f};

    int type = 2;
};

Light CreateSpotLight();

Light CreateDirectionLight();

Light CreatePointLight();

class LightManager {
    private: 
    LightManager() = default;  // private constructor
    public:
        std::vector<Light> lights;
        void Submit(const Light& light);
        void Flush();
        void Upload(const Shader& shader);
        static LightManager& Get()
        {
            static LightManager instance;  // Meyers singleton
            return instance;
        }

};