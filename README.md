# sunny
A screenshot tool, and to achieve the function of the general module encapsulation











```bash
#【方式一】 在 Ubuntu 22.04 打包为 .deb ----------------------------------------------
nano ~/.bashrc
# 补充 git tab 补全
if [ -f /etc/bash_completion ]; then
  . /etc/bash_completion
fi
# 全局生效 qt 的依赖
export Qt5_DIR=/opt/Qt/5.15.2/gcc_64/lib/cmake/Qt5
export QT_DIR=/opt/Qt/5.15.2/gcc_64/lib/cmake/Qt5

source ~/.bashrc

// 然后执行后，就打包成了 .deb 软件包
mkdir bin & cd bin
cmake ../
make -j16
cpack -G DEB

# 注意大小写等
dpkg -I ./Sunny-1.5.0-Linux-x86_64.deb            # 查看软件包的信息
dpkg -L sunny                                     # 查看包的文件等拷贝等信息
ldd /usr/bin/Sunny                                # 查看依赖的 dll

# 临时生效
sudo apt install libqt5x11extras5
export LD_LIBRARY_PATH=/opt/Qt/5.15.2/gcc_64/lib:$LD_LIBRARY_PATH

#【方式2】使用 ldd.sh + Sunny.sh 方式打包，在 Ubuntu 22.04 打包 -----------------------------
export QT_DEBUG_PLUGINS=1         # 终端输入，可以打印详细信息

# 使用 sudo 打开 ~/.bashrc 并在末尾插入以下内容
export QT_BIN_PATH=/opt/Qt/5.15.2/gcc_64/bin
export QT_LIB_PATH=/opt/Qt/5.15.2/gcc_64/lib
export QT_PLUGINS_PATH=/opt/Qt/5.15.2/gcc_64/plugins
export PATH=$QT_BIN_PATH:$QT_LIB_PATH:$QT_PLUGINS_PATH:$PATH

# 使配置生效并验证可用性，如果输入的 $PATH 有你刚刚的路径则成功
source ~/.bashrc
echo $PATH

#【方式3】使用 linuxdeployqt 方式打包，在 Ubuntu 22.04 打包，不可以使用 -----------------------------
####linuxdeployqt-continuous-x86_64.AppImage 方案可在 Ubuntu 22.04 上面不可行####
$ ../linuxdeployqt-continuous-x86_64.AppImage Sunny -appimage
$ sudo apt install  libfuse2

但是由于过于作者的固执坚守旧的版本，所以无法使用，理由和可能的解决如下：
https://github.com/probonopd/linuxdeployqt/issues/340#issuecomment-932712016
即：使用linuxdeploy和linuxdeploy-plugin-qt

#####linuxdeploy-x86_64.AppImage + linuxdeploy-plugin-qt-x86_64.AppImage 下面方案可行#####
https://github.com/BearKidsTeam/thplayer/blob/master/.github/workflows/linux.yml#L54

$ sudo apt update
$ sudo apt install qtbase5-dev qtmultimedia5-dev libqt5multimedia5-plugins
$ sudo add-apt-repository universe
$ sudo apt install libfuse2

$ wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
$ wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
$ chmod +x linuxdeploy-*.AppImage

$ mkdir build && cd build
$ cmake ..
$ cmake --build . -j$(nproc)
$ cd ..

$ ../linuxdeploy-x86_64.AppImage --appdir AppDir -e bin/Sunny -d bin/resources/linux/debian/tech.xmuli.sunny.desktop -i bin/resources/logo/logo.svg --icon-filename Sunny -p qt -o appimage


$  ./linuxdeploy-x86_64.AppImage --appdir AppDir -e build/thplayer -d assets/thplayer.desktop -i assets/thplayer.svg --icon-filename thplayer -p qt -o appimage
$ mv TouHou_Player*.AppImage thplayer-linux.AppImage
```

