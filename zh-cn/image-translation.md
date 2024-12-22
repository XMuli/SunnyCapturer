将图片里面的源语言文字，翻译为另一种目标语言文字的图片展示出来；仅支持云引擎翻译。
> 对于 『图片翻译』(Image Translattion)功能，现已支持使用私人账号，可自行输入

<!-- ## 在线图片翻译 -->

位置：`Setting - Recogniton - Picture translation`

当前一共支持的引擎： 腾讯云，百度云，有道云线 （ `Tencent Cloud`，`Baidu Cloud`，`Youdao Cloud`）

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201715434.png" width="60%"/>



创建自己的 key：

## 腾讯云 | Tencent Cloud

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

  
## 百度云 | Baidu Cloud

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

  



## 有道云 | Youdao Cloud

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

  

