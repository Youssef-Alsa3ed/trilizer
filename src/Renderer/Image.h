#pragma once
#include <iostream>
#include "stb/stb_image.h"
#include "Logger/Logger.h"
#include "Renderer/GLDEBUGGING.hpp"
struct Image{
    unsigned char *imageData;
    int width, height, nrChannels;


    Image(std::string path, bool flipVertically = false){
        stbi_set_flip_vertically_on_load_thread(flipVertically);
        imageData = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        if(!imageData){
            ERRLOG("Error Loading File.");
        }
    }

    void Free(){
        stbi_image_free(imageData);
    }
};


inline int GetImageChannelsFormatGL(int numberOfChannels){
    switch (numberOfChannels)
    {
    case 1:
        return GL_RED;
        break;
    case 2:
        return GL_RG;
        break;
    case 3:
        return GL_RGB;
        break;
    case 4:
        return GL_RGBA;
        break;
    default:
        return GL_RGB;
        ERRLOG("INVALID NUMBER OF CHANNELS PASSED TO RENDER TEXTURE.");
        break;
    }
}

inline void FillTextureData(GLenum type, Image &image){
    GLCALL(glTexImage2D(
        type,
        0, //mipmap level
        GetImageChannelsFormatGL(image.nrChannels), // Image Format
        image.width, // Image Width
        image.height, // Image Height
        GL_FALSE, // Generate Borders?
        GetImageChannelsFormatGL(image.nrChannels), // storage format
        GL_UNSIGNED_BYTE, //channel data type
        image.imageData // pixels
    ));  
}



