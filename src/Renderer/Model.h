#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "TriMesh.h"
#include "enginepch.h"
#include "Image.h"


class Model 
{
    public:
        glm::mat4 transform = glm::mat4(1.0f);
        Model(const string &path)
        {
            loadModel(path);
        }
        Model(const string &path, bool isFlipped) : loadTexturesFlipped(isFlipped){
            loadModel(path);
        }
        void Draw(Shader &shader);
        void Draw(Shader &shader, TriMat &mat);	

        std::unordered_map<std::string, std::shared_ptr<Texture>> loadedTextures;

        ~Model();
    private:
        // model data
        vector<TriMesh> meshes;
        vector<TriMat> mats;
        string directory;
        bool loadTexturesFlipped = false;

        void loadModel(string const &path);
        void processNode(aiNode *node, const aiScene *scene);
        void processMesh(aiMesh *mesh, const aiScene *scene);
        vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             string typeName);
};