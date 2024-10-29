#include <windows.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <stdexcept>
#include <random>
#include <string>

namespace fs = std::filesystem;

void resizeImage(const std::string& filePath) {
    // ��ȡͼƬ
    cv::Mat image = cv::imread(filePath);
    if (image.empty()) {
        std::cerr << "�޷���ȡͼƬ: " << filePath << std::endl;
        return;
    }

    // ��СͼƬ��50%
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(), 0.5, 0.5);
    fs::path outputDir = fs::current_path() / "resize";
    // ��� "resize_" ǰ׺��������С���ͼƬ�� resize �ļ���
    std::string outputFilePath = (outputDir / ("resize_" + fs::path(filePath).filename().string())).string();
    cv::imwrite(outputFilePath, resizedImage);
    std::cout << "�ѱ�����С���ͼƬ: " << outputFilePath << std::endl;
}

std::string generateRandomNumberString(int length) {
    std::string randomNumber;
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 9); // ���� 0 �� 9 �������

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
    std::cout << "��������ͼƬ: " << newFile.string() << std::endl;
    return newFile.string();
}

void moveOriginalImage(const std::string& originalPath) {
    fs::path originalFile(originalPath);
    fs::path originalDir = originalFile.parent_path() / "original";

    fs::path newFilePath = originalDir / originalFile.filename();
    fs::rename(originalFile, newFilePath);
    std::cout << "���ƶ�ԭͼ��: " << newFilePath.string() << std::endl;
}

int main() {

    // ���� resize �ļ��У���������ڣ�
    fs::path outputDir = fs::current_path() / "resize";
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }
    // ���� original �ļ��У���������ڣ�
    fs::path originalDir = fs::current_path() / "original";
    if (!fs::exists(originalDir)) {
        fs::create_directory(originalDir);
    }
    
    // ������ǰ·���µ������ļ�
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // ����ļ���չ���Ƿ�ΪͼƬ��ʽ
            if (extension == ".jpg" || extension == ".jpeg" || extension == ".png" || extension == ".bmp") {
                std::string new_name = renameImage(entry.path().string());
                resizeImage(new_name);
                moveOriginalImage(new_name);
            }
        }
    }

    // �ȴ��û������Թرտ���̨
    std::cout << "��������رտ���̨..." << std::endl;
    std::cin.get(); // �ȴ��û����»س���

    return 0;
}
