#include <Utility/UI_LoadImage.h>
#include <stb_image.h>
#include <iostream>


GLuint UIEditor::LoadImageForImGui(const std::string& filepath, int& outWidth, int& outHeight) {
    // Load image from disk
    int channels = 0;
    stbi_set_flip_vertically_on_load(1); // optional: flip Y for ImGui
    unsigned char* pixels = stbi_load(filepath.c_str(), &outWidth, &outHeight, &channels, 4);
    if (!pixels) {
        std::cerr << "Failed to load image: " << filepath << std::endl;
        return 0;
    }

    // Generate OpenGL texture
    GLuint texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, outWidth, outHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(pixels);
    return texID;
}
