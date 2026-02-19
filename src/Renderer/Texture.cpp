#include "Texture.h"
#include "glad/glad.h"
#include "Logger/Logger.h"
#include "Renderer/GLDEBUGGING.hpp"

void Texture::LoadTextureFromFile(const std::string &path)
{
    // glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // filling texture buffer
    Image image(path);
    if (image.imageData)
    {
        FillTextureData(GL_TEXTURE_2D, image);
        //GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        ERRLOG("Failed to load texture at path: " + path);
        return;
    }
}
void Texture::DeleteTexture()
{
    GLCALL(glDeleteTextures(1, &textureID));
}
// Texture::Texture(Texture &&other) noexcept
//     : textureID(other.textureID)
// {
//     other.textureID = 0;
// }
// Texture &Texture::operator=(Texture &&other) noexcept
// {
//     if (this != &other)
//     {
//         if (textureID != 0)
//             glDeleteTextures(1, &textureID);

//         textureID = other.textureID;
//         other.textureID = 0;
//     }
//     return *this;
// }
// Texture::Texture(std::string path)
// {
//     stbi_set_flip_vertically_on_load(false);
//     LoadTextureFromFile(path);
// }

// Texture::Texture(std::string path, bool flipVertically)
// {
//     stbi_set_flip_vertically_on_load(flipVertically);
//     LoadTextureFromFile(path);
// }

void Texture::Bind()
{
    TryUploadToGPU();

    if (uploaded){
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    else {
        TRACELOG("Texture Is Not Loaded Yet.");
    }
}

void Texture::Bind(unsigned int slot) 
{
    TryUploadToGPU();

    if (uploaded){
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}

Texture::~Texture()
{

}


void Texture::LoadAsync(const std::string path)
{
    stbi_set_flip_vertically_on_load(isFlipped);
    futureImage = std::async(std::launch::async, [path]() {
        Image img(path.c_str());
        return img;
    });
}

void Texture::TryUploadToGPU()
{
        if (uploaded)
        return;

    if (futureImage.valid() &&
        futureImage.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        Image img = futureImage.get();

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        if(img.imageData){
            FillTextureData(GL_TEXTURE_2D, img);
            GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
            
        }
        else {
            ERRLOG("FAILED TO LOAD IMAGE.");
        }

        img.Free();

        uploaded = true;
    }
}

void Texture::LoadAsync(const std::string path, bool loadFlipped)
{
    isFlipped = loadFlipped;
    LoadAsync(path);
}
