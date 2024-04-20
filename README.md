# sunny
A screenshot tool, and to achieve the function of the general module encapsulation



```bash
# ****************** Linux 安装 qt5.15.x 环境和编译打包 .deb ******************
sudo apt update
sudo apt install git cmake gcc g++ wget tar dh-make lintian devscripts dos2unix   
sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtcreator qttools5-dev libqt5svg5-dev   libqt5x11extras5-dev  # qt可以运行一个初始项目
sudo apt install debmake libqt5*-dev    # 还是失败就安装这个带 * 的；
# 使用 dch -i 1.5.0 或用 dch -v 1.5.0 来指定版本，使用中 debian 文件夹要在 Sunny 下，或使用参数跳过这一限制 


#【编译 .deb 步骤， linux 发行版基本都是通用的】 
# 然后执行后，就打包成了 .deb 软件包
$ dos2unix ../src/resources/cpack/tech.xmuli.sunny.desktop       // 将文件换行符修改为 Linux 格式; 
$ cat -A      src/resources/cpack/tech.xmuli.sunny.desktop       // `$` 表示 LF(LF-换行-Unix);  `^M$`表示 （CRLF-回车换行-Windows
mkdir bin & cd bin
cmake ../
make -j16
cpack -G DEB

$ sudo apt install ./Sunny-1.5.0.deb    # 安装
$ sudo apt remove sunny                 # 卸载

# 注意大小写等
dpkg -I ./Sunny-1.5.0.deb            # 查看软件包的信息
dpkg -L sunny                        # 查看包的文件等拷贝等信息
ldd /usr/bin/Sunny                   # 查看依赖的 dll

#【Ubuntu 22.04 LTS，仅支持 X11，使用 apt 安装 qt5.15.3 环境和编译】------------------------------
注：${CMAKE_INSTALL_LIBDIR} 是 /usr/lib 非 lib64，是可以被系统找到的； 然后直接 cmake 编译那一套流程就行

##【 linuxdeployqt 的方式构建 AppImage 安装，成功】
$ sudo apt install  libfuse2
$ wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
$ wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
$ chmod +x linuxdeploy*.AppImage

$ dos2unix src/resources/cpack/tech.xmuli.sunny.desktop
$ mkdir build && cd build      #【是 build 文件夹，而非 bin 文件夹】
$ cmake ..
$ cmake --build . -j$(nproc)   #【注意实心点】
$ cd ..

$ ../linuxdeploy-x86_64.AppImage --appdir AppDir -e bin/Sunny -d bin/resources/cpack/tech.xmuli.sunny.desktop -i bin/resources/logo/logo.svg --icon-filename tech.xmuli.sunny -p qt -o appimage         // Ubuntu 22.04 成功;  deepin v20.9和23 因 qmake 原因失败，尝试 sudo find / -name qmake 查找名字叫 qmake 的所有路径

#【deepin v20.9 使用 apt 安装 qt5.15.8 环境和编译】------------------------------
直接 cmake 编译那一套流程就行

#【deepin v23 beta3 使用 apt 安装 qt5.15.8 环境和编译】------------------------------
直接 cmake 编译那一套流程就行

#【openKylin 1.0.1/1.0.2 使用 apt 安装 qt5.15.8 环境和编译】------------------------------
[注：${CMAKE_INSTALL_LIBDIR} 是 /usr/lib64， 而非是 /usr/lib 这默认86的，所以会报错找不到依赖；
解决方法 Sunny\src\CMakeLists.txt 里面的三处给写死，避免找不到依赖]

#【MacOS Ventura 13.2.1】-------------------------------------------------------------
新的方法(2020-05-22)：
前提： 进入 /Users/winks/Qt/5.15.2/clang_64/bin 路径下
先使用 qtcreator 进行编译后，打包为 *.dmg 格式：
☁  bin [master] ⚡  /Users/winks/Qt/5.15.2/clang_64/bin/macdeployqt Sunny.app -dmg


#【主机开放 https 作为局域网临时下载和上传】-------------------------------------------------------------
❯ git clone --depth 1 https://github.com/JevenM/HTTP_SERVER.git        // 下载服务http共享脚本
❯ cd D:\net_public_share_folder\                                       // 进入要共享的文件夹
❯ python "D:\projects\HTTP_SERVER\HTTP_SERVER.py" 8888                 // 端口号可以随意数值，开启 hht 上传和下载服务，大概要十几秒才启动完成

其它电脑浏览器访问  http://192.168.31.48:8888 即可

```

