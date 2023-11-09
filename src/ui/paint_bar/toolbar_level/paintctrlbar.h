#ifndef PAINTCTRLBAR_H
#define PAINTCTRLBAR_H

#include <QPointer>
#include <QWidget>
#include <QSlider>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFontComboBox>
#include "colorpicker.h"
#include "common/absbtnsctrl.h"
#include "ocr_text/xocrtextctrl.h"
#include "ocr_translate/ocrtranslatectrl.h"
#include "paintbarhelper.h"

class PaintCtrlBar : public QWidget
{
    Q_OBJECT
public:
    explicit PaintCtrlBar(const int& colorPickerIconSize, const Qt::Orientation& orie = Qt::Horizontal, QWidget *parent = nullptr);
    virtual ~PaintCtrlBar();

private:
    void initUI();
    void initBtns();
    void hideAllBtnsCtrl();
    int  btnIdIschecked(const PaintType& type, const bool &isCheckable, const bool &isChecked);
    AbsBtnsCtrl *initSliderCtrl();
    AbsBtnsCtrl *initOcrTranslateCtrl();

    void  setCurrMosaicBtnfuzzyValue();
    void addWidget(QWidget *w, const bool& bAddSpaceLine = true, int stretch = 0, Qt::Alignment alignment = Qt::AlignCenter);

signals:
    void sigPaintCtrlIdReleased(const int& id);
    void sigTextCtrlToggled(const TextFlags& flages);
    void sigPointCtrlReleased(const int& id);                               // PaintCtrlBar PointCtrl 被按下
    void sigMosaicSliderValueChanged(int id, int val);                      // PaintCtrlBar Slider 数值被发生改变时候
    void sigPickedColor(const QColor& color);
    void sigTextFontFamilyChanged(const QFont &font);                       // PaintCtrlBar 字体 被发生改变时候
    void sigTextFontSizeChanged(const QString &fontSize);                   // PaintCtrlBar 字体大小 数值被发生改变时候
    void sigOcrTranslateCtrlIdReleased(const OcrTranslateData& data);       // 传递 OCR 翻译
    void sigOcrTextCtrlIdReleased(const OcrTextData& data);                 // 传递 OCR Text

private slots:
    // 传递 OCR 翻译具体
    void onOcrTranslateStatusChanged(bool checked);
    void onOcrTranslateCopy(bool checked);
    void onCbbFromCurrentTextChanged(const QString &text);
    void onCbbToCurrentTextChanged(const QString &text);
    // 传递 OCR Text

public slots:
    void onIdReleased(int id);
    void onTextCtrlToggled(int id, bool checked);
    void onMosaicCtrlIdReleased(int id);
    void onPaintBtnRelease(const PaintType& type, const bool &isCheckable, const bool &isChecked);
    void onSetTextFontSizeComboBoxValue(const QString &fontSize);     // sync 修改下拉列表的字体的大小

private:
    QBoxLayout*                   m_layout;
    Qt::Orientation               m_orie;
    OcrTranslateData              m_ocrTranslateDate;
    OcrTextData                   m_ocrTextDate;

    QPointer<AbsBtnsCtrl>         m_rectCtrl;                      // 矩形
    QPointer<AbsBtnsCtrl>         m_ellipseCtrl;                   // 圆形
    QPointer<AbsBtnsCtrl>         m_arrowCtrl;                     // 箭头
    QPointer<AbsBtnsCtrl>         m_markerPenCtrl;                 // 记号笔
    QPointer<AbsBtnsCtrl>         m_mosaicCtrl;                    // 马赛克
    QPointer<AbsBtnsCtrl>         m_textCtrl;                      // 文本
    QPointer<AbsBtnsCtrl>         m_serialCtrl;                    // 序号图形
    QPointer<AbsBtnsCtrl>         m_pointCtrl;                     // 线宽
    QPointer<OcrTranslateCtrl>    m_ocrTranslateCtrl;                  // OCR 翻译
    QPointer<AbsBtnsCtrl>         m_ocrTextCtrl;                       // OCR 转换为文本
    QPointer<ColorPicker>         m_colorPicker;                   // 取色板
    QPointer<QFontComboBox>       m_fontFamily;
    QPointer<QComboBox>           m_fontScale;
    QPointer<AbsBtnsCtrl>         m_mosaicSliderCtrl;
};

#endif // PAINTCTRLBAR_H
