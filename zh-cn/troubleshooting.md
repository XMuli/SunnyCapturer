

以下为运行 SunnyCapturer 可能遇到的错误及其解决方案。



## Windows

**运行后遇到提示计算机中丢失 api-ms-win-crt-runtime-l1-1-0.dll 错误**

- 您的操作系统缺乏基础的微软 VC++ 基础运行库，请根据你的操作系统版本（32位/64位），下载和安装对应版本 `微软 Visual C++ 2015-2022 可再发行组件包`： [32位](https://aka.ms/vs/17/release/vc_redist.x86.exe) | [64位](https://aka.ms/vs/17/release/vc_redist.x64.exe) 

  

**Windows SmartScreen 筛选器已阻止启动一个未识别的应用**

- 请点击“更多信息”，然后点击“仍要运行”
- 这只是因为 SunnyCapturer 未使用企业证书进行数字签名，只要是从 [官网下载](https://sunnycapturer.xmuli.tech) 的，可以放心使用；但是使用了个人代码签名证书签名，当在 Microsoft Windows 上积累了足够多的用户量，此版本便不会再弹出



## Linux

?> 对于 Debian 系的发行版，提供原生的 `.deb` 离线包；除此之外的 Linux 发行版可使用 `.AppImage` ；Linux 系统仅支持 `x11` 架构，暂不支持 `Wayland`



> Debian 系列发行版，推荐用 `.deb` 包

**Ubuntu 24.04 截图后，显示为白屏**

- Ubuntu 24.04 默认是 `Wayland`，因 SunnyCapturer 仅支持 `x11`，故将 Ubuntu 设置为 `x11` 模式即正常。操作步骤如下： [#55](https://github.com/XMuli/SunnyCapturer/issues/55)

  ```bash
  sudo vim /etc/gdm3/custom.conf
  # 将 WaylandEnable=false 的这行注释解开
  sudo systemctl restart gdm3  # 重启系统
  ```



**Ubuntu20.04 运行成功且截图，但使用 OCR 和 图片翻译等网络服务无响应** 

- 因 Ubuntu20.04 自带的 SSL 版本过低，会提示如下：

  ```
  qt.tlsbackend.ossl: Incompatible version of OpenSSL (built with OpenSSL >= 3.x, runtime version is < 3.x)
  qt.network.ssl: Active TLS backend does not support key creation
  qt.network.ssl: The backend "cert-only" does not support QSslSocket
  qt.network.ssl: The backend named "cert-only" does not support TLS
  ```

  解决方法升级系统 SSL 到 3.0+ 版本即可；或升级到更高版本的 Ubuntu 22.04 / 24.04+



> 非 Debian 系列的发行版，推荐用 `.AppImage` 包

**`.Appimage` 格式包修改配置后重启软件不会被重置**

- `.Appimage`  格式自身的的缺陷，实际不能修改 AppDir 之内对应的路径的配置文件，故暂时无解；但不影响功能使用



## macOS

?> 1.x-2.x 仅支持 Intel 的芯片，但 3.x 支持 Apple M 芯和 Intel 芯



**dmg 文件无法安装**

- 请尝试以右键点击运行
- 点击 dmg 后，将里面的 .app 拖曳到 `访达`-`应用程序` 中



**截图时只显示了桌面**

- macOS 10.14 之后的系统新增了 `屏幕录制` 权限，请到 `系统偏好设置` - `安全性与隐私` - `隐私` - `屏幕录制` 中添加 SunnyCapturer
- 如 SunnyCapturer 已经勾选但仍有问题，请先取消勾选 SunnyCapturer，然后退出 SunnyCapturer，勾选权限后再重启 SunnyCapturer
