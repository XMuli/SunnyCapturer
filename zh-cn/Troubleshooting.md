# 介绍

This is a normal page, which contains VuePress basics.

## 简介

You can add markdown files in your vuepress directory, every markdown file will be converted to a page in your site.

See [routing][] for more details.



## 安装

界面安装

静默安装

Every markdown file [will be rendered to HTML, then converted to a Vue SFC][content].

VuePress support basic markdown syntax and [some extensions][synatex-extensions], you can also [use Vue features][vue-feature] in it.

## 国际化（多语言）

VuePress use a `.vuepress/config.js`(or .ts) file as [site configuration][config], you can use it to config your site.

For [client side configuration][client-config], you can create `.vuepress/client.js`(or .ts).

Meanwhile, you can also add configuration per page with [frontmatter][].




## 图片翻译（Image Translattion）

位置：`Setting - Recogniton - Picture translation`

当前一共支持的引擎： 腾讯云，百度云，有道离线 （ `Tencent Cloud`，`Baidu Cloud`，`Youdao Cloud`）

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201715434.png" width="60%"/>



### 创建自己的 key：

#### Tencent Cloud | 腾讯云

- 同上，参见 OCR - Tencent Cloud 的设置

- 支持的语言，支持常见 14 种语言

  ```cpp
          {"zh", QObject::tr("Simplified Chinese")},       // 简体中文
          {"zh-TW", QObject::tr("Traditional Chinese")},   // 繁体中文
          {"en", QObject::tr("English")},                  // 英语
          {"ja", QObject::tr("Japanese")},                 // 日语
          {"ko", QObject::tr("Korean")},                   // 韩语
          {"ru", QObject::tr("Russian")},                  // 俄语
          {"fr", QObject::tr("French")},                   // 法语
          {"de", QObject::tr("German")},                   // 德语
          {"it", QObject::tr("Italian")},                  // 意大利语
          {"es", QObject::tr("Spanish")},                  // 西班牙语
          {"pt", QObject::tr("Portuguese")},               // 葡萄牙语
          {"ms", QObject::tr("Malay")},                    // 马来西亚语
          {"th", QObject::tr("Thai")},                     // 泰语
          {"vi", QObject::tr("Vietnamese")}                // 越南语
  ```

  



#### Baidu Cloud | 百度云

- 注册位置：参考上面，最后一步选择 `机器翻译`

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201726512.png" width="100%"/>

- 免费额度

  总量1万次赠送，有效期为 1 年内；QPS/并发限制 为  5；

- 支持的语言，支持主流 20 种语言

  ```cpp
          {"en", QObject::tr("English")},                  // 英语
          {"zh", QObject::tr("Simplified Chinese")},       // 简体中文
          {"jp", QObject::tr("Japanese")},                 // 日语
          {"kor", QObject::tr("Korean")},                  // 韩语
          {"pt", QObject::tr("Portuguese")},               // 葡萄牙语
          {"fra", QObject::tr("French")},                  // 法语
          {"de", QObject::tr("German")},                   // 德语
          {"it", QObject::tr("Italian")},                  // 意大利语
          {"spa", QObject::tr("Spanish")},                 // 西班牙语
          {"ru", QObject::tr("Russian")},                  // 俄语
          {"nl", QObject::tr("Dutch")},                    // 荷兰语 +
          {"may", QObject::tr("Malay")},                   // 马来语 +
          {"dan", QObject::tr("Danish")},                  // 丹麦语 +
          {"swe", QObject::tr("Swedish")},                 // 瑞典语 +
          {"id", QObject::tr("Indonesian")},               // 印尼语 +
          {"pl", QObject::tr("Polish")},                   // 波兰语 +
          {"rom", QObject::tr("Romanian")},                // 罗马尼亚语 +
          {"tr", QObject::tr("Turkish")},                  // 土耳其语 +
          {"el", QObject::tr("Greek")},                    // 希腊语 +
          {"hu", QObject::tr("Hungarian")}                 // 匈牙利语 +
  ```

  



#### Youdao Cloud | 有道云

