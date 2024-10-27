#include "JSONParser.h"
#include <stdexcept>
#include <iostream> // 添加这个头文件

/**
 * @函数名: parse
 * @描述: 解析给定的 JSON 字符串，并返回一个包含解析结果的对象映射
 * @参数: jsonString JSON 字符串
 * @返回: std::map<std::string, JSONValue> 解析后的对象映射
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
std::map<std::string, JSONParser::JSONValue> JSONParser::parse(const std::string& jsonString) {
    size_t pos = 0;
    std::cout << "Parsing JSON: " << jsonString << std::endl; // 日志输出
    return parseObject(jsonString, pos);
}

/**
 * @函数名: parseObject
 * @描述: 解析 JSON 对象，并返回一个包含键值对的映射
 * @参数: jsonString JSON 字符串
 * @参数: pos 解析的位置
 * @返回: std::map<std::string, JSONValue> 解析后的对象映射
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
std::map<std::string, JSONParser::JSONValue> JSONParser::parseObject(const std::string& jsonString, size_t& pos) {
    std::map<std::string, JSONValue> object;
    skipWhitespace(jsonString, pos);

    if (jsonString[pos] != '{') throw std::runtime_error("Expected '{'");
    pos++; // Skip '{'

    skipWhitespace(jsonString, pos);
    while (jsonString[pos] != '}') {
        std::cout << "Current position before key parsing: " << pos << std::endl; // 调试输出
        std::string key = parseString(jsonString, pos);
        std::cout << "Parsed key: " << key << std::endl;

        skipWhitespace(jsonString, pos);
        if (jsonString[pos] != ':') throw std::runtime_error("Expected ':'");
        pos++; // Skip ':'

        // 跳过空白字符
        skipWhitespace(jsonString, pos);

        std::cout << "Current position before value parsing: " << pos << std::endl; // 调试输出
        JSONValue value = parseString(jsonString, pos); // 仅支持字符串值
        object[key] = value;
        std::cout << "Parsed value for key '" << key << "': " << value << std::endl;

        skipWhitespace(jsonString, pos);
        if (jsonString[pos] == ',') {
            pos++; // Skip ','
            std::cout << "Current position after ',': " << pos << std::endl; // 调试输出
            skipWhitespace(jsonString, pos); // 再次跳过空白字符
        }
    }
    pos++; // Skip '}'

    return object;
}



/**
 * @函数名: skipWhitespace
 * @描述: 跳过 JSON 字符串中的空白字符
 * @参数: jsonString JSON 字符串
 * @参数: pos 当前解析的位置
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
void JSONParser::skipWhitespace(const std::string& jsonString, size_t& pos) {
    while (pos < jsonString.length() && (jsonString[pos] == ' ' || jsonString[pos] == '\n' || jsonString[pos] == '\t' || jsonString[pos] == '\r')) {
        pos++;
    }
}

/**
 * @函数名: parseString
 * @描述: 解析 JSON 字符串值
 * @参数: jsonString JSON 字符串
 * @参数: pos 当前解析的位置
 * @返回: std::string 解析出的字符串
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
std::string JSONParser::parseString(const std::string& jsonString, size_t& pos) {
    std::cout << "Current position at parseString: " << pos << " - Current char: " << jsonString[pos] << std::endl; // 调试输出
    if (jsonString[pos] != '"') throw std::runtime_error("Expected '\"'");
    pos++; // Skip '"'
    std::string result;
    while (pos < jsonString.length() && jsonString[pos] != '"') {
        result += jsonString[pos++];
    }
    if (jsonString[pos] != '"') throw std::runtime_error("Expected '\"'");
    pos++; // Skip '"'
    return result;
}


/**
 * @函数名: parseNumber
 * @描述: 解析 JSON 数字值
 * @参数: jsonString JSON 字符串
 * @参数: pos 当前解析的位置
 * @返回: int 解析出的整数值
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
int JSONParser::parseNumber(const std::string& jsonString, size_t& pos) {
    size_t start = pos;
    if (jsonString[pos] == '-') pos++; // Handle negative numbers
    while (pos < jsonString.length() && isdigit(jsonString[pos])) {
        pos++;
    }
    return std::stoi(jsonString.substr(start, pos - start));
}

/**
 * @函数名: getString
 * @描述: 从 JSON 对象中获取字符串值
 * @参数: jsonObj JSON 对象映射
 * @参数: key 键名
 * @返回: std::string 对应的字符串值
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
std::string JSONParser::getString(const std::map<std::string, JSONValue>& jsonObj, const std::string& key) {
    auto it = jsonObj.find(key);
    if (it != jsonObj.end()) {
        return it->second;
    }
    throw std::runtime_error("Key not found");
}

/**
 * @函数名: getInt
 * @描述: 从 JSON 对象中获取整数值
 * @参数: jsonObj JSON 对象映射
 * @参数: key 键名
 * @返回: int 对应的整数值
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
int JSONParser::getInt(const std::map<std::string, JSONValue>& jsonObj, const std::string& key) {
    return std::stoi(getString(jsonObj, key)); // 简单处理，假设值是整数
}

/**
 * @函数名: getBool
 * @描述: 从 JSON 对象中获取布尔值（未实现）
 * @参数: jsonObj JSON 对象映射
 * @参数: key 键名
 * @返回: bool 对应的布尔值
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 未完成
 */
bool JSONParser::getBool(const std::map<std::string, JSONValue>& jsonObj, const std::string& key) {
    // 需要进一步扩展以处理布尔值
    throw std::runtime_error("Boolean values are not supported");
}
