#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>
#include <iostream>

class ResourceManager
{
public:
    // 싱글턴 인스턴스를 가져오는 함수
    static ResourceManager& getInstance();

private:
    // 생성자와 소멸자를 private으로 선언하여 외부 생성을 막음
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // 리소스를 저장할 맵
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Font> m_fonts;
};

