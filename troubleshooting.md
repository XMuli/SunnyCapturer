Below are common errors you might encounter while using SunnyCapturer and their solutions.  



## Windows  

**Error: Missing api-ms-win-crt-runtime-l1-1-0.dll upon running**  

- Your operating system lacks the required Microsoft VC++ runtime library. Depending on your system version (32-bit/64-bit), download and install the appropriate version of the **Microsoft Visual C++ 2015-2022 Redistributable Package**:  [32-bit](https://aka.ms/vs/17/release/vc_redist.x86.exe) | [64-bit](https://aka.ms/vs/17/release/vc_redist.x64.exe)  



**Windows SmartScreen has blocked an unrecognized app**  

- Click "More Info" and then "Run Anyway."  
- This occurs because SunnyCapturer is not digitally signed with an enterprise certificate. If you downloaded it from the [official website](https://sunnycapturer.xmuli.tech), it is safe to use.  However, a personal code signing certificate has been applied. Once this version accumulates a sufficient number of users on Microsoft Windows, this prompt will no longer appear.



## Linux  

?> For Debian-based distributions, `.deb` offline packages are available. For other Linux distributions, use the `.AppImage` package. Note: SunnyCapturer only supports the `x11` architecture and does not support `Wayland` yet.  



> Debian-based distributions: Use `.deb` packages whenever possible.  

**White screen appears after taking a screenshot on Ubuntu 24.04**  

- By default, Ubuntu 24.04 uses `Wayland`, but SunnyCapturer only supports `x11`. To resolve this, switch Ubuntu to `x11` mode:  Edit the configuration file:  [#55](https://github.com/XMuli/SunnyCapturer/issues/55)  
  ```bash
  sudo vim /etc/gdm3/custom.conf
   # at the beginning of the line WaylandEnable=false to uncomment it, then save the file. 
  sudo systemctl restart gdm3  // reboot the system
  ```



**OCR and image translation services are unresponsive on Ubuntu 20.04**  

- The built-in SSL version of Ubuntu 20.04 is outdated, resulting in the following errors:  
  ```  
  qt.tlsbackend.ossl: Incompatible version of OpenSSL (built with OpenSSL >= 3.x, runtime version is < 3.x)  
  qt.network.ssl: Active TLS backend does not support key creation  
  qt.network.ssl: The backend "cert-only" does not support QSslSocket  
  qt.network.ssl: The backend named "cert-only" does not support TLS  
  ```
  Solution: Upgrade the SSL library to version 3.0 or above, or update your OS to Ubuntu 22.04/24.04+.  



> Non-Debian distributions: Use `.AppImage` packages.  

**Changes to configuration files are not retained in `.AppImage` packages**  

- This is a limitation of the `.AppImage` format. Configuration files located within the AppDir cannot be modified. While this issue has no current resolution, it does not affect the functionality of the software.  



## macOS  

?> Version 1.x–2.x supports Intel chips only, while version 3.x supports both Apple M chips and Intel chips.  



**Cannot install the .dmg file**  

- Try right-clicking the file and selecting "Open."  
- After opening the `.dmg` file, drag the `.app` file into the `Finder` -> `Applications` folder.  



**Only the desktop is visible during screenshots**  

- macOS 10.14 and later require screen recording permissions. Go to `System Preferences` -> `Security & Privacy` -> `Privacy` -> `Screen Recording` and add SunnyCapturer.  
- If SunnyCapturer is already checked but the issue persists, uncheck it, quit SunnyCapturer, re-enable the permission, and then restart SunnyCapturer.  