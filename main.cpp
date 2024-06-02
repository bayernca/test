#include <iostream>
#include <dirent.h>
#include <string>
#include <vector>
#include <algorithm>

class Findupdate {
private:
    std::string directoryPath;

    // 辅助函数，用于构建新的文件名
    std::string buildNewFileName(const std::string& oldFileName, int index) {
        size_t dotPosition = oldFileName.rfind(".");
        return oldFileName.substr(0, dotPosition) + "-0" + std::to_string(index) + oldFileName.substr(dotPosition);
    }

public:
    // 构造函数，初始化目录路径
    Findupdate(const std::string& path) : directoryPath(path) {}

    // 检查文件并重命名
    void renameFiles() {
        DIR* dir;
        struct dirent* entry;
        
        // 打开目录
        dir = opendir(directoryPath.c_str());
        if (!dir) {
            std::cerr << "Cannot open directory: " << directoryPath << std::endl;
            return;
        }

        // 存储.stp文件名及其原始索引
        std::vector<std::pair<std::string, int>> stpFiles;
        int index = 1; // 新文件名的起始序号

        // 读取目录中的文件
        while ((entry = readdir(dir)) != nullptr) {
            std::string fileName = entry->d_name;
            // 检查文件是否以.stp结尾
            if (fileName.length() > 4 && fileName.compare(fileName.length() - 4, 4, ".stp") == 0) {
                stpFiles.emplace_back(fileName, index++);
            }
        }
        closedir(dir);

        // 对文件进行排序（如果需要）
        std::sort(stpFiles.begin(), stpFiles.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second < b.second;
        });

        // 重命名文件
        for (const auto& file : stpFiles) {
            std::string oldFileName = directoryPath + "/" + file.first;
            std::string newFileName = directoryPath + "/" + buildNewFileName(file.first, file.second);
            // 重命名文件
            if (rename(oldFileName.c_str(), newFileName.c_str()) != 0) {
                std::cerr << "Error renaming file: " << oldFileName << " to " << newFileName << std::endl;
            }
        }
    }
};

int main() {
    // 替换为你的目录路径
    std::string directoryPath = "C:\\test"; // 注意Windows路径分隔符
    Findupdate finder(directoryPath);
    finder.renameFiles();

    return 0;
}