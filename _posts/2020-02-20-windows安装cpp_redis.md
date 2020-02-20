---
layout: post
title:  "cpp_redis安装手册"
date:   2020-03-20 20:00:51 +0800
categories: jekyll update
---


# 参考文章
* [安装文档](ttps://github.com/Cylix/cpp_redis/wiki/Installation)
 

在cmake中依赖第三方库的工程在构件windows项目中总会遇到问题



使用 git submodule 获取依赖库的方式

```
git submodule init && git submodule update
```



Windows Install
Simon Ninon edited this page on 22 Oct 2017 · 9 revisions
Requirement
C++11
Compiling and installing
Visual Studio Solution
This documentation aims to provide the building steps for a Windows environment containing the following tools:

Appropriate terminal/shell
Git (throw command-line)
VS 2012+ (for C++11 support)
The library provides a MSVC15 solution located in msvc15 that you can use if you want to quickly build the library without CMake.

In order to build the library, you have to open a terminal and follow these steps:

- Clone the project
```
git clone https://github.com/Cylix/cpp_redis.git
```
- Go inside the the project directory
```
cd cpp_redis
```
- Get tacopie submodule
```
git submodule init && git submodule update
```


Then, you can open the cpp_redis solution in the msvc15 directory of the repository and build the solution in the configuration (Debug, Release, ...) and for the platform (32, 64) of your choice.

Note that this solution is configured to target Windows 8.1 with the build tools v140.

If you need any other settings or if this configuration is not compatible with your environment, you will need to re-configure the solution:

right click on the cpp_redis solution in visual studio
then properties
finally update the corresponding target and build tools version (or any other relevant settings)
Otherwise, you will need to build cpp_redis using CMake.

Once built, you can update your VC++ project configuration:

VC++ Directories > Include Directories: Add CPP_REDIS_FOLDER_PATH/includes and CPP_REDIS_FOLDER_PATH/tacopie/includes (with the appropriate value of CPP_REDIS_FOLDER_PATH).
VC++ Directories > Library Directories: Add CPP_REDIS_FOLDER_PATH/{Debug|Release|...}/ (with the appropriate value of CPP_REDIS_FOLDER_PATH and configuration).
Linker > Input > Additional Dependencies: Add cpp_redis.lib and tacopie.lib.
Finally, you just have to include <cpp_redis/cpp_redis> in your source files.

CMake
This documentation aims to provide the building steps for a Windows environment containing the following tools:

Appropriate terminal/shell
Git (throw command-line)
CMake (throw command-line)
VS 2012+ (for C++11 support)
The library is based on cmake for the compilation. In order to build the library, you have to open a terminal and follow these steps:

- Clone the project
git clone https://github.com/Cylix/cpp_redis.git
- Go inside the the project directory
cd cpp_redis
- Get tacopie submodule
git submodule init && git submodule update
- Generate the VC++ solution using CMake
cmake .
- Generate the VC++ solution using CMake, for x64 platform
cmake . -G "Visual Studio 15 2017 Win64" # Or any other valid platform/compiler version.
This will generate a cpp_redis.sln file (that is, a Visual Studio solution).

Open this file with Visual Studio.
Build the solution in the configuration (Debug, Release, ...) and for the platform (32, 64) of your choice.
Then, you can update your VC++ project configuration:

VC++ Directories > Include Directories: Add CPP_REDIS_FOLDER_PATH/includes and CPP_REDIS_FOLDER_PATH/tacopie/includes (with the appropriate value of CPP_REDIS_FOLDER_PATH).
VC++ Directories > Library Directories: Add CPP_REDIS_FOLDER_PATH/{Debug|Release|...}/ (with the appropriate value of CPP_REDIS_FOLDER_PATH and configuration).
Linker > Input > Additional Dependencies: Add cpp_redis.lib and tacopie.lib.
Finally, you just have to include <cpp_redis/cpp_redis> in your source files.

Customizing compilation and installation
Please refer to this page of the wiki.

Troubleshooting
Runtime Library
If you have a warning like the following when trying to link the library with your project:

error LNK2038: mismatch detected for 'RuntimeLibrary': value 'MTd_StaticDebug' doesn't match value 'MDd_DynamicDebug'
This is because there is a conflict between the Runtime Library used to compile cpp_redis and the Runtime Library used by another library you are using.

By default, cpp_redis is compiled using /MT mode, but you can use the MSVC_RUNTIME_LIBRARY_CONFIG cmake variable to override this setting.

Please refer to this page of the wiki.

ws2_32
Sometimes, it appears that CMake does not get along very well with Visual Studio and fails to generate project files with linkage against ws2_32.

If you get errors such as undefined external symbol _impl_socket, this is probably the case.

If so, manually add it the link as follow: (tacopie)Project->Properties->Librarian->Additional Dependencies->(input)ws2_32.lib

Need more information? Contact me.

