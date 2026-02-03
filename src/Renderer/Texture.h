#pragma once
#include <string>


class Texture
{
private:
    unsigned int textureID;

    int width, height, nrChannels;

    void LoadTextureFromFile(const std::string& path);
public:
    Texture(std::string path);
    Texture(std::string path, bool flipVertically);
    void Bind() const;
    void Bind(unsigned int slot) const;
    ~Texture();
};