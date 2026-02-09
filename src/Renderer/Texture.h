#pragma once
#include <string>


class Texture
{
private:

int width, height, nrChannels;
void LoadTextureFromFile(const std::string& path);
public:
    unsigned int textureID;


    void DeleteTexture();
    
    // // ❌ NO COPYING
    // Texture(const Texture&) = delete;
    // Texture& operator=(const Texture&) = delete;

    //     // ✅ MOVE CONSTRUCTOR
    // Texture(Texture&& other) noexcept;

    // // ✅ MOVE ASSIGNMENT
    // Texture& operator=(Texture&& other) noexcept;

    Texture() = default;
    Texture(std::string path);
    Texture(std::string path, bool flipVertically);
    void Bind() const;
    void Bind(unsigned int slot) const;
    ~Texture();
};