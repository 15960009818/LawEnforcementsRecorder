#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <map>
#include <vector>

/**
 * @类名: JSONParser
 * @描述: 负责解析 JSON 格式的字符串并提供对解析结果的访问方法。
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
class JSONParser {
public:
    /**
     * @类型: JSONValue
     * @描述: 用于表示 JSON 值，当前实现为字符串类型。
     */
    using JSONValue = std::string;

    /**
     * @函数名: parse
     * @描述: 解析给定的 JSON 字符串并返回一个 JSON 对象。
     * @参数: jsonString - 需要解析的 JSON 格式字符串。
     * @返回: 返回一个包含解析结果的 std::map，键为字符串，值为 JSONValue。
     */
    static std::map<std::string, JSONValue> parse(const std::string& jsonString);

    /**
     * @函数名: getString
     * @描述: 从 JSON 对象中获取指定键的字符串值。
     * @参数: jsonObj - JSON 对象，key - 需要获取的键。
     * @返回: 返回对应键的字符串值。
     * @异常: 如果键不存在，将抛出异常。
     */
    static std::string getString(const std::map<std::string, JSONValue>& jsonObj, const std::string& key);

    /**
     * @函数名: getInt
     * @描述: 从 JSON 对象中获取指定键的整数值。
     * @参数: jsonObj - JSON 对象，key - 需要获取的键。
     * @返回: 返回对应键的整数值。
     * @异常: 如果键不存在，将抛出异常。
     */
    static int getInt(const std::map<std::string, JSONValue>& jsonObj, const std::string& key);

    /**
     * @函数名: getBool
     * @描述: 从 JSON 对象中获取指定键的布尔值。
     * @参数: jsonObj - JSON 对象，key - 需要获取的键。
     * @返回: 返回对应键的布尔值。
     * @异常: 如果键不存在，将抛出异常。
     */
    static bool getBool(const std::map<std::string, JSONValue>& jsonObj, const std::string& key);

private:
    /**
     * @函数名: parseObject
     * @描述: 解析 JSON 对象并返回。
     * @参数: jsonString - JSON 格式字符串，pos - 当前解析的位置。
     * @返回: 返回解析后的 JSON 对象。
     */
    static std::map<std::string, JSONValue> parseObject(const std::string& jsonString, size_t& pos);

    /**
     * @函数名: skipWhitespace
     * @描述: 跳过字符串中的空白字符。
     * @参数: jsonString - JSON 格式字符串，pos - 当前解析的位置。
     */
    static void skipWhitespace(const std::string& jsonString, size_t& pos);

    /**
     * @函数名: parseString
     * @描述: 解析 JSON 字符串值。
     * @参数: jsonString - JSON 格式字符串，pos - 当前解析的位置。
     * @返回: 返回解析后的字符串值。
     * @异常: 如果未找到结束引号，将抛出异常。
     */
    static std::string parseString(const std::string& jsonString, size_t& pos);

    /**
     * @函数名: parseNumber
     * @描述: 解析 JSON 数字值。
     * @参数: jsonString - JSON 格式字符串，pos - 当前解析的位置。
     * @返回: 返回解析后的整数值。
     */
    static int parseNumber(const std::string& jsonString, size_t& pos);
};

#endif // JSONPARSER_H
