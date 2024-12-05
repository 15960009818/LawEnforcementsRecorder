#include "jsonbuilder.h"
#include <sstream>
#include <iostream>

/**
 * @函数名: addString
 * @描述: 向 JSON 中添加一个字符串类型的键值对
 * @参数: key 键名
 * @参数: value 字符串类型的值
 * @返回: JSONBuilder& 允许链式调用
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
JSONBuilder& JSONBuilder::addString(const std::string& key, const std::string& value) {
    jsonPairs[key] = "\"" + value + "\"";
    std::cout << "Added string key-value pair: \"" << key << "\": \"" << value << "\"" << std::endl;
    return *this;
}

/**
 * @函数名: addInt
 * @描述: 向 JSON 中添加一个整数类型的键值对
 * @参数: key 键名
 * @参数: value 整数类型的值
 * @返回: JSONBuilder& 允许链式调用
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
JSONBuilder& JSONBuilder::addInt(const std::string& key, int value) {
    jsonPairs[key] = std::to_string(value);
    std::cout << "Added integer key-value pair: \"" << key << "\": " << value << std::endl;
    return *this;
}

/**
 * @函数名: addBool
 * @描述: 向 JSON 中添加一个布尔类型的键值对
 * @参数: key 键名
 * @参数: value 布尔类型的值
 * @返回: JSONBuilder& 允许链式调用
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
JSONBuilder& JSONBuilder::addBool(const std::string& key, bool value) {
    jsonPairs[key] = value ? "true" : "false";
    std::cout << "Added boolean key-value pair: \"" << key << "\": " << (value ? "true" : "false") << std::endl;
    return *this;
}

/**
 * @函数名: build
 * @描述: 构建并返回完整的 JSON 字符串
 * @返回: std::string 生成的 JSON 字符串
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
std::string JSONBuilder::build() const {
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& pair : jsonPairs) {
        if (!first) oss << ", ";
        oss << "\"" << pair.first << "\": " << pair.second;
        first = false;
    }
    oss << "}";
    std::string jsonString = oss.str();
    std::cout << "Built JSON string: " << jsonString << std::endl;
    return jsonString;
}