- 官网：https://ai.youdao.com

- 注册位置

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201731545.png" width="80%"/>

- 无任何免费额度，都需要付费才能使用

- 支持的语言，支持 103 种语言，比较丰富，即使是部分小语种支持

  ```cpp
          {"ar", QObject::tr("Arabic")},                      // 阿拉伯语
          {"de", QObject::tr("German")},                      // 德语
          {"en", QObject::tr("English")},                     // 英语
          {"es", QObject::tr("Spanish")},                     // 西班牙语
          {"fr", QObject::tr("French")},                      // 法语
          {"hi", QObject::tr("Hindi")},                       // 印地语
          {"id", QObject::tr("Indonesian")},                  // 印度尼西亚语
          {"it", QObject::tr("Italian")},                     // 意大利语
          {"ja", QObject::tr("Japanese")},                    // 日语
          {"ko", QObject::tr("Korean")},                      // 韩语
          {"nl", QObject::tr("Dutch")},                       // 荷兰语
          {"pt", QObject::tr("Portuguese")},                  // 葡萄牙语
          {"ru", QObject::tr("Russian")},                     // 俄语
          {"th", QObject::tr("Thai")},                        // 泰语
          {"vi", QObject::tr("Vietnamese")},                  // 越南语
          {"zh-CHS", QObject::tr("Simplified Chinese")},      // 简体中文
          {"zh-CHT", QObject::tr("Traditional Chinese")},     // 繁体中文
          {"af", QObject::tr("Afrikaans")},                   // 南非荷兰语
          {"az", QObject::tr("Azeerbaijani")},                // 阿塞拜疆语
          {"be", QObject::tr("Belarusian")},                  // 白俄罗斯语
          {"bg", QObject::tr("Bulgarian")},                   // 保加利亚语
          {"bn", QObject::tr("Bangla")},                      // 孟加拉语
          {"bs", QObject::tr("Bosnian (Latin)")},             // 波斯尼亚语
          {"ca", QObject::tr("Catalan")},                     // 加泰隆语
          {"ceb", QObject::tr("Cebuano")},                    // 宿务语
          {"co", QObject::tr("Corsican")},                    // 科西嘉语
          {"cs", QObject::tr("Czech")},                       // 捷克语
          {"cy", QObject::tr("Welsh")},                       // 威尔士语
          {"da", QObject::tr("Danish")},                      // 丹麦语
          {"el", QObject::tr("Greek")},                       // 希腊语
          {"eo", QObject::tr("Esperanto")},                   // 世界语
          {"et", QObject::tr("Estonian")},                    // 爱沙尼亚语
          {"eu", QObject::tr("Basque")},                      // 巴斯克语
          {"fa", QObject::tr("Persian")},                     // 波斯语
          {"fi", QObject::tr("Finnish")},                     // 芬兰语
          {"fy", QObject::tr("Frisian")},                     // 弗里西语
          {"ga", QObject::tr("Irish")},                       // 爱尔兰语
          {"gd", QObject::tr("Scots Gaelic")},                // 苏格兰盖尔语
          {"gl", QObject::tr("Galician")},                    // 加利西亚语
          {"gu", QObject::tr("Gujarati")},                    // 古吉拉特语
          {"ha", QObject::tr("Hausa")},                       // 豪萨语
          {"haw", QObject::tr("Hawaiian")},                   // 夏威夷语
          {"he", QObject::tr("Hebrew")},                      // 希伯来语
          {"hr", QObject::tr("Croatian")},                    // 克罗地亚语
          {"ht", QObject::tr("Haitian Creole")},              // 海地克里奥尔语
          {"hu", QObject::tr("Hungarian")},                   // 匈牙利语
          {"hy", QObject::tr("Armenian")},                    // 亚美尼亚语
          {"ig", QObject::tr("Igbo")},                        // 伊博语
          {"is", QObject::tr("Icelandic")},                   // 冰岛语
          {"jw", QObject::tr("Javanese")},                    // 爪哇语
          {"ka", QObject::tr("Georgian")},                    // 格鲁吉亚语
          {"kk", QObject::tr("Kazakh")},                      // 哈萨克语
          {"km", QObject::tr("Khmer")},                       // 高棉语
          {"kn", QObject::tr("Kannada")},                     // 卡纳达语
          {"ku", QObject::tr("Kurdish")},                     // 库尔德语
          {"ky", QObject::tr("Kyrgyz")},                      // 柯尔克孜语
          {"la", QObject::tr("Latin")},                       // 拉丁语
          {"lb", QObject::tr("Luxembourgish")},               // 卢森堡语
          {"lo", QObject::tr("Lao")},                         // 老挝语
          {"lt", QObject::tr("Lithuanian")},                  // 立陶宛语
          {"lv", QObject::tr("Latvian")},                     // 拉脱维亚语
          {"mg", QObject::tr("Malagasy")},                    // 马尔加什语
          {"mi", QObject::tr("Maori")},                       // 毛利语
          {"mk", QObject::tr("Macedonian")},                  // 马其顿语
          {"ml", QObject::tr("Malayalam")},                   // 马拉雅拉姆语
          {"mn", QObject::tr("Mongolian")},                   // 蒙古语
          {"mr", QObject::tr("Marathi")},                     // 马拉地语
          {"ms", QObject::tr("Malay")},                       // 马来语
          {"mt", QObject::tr("Maltese")},                     // 马耳他语
          {"my", QObject::tr("Myanmar (Burmese)")},           // 缅甸语
          {"ne", QObject::tr("Nepali")},                      // 尼泊尔语
          {"no", QObject::tr("Norwegian")},                   // 挪威语
          {"ny", QObject::tr("Nyanja (Chichewa)")},           // 齐切瓦语
          {"pa", QObject::tr("Punjabi")},                     // 旁遮普语
          {"pl", QObject::tr("Polish")},                      // 波兰语
          {"ps", QObject::tr("Pashto")},                      // 普什图语
          {"ro", QObject::tr("Romanian")},                    // 罗马尼亚语
          {"sd", QObject::tr("Sindhi")},                      // 信德语
          {"si", QObject::tr("Sinhala (Sinhalese)")},         // 僧伽罗语
          {"sk", QObject::tr("Slovak")},                      // 斯洛伐克语
          {"sl", QObject::tr("Slovenian")},                   // 斯洛文尼亚语
          {"sm", QObject::tr("Samoan")},                      // 萨摩亚语
          {"sn", QObject::tr("Shona")},                       // 修纳语
          {"so", QObject::tr("Somali")},                      // 索马里语
          {"sq", QObject::tr("Albanian")},                    // 阿尔巴尼亚语
          {"sr-Cyrl", QObject::tr("Serbian (Cyrillic)")},     // 塞尔维亚语(西里尔文)
          {"sr-Latn", QObject::tr("Serbian (Latin)")},        // 塞尔维亚语(拉丁文)
          {"st", QObject::tr("Sesotho")},                     // 塞索托语
          {"su", QObject::tr("Sundanese")},                   // 巽他语
          {"sv", QObject::tr("Swedish")},                     // 瑞典语
          {"sw", QObject::tr("Kiswahili")},                   // 斯瓦希里语
          {"ta", QObject::tr("Tamil")},                       // 泰米尔语
          {"te", QObject::tr("Telugu")},                      // 泰卢固语
          {"tg", QObject::tr("Tajik")},                       // 塔吉克语
          {"tl", QObject::tr("Filipino")},                    // 菲律宾语
          {"tr", QObject::tr("Turkish")},                     // 土耳其语
          {"uk", QObject::tr("Ukrainian")},                   // 乌克兰语
          {"ur", QObject::tr("Urdu")},                        // 乌尔都语
          {"uz", QObject::tr("Uzbek")},                       // 乌兹别克语
          {"xh", QObject::tr("Xhosa")},                       // 科萨语
          {"yi", QObject::tr("Yiddish")},                     // 意第绪语
          {"yo", QObject::tr("Yoruba")},                      // 约鲁巴语
          {"zu", QObject::tr("Zulu")},                        // 祖鲁语
  ```

  





