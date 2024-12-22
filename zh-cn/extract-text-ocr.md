?> 对于『提取文字』(Extract Text - OCR)功能，现已支持使用私人账号，可自行输入;

> OCR： 从图片中提取文字

##  在线 OCR

> 通过多种云引擎，在线提取图片中的文字

位置：`Setting - Recogniton - Extract text`

当前一共支持的引擎： 腾讯云，百度云，本地离线 （ `Tencent Cloud`，`Baidu Cloud`，`Offline Local`）

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201722358.png" width="40%"/>



?> 使用私人账号的 key:

#### 腾讯云 | Tencent Cloud
- 网址：https://cloud.tencent.com
- 密钥生成：https://console.cloud.tencent.com/cam/capi
- 注册位置：

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201706427.png" width="40%"/>

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201708240.png" width="40%"/>

- 免费额度，每个月 1 号重置

  文字翻译字符数-500w免费包， 是完全足够的

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201713474.png" width="70%"/>

<br>

#### 百度云 | Baidu Cloud

- 网址：https://cloud.baidu.com

- 注册位置：

  <img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2024/202410201654050.png" width="40%"/>

- 免费额度 3500次/月，每个月 1 号重置

  | API                            | 调用量限制    | QPS/并发限制 |
  | ------------------------------ | ------------- | ------------ |
  | 通用文字识别（标准版）         | 1000次/月赠送 | 2            |
  | 通用文字识别（标准含位置版）   | 1000次/月赠送 | 2            |
  | 通用文字识别（高精度版）       | 1000次/月赠送 | 2            |
  | 通用文字识别（高精度含位置版） | 500次/月赠送  | 2            |

<br>

## 离线 OCR (CPU & GPU)

> 通过本地离线引擎，离线提取图片中的文字

- 无需要任何注册和 key 的输入
- 全程断网离线本地运行和识别
- 支持 CPU 和 GPU 模式（叉腰）：
  - CPU 版本：通用性强，占用内存更少，对于单张图片解析快，批量图片耗时大（普通用户推荐）
  - GPU 版本：仅支持 N 卡，占用内用多，但对于批量解析大量图片，耗时是 CPU 版本是 1/2 ~ 1/3 时间，很快（高级 N 卡推荐）
  - 仅支持 win 64 bit 系统；均支持单张图片和批量图片识别解析，直接拖曳到窗口即可。
