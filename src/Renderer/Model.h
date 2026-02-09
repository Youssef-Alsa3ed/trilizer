#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "TriMesh.h"
#include <unordered_map>

class Model 
{
    public:
        Model(const string &path)
        {
            loadModel(path);
        }
        Model(const string &path, bool isFlipped) : loadTexturesFlipped(isFlipped){
            loadModel(path);
        }
        void Draw(Shader &shader);
        void Draw(Shader &shader, TriMat &mat);	

        std::unordered_map<std::string, Texture> loadedTextures;
    private:
        // model data
        vector<TriMesh> meshes;
        vector<TriMat> mats;
        string directory;

        bool loadTexturesFlipped = false;

        void loadModel(string const &path);
        void processNode(aiNode *node, const aiScene *scene);
        void processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             string typeName);
};