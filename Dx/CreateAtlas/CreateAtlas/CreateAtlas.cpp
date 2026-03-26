#define _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct ImageData
{
    int width;
    int height;
    int channels;
    unsigned char* pixels;
};

bool CreateAtlas(const vector<string>& files, const string& output, int padding)
{
    vector<ImageData> images;

    int atlasWidth = 0;
    int atlasHeight = 0;

    // 이미지 로드
    for (auto& f : files)
    {
        ImageData img;

        img.pixels = stbi_load(f.c_str(), &img.width, &img.height, &img.channels, 4);

        if (!img.pixels)
        {
            printf("Failed to load image: %s\n", f.c_str());
            return false;
        }

        if (img.pixels)
        {
            printf("Succes to load image!\n");
        }

        atlasWidth += img.width;
        atlasHeight = max(atlasHeight, img.height);

        images.push_back(img);
    }

    atlasWidth += padding * (images.size() - 1);

    vector<unsigned char> atlas(atlasWidth * atlasHeight * 4);

    int offsetX = 0;

    // 이미지 복사
    for (auto& img : images)
    {
        for (int y = 0; y < img.height; y++)
        {
            for (int x = 0; x < img.width; x++)
            {
                int atlasIndex = ((y * atlasWidth) + (x + offsetX)) * 4;
                int imgIndex = ((y * img.width) + x) * 4;

                atlas[atlasIndex + 0] = img.pixels[imgIndex + 0];
                atlas[atlasIndex + 1] = img.pixels[imgIndex + 1];
                atlas[atlasIndex + 2] = img.pixels[imgIndex + 2];
                atlas[atlasIndex + 3] = img.pixels[imgIndex + 3];
            }
        }

        offsetX += img.width + padding;
    }

    stbi_write_png(output.c_str(), atlasWidth, atlasHeight, 4,
        atlas.data(), atlasWidth * 4);

    for (auto& img : images)
        stbi_image_free(img.pixels);


    return true;
}