## 文字提取 OCR



# 疑难解答

## Windows

### Windows protected your PC

此EXE 已通过 Microsoft Defender SmartScreen 的病毒检测，仍还需足够多的安装量积累时，在随着时间增加，积累的Microsoft信誉到达阈值，此窗口便会取消。

若是文章对你有价值，亦可帮我积累Sunny的微软信誉，或者在Linux 商店的好评，甚至感谢🙇‍ ； Windows 用户推荐的下载





# How to Enable Offline OCR

XMuli edited this page last week · [3 revisions](https://github.com/XMuli/SunnyPages/wiki/7.-How-to-Enable-Offline-OCR/_history)

当前离线版本 OCR 仅支持 Windows 64 bit 版本，分为 CPU 和 GPU 版本

Offline Local | 离线本地

- 无需要任何注册和 key 的输入
- 全程断网离线本地运行和识别
- 支持 CPU 和 GPU 模式（叉腰）：
  - CPU 版本：通用性强，占用内存更少，对于单张图片解析快，批量图片耗时大（普通用户推荐）
  - GPU 版本：仅支持 N 卡，占用内用多，但对于批量解析大量图片，耗时是 CPU 版本是 1/2 ~ 1/3 时间，很快（高级 N 卡推荐）
- 仅支持 win 64 bit 系统；均支持单张图片和批量图片识别解析，直接拖曳到窗口即可。

一种简单的下载方式：下载带有`_offline_ocr_cpu` `_offline_ocr_gpu` 字样的离线包即可。如：

- [sunny_setup_2.3.0_x64_offline_ocr_cpu.exe](https://github.com/XMuli/SunnyPages/releases/download/v2.3/sunny_setup_2.3.0_x64_offline_ocr_cpu.exe)
- [sunny_setup_2.3.0_x64_offline_ocr_gpu.exe](https://github.com/XMuli/SunnyPages/releases/download/v2.3/sunny_setup_2.3.0_x64_offline_ocr_gpu.exe)
- [sunny_protable_2.3.0_x64_offline_ocr_cpu.zip](https://github.com/XMuli/SunnyPages/releases/download/v2.3/sunny_protable_2.3.0_x64_offline_ocr_cpu.zip)
- [sunny_protable_2.3.0_x64_offline_ocr_gpu.zip](https://github.com/XMuli/SunnyPages/releases/download/v2.3/sunny_protable_2.3.0_x64_offline_ocr_gpu.zip)



# Error Code | 错误码返回



> 翻译功能 -> 和网络有关，或者输入自己的 YouDao API Token 看图识别功能 -> 和网络有关，或者输入自己的 BaiDu APIToken

可能原因：

- PROXY 不正确，请尝试切换节点
- API Token 内置作者账号的费用透支了，需要充值
- API Token 使用私人账号的也没钱了，需要自行购买额度





## MacOS

### MacOS 无法截屏，没有权限

Here are common configuration controlling layout of `@vuepress/theme-default`:

- [navbar][]
- [sidebar][]

Check [default theme docs][default-theme] for full reference.

You can [add extra style][style] with `.vuepress/styles/index.scss` file.

[routing]: https://vuejs.press/guide/page.html#routing
[content]: https://vuejs.press/guide/page.html#content
[synatex-extensions]: https://vuejs.press/guide/markdown.html#syntax-extensions
[vue-feature]: https://vuejs.press/guide/markdown.html#using-vue-in-markdown
[config]: https://vuejs.press/guide/configuration.html#client-config-file
[client-config]: https://vuejs.press/guide/configuration.html#client-config-file
[frontmatter]: https://vuejs.press/guide/page.html#frontmatter
[navbar]: https://vuejs.press/reference/default-theme/config.html#navbar
[sidebar]: https://vuejs.press/reference/default-theme/config.html#sidebar
[default-theme]: https://vuejs.press/reference/default-theme/
[style]: https://vuejs.press/reference/default-theme/styles.html#style-file
