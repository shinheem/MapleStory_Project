#include <vector>
#include <string>

bool CreateAtlas(const std::vector<std::string>& files,
    const std::string& output,
    int padding);

using namespace std;

int main()
{
    string FilePath = "D:\\HM\\Dx\\CreateAtlas\\CreateAtlas\\x64\\Debug\\image\\";
    string ForderName= "Right_Head_attack1\\";
    string PNGName = "Right_Head_attack1_";
    
    string AtlasForderName = "HonTail_attack1\\Right_Head_attack1_Atlas\\";
    string AtlasName = "Right_Head_attack1_Atlas";
    int PngCount = 24;

    //// 반복재생 형식으로 제작할 때
    //vector<string> images = {
    //FilePath + ForderName + PNGName + to_string(0) + ".png",
    //FilePath + ForderName + PNGName + to_string(1) + ".png",
    //FilePath + ForderName + PNGName + to_string(2) + ".png",
    //FilePath + ForderName + PNGName + to_string(1) + ".png"
    //};

    // 순차적으로 생성할 때
    vector<string> images;

    for (int i = 0; i < PngCount; i++)
        images.push_back(FilePath + ForderName + PNGName + to_string(i) + ".png");


    CreateAtlas(images, "D:\\HM\\Dx\\CreateAtlas\\ResultAtlas\\"+ AtlasForderName + AtlasName + ".png", 0);

    return 0;
}