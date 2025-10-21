// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XGLOBAL_H
#define XGLOBAL_H

//#define _MYDEBUG // 调试

// 1. HAIF_INTERVAL 为一半间隔，边框宽度一半高; 2 * HAIF_INTERVAL 为边框的宽度，为 getOuterRect - getRect  == getRect - getInnerRect == HAIF_INTERVAL
// 2. HAIF_INTERVAL 为边框一般的灵敏度，光标移动到上面便会变化形态
#define HAIF_INTERVAL 4
#define HAIF_R_BORDER_MARK 4               // 边框上标记点的半径

// 截图-选中矩形
//#define SELECT_RECT_PEN_WIDTH  1           // 选中矩形的边框, 其画笔的默认宽度
#define SELECT_ASSIST_RECT_PEN_WIDTH  4    // 四角落辅助的 ∟，其画笔的默认宽度 = 矩形的边框 + 4
#define SELECT_ASSIST_RECT_WIDTH  30       // 四角周辅助的 ∟ 的宽度
#define SELECT_ASSIST_RECT_HEIGHT 30       // 四角周辅助的 ∟ 的高度

// 默认图片宽度、高度 px （再乘以缩放比）
#define ICON_WIDTH  24
#define ICON_HEIGHT 24

#define ICON_WIDTH_IN_MARGIN  5
#define ICON_HEIGHT_IN_MARGIN  5


// 上面、下面两个 SelectBar、 ParameterBar 的一些公共参数
//#define B_SPACER_LINE_WIDTH  1            // 分割线的宽度
#define B_SPACER_LINE_HEIGHT 20             // 分割线的高度
#define B_RADIRS 6                          // 圆角矩形的圆角大小 eg: 绘画工具栏
#define B_SPACEING 15                       // 圆角矩形的圆角大小 eg: 绘画工具栏 (没有 * 比例)
                                            
// 顶部 SelectBar 中的项目                   
#define SB_MARGIN_HOR 10                    // 两个 bar； 选取的最小边框. 左右间距
#define SB_MARGIN_VER 8                     // 两个 bar； 选取的最小边框. 上下间距  --> 取色盘 不显示时
                                            
#define SB_ITEM_SPACE 10                    // item 之间间隔

// 底部 ParameterBar 中的项目
#define PB_ITEM_SPACE 0
#define PB_MARGIN_HOR 0
#define PB_MARGIN_VER 0

// 底部 ParameterBar 中的子项, 如 ManageBar 的间距和一些参数
#define MB_ITEM_SPACE 0                    // ManageBar 中 item 的间距
#define MB_MARGIN_HOR 10                   // ManageBar 选取的最小边框. 左右间距
#define MB_MARGIN_VER 8                    // ManageBar 选取的最小边框. 上下间距 

// 取色盘 ColorParaBar 的八个圆 
  // Mode: CPB_ParaBar --> QGridLayout
#define CPB_PB_LABEL_WIDTH 12              // 圆形取色盘宽度
#define CPB_PB_MARGIN_HOR 10               // 取色盘, 圆形按钮的边框 左右 距离
#define CPB_PB_MARGIN_VER 6                // 取色盘, 圆形按钮的边框 上下 距离
#define CPB_PB_SPACING_HOR  4              // 取色盘, 圆形按钮之间的水平间距
#define CPB_PB_SPACING_VER  4              // 取色盘, 圆形按钮之间的竖直间距

  // Mode: CPB_HighLight  -> QHBoxLayout
#define CPB_HL_LABEL_WIDTH 16              // 圆形取色盘宽度
#define CPB_HL_MARGIN_HOR 10               // 取色盘, 圆形按钮的边框 左右 距离
#define CPB_HL_MARGIN_VER 6                // 取色盘, 圆形按钮的边框 上下 距离
#define CPB_HL_SPACING_HOR  5              // 取色盘, 圆形按钮之间的水平间距
#define CPB_HL_SPACING_VER  0              // 取色盘, 圆形按钮之间的竖直间距

  // 其中的外圆
#define CPB_MARGIN_SELECTED  2             // 取色盘, 选中颜色的内圆距离外框圆的距离
#define CPB_WIDTH_SELECTED  2              // 取色盘, 外圆画笔的宽度
                                           
// 几个 combobox 的详细（序号、大小）        
#define COMBOBOX_WIDTH 48                  // combobox 宽度
#define COMBOBOX_HEIGHT 20                 // combobox 宽度

