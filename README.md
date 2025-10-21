

This project has gone through multiple major iterations and releases. Early source code is now open-sourced for reference. Each folder contains the complete source code for a specific period/version.



> Provide a README introduction in multiple languages
>
> 提供多语言版本的README介绍
>
> → [English](./README.md) / [简体中文](./README.zh_CN.md) 



## Official website

- Official site / Changelog: [https://sunnycapturer.xmuli.tech](https://sunnycapturer.xmuli.tech/)
- Download: https://github.com/XMuli/SunnyCapturer/releases
- App stores: Microsoft Store & major Linux app stores

Note: For supported platform versions and chips, refer to the changelog; GitHub Releases are always the primary first-release channel, while app stores may lag slightly.



## Milestones

### 5th Version - SunnyCapturer 5.x

- Name: SunnyCapturer 5.x

- Summary: Capturer is an app focused on fast screenshots, pasting images, quick preview, screen recording and GIF recording. Use hotkeys to quickly select a desktop area or window to capture/record, pin screenshots to the desktop, and it also supports OCR (both multi-platform online access and local CPU/GPU) and image translation.

- Notes:

  1. Primary development period: 6/9/2025 - Present; actively maintained. This is the current latest version.

  2. Core features:

     - Completely new UI design: simpler and more aesthetic, with multiple dark themes supported
     - Retains all previous screenshot and annotation features; offline OCR (CPU/GPU) and online OCR; online image translation
     - Published to Microsoft Store and multiple Linux distributions' app stores
     - Supports video recording
     - Supports GIF recording
     - Supports camera photo capture
     - Supports quick preview for multiple file types
     - When multiple devices are present, can switch the active microphone, speaker, and camera
     - Supports choosing installation path for all users (system-wide) or only the current user
     - Multilingual translations supported: English, Simplified Chinese, Traditional Chinese, 日本語, Português Brasil, Русский, Deutsch

  3. Build environment:

     Qt >= 6.8 | CMake >= 3.20 | MSVC >= 2022 | GCC >= 13

  4. Supported platforms:

     Windows 10-11 / Linux (Ubuntu 24.04 LTS / Deepin V23)

- Source code:

  1. Official intro: [https://sunnycapturer.xmuli.tech](https://sunnycapturer.xmuli.tech/)
  2. Source mirror: [3rd-SunnyCapturer-v2.x-Code](https://github.com/XMuli/SunnyCapturer/tree/master/3rd-SunnyCapturer-v2.x-Code)
  3. Original repo: GitHub
  4. Total released versions: [Release](https://github.com/XMuli/SunnyCapturer/releases) V5.0.0 - Present (≥ 2 versions so far)

- Screenshots:

   

  

### 4th Version - SunnyCapturer 3.x

- Name: SunnyCapturer 3.x

- Summary: A screenshot tool, with general module encapsulation.

- Notes:

  1. Primary development period: 11/22/2024 - 5/16/2025; development has stopped. Work shifted to SunnyCapturer v5.x.

  2. Core features:

     - New UI design: simpler and more aesthetic, with multiple dark themes supported
     - Retains all previous screenshot and annotation features; offline OCR (CPU/GPU) and online OCR; online image translation
     - Published to Microsoft Store and multiple Linux distributions' app stores

  3. Build environment:

     Qt >= 6.8 | CMake >= 3.20 | MSVC >= 2022 | GCC >= 913

  4. Supported platforms:

     Windows 10-11 / Linux (Ubuntu 24.04 LTS / Deepin V23)

- Source code:

  1. Official intro: [https://sunnycapturer.xmuli.tech](https://sunnycapturer.xmuli.tech/)
  2. Source mirror: [3rd-SunnyCapturer-v2.x-Code](https://github.com/XMuli/SunnyCapturer/tree/master/3rd-SunnyCapturer-v2.x-Code)
  3. Original repo: GitHub
  4. Total released versions: [Release](https://github.com/XMuli/SunnyCapturer/releases) V3.0 - V3.2 (≥ 6 versions)

- Screenshot:

    <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2025/202510220110391.png" width="70%"/>

  



### 3rd Version - SunnyCapturer 2.x

- Name: sunny-screenshot

- Summary: A screenshot tool, with general module encapsulation.

- Notes:

  1. Primary development period: 8/8/2024 - 12/1/2024; development has stopped. Work shifted to SunnyCapturer v3.x.

  2. Core features:

     - Online OCR and online image translation
     - Supports scripts for automatic packaging on Windows and Linux (.deb / .dmg / …)
     - **Offline OCR (CPU & GPU)**

     > Extract text from images locally using an offline engine

     - No registration or API key required
     - Fully offline local operation and recognition
     - Supports CPU and GPU modes:
       - CPU version: more general, uses less memory, faster for single-image parsing; slower for large batches (recommended for regular users)
       - GPU version: supports NVIDIA GPUs only, uses more memory, but for batch parsing of many images it can be 1/2 to 1/3 the time of the CPU version (recommended for advanced users with NVIDIA GPUs)
       - Only supports Windows 64-bit; both single-image and batch recognition supported, drag-and-drop into the window to use.

  3. Build environment:

     Qt >= 5.15.2 | CMake >= 3.16 | MSVC >= 2019 | MinGW >= 8.1.0 | GCC >= 9.4 | Clang >= 12.0

  4. Supported platforms:

     Windows 10-11 / macOS 13.2 / Linux (Ubuntu 24.04 LTS / Deepin V20.9-V23 / openKylin 1.0.x)

  5. FLIPPED is licensed under the MIT license; all code is open-source to help newcomers lower the barrier to C++/Qt and easily create a screenshot project.

- Source code:

  1. Official intro: [https://sunny.xmuli.tech](https://sunny.xmuli.tech/#/)
  2. Source mirror: [3rd-SunnyCapturer-v2.x-Code](https://github.com/XMuli/SunnyCapturer/tree/master/3rd-SunnyCapturer-v2.x-Code)
  3. Original repo: GitHub
  4. Total released versions: [Release](https://github.com/XMuli/SunnyCapturer/releases) V2.0 - V2.4 (≥ 5 versions)

- Screenshots:

      <img src="https://private-user-images.githubusercontent.com/33887845/379350274-e5bfceb6-bd49-4a14-9438-009977e7c4f8.jpg?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NjEwNjYwODMsIm5iZiI6MTc2MTA2NTc4MywicGF0aCI6Ii8zMzg4Nzg0NS8zNzkzNTAyNzQtZTViZmNlYjYtYmQ0OS00YTE0LTk0MzgtMDA5OTc3ZTdjNGY4LmpwZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNTEwMjElMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjUxMDIxVDE2NTYyM1omWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPWFjMzBjOTYxMGZkODMxZTZmNDQzYjEwOTY3MDQxZGUxY2VlNTFlMTM0MDk1NTE4Y2FmNDc1ZWY2OWQ4MWYxZGQmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0In0.cHmAinONSWcPzwXoDj6A0JGkuaEHU-dIyj1vU2ociqo" width="70%"/>

     <img src="https://private-user-images.githubusercontent.com/33887845/379350590-090e075d-4ee0-4cd4-a776-de4bc13053a1.jpg?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NjEwNjYwODMsIm5iZiI6MTc2MTA2NTc4MywicGF0aCI6Ii8zMzg4Nzg0NS8zNzkzNTA1OTAtMDkwZTA3NWQtNGVlMC00Y2Q0LWE3NzYtZGU0YmMxMzA1M2ExLmpwZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNTEwMjElMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjUxMDIxVDE2NTYyM1omWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTI3ODU3MDgxN2E0ZDZiYzNlZGZjZGYzMTc1NmM5MTI4OGMyOTk1MGQxNGE2NTBmMjJmZTVmNmEwMTEwNzA3MDkmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0In0.dNs2mMI8mzWp2rq0PlnRToSTpUte304sSC-dbffbP9w" width="70%"/>

  

### 2nd Version - SUNNY 1.x

- Name: Sunny

- Summary: A screenshot tool, with general module encapsulation.

- Notes:

  1. Primary development period: 8/23/2023 - 5/28/2024, maintenance until 8/28/2024; development has stopped. Work shifted to SunnyCapturer v2.x.

  2. Core functionality: supports online OCR and image translation

  3. Build environment:

     Qt >= 5.15.2 | CMake >= 3.16 | MSVC >= 2019 | MinGW >= 8.1.0 | GCC >= 9.4 | Clang >= 12.0

  4. Supported platforms:

     Windows 10-11 / macOS 13.2 / Linux (Ubuntu 22.04 LTS / Deepin V20.9-V23 / openKylin 1.0.x)

  5. FLIPPED is licensed under the MIT license; all code is open-source to help newcomers lower the barrier to C++/Qt and easily create a screenshot project.

- Source code:

  1. Official intro: [https://sunny.xmuli.tech](https://sunny.xmuli.tech/#/)
  2. Demo video: [FLIPPED cross-platform screenshot/paste tool — macOS / Windows / Linux demo](https://www.bilibili.com/video/BV1rX4y1D7EZ)
  3. Source mirror: [2nd-SunnyCapturer-v1.x-Code](https://github.com/XMuli/SunnyCapturer/tree/master/2nd-SunnyCapturer-v1.x-Code)
  4. Original repo: [SunnyCapturer-2nd](https://github.com/SunnyCapturer/SunnyCapturer-2nd)
  5. Total released versions: [Release](https://github.com/XMuli/SunnyCapturer/releases?page=2) V1.0 - V1.6.3 & [SunnyPages](https://github.com/XMuli/SunnyPages) (≥ 8 versions)

- Screenshots:

     <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2025/202510220040440.png" width="70%"/>

     <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2025/202510220041960.png" width="70%"/>





### 1st Version - FLIPPED

- Name: FLIPPED

- Summary: A simple and beautiful screenshot tool that supports Windows, macOS and Linux platforms.

- Notes:

  1. Primary development period: 9/25/2021 - 8/21/2023, maintenance until 1/25/2025; development has now stopped. Work shifted to SunnyCapturer v1.x.

  2. Core functionality: basic offline full annotation features, and native packaging for the three major operating systems.

  3. Build environment:

     Qt >= 5.15.2 | CMake >= 3.16 | MSVC >= 2019 | MinGW >= 8.1.0 | GCC >= 9.4 | Clang >= 12.0

  4. Supported platforms:

     Windows 10-11 / macOS 13.2 / Linux (Ubuntu 22.04 LTS / Deepin V20.9-V23 / openKylin 1.0.x)

  5. FLIPPED is licensed under the MIT license; all code is open-source to help newcomers lower the barrier to C++/Qt and easily create a screenshot project.

- Source code:

  1. Official intro: https://flipped.xmuli.tech/
  2. Demo video: [FLIPPED cross-platform screenshot/paste tool — macOS / Windows / Linux demo](https://www.bilibili.com/video/BV1rX4y1D7EZ)
  3. Source mirror: [1st_FLIPPED-Screenshot-Code](https://github.com/XMuli/SunnyCapturer/tree/master/1st_FLIPPED-Screenshot-Code)
  4. Original repo: [FLIPPED](https://github.com/SunnyCapturer/FLIPPED)
  5. Total released versions: [releases](https://github.com/SunnyCapturer/FLIPPED/releases) V0.1 - V1.3.2 (≥ 27 versions)

- Screenshot:

   <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2025/202510220033729.png" width="80%"/>
