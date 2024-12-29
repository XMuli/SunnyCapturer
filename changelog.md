## v3.0

> 2025.01.01

New year, new version upgrade, bring different feeling 🎀; new official website and user manual [https://sunnycapturer.xmuli.tech](https://sunnycapturer.xmuli.tech/)

**Features:**

- OCR text extraction, online and offline mode
- Image translation, support 100+ languages
- Customizable stickers
- Pin images to desktop
- Light and dark themes
- New logo activation



## v2.4

> 2024.12.02

**Changelog:**

- Changed styling to a navigation bar format.
- Fixed an issue where pressing **C** and **Shift** while using the magnifying glass had no effect.
- special thanks to @thiagodalsoto for providing Brazilian Portuguese translations
- Additional bug fixes and experience improvements. 

**Q&A:**

**Linux / Mac**

- **Q1: Linux won't run after installing a new version?**  
  Uninstall Sunny version 1.x or earlier, and try removing old configuration files `  rm -rf ~/.config/Sunny/*`


- **Q2: On Ubuntu 24.04, screenshots capture a white desktop. Why?**  
  To support more Linux distributions, Sunny 1-2.x currently supports only `x11` and does not support `Wayland` (Ubuntu's default). Switch to `x11` with the following steps (details in [#55](https://github.com/XMuli/SunnyCapturer/issues/55):  
  
  ```bash
  sudo vim /etc/gdm3/custom.conf
  # Uncomment the line: WaylandEnable=false, then save the file.
  sudo systemctl restart gdm3  # Reboot the system
  ```
  
- **Q3: How can I use Sunny on non-Debian-based distributions?**  
  Please download and use the `sunny_x64.AppImage` package. For a list of supported Linux distributions, refer to [supportp_platform](https://sunny.xmuli.tech/article/supportp_platform.html).

- **Q4: How can I download Sunny for ARM and LOONGSON chips?**  
  You can download [v1.6.2](https://github.com/XMuli/SunnyCapturer/releases/tag/v1.6.2). When build machines become available, version 2.x will also be uploaded.

- **Q5: Why can’t Sunny run on my Mac?**  
  Currently, Sunny supports only Intel-based Macs and does not support M-series chips.

- ps: The next version is expected to support Mac's M-series chips and the Linux Wayland architecture.



**Offline OCR (CPU & GPU)**

> Extract text from images using a local offline engine.

- No registration or key input required.  
- Fully offline, with all processing done locally.  
- Supports both CPU and GPU modes:
  - **CPU Version**: Highly compatible, uses less memory, processes single images quickly but is slower for batch processing (recommended for general users).
  - **GPU Version**: Supports NVIDIA GPUs, uses more memory, but processes batch images significantly faster (1/2 to 1/3 the time of the CPU version, recommended for advanced users with NVIDIA cards).
- Only supports 64-bit Windows systems. Both versions handle single and batch image recognition. Simply drag and drop images into the window.



## v2.3

> 2024.10.17

This is a major version of the feature update, welcome to use and feedback!

**Features.**
- Support online and offline engine, “Extract Text(OCR)”.
- Support online “Picture Translation”, 30+ languages translation.
- Support offline local “OCR” engine, support both CPU and GPU modes.
- Support using private key for OCR and image translation.
- Support algorithm switching and closing of auto-detection windows.
- Tray menu support provides richer options
- Support for logging module
- Updated multi-language translations (Thanks to @thiagodalsoto for the Portuguese-Brazilian translation)
- Interface optimization and adaptation to more Linux distributions.

**Fixes.**
- Linux crashes when clicking on updates with .AppImage #47
- AppImage crashes when clicking on update for Linux #47 Dark theme icons are not visible in some distributions such as Linux KDE #49
- Screenshot startup lags a bit #50
- Other bug fixes

**Note:**
- For Linux distributions, if you have installed a lower version such as `1.x`, if you encounter a runtime failure, please run `$ rm -rf ~/.config/Sunny` in the terminal first, and then it will run successfully.

**Cross-platform text extraction (OCR):**

**Debian OCR:**<br>
<img src="https://github.com/user-attachments/assets/090e075d-4ee0-4cd4-a776-de4bc13053a1" width="400"/><br>

**Deepin V23 OCR:**<br>
<img src="https://github.com/user-attachments/assets/829892f9-3c0e-4a45-8f6e-55a74fbd2c60" width="400"/><br>



## v2.2.1

> 2024.09.21

**Major version 2.2.1 is officially released🎉🎉🎉**

- Beautiful UI/UX styles
- Architecture reorganization
- New version detection
- Support offline OCR: support vertical Chinese character recognition, horizontal layout recognition.

**Attention:**

- The current version only supports offline OCR for `sunny_protable_2.2.1_x64_offline_ocr.zip` and `sunny_setup_2.2.1_x64_offline_ocr.exe`; If you want to use “Image Translation” and “Online OCR”, please download [v1.6.3](https://github.com/XMuli/SunnyCapturer/releases/tag/v1.6.3).
- For Linux distributions, if some users have installed version 1.x, if the program fails to run, please execute `$ rm -rf ~/.config/Sunny` in the terminal first, and then it will run successfully.



## v2.1 (Pre)

> 2024.09.17

**Happy Mid-Autumn Festival 🌕**

Note: This is a test version of internal channel, some functions are unstable. It is provided for internal test users and taste test use, feedback of defects and suggestions are welcome.

- Support smart window recognition (tab Button to switch levels)
- Fix text editing
- and fix some bugs



## v2.0 (Pre)

> 2024.09.7

**Note: This is an internal beta version, the function is not stable yet.**
Supports offline version of OCR for image text extraction, Support vertical text recognition. **Please download the program with `_with_offline_ocr` in the name to support the offline OCR function.**

 <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202409071533042.png" width="400"/>



## v1.6.3

> 2024.05.28

**It is also in its own use, actively developing and improving its experience; Fix some bugs and optimize some UI styles to improve user experience. (Lots of improvements)**

**Functions**

- Added “German” translation
- Updated “Portugal-Brazil” translation.
- Support operation guide tips window

**Fixes**

- Crosshairs block the magnifying glass area [#25](https://github.com/XMuli/SunnyCapturer/issues/25)
- Size anomaly after stretching selected rectangle when clicking in Linux [#26](https://github.com/XMuli/SunnyCapturer/issues/26)
- Tray preview translation doesn't work immediately [#19](https://github.com/XMuli/SunnyCapturer/issues/19)
- Other minor optimizations

**Contribute**

Thanks to [@thiagodalsoto](https://github.com/thiagodalsoto) for the Portuguese-Brazilian translation and to @Kthargas for the German translation;

to contribute a translation, please click on the [link](https://github.com/XMuli/SunnyCapturer/tree/master/translations); Here is the full [list of acknowledgements🙇‍](https://sunny.xmuli.tech/acknowledgement.html)



## v1.6.2

> 2024.05.21

**Sunny Screenshot New Website:** [sunny.xmuli.tech](https://sunny.xmuli.tech/)  📢: [English issues](https://github.com/XMuli/SunnyCapturer/issues) | [中文讨论组](https://txc.qq.com/products/649489)

**Functions**

- Support “Portugal-Brazil” translation.
- Support displaying quasar cursor in drawing state
- Support immersive display for image translation
- Support shortcut keys for OCR and image translation
- Support OCR new window design, remember the last stretch ratio
- Optimize OCR to switch available lines automatically
- Optimize saving images will use the last suffix format by default [#16](https://github.com/XMuli/SunnyCapturer/issues/16)
- Optimize seamless upgrade solution for Linux GUI installation DEB.
- Optimize cross-platform output of logs

**Fixes**

- Toolbar and parameter information bar may intersect and overwrite [#13](https://github.com/XMuli/SunnyCapturer/issues/13)
- Failure to save after configuration [#12](https://github.com/XMuli/SunnyCapturer/issues/12)
- Debian12 + KDE fractional scaling, display exception bugs
- Toolbars are not fully displayed at screen boundaries
- Old hotkey combinations are not canceled when hotkeys are deleted.
- Tray area icon is not displayed under Debian system.
- Text edit box fails when switching colors

**UI interface**

- Added color picker and magnifying glass function [#13](https://github.com/XMuli/SunnyCapturer/issues/13)
- Add detailed information about monitor layout.
- Adjust the height of main window
- Toolbar vertical mode, add lighter scribe line

**Contribute**

Thanks to [@thiagodalsoto](https://github.com/thiagodalsoto) for the Portuguese-Brazilian language translation; to contribute a translation, please click on the [link](https://github.com/XMuli/SunnyCapturer/tree/master/translations); Here is the full [list of acknowledgements🙇‍](https://sunny.xmuli.tech/acknowledgement.html)





## v1.5

> 2024.04.16

**It is also in its own use, actively developing and improving its experience; Fix many bugs and optimize some UI styles to improve user experience. (Lots of improvements)**
Fixes

- Shortcut keys for the first level toolbar are not working
- Auto Window Detection that mistakenly catches non-displayed system windows
- Exception logic of auto-save and quick-save

UI enhancement

- The first and second level toolbars adopt highlight color; and there are acrylic frosted and common border effects.
- The style of ToolTip in the primary toolbar is white background.
- Support zoom tip percentage in pinning window, default 2 seconds.
- Design Tokens UI with QStackedWidget effect.
- Hide the corners of the Sunny border
- Highlight color effect for checkboxes

Optimization

- Optimize auto-detect window size without shadow area
- Optimize backend data storage, .ini changed to .json format.
- OCR with _local option, the extracted text has a neat layout format.
- OCR provides configuration parameters to customize the error data to fine-tune the layout accuracy.
- New "Engineer Advanced" switch: save image with window details.

Other etc.

<br>

**Acknowledgments:** 
        Thanks to [shenmo](https://github.com/shenmo7192) and the [Spark-Store](https://gitee.com/spark-store-project/spark-store) team for their help and support in porting it to the ARM and Longxin architectures. The program can also be downloaded directly from the Spark Store
`sunny_1.5.0_arm64.deb`, `sunny_1.5.0_loong64.deb` This architecture is only supported on deepin V23, Debian 12+ platforms;



## v1.4

> 2024.03.27

- Support channel switching, "OCR" and "Images Translate".
- Increase the number of recognition times
  - OCR times increased to **3500 times/month**
  - Increase the number of picture recognition to **10000 times/year** + Others
- Adopt new style pop-up window design.
- Optimise and fix some bugs
- more: [Wiki: 6. Image Translation & OCR](https://github.com/XMuli/SunnyCapturer/wiki/6.-Image-Translation-&-OCR-|-图片翻译-&-文字提取)



## v1.3

> 2024.02.01

Recently, Sunny's screenshot got promoted by well-known internet users and software websites, suddenly gaining a lot of traffic and stars. It's an unexpected delight.

This project started in 2023 and will be developed and maintained for at least ten years, and it's free! Additionally, since this version has added code signing, please verify the original files after downloading.

<br>

For the **Translate** and **OCR** functions, the private accounts of the individuals who use them are now supported and can be entered by themselves if needed:

- **Baidu Interface** [https://console.bce.baidu.com/ai](https://console.bce.baidu.com/ai)

  Image Text Extraction ``OCR API - Baidu API - Text Recognition``

  1. Universal Text Recognition (Standard)  
  2. Universal Text Recognition (Standard with Position)  
  3. Universal Text Recognition (High Accuracy)  
  4. Universal Text Recognition (high-precision version with location) → Most Recommended


- **Youdou Interface** [https://ai.youdao.com/console](https://ai.youdao.com/console)

  Translate Chinese, Japanese, English, Korean, Japanese, etc. ``Translate - Natural Language Translation Service - Image Translation`''

---

**Code Signing Certificate:**  
In addition, since this version has been added code signature, after downloading, please check the code signature of the file is consistent with the following.

**Acknowledgments: 2024.03.01**    



## v1.2

> 2023.11.20

**New Features:**

- Support "Picture Translation" and "OCR Picture Recognition Text".
- Fix some bugs, and optimize painting
- Sunny uploaded to UOS/Deepin Store and Spark Store.



## v1.1

> 2023.10.28

**Sunny is a simple and beautiful screenshot software tool for Windows, MacOS and Linux.**

**Features**

- Support for various system style themes.
- Customizable transparent frosted glass effect (acrylic).
- Switchable orientation for the drawing toolbar: horizontal or vertical.
- Support for high-contrast color theme switching (via configuration file modification).
- Capabilities for regular screenshots, delayed screenshots, and custom screenshots.
- Quick and automatic saving options.
- Screen detection for capturing window content (window information and window depth).
- Ability to pin images to the screen with scaling and transparency settings.
- Simultaneous support for multiple text formats during editing.
- And more.

**update:**

- Port to Deepin V20.9 compiled and ran successfully.
- Added the shadow effect of pins on the screen
- Updated translations (Chinese-Simplified, Chinese-Traditional).



## v1.0

> 2023.10.28

Celebrating the release of the first official version v1.0.0 🎉🎉🎉🎉

Simple and beautiful screenshot software tool for Windows, MacOS and Linux.
Welcome to use it and give feedback on bugs / suggestions. and It is also possible to suggest features that you would like to see implemented. Active development, happy programmer's day.