// 选择线宽度 WidthParaBar 的 3 个圆，已经替换，后面可以删
#define WIDTH_LABEL_WIDTH 20               // 色盘宽度
#define WIDTH_PARA_MARGIN 0                // 等大的小圆形按钮的边框距离
#define WIDTH_PARA_HOR_SPACING  0          // 等大的小圆形按钮之间的水平间距
#define WIDTH_PARA_VER_SPACING  0          // 等大的小圆形按钮之间的竖直间距

// 左上角用来显示，选中矩形区域 SelectSize 的大小
#define SS_MARGIN_LEFT 3                   // 边框左部的间距
#define SS_MARGIN_RIGHT 3                  // 边框右部的间距
#define SS_MARGIN_TOP 3                    // 边框顶部的间距
#define SS_MARGIN_BOTTOM 3                 // 边框底部的间距
#define SS_SPACE_TO_SELECTRECT 10          // 此部件 底部距离选中框的顶部的距离
#define SS_RADIRS 2                        // 此部件 圆角矩形的圆角
//#define SS_MARGIN_BOTTOM 3               // 边框底部的间距
//#define SS_HOR_SPACING  0                // 水平间距
//#define SS_VER_SPACING  0                // 竖直间距
                                            




// 偏好主 UI 界面 Preference 缩写 PRE_
#define PRE_MARGIN_HOR 0               // 布局到水平间隔
#define PRE_MARGIN_VER 0               // 布局到垂直间隔

// Tab General QVBoxLayout: 缩写 TIV_     
#define TGV_MARGIN_HOR  21              // QVBoxLayout 的水平间隔
#define TGV_MARGIN_VER_TOP  16          // QVBoxLayout 的 top 间隔
#define TGV_MARGIN_VER_BOTTOM  14       // QVBoxLayout 的 bottom 间隔
#define TGG_SPACING_VER  16             // GridLayout 的 ver spacing
#define TGG_SPACING_HOR  7              // GridLayout 的 hor spacing

// Tab Output QVBoxLayout: 缩写 TOV_     
#define TOV_MARGIN_HOR  21              // QVBoxLayout 的水平间隔
#define TOV_MARGIN_VER_TOP  14          // QVBoxLayout 的 top 间隔
#define TOV_MARGIN_VER_BOTTOM  14       // QVBoxLayout 的 bottom 间隔
#define TOG_SPACING_VER  7             // GridLayout 的 ver spacing
#define TOG_SPACING_HOR  7              // GridLayout 的 hor spacing

// Tab Pin QVBoxLayout: 缩写 TPV_     
#define TPV_MARGIN_HOR  21              // QVBoxLayout 的水平间隔
#define TPV_MARGIN_VER_TOP  17          // QVBoxLayout 的 top 间隔
#define TPV_MARGIN_VER_BOTTOM  14       // QVBoxLayout 的 bottom 间隔
#define TPG_SPACING_VER  13             // GridLayout 的 ver spacing
#define TPG_SPACING_HOR  7              // GridLayout 的 hor spacing

// Tab Interface QVBoxLayout: 缩写 TIV_     
#define TIV_MARGIN_HOR  21              // QVBoxLayout 的水平间隔
#define TIV_MARGIN_VER_TOP  16          // QVBoxLayout 的 top 间隔
#define TIV_MARGIN_VER_BOTTOM  14       // QVBoxLayout 的 bottom 间隔
#define TIG_SPACING_VER  10             // GridLayout 的 ver spacing
#define TIG_SPACING_HOR  7              // GridLayout 的 hor spacing

// Tab Hotkeys QVBoxLayout: 缩写 THV_
#define THV_MARGIN_HOR  21              // QVBoxLayout 的水平间隔
#define THV_MARGIN_VER_TOP  14          // QVBoxLayout 的 top 间隔
#define THV_MARGIN_VER_BOTTOM  14       // QVBoxLayout 的 bottom 间隔
#define THG_SPACING_VER  13             // GridLayout 的 ver spacing
#define THG_SPACING_HOR  7              // GridLayout 的 hor spacing

// Tab About QVBoxLayout: 缩写 TAV_     
#define TAV_MARGIN_HOR  21              // QVBoxLayout 的水平间隔
#define TAV_MARGIN_VER_TOP  21          // QVBoxLayout 的 top 间隔
#define TAV_MARGIN_VER_BOTTOM  25       // QVBoxLayout 的 bottom 间隔
#define TAG_SPACING_VER  0             // GridLayout 的 ver spacing
#define TAG_SPACING_HOR  12              // GridLayout 的 Hor spacing


// PinWidget 
#define PW_MARGIN 7                    // pinwidgt 对应的 shadow 宽度
#endif // XGLOBAL_H


// SerialNumber pen.widthF()
#define SN_Min 12
#define SN_Max 100
