#ifndef JSONBUILDER_H
#define JSONBUILDER_H

#include <string>
#include <map>

/**
 * @类名: JSONBuilder
 * @描述: 用于构建 JSON 格式的字符串，通过添加键值对实现
 * @作者: 翔君
 * @创建日期: 2024/9/18
 */
class JSONBuilder {
public:
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
    JSONBuilder& addString(const std::string& key, const std::string& value);

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
    JSONBuilder& addInt(const std::string& key, int value);

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
    JSONBuilder& addBool(const std::string& key, bool value);

    /**
     * @函数名: build
     * @描述: 构建并返回完整的 JSON 字符串
     * @返回: std::string 生成的 JSON 字符串
     * @作者: 翔君
     * @创建日期: 2024/9/18
     * @是否完成: 完成
     */
    std::string build() const;

private:
    std::map<std::string, std::string> jsonPairs; // 存储键值对的映射
};

#endif // JSONBUILDER_H
