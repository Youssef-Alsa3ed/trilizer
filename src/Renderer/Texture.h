#pragma once
#include "enginepch.h"
#include <string>
#include "Renderer/Image.h"


class Texture
{
private:

std::future<Image> futureImage;
int width, height, nrChannels;
void LoadTextureFromFile(const std::string& path);


bool uploaded = false;
bool isFlipped = false;
public:
    unsigned int textureID = 0;


    void DeleteTexture();

    // ❌ disable copy
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // ✅ enable move
    Texture(Texture&&) noexcept = default;
    Texture& operator=(Texture&&) noexcept = default;

    Texture() = default;
    //Texture(std::string path);
    //Texture(std::string path, bool flipVertically);
    void LoadAsync(const std::string path);
    void LoadAsync(const std::string path, bool loadFlipped);
    void TryUploadToGPU();  
    void Bind();
    void Bind(unsigned int slot);
    ~Texture();
};