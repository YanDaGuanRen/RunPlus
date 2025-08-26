# RunPlus

RunPlus 是一个Windows应用程序，用于演示如何读取INI配置文件并在GUI中显示参数信息。

## 功能特性

1. 读取UTF-8编码的INI配置文件
2. 支持不区分大小写的section和key读取
3. 在GUI窗口中显示命令行参数和配置信息
4. 处理空值键的情况
5. 文件操作功能：
   - 获取目录下所有指定后缀的文件
   - 删除文件
   - 复制文件

## 配置文件格式

程序使用YanBinCfg.ini作为配置文件，支持以下section：
- Settings
- ExecCmd
- ExecProcess
- Config

## 编译和运行

使用Visual Studio打开RunPlus.sln解决方案文件，然后编译和运行项目。

## 日志功能

程序支持日志功能，但默认情况下不写入日志文件。要启用日志功能，需要在启动时添加`--debug`参数：

```
RunPlus.exe --debug
```

日志文件将被创建在程序执行目录下，文件名为`RunPlus.log`。

## 文件操作功能说明

新添加的文件操作功能包括：

1. `GetFilesByExtension(const std::wstring& directory, const std::wstring& extension)` - 获取指定目录下的所有指定后缀文件
2. `DeleteFile(const std::wstring& filePath)` - 删除指定文件
3. `CopyFile(const std::wstring& sourcePath, const std::wstring& destPath)` - 复制文件

这些功能在`file_manager.h`和`file_manager.cpp`文件中实现。

注意：为了安全起见，删除和复制文件功能在演示中未启用，实际使用时请确保提供正确的文件路径。