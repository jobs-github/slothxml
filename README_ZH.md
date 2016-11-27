[English](README.md)

# slothxml - 为懒人打造的xml对象序列化神器 #
![slothxml logo](res/logo.png)

`slothxml` 是 [slothjson](https://github.com/jobs-github/slothjson) 的姊妹篇，基于 [`rapidxml`](https://sourceforge.net/projects/rapidxml) 之上打造的一款 **全自动化** 的对象序列化神器。  

以下是 `slothxml` 的设计图：
![design](res/design.png)

## slothxml是什么？ ##
参考 [slothjson](https://github.com/jobs-github/slothjson)

## slothxml有什么？ ##
参考 [slothjson](https://github.com/jobs-github/slothjson)  

## slothxml怎么用？ ##

首先，你需要将以下内容放到你的项目中：

* `rapidxml`: 对应 `include/rapidxml`，这个是世界上最快的 `xml` 解析库
* `slothxml`: 对应 `include/slothxml.h` 以及 `include/slothxml.cpp`，这两个是 `slothxml` 基础库文件

**以上就是全部的依赖** ，是不是很简单？ ^_^

接下来，你需要用几秒钟了解 `slothxml` 提供的最重要的接口：

	namespace slothxml
	{
        // 将C++对象编码成xml字符串
        template<typename T>
        bool encode(const T& obj_val, const char * root_name, std::string& xml_val);

        // 将xml字符串解码成C++对象
        template<typename T>
        bool decode(const std::string& xml_val, const char * root_name, T& obj_val);
	}

	namespace slothxml
	{
        // 将C++对象编码成json字符串，并保存到文件中
        template<typename T>
        bool dump(const T& obj_val, const char * root_name, const char * path);

        // 从文件中读取json字符串，并解码成C++对象
        template<typename T>
        bool load(const char * path, const char * root_name, T& obj_val);
	};

`root_name` 参数用于设置 `xml` 根节点的名字。

以上两组接口基本可以满足所有的需求。

**举例** ，编写 `schema` 文件 `fxxx_gfw.json` 如下：

	{
	    "structs": 
	    [
	        {
	            "type": "fxxx_gfw_t",
	            "members": 
	            [
                    ["bool", "bool_val", "true"],
                    ["int8_t", "int8_val", "'a'"],
	                ["int32_t", "int32_val", "-111111"],
	                ["uint64_t", "uint64_val", "0xffff"],
	                ["double", "double_val", "111111.111111"],
	                ["string", "str_val", "test"],
	                ["[int32_t]", "vec_val"],
	                ["{string}", "dict_val"]
	            ]
	        }
	    ]
	}

运行如下命令：

    python generator/slothxml.py -f src/fxxx_gfw.json

生成 `fxxx_gfw.h` 和 `fxxx_gfw.cpp` 之后，将它们添加到你的项目中。  
接下来你可以这样编写 `xml` 序列化的代码：

    slothxml::fxxx_gfw_t obj_val;
    // 设置obj_val的内容
    ......
    // 输出为字符串
	std::string xml_val;
	bool rc = slothxml::encode(obj_val, "object", xml_val);
    // 输出到文件
    std::string path = "fxxx_gfw_t.xml";
    bool rc = slothxml::dump(obj_val, "object", path);

如果不想序列化全部字段，可以这样编写：

    obj_val.skip_dict_val(); // 调用 skip_xxx 方法
反序列化的代码类似：

    // 从字符串读取
	std::string xml_val;
    // 设置xml_val的内容
    ......
    slothxml::fxxx_gfw_t obj_val;
	bool rc = slothxml::decode(xml_val, "object", obj_val);

    // 从文件读取
	std::string path = "fxxx_gfw_t.xml";
    slothxml::fxxx_gfw_t obj_val;
	bool rc = slothxml::load(path, "object", obj_val);

如果想判断指定的字段是否在 `xml` 中，可以这样编写：

    if (obj_val.xml_has_dict_val()) // 调用 xml_has_xxx 方法
    {
         ......
    }

以上就是 `slothxml` 的用法，很简单吧？  

## slothxml支持哪些平台？ ##

参考 [slothjson](https://github.com/jobs-github/slothjson)

## 进阶 ##

`slothxml` 和 `slothjson` 的 **设计基本一致，并且使用相同的数据描述文件** ，唯一不同的是，`slothjson` 对应的代码生成器为 `slothjson.py`，而 `slothxml` 对应的代码生成器为 `slothxml.py`。

具体细节可参考 [slothjson](https://github.com/jobs-github/slothjson)  的相关设计。

## License ##

`slothxml` is licensed under [New BSD License](https://opensource.org/licenses/BSD-3-Clause), a very flexible license to use.

## 作者 ##

* 程卓 (jobs, yao050421103@163.com)  

## 更多 ##

- 姊妹篇 - [slothjson](https://github.com/jobs-github/slothjson)
- 高性能分布式存储服务 - [huststore](https://github.com/Qihoo360/huststore)