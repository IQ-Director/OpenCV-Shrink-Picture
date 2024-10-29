#include <windows.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <stdexcept>
#include <random>
#include <string>

namespace fs = std::filesystem;

void resizeImage(const std::string& filePath) {
    // 读取图片
    cv::Mat image = cv::imread(filePath);
    if (image.empty()) {
        std::cerr << "无法读取图片: " << filePath << std::endl;
        return;
    }

    // 缩小图片至50%
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(), 0.5, 0.5);
    fs::path outputDir = fs::current_path() / "resize";
    // 添加 "resize_" 前缀并保存缩小后的图片到 resize 文件夹
    std::string outputFilePath = (outputDir / ("resize_" + fs::path(filePath).filename().string())).string();
    cv::imwrite(outputFilePath, resizedImage);
    std::cout << "已保存缩小后的图片: " << outputFilePath << std::endl;
}

std::string generateRandomNumberString(int length) {
    std::string randomNumber;
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 9); // 生成 0 到 9 的随机数

    for (int i = 0; i < length; ++i) {
        randomNumber += std::to_string(distribution(generator));
    }
    return randomNumber;
}

std::string renameImage(const std::string& originalPath) {
    fs::path originalFile(originalPath);
    std::string randomNum = generateRandomNumberString(30); 
    fs::path newFile = originalFile.parent_path() / (randomNum + originalFile.extension().string());
    fs::rename(originalFile, newFile);
    std::cout << "已重命名图片: " << newFile.string() << std::endl;
    return newFile.string();
}

void moveOriginalImage(const std::string& originalPath) {
    fs::path originalFile(originalPath);
    fs::path originalDir = originalFile.parent_path() / "original";

    fs::path newFilePath = originalDir / originalFile.filename();
    fs::rename(originalFile, newFilePath);
    std::cout << "已移动原图到: " << newFilePath.string() << std::endl;
}

int main() {

    // 创建 resize 文件夹（如果不存在）
    fs::path outputDir = fs::current_path() / "resize";
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }
    // 创建 original 文件夹（如果不存在）
    fs::path originalDir = fs::current_path() / "original";
    if (!fs::exists(originalDir)) {
        fs::create_directory(originalDir);
    }
    
    // 遍历当前路径下的所有文件
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // 检查文件扩展名是否为图片格式
            if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" || extension == ".bmp") {
                std::string new_name = renameImage(entry.path().string());
                resizeImage(new_name);
                moveOriginalImage(new_name);
            }
        }
    }

    // 等待用户输入以关闭控制台
    std::cout << "按任意键关闭控制台..." << std::endl;
    std::cin.get(); // 等待用户按下回车键

    return 0;
}
