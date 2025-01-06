## v3.1

> 2025.01.06

**版本优化:**

- 新增翻译：日本语，感谢 [coolvitto](https://github.com/coolvitto)
- 修复 Linux 下多屏截图异常 #64
- 修复快捷键重复上次截图区域失效 #64
- 修复 Tooltop 的主题颜色效果 #63
- 优化安装包的运行检测逻辑
- 构建自动打包 Win 和 Linux 的脚本



## v3.0

> 2025.01.01

新的一年，全新版本升级，带来不一样的感觉🎀；新的官网和用户手册 [https://sunnycapturer.xmuli.tech](https://sunnycapturer.xmuli.tech/)



**特色功能:**

- OCR 提取文本，在线和离线模式
- 图片翻译，支持 100+ 种语言
- 自定义贴图
- 钉图于桌面
- 浅色和暗色主题
- 启用新的 LOGO



## v2.4

> 2024.12.02

**功能:**

- 设置样式改为导航栏样式
- 修复使用放大镜时，按下 C 和 Shift 不生效
- 感谢 @thiagodalsoto 提供葡萄牙语-巴西语翻译
- 更多bug修复和体验优化...

**常见答疑**

**Linux / Mac**

- Q1：Linux 安装新版后，无法运行？

  卸载低 1.x 版本，且尝试运行删除旧的配置文件   `rm -rf ~/.config/Sunny/*`

- Q2：在 Ubuntu 24.04 截屏，捕捉的桌面为白色？

  Sunny 1-2.x 版本为兼容更多的 Linux 发行版，当前仅支持 `x11`，不支持 `Wayland` (Ubuntu 默认); 切换 `x11` 方式如下，详细参考 [#55](https://github.com/XMuli/SunnyCapturer/issues/55)

  ```bash
  sudo vim /etc/gdm3/custom.conf
   # at the beginning of the line WaylandEnable=false to uncomment it, then save the file. 
  sudo systemctl restart gdm3  // reboot the system
  ```

- Q3: 非 Debian 系，如何使用？

  请下载和使用 `sunny_x64.AppImage` 安装包；已知道支持的 Linux 发行版可参考 [已兼容平台](https://sunny.xmuli.tech/article/supportp_platform.html)

- Q4：ARM 和 LOONGSON 芯片如何下载？

  可先下载  [v1.6.2](https://github.com/XMuli/SunnyCapturer/releases/tag/v1.6.2)；若编译机器有空时，2.x版本便会构建上传

- Q5：Mac 无法运行？

  同理，对于 Mac 当前仅支持 `intel` 芯片，不支持 `M` 系列芯片

- ps: 预计下一个版本将会支持 Mac 的 M 芯片和 Linux Wayland 架构



**离线 OCR (CPU & GPU)**

> 通过本地离线引擎，离线提取图片中的文字

- 无需要任何注册和 key 的输入
- 全程断网离线本地运行和识别
- 支持 CPU 和 GPU 模式（叉腰）：
  - CPU 版本：通用性强，占用内存更少，对于单张图片解析快，批量图片耗时大（普通用户推荐）
  - GPU 版本：仅支持 N 卡，占用内用多，但对于批量解析大量图片，耗时是 CPU 版本是 1/2 ~ 1/3 时间，很快（高级 N 卡推荐）
  - 仅支持 win 64 bit 系统；均支持单张图片和批量图片识别解析，直接拖曳到窗口即可。
  
  

## v2.3

> 2024.10.17

这是一个大版本功能的更新，欢迎使用和反馈

**功能:**

- 支持在线和离线引擎， "提取文本(OCR)"
- 支持在线的 "图片翻译"，30+语言翻译
- 支持离线本地"提取文本(OCR)"引擎，支持 CPU 和 GPU 两种模式
- 对 OCR 和图片翻译，支持使用私人 key
- 支持自动检测窗口的算法切换和关闭
- 托盘菜单支持提供更丰富的选项
- 支持日志模块
- 更新多语言的翻译（感谢 @thiagodalsoto 提供葡萄牙语-巴西语翻译）

**修复:**

- 界面优化和更多Linux发行版的适配
- 记住上次使用的颜色和画笔宽度
- OCR 独立窗口在保存图片时，无自动命名
- 编辑文字时，发布时刻去掉框，且选择记住的字体
- 字体宽度（小中大圆形控件）跟随滑轮滚动而变化
- 字体默认为微软雅黑（宋体缩小时，会看不清楚）
- Linux 下 .AppImage 格式更新会崩溃异常   #47

- ubuntu unity（ubuntuunity.org） 22.04.1 LTS，其截图区域不对
- 优化用户体验，软件开机后自动检测到无可用的新版本，屏蔽此场景的提示
- Linux(KDE) 下暗色主题，其工具栏背景无法看清楚 #49
- 截图启动有些卡顿 #50
- 自动检测窗口支持切换算法和关闭
- 其它一些 bug 的修复

**注意：**
- 对于 Linux 发行版，若安装过低版本 如 `1.x`，若遇到运行失败，请先终端执行 `$ rm -rf ~/.config/Sunny` 后，成功运行

**Cross-platform text extraction (OCR):**

**Debian OCR:**<br>
<img src="https://github.com/user-attachments/assets/090e075d-4ee0-4cd4-a776-de4bc13053a1" width="400"/><br>

**Deepin V23 OCR:**<br>
<img src="https://github.com/user-attachments/assets/829892f9-3c0e-4a45-8f6e-55a74fbd2c60" width="400"/><br>



## v2.2.1

> 2024.09.21

**大版本 2.2.1 正式发布🎉🎉🎉**

- 漂亮 UI/UX 样式
- 架构重构
- 新版本检测
- 支持离线 OCR：支持中文汉字竖排识别，水平布局识别


**注意：**
- 当前版本仅 `sunny_protable_2.2.1_x64_offline_ocr.zip` 和  `sunny_setup_2.2.1_x64_offline_ocr.exe` 的支持离线 OCR 识别；若想要使用“图片翻译” 和 “在线OCR” 请下载  [v1.6.3](https://github.com/XMuli/SunnyCapturer/releases/tag/v1.6.3) 版本
- 对于 Linux 发行版，若安装过 1.x 版本，若遇到运行失败，请先终端执行 `$ rm -rf ~/.config/Sunny`后，成功运行



## v2.1 (Pre)

> 2024.09.17

中秋节快乐🌕

注意：这是一个内部通道的测试版本，部分功能是不稳定的。提供内测用户和尝鲜测试使用，欢迎反馈缺陷和建议。

- 支持智能窗口识别（tab 按键切换层级）
- 修复文本编辑
- 和修复一些 bug



## v2.0 (Pre)

> 2024.09.7

**注意：此为内测版本，功能尚不稳定**
支持离线版本 OCR，用于提取图片的文本，支持竖版文字识别；**请下载名字带有 `_with_offline_ocr` 才支持 离线 OCR功能**

 <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202409071533042.png" width="400"/>



## v1.6.3

> 2024.05.28

**自用中，积极开发功能和修复缺陷，以和提升用户体验。**

**功能**

  - 新增"德语"翻译
  - 更新"葡萄牙-巴西"翻译
  - 支持操作引导提示窗口

**修复**
  - 十字线会遮挡放大镜的区域 #25
  - Linux 中点击时拉伸选中矩形后尺寸异常 #26
  - 托盘预览的翻译未立刻生效 #19
  - 其它小功能的优化

**感谢贡献**

感谢 @thiagodalsoto  提供葡萄牙语-巴西语翻译，感谢 @Kthargas 提供了德语翻译

若想参与翻译，请点击[链接](https://github.com/XMuli/SunnyCapturer/tree/master/translations)；此为完整的[鸣谢名单🙇‍](https://sunny.xmuli.tech/zh/acknowledgement.html)



## v1.6.2

> 2024.05.21

**Sunny Screenshot New Website:** [sunny.xmuli.tech](https://sunny.xmuli.tech)     📢: [English issues](https://github.com/XMuli/SunnyCapturer/issues)    |   [中文讨论组](https://txc.qq.com/products/649489)

**功能**

  - 支持"葡萄牙-巴西"翻译
  - 支持绘画状态显示准星光标
  - 支持图片翻译的沉浸式显示
  - 支持 OCR 和 图片翻译的快捷键
  - 支持 OCR 新窗口设计，记住上次拉伸比例
  - 优化 OCR 自动切换可用线路
  - 优化保存图片会默认使用上次的后缀格式 #16
  - 优化 Linux GUI 安装 DEB 无缝升级方案
  - 优化日志的跨平台输出

**修复**
  - 工具栏和参数信息栏可能会相交覆盖 #13
  - 配置后保存失效 #12
  - Debian12 + KDE 小数缩放的异常
  - 工具栏在屏幕边界显示不全
  - 热键删除时，旧的热键组合未取消
  - Debian 系下托盘区域图标不显示
  - 文字编辑框切换颜色时失败

**UI 界面**
  - 新增加取色器和放大镜功能  #13
  - 新增显示器布局详细的信息
  - 调节主窗口高度智能自适应
  - 工具栏竖直模式，添加较浅的刻线

<br>

**感谢贡献**

感谢 @thiagodalsoto 提供葡萄牙语-巴西语翻译；若想参与翻译，请点击[链接](https://github.com/XMuli/SunnyCapturer/tree/master/translations)；此为完整的[鸣谢名单🙇‍](https://sunny.xmuli.tech/zh/acknowledgement.html)





## v1.5

> 2024.04.16

**自用中，积极开发和提升用户体验中；修复大量 bug 和美化 UI 设计，提升用户体验。**
修复
 - 一级工具栏的快捷键失效
 - 自动窗口检测，会误捕捉到不显示的系统窗口
 - 自动保存和快速保存的异常逻辑

UI 提升
 - 一二级工具栏，采用高亮色；且有亚克力磨砂和普通边框效果
 - 一级工具栏的 ToolTip 的样式为白底
 - 钉图窗口支持缩放提示百分比，默认 2 秒
 - 设计 Tokens UI 为 QStackedWidget 效果
 - 隐藏 Sunny 边框的四角样式
 - 选中框为高亮色的效果

优化
 - 自动检测窗口尺寸优化，不含阴影区域
 - 后端数据存储优化，.ini 改为 .json 格式
 - OCR 若选带_local 项，提取的文字具有整齐的排版格式
 - OCR 提供配置参数，自定义误差数据微调排版准确率
 - 新增加"工程师高级"的开关：使保存图片带上窗口详细信息

其它等

**内置多条免费线路，可切换 OCR 和图片翻译**

<br>

**鸣谢：** 
        感谢 [shenmo](https://github.com/shenmo7192) 以及 [Spark-Store](https://gitee.com/spark-store-project/spark-store) 团队的帮助和支持，将其移植到 ARM 和 龙芯 架构平台，亦可直接在星火商店下载
`sunny_1.5.0_arm64.deb`, `sunny_1.5.0_loong64.deb`  此架构仅支持 deepin V23、Debian 12+ 系的平台；



## v1.4

> 2024.03.27

【中文】
1. 支持线路切换，"OCR"和"图片识别"
2. 提升识别次数
    - OCR 次数提升至 **3500次/月** 
	- 图片识别次数提升为 **10000次/年** + 其它
3. 采用新的样式弹出窗口设计
4. 优化和修复部分 bug
5. 设置参考: [Wiki: 6. Image Translation & OCR](https://github.com/XMuli/SunnyCapturer/wiki/6.-Image-Translation-&-OCR-%7C-%E5%9B%BE%E7%89%87%E7%BF%BB%E8%AF%91-&-%E6%96%87%E5%AD%97%E6%8F%90%E5%8F%96)

<br>

- Channel Switching | 线路切换

- OCR | 提取文字

- Images Translate | 图片翻译



## v1.3

> 2024.02.01

最近 Sunny 截图被大佬和软件网站给翻牌子，突然多了很多流量和 star，属意外的惊喜。
但同时新用户也比较多，目前 **翻译** 和 **OCR** 的个人账号余额已见底🤣🤣

<br>

对于 **翻译** 和 **OCR** 功能，现已支持使用的个人的私人账号，若有需，可自行输入：

- **百度接口** [https://console.bce.baidu.com/ai](https://console.bce.baidu.com/ai)

    图片提取文字 `"OCR API - Baidu API - 文字识别"`
    1. 通用文字识别（标准版）  
    2. 通用文字识别（标准含位置版）  
    3. 通用文字识别（高精度版）  
    4. 通用文字识别（高精度含位置版） → 最推荐


- **有道接口**  [https://ai.youdao.com/console](https://ai.youdao.com/console)

    翻译中日英韩日等 `"Translate - 自然语言翻译服务 - 图片翻译"`

---

**代码签名证书:**  
另外，自从本版本已经添加代码签名，下载后，请校验文件的代码签名和如下一致:

**鸣谢： 2024.03.01**    
感谢  [XXTXTOP](http://www.xiongshijie.top) 帮助上架 Openkylin Store 麒麟商店！此 Linux 发行版也可直接下载了  



## v1.2

> 2023.11.20

**新功能:**
- 新支持  “图片翻译” 和 “OCR 图片识别文字” 功能
- 修复部分bug，和优化绘画
- Sunny 上架到 UOS/Deepin Store 和 Spark Store



## v1.1

> 2023.10.28

**Sunny 截图是一款简洁且漂亮的截图的软件工具，支持 Windows，MacOS，Linux 平台**

**特色功能**:

- 支持多种系统风格样式
- 支持自定义透明磨砂效果(亚克力)
- 支持绘画工具栏方位切换：水平或竖直
- 支持高亮色主题切换(配置文件修改)
- 支持常规截图、延时截图、自定义截图
- 支持快捷保存和自动保存
- 支持篡窗口的屏幕检测(窗口信息和窗口深度)
- 支持图片钉在屏幕，且缩放和透明度设置
- 文字编辑时刻，同一时刻支持多种不同格式
- 更多

**本次更新：**
- 移植到 Deepin V20.9 成功编译和运行
- 新增图钉在屏幕的阴影效果
- 更新翻译(中文-简体、中文-繁体)



## v1.0

> 2023.10.28

庆祝第一个正式版本 v1.0.0 发布 🎉🎉🎉🎉

适用于 Windows、MacOS 和 Linux 的简单美观的截图软件工具。
欢迎使用并反馈错误/建议，也可以提出您希望实现的功能。积极开发，程序员节快乐。
