# 如何开发一款截图软件呢？

在写和发布的后的期间，也遇到很多私聊请教 、 邮件沟通某个功能实现？反馈 Bug 和给出使用心得和建议；都给答疑了，但想来可写为一整篇，中间遇到的困难点都写出来，公开出来提供后来者参考。



## 开源代码

|                          Name                           |    License    |                           Describe                           |
| :-----------------------------------------------------: | :-----------: | :----------------------------------------------------------: |
|  [FLIPPED](https://github.com/SunnyScreenshot/FLIPPED)  |      MIT      | 简洁且漂亮的截图的软件工具，支持 Windows，MacOS，Linux 平台，亦一个极佳练手的截图完整项目；适合已有经验的工程师 |
|    [ShotX](https://github.com/SunnyScreenshot/ShotX)    |      MIT      |            一款极简的跨平台截图工具，适合新手学习            |
| [Sunny Screenshot](https://github.com/SunnyScreenshot/) | Organizations |                              -                               |

<br>

## 编译环境

　　💻 `MacOS 13 ` 📎 `Qt 5.15.x` 📎 `gcc/g++ 9.2` 📎 `gdb8.3` 

　　💻 `Ubuntu 22.04` 📎`Deepin 20-23+` 📎 `Qt 5.15.x` 📎 `gcc/g++ 9.0`  📎 `gdb8.0`

　　💻  `Win10 22H2` 📎 `Qt 5.15.x` 📎  `Visual Studio 2022` 📎 `C++17`

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202405112034475.png" width="100%"/>

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2023/202402281723559.png" width="80%"/>

<br>

## 整体思路

- **基础窗口：** 创建一个 QWidget 窗口，去掉标题栏后，全屏且置顶，捕获此刻多屏幕状态保存为 QPixamp，然后绘画在 QWidget 最底层，再绘画一层透明黑色作为遮罩
- **绘画工具栏：** 作为是一个单独的子窗口，包含两个一级和二级的绘画工具栏，控制二级的显隐
- **鼠标光标：** 将 QWidget 放于虚拟桌面的左上角；相对坐标和绝对坐标的转换
- **功能思路：** 时刻判断当前所处模式：Wait / Select / Move / Draw / Stretch 标记；根据模式标记，对鼠标的 Press / Move / Release 事件进行对应的操作；重点是鼠标放下和松开时的 QPoint
  - 捕获模式：智能窗口 / 全屏截图 / 延时截图 / 自定义截图 等
  - 绘画模式则细分：一级绘画栏和二级绘画栏（愈加精确的参数）
  - 拉伸可为：拉伸已绘图形 / 选中框 / … ，操作是可见区域的任意一个图案
  - 移动同上
- **钉图功能：** 独立的窗口，将图片绘画在最底层，且需要重绘缩小一圈，为毛玻璃的彩虹灯预留位置
- **杂项但重要：** 国际化，不重启切换语言字体，编译，打包，CI /CD，热键，窗口尺寸遍历，显示窗口详细信息及大小，代码签名证书，上架应用商店；太多了，单独成篇写在下面

## 如何购买代码签名

- 『问题』写的 EXE 如何进行代码签名？如何购买代码签名，怎么买最便宜？EV / Standard / Open Source Signing Certificates 的区别是什么？

  详细解答这些问题 《[分享如何拥有一份私人的『开源代码签名证书』](https://xmuli.blog.csdn.net/article/details/135487951)》，并且指导最后如何签名。

## 如何上架应用商城

- 『问题』如何上架到微软的 Microsoft Store？如何上架 Linux 的深度/统信/麒麟商城，以及如三方的星火商店呢？

  篇幅太长，单写了一篇，包含详细上架 Windows Store， Deepin/UOS Store, 三方 星火商店等。

  👉 《[Sunny截图上架Microsoft Store及Linux商店流程的指北](https://blog.csdn.net/qq_33154343/article/details/136334975)》

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202402192349047.jpg" width="150"/>

  **Note:** 个人作品上架微软商店的流程很折磨，最后上架成功后也是拨开云雾；
  <font color=#D0087E size=4 face="STFangsong">若是文章对你有价值，下载对应 Windows 安装版本，亦可帮我积累 Sunny 截图的微软信誉，或者在Linux等商店的好评，甚至感谢🙇‍</font> 


## 打包发布于 Windows / MacOS / Linux

::: info
① Windows：绿色便携版和安装包 `EXE`
:::

- 『问题』Windows 如何构建打包为 .exe 文件？如何生成构绿色版和安装版？

  《[QT 项目在 Windows 平台上面发布成可执行程序](https://blog.csdn.net/qq_33154343/article/details/96448388)》


::: info
②MacOS：`.APP` 和 `.IMG`
:::

- 『问题』MacOS 如何构建打包为 .dmg 文件？

  《[QT 项目在 MacOS 平台上面发布成可执行程序](https://xmuli.blog.csdn.net/article/details/96448938#comments)》


::: info
③Linux: `.DEB`， `.APPIMAGE` 和绿色版
:::

- 『问题』Linux 如何构建打包为 .deb 文件、如何打包为通用的 .AppImage 格式？

  - Linux下又多种打包 `.deb` 打包方法：

    - 〖方法一〗通过 ldd.sh + Sunny.sh 两个脚本打包依赖，参考《[QT 项目在 Linux 平台上面发布成可执行程序](https://blog.csdn.net/qq_33154343/article/details/96448621)》

    - 〖方法二〗通过 `dh_make` + `dpkg-buildpackage` 命令《[Linux 中用 dh_make 将 Qt + CMake 项目打包为 deb 文件](https://blog.csdn.net/qq_33154343/article/details/123778207)》

    - 〖方法三，最推荐〗通过 CMake 的 `cpack` 命令，那样就不需要填写 debian 文件夹下的 control 等文件，直接拷贝相关资源文件过去。 CMakeLists.txt 底部加上 CPack 的相关代码，核心如下：

      ```cmake
      # CPACK: General Settings
      set (CPACK_GENERATOR "TBZ2")
      set (CPACK_PACKAGE_NAME "${project_name}")
      set (CPACK_PACKAGE_VERSION "${project_version}")
      set (CPACK_PACKAGE_VENDOR "https://github.com/XMuli")
      set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple and beautiful screenshot software tool for Windows, MacOS and Linux")
      set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
      set (CPACK_PACKAGE_CONTACT "https://sunny.xmuli.tech")
      # 设置Debian软件包的依赖关系
      set (CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5x11extras5, libqt5svg5")
      set (CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
      set (CPACK_DEBIAN_PACKAGE_SHILIBDEPS ON)
      
      include(CPack)
      ```

  - Linux下有多种打包  `.AppImage`  打包方法

    ```bash
    #【方式四】使用 linuxdeployqt 方式打包，在 Ubuntu 22.04 打包，不可以使用 -----------------------------
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
    $ chmod +x linuxdeploy*.AppImage
    
    $ mkdir build && cd build
    $ cmake ..
    $ cmake --build . -j$(nproc)
    $ cd ..
    
    $ ../linuxdeploy-x86_64.AppImage --appdir AppDir -e bin/Sunny -d bin/resources/cpack/tech.xmuli.sunny.desktop -i bin/resources/logo/logo.svg --icon-filename tech.xmuli.sunny -p qt -o appimage
    
    $  ./linuxdeploy-x86_64.AppImage --appdir AppDir -e build/thplayer -d assets/thplayer.desktop -i assets/thplayer.svg --icon-filename thplayer -p qt -o appimage
    $ mv TouHou_Player*.AppImage thplayer-linux.AppImage
    ```

- 『问题』如何书写 .yml 的脚本，通过 GitHub 的Action 资源，自动打包构建生成 Release 呢？

  通过写三个系统的 .yml 脚本，路径必须是 `.github/workflows` ，随着时间的流逝⌛，想要持续构建对应的云系统和 Kit 也必须更新，文档和版本参见 [images](https://github.com/actions/runner-images/tree/main/images) ，直接往 .yml  修改；这是一个实际可跑的脚本 [*.yml](https://github.com/XMuli/ChineseChess/tree/master/.github/workflows)  都是可以编译成功的，失败可能是额度时间不够了，如某次成功的示例，可看到头像是 GitHub 的头像发布的 [Release-v6.1](https://github.com/XMuli/ChineseChess/releases/tag/v6.1) ；


## UI / UX 设计的效果

- 『问题』截图的一级二级的菜单工具栏，如何实现 Windows7 的透明磨砂 /  Windows 的亚克力的效果，且能够支持 Windows / Linux / MacOS？

  单纯实现亚克力效果不难，难在Linux和 MacOS 上也能实现这个效果？这是当时的一些探索和经验，总结了四种方法放置于 [GitHub - AcrylicWindow](https://github.com/XMuli/AcrylicWindow)

- 『问题』如何实现一个完美的无边框窗口跨平台，且还要占用低，没有瑕疵BUG，还能白嫖？

  也折腾过，难度也很大，后来发现对于截图，费力可以实现，但是没必要，成本太大；结论：这样现成的没有，目前效果和跨平台都最佳的方案是[framelesshelper](https://github.com/wangwenx190/framelesshelper)， 有时放弃也是一种解决方案。

- 『问题』如何实现国际化多语言的切换？尤其是未使用 Qt Designer 来创建 .ui 文件，遇到无 `ui->retranslateUi(this)` 函数？不重启软件便可以切换语言

  对于有有 .ui 的部分，可以通过 《[Qt 项目(CMake)设置国际化支持](https://xmuli.blog.csdn.net/article/details/114439385)》来解决。对于存手写的控件实现的，且大致实现的思路是：

  下拉框中切换语言时，发射信号 → 全局单例 → 信号和槽函 → 到主窗口的槽函数，在里面进行重新加载语言，所有相关的控件的默认文本，都写在这个函数里面，便可以不重启软件，直接实现语言切换成功

## 用户体验细节

- 『问题』是否需要管理员权限才能运行？

  全程不会弹 UAC 弹窗，不需要管理员权限就可以使用所有功能，也不会中途提权，仅普通用户权限即可，包括导向安装，静默安装，使用卸载；

- 『问题』Windows 和 Linux 支持一次截多个屏幕，MacOS 仅只能截图单个屏幕，如何实现呢？

  MacOS 除了系统自带的截图支持外，至今没有任何一个三方软件可以做到这点，包括大厂等某企鹅的截图的，无解。根原是属于此苹果接口没公开，至少没人能够发现。

- 『问题』如何确保 MacOS 上的效果和 Window上面保持外观的一致？

  一个难点是再 MacOS 上也要和再 Windows 的效果保持一致，于是对不同风格进行对比，但 Fusion 又会倒是 Setting 窗口非原生的样式，但好在十分接近；选取一个平衡点。另外还手绘画了一个二十多个自定义或者复杂控件。

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2023/202402291521977.gif" width="70%"/>

## 看不见的优化

- 『问题』如何解决使用 ESC 取消截图后的内存泄露问题？QPointer 、智能指针、还是单例？

  也花了大力气来探究，在完成一次截图之后，内存的占用会在合适时机自动释放出来；这是定位在消耗内存的变量；开发环境显示器为 4K 27 寸 + 3K笔记本双屏；

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2023/202402261748498.png" width="100%"/>

- 『问题』对于使用单例模式不止一处时，有序需要多个单例用来传递或者保存数值时候，重复写很容易

  可采用[奇异递归模板](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)的方式，然后添加一个宏展开为友元类；多个单例都只用写一份，而前提是需要对 《[C++ 类的六个特殊成员函数](https://blog.csdn.net/qq_33154343/article/details/128367884)》 很熟悉，才能理解，属于优雅的一种实现。

- 『问题』日志和崩溃生成 DUMP 记录？

  日志可以通过配置文件修改，若是遇到传说的崩溃，亦会自动生成 .dmp 和 崩溃原因；

  转储文件存放： `C:/Users/用户名/AppData/Local/XMuli/Sunny/cache/Sunny_Dumps/dump_2024_02_29_11_31_30_714.dmp` 

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2023/202402291131796.png" width="60%"/>

  实现方法可通过 WIN API 来实现

  ```cpp
  #ifdef _MSC_VER
  SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注冊异常捕获函数
  #endif
  ```

  额，你没接触过 DUMP，完全不会对其进行解剖分析？也简单写了一个使用 WinDbg 进行入门

  《[WinDbg：入门分析 dmp 文件『一』](https://xmuli.blog.csdn.net/article/details/123563647)》、《[WinDbg：调试之附加进程生成 dmp『二』](https://xmuli.blog.csdn.net/article/details/123563753)》

- 『问题』希望单例运行EXE，确保全局唯一性？

  可以通过共享内存QSharedMemory 和系统信号量 QSystemSemaphore，双重保证程序在一台终端上，仅会运行一个；

  也能杜绝很罕见的一种情况，即使上次程序崩溃之后，但仍有残留的僵死进程，被误判当前没有启动。严谨（中指推一下眼镜）

  ```cpp
  QString uniqueKey = "SunnyUniqueKey"; // 使用唯一的标识符来创建共享内存和系统信号量
  QSharedMemory sharedMemory;
  sharedMemory.setKey(uniqueKey);
  
  // 尝试附加到现有的共享内存并分离
  if (sharedMemory.attach()) {
      sharedMemory.detach();
  }
  
  // 尝试创建共享内存，如果已经存在，表示已经有一个实例在运行, 判断是为了确保在同一台计算机上只能运行一个相同实例的程序。
  if (!sharedMemory.create(1)) {
      qDebug() << "There is already an instance of the application running (by QSharedMemory)!";
      return 1;
  }
  
  // 创建系统信号量, 再尝试获取系统信号量，如果已经被其他实例持有，程序就退出, 判断是为了确保在多个进程同时启动时，只有一个进程能够继续执行。QSystemSemaphore用于创建系统信号量，如果系统信号量已经被其他实例持有（比如由于上一次程序异常退出导致信号量未被释放），则acquire函数会返回false，
  QSystemSemaphore systemSemaphore(uniqueKey, 1, QSystemSemaphore::Open);
  if (!systemSemaphore.acquire()) {
      qDebug() << "There is already an instance of the application running (by QSystemSemaphore)!";
      return 1;
  }
  
  // ...程序其它逻辑
  
  // 释放系统信号量
  systemSemaphore.release();
  ```

## Qt / C++ 编码问题

- 『问题』截图项目运行直接置顶显示后，按下按键后，窗口无任何响应，需额外点击一下才能开始截图？

  分析和解决方案《[Qt新弹窗不响应键盘按键，难道也是无焦点？](https://blog.csdn.net/qq_33154343/article/details/124639169)》

- 『问题』热键输入框控件，输入后显示的方块 ■◆ 乱码？

  分析和解决方案《[创建 QKeySequenceEdit() 后，显示方块■◆乱码](https://blog.csdn.net/qq_33154343/article/details/125775732)》，还是多看下 Qt Assistant 解围粗心。

- 『问题』使用 VS2022 和 QtCreator 如何调试 Qt 5.15 的源代码？

  解决方案为《[VS2022 And QtCreator10 调试 Qt 源码教程](https://blog.csdn.net/qq_33154343/article/details/131491715)》、《[VS2017调试Qt源码](https://blog.csdn.net/qq_33154343/article/details/120339797)》

- 『问题』截图会有很多个属性的校验和“且”的属性使用？

  虽简单，但移除标志位容易忘却，这里简单列举一下《[C++ 标志位使用：校验、添加、删除](https://blog.csdn.net/qq_33154343/article/details/126322317)》

- 『问题』Qt Creator 报警告⚠ Misleading indentation； statement is not part of the previous ‘if‘

  那日强迫症，且需要删除空格才能解决，属实比较稀少；记录下解决方案 《[Misleading indentation； statement is not part of the previous ‘if‘](https://blog.csdn.net/qq_33154343/article/details/126215870)》

- 『问题』重构此版时候，也会大量使用 Lambda 表达式

  展开讲解一下，若未接触过，属会用会看即可《[Lambda 表达式详解](https://blog.csdn.net/qq_33154343/article/details/125775706)》

- 『问题』Visual Studio 断点偶遇进不去相关函数？

  Release实际也是可以调试的，新手容易不知晓，知晓的亦可能会翻车，值得记录下；属于 Release / Release with Debug Info / Debug 的差异《[Visual Studio 断点调试之箭头偏移进错函数，捉虫记](https://blog.csdn.net/qq_33154343/article/details/124482152)》

- 『问题』源码不能外发的情况下，如何进行调试？ 

  详细示例《[Visual Studio 2019 进行远程调试](https://blog.csdn.net/qq_33154343/article/details/123855765)》 


## 项目杂项

- 『问题』 Sunny 截图在在不同系统、编译器上开发、使用不同的 Kit Tools 上面如何解决乱码问题？且有时调试窗口乱码？还有 ANSI，UTF8，UTF8-BOM 采用哪种？

  参考《 [愿编程不再乱码(含Qt)-根因深究](https://blog.csdn.net/qq_33154343/article/details/120661967)》，以及 [QtExamples](https://github.com/XMuli/QtExamples) 的 "「第 6 章」 QT / IDE 乱码根因和解决"

- 『问题』 遇到需要使用的开源三方库，如何优雅的使用 Git 管理，确保拉取三方库即最新，又不会打乱本仓库的历史线？

  开源三方库引入，想要优雅，基本 `git submodule`  或  `git treemodule`  命令之间二选一；推荐前者，理由为 《[git submodule 基本用法](https://blog.csdn.net/qq_33154343/article/details/123453541)》

- 『问题』 如何选用 LOG 日志库？自己简单封装一套，还是选用开源库？便于后面定位和分析

  比较犹豫和纠结的一个问题，两种都试过；现总结为：项目初期使用自带的 QDebug 即，不够用再写一个类，和宏封装一套，满足需求即可，勿跑偏，功能才是重点。最后期可以选用三方库引入：

  一点经验参考 《[Log：日志选型调研『一』](https://blog.csdn.net/qq_33154343/article/details/123457938)》、《[Log：日志之 Spdlog 极简用法示范『二』](https://xmuli.blog.csdn.net/article/details/123488323)》、《[Log：日志之 Spdlog 核心构成『三』](https://xmuli.blog.csdn.net/article/details/123489094)》

- 『问题』CMake 管理跨平台项目，生各平台的 IDE 的解决方案？以及 Window 上自带一些基础宏的数据类型含义？

  现在已经幸福多了，ChatGPT 横空出世，已经可以解答了，故介绍一些高频或者重点宏，自己结合去搜🔍

  CMake 的重要宏：《[CMake 设置 Target 输出目录和后缀名](https://blog.csdn.net/qq_33154343/article/details/125932219)》、《[CMake 之 BUILD_SHARED_LIBS 和 CMAKE_BUILD_TYPE 用法教程](https://blog.csdn.net/qq_33154343/article/details/125928773)》

  Windows 的重要宏：《[LPSTR/LPCSTR/LPTSTR/HWND/HANDLE/HMODULE/HINSTANCE 等含义和区别](https://blog.csdn.net/qq_33154343/article/details/125775718)》


- 编译遇坑、杂谈经验总结等，有空再更...


## 加油打气(ง •_•)ง

若对您有帮助，或者觉得有用，**您可以点击该仓库的⭐ Star 🍴 Fork 两个图标，方便抬手之间，说点赞的手，** 手留余香；其次可以我喝一杯加冰的肥宅快乐水。

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202302282339037.png" width="80%"/>

<!-- <details>
    <summary> <b>"当然，你也可以送我一听冰凉的可乐 [捐赠/打赏 ← 点击展开二维码]"</b></summary>
  <p> 如果对您有帮助，或者觉得有用，**您可以点击该仓库的⭐ Star 🍴 Fork 两个图标，方便抬手之间，说点赞的手，** 手留余香；其次可以我喝一杯冰的快乐水 - </p>
  <pre><img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2022/202302282339037.png" width="80%"/></pre>
</details> -->

## 系列地址

[QtExamples](https://github.com/XMuli/QtExamples)  欢迎 `star` ⭐ 和 `fork` 🍴 这个系列的 `C++ / QT / DTK` 学习，附学习由浅入深的目录，这里你可以学到如何亲自编写这类软件的经验，这是一系列完整的教程，并且**永久免费**！
