#include "Texture.h"
#include "glad/glad.h"
#include "Logger/Logger.h"
#include "Renderer/GLDEBUGGING.hpp"
#include "Renderer/Image.h"

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
        GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
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
Texture::Texture(std::string path)
{
    stbi_set_flip_vertically_on_load(false);
    LoadTextureFromFile(path);
}

Texture::Texture(std::string path, bool flipVertically)
{
    stbi_set_flip_vertically_on_load(flipVertically);
    LoadTextureFromFile(path);
}

void Texture::Bind() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureID));
}

void Texture::Bind(unsigned int slot) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureID));
}

Texture::~Texture()
{

}
