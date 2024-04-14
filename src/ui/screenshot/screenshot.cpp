// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "screenshot.h"
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QIcon>
#include <QMessageBox>
#include <QClipboard>
#include <QDateTime>
#include <QFileDialog>
#include <QCursor>
#include <QFont>
#include <QShortcut>
#include <QTextCharFormat>
#include <QKeySequence>
#include "xtextedit.h"
#include "../paint_bar/toolbar_level/pin/pinwidget.h"
#include "../../data/configmanager.h"

QFont ScreenShot::m_textFont = QFont("Microsoft YaHei", 16);

ScreenShot::ScreenShot(const HotKeyType &type, const Qt::Orientation &orie, QWidget *parent)
    : QWidget(parent)
    , m_primaryScreen(qGuiApp->primaryScreen())
    , m_screens(qGuiApp->screens())
    , m_origPix()
    , m_vdRect()
    , m_bFistPressed(false)
    , m_bAutoDetectRect(CJ_GET("interface.auto_detect_windows").get<bool>())
    , m_HotKeyType(type)
    , m_actionType(ActionType::AT_wait)
    , m_paintBar(new PaintBar(orie, this))
    , m_networkOCR(new NetworkOCR(this))
    , m_stretchPickedRectOrieType(OrientationType::OT_empty)
    , m_orie(orie)
    , m_edit(new XTextEdit(this))
    , m_ocrTextEdit(new XOcrTextEdit(this))  // [不理解为什么添加 this 之后，按下快捷键 ctrl+c，会导致其父对象会被析构；已找到一个原因是设置为只读就会这样，但是可编辑则不会]
    , m_ocrDlg(new XOcrDlg(nullptr))
    , m_imgTranslateDlg(new ImageTranslateDlg(nullptr))
    , m_pointTips(new Tips("", TipsType::TT_point_changed_tips, this))
    , m_pickedRectTips(new Tips("", TipsType::TT_picked_rect_tips, this))
    , m_timerPoint(new QTimer(this))
{
    initUI();
    initConnect();

    if (m_actionType == ActionType::AT_wait) {
        if (m_bAutoDetectRect) {
            m_actionType = ActionType::AT_picking_detection_windows_rect;
            setMouseTracking(true);
        } else {
            m_actionType = ActionType::AT_picking_custom_rect;
            setMouseTracking(true);
        }
    }

    // qDebug() << "------#1---->" << hotKeyTypeToString(m_HotKeyType);
    const auto& customSizeEnable = CJ_GET("interface.custom_size_enable").get<bool>();
    const auto& topleftEnable = CJ_GET("interface.topleft_enable").get<bool>();
    const auto& sizeEnable = CJ_GET("interface.size_enable").get<bool>();

    if (m_HotKeyType == HotKeyType::HKT_capture) {
    } else if (m_HotKeyType == HotKeyType::HKT_delay_capture) {
        // 不给预设的初始化矩形即可
    } else if (m_HotKeyType == HotKeyType::HKT_custiom_capture) {


        if (customSizeEnable) {
            if (topleftEnable)
                m_node.p1 = QPoint(CJ_GET("interface.custom_rect_left").get<int>(), CJ_GET("interface.custom_rect_top").get<int>());

            if (sizeEnable) {
                m_node.pickedRect = QRect(m_node.p1, QSize(CJ_GET("interface.custom_rect_width").get<int>(), CJ_GET("interface.custom_rect_height").get<int>()));
                m_node.absoluteRect = m_node.pickedRect;
                m_node.p2 = m_node.p3 = m_node.pickedRect.bottomRight();
                m_actionType = ActionType::AT_wait;
                setMouseTracking(false);
            }

            if (topleftEnable || sizeEnable)
                m_bFistPressed = true;
        }

    } else {
    }


}


ScreenShot::~ScreenShot()
{
    preDestruction();
}

void ScreenShot::capture()
{
    originalPixmap();

#ifdef Q_OS_WIN
    show();
#else
    showFullScreen();         // Linux supports virtual multi-screen, Mac only one screen
#endif
}

void ScreenShot::btnPin()
{
#ifdef Q_OS_MAC
    setWindowFlags(Qt::Dialog);
    showNormal();
#endif

    const auto& rect = m_node.absoluteRect;

    PinWidget* w = new PinWidget(finishDrewPixmap(rect, true), nullptr);
    const int opacity = CJ_GET("pin.opacity");
    const int max = CJ_GET("pin.maximum_size");
    const int& margin = w->layoutMargin();
    w->setWindowOpacity(opacity / 100.0);
    w->setMaximumSize(QSize(max, max));
    w->resize(rect.size());
    w->move(mapToGlobal(rect.topLeft()) - QPoint(margin, margin)); // fix: 边框偏移给归位
    w->show();

    close();
}

void ScreenShot::btnUndo()
{
    if (m_redo.empty()) return;

    m_undo.emplace_back(std::move(m_redo.back())); // 在 m_undo 中构造新元素并移动
    m_redo.pop_back(); // 从 m_redo 中移除最后一个元素

    if (m_paintNode.pst == PaintShapeType::PST_text) {          // 撤销时候就隐藏
        XTextEdit* xTextEdit = m_undo.back().xTextEdit;
        if (xTextEdit) xTextEdit->hide();
    } else if (m_paintNode.pst == PaintShapeType::PST_serial) {  // 被撤销时，序号自然减去一
        if (m_paintNode.id == 0) {
            m_paintNode.serialNode.number--;
        } else if (m_paintNode.id == 1) {
            QChar serialChar = m_paintNode.serialNode.letter.unicode() - 1;
            m_paintNode.serialNode.letter = serialChar;
        }
    }

    autoDisableUndoAndRedo();
}

void ScreenShot::btnRedo()
{
    if (m_undo.empty()) return;

    m_redo.emplace_back(std::move(m_undo.back())); // 移动最后一个元素到 m_redo
    m_undo.pop_back(); // 从 m_undo 中移除最后一个元素

    if (m_paintNode.pst == PaintShapeType::PST_text) {          // 重做时候就显示
        XTextEdit* xTextEdit = m_redo.back().xTextEdit;
        if (xTextEdit) xTextEdit->show();
    }

    autoDisableUndoAndRedo();
}

void ScreenShot::btnSave()
{
    QString path = imageSavePath(ImageSaveType::IST_manual_save);
    imageSave(path);
}

void ScreenShot::btnCancel()
{
    close();
}

void ScreenShot::btnFinish()
{
    const QPixmap& pixmap = finishDrewPixmap(m_node.absoluteRect, true);

//    QPixmap tPix1 = finishDrewPixmap(m_node.absoluteRect);
//    QPixmap tPix2= finishDrewPixmap(m_node.absoluteRect);

//#if 1 // 测试效果代码
//    smoothMosaic(tPix1);
//    pixelatedMosaic(tPix2);

//    tPix1.save("D:/savePix1.png");
//    tPix2.save("D:/savePix2.png");
//#endif

    if (!pixmap.isNull()) {
        // 将新的QPixmap复制到剪贴板
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setPixmap(pixmap);  // 由于 Qt 库会在应用程序终止时释放剪贴板对象; 故此处内存增加是必然的，不用额外 delete 处理； 氪！排查这么久结论居然是这个

        // 同时自动保存
        if (CJ_GET("output.auto_save_enable").get<bool>()) {
            const auto& path = imageSavePath(ImageSaveType::IST_auto_save);
            pixmap.save(path, nullptr, CJ_GET("output.image_quailty").get<int>());
        }
    }

    close();
}

// 深拷贝的 QPixmap，但在此情况下，您需要确保在调用该函数后适当地处理返回的 QPixmap 对象，以防止内存泄漏。
// QPixmap 是 Qt 中的一个值对象，不需要手动分配或释放内存，它的内存管理是自动的。当QPixmap超出其作用域或不再被引用时，其内存将自动被释放。
// QPixmap m_finshPix = m_origPix 的深拷贝 + 在上面绘画一些图案
QPixmap ScreenShot::finishPixmap()
{
    setMosaicPix();
    return m_mosaicPix.copy(m_node.absoluteRect);
}

QPixmap ScreenShot::finishDrewPixmap(const QRect &rect, const bool& isDrawOnOrigPix)
{
    if (isDrawOnOrigPix) {

        if (m_ocrGeneratePix.isNull()) {
            QPainter pa(&m_origPix);
            pa.save();
            for (const auto& it : m_redo) drawShape(it, pa, true);
            //    drawShape(m_paintNode, pa, true);

            if (CJ_GET("advanced.develpe_enginner.save_image_with_detail").get<bool>())
                prinftWindowsRects(pa);
            pa.restore();
            return m_origPix.copy(rect);

        } else {
            return m_ocrGeneratePix.copy();
        }

    } else {
        QPixmap pix = m_origPix.copy(rect);  // 默认深拷贝全部大小
        QPainter pa(&pix);
        pa.save();
        for (const auto& it : m_redo) drawShape(it, pa);

        //    drawShape(m_paintNode, pa);
        pa.restore();
        return pix;
    }
}


void ScreenShot::showPointTips(const QString &text)
{
    const QScreen *scrn = currentScreen();
    if (!scrn) scrn = m_primaryScreen;

    m_pointTips->setText(text);
    m_pointTips->move(mapFromGlobal(scrn->geometry().topLeft()));
    m_pointTips->show();

    m_timerPoint->setSingleShot(true);
    m_timerPoint->stop();
    m_timerPoint->start();
    m_pointTips->update();
}

void ScreenShot::onPaintBtnRelease(const PaintType &type, const bool &isCheckable)
{
    static PaintType paintType = PaintType::PT_rectangle;

    if (isCheckable) {

        if (m_paintBar->hadDrawBtnsChecked()) {
            m_actionType = ActionType::AT_drawing_shap;

            if (paintType == PaintType::PT_img_translate && type != PaintType::PT_img_translate) {
                m_ocrGeneratePix = QPixmap();
                update();
            } else if (paintType == PaintType::PT_ocr && type != PaintType::PT_ocr) {
                m_ocrTextEdit->clear();
                m_ocrTextEdit->hide();
                update();
            }

            if (type == PaintType::PT_rectangle) {
                m_paintNode.pst = PaintShapeType::PST_rect;
            } else if (type == PaintType::PT_ellipse) {
                m_paintNode.pst = PaintShapeType::PST_ellipse;
            } else if (type == PaintType::PT_arrow) {
                m_paintNode.pst = PaintShapeType::PST_arrow;
            } else if (type == PaintType::PT_pencil) {
                m_paintNode.pst = PaintShapeType::PST_pen;
            } else if (type == PaintType::PT_marker_pen) {
                m_paintNode.pst = PaintShapeType::PST_marker_pen;
            } else if (type == PaintType::PT_mosaic) {
                m_paintNode.pst = PaintShapeType::PST_mosaic;
            } else if (type == PaintType::PT_text) {
                m_paintNode.pst = PaintShapeType::PST_text;
            } else if (type == PaintType::PT_serial) {
                m_paintNode.pst = PaintShapeType::PST_serial;
            } else {
                m_paintNode.pst = PaintShapeType::PST_empty;
            }
        } else {
            m_actionType = ActionType::AT_wait;
            setMouseTracking(true);
        }

    } else {

        if (type == PaintType::PT_pin) {
            btnPin();
        } else if (type == PaintType::PT_undo) {
            btnUndo();
            update();
            qDebug() << "----#a-->m_undo.size():" << m_undo.size() << "m_redo.size():" << m_redo.size();
        } else if (type == PaintType::PT_redo) {
            btnRedo();
            update();
            qDebug() << "----#b-->m_undo.size():" << m_undo.size() << "m_redo.size():" << m_redo.size();
        } else if (type == PaintType::PT_save) {
            btnSave();
        } else if (type == PaintType::PT_cancel) {
            btnCancel();
        } else if (type == PaintType::PT_finish) {
            btnFinish();
        } else {
            qDebug() << "type is unknow PaintType!";
        }
    }


    // 优化, 不使用马赛克功能时候，就释放相关内存
    if (type != PaintType::PT_mosaic && !m_mosaicPix.isNull())
        m_mosaicPix = QPixmap();

    paintType = type;
}

void ScreenShot::onPaintCtrlIdReleased(const int &id)
{
    m_paintNode.id = id;
}

void ScreenShot::onTextCtrlToggled(const TextFlags& flages)
{
    const bool& blod = flages & TextFlag::TF_blod;
    const bool& italic = flages & TextFlag::TF_italic;
    const bool& outline = flages & TextFlag::TF_outline;
    const bool& strikeout = flages & TextFlag::TF_strikeout;
    const bool& underline = flages & TextFlag::TF_underline;

    QTextCharFormat format = m_edit->currentCharFormat();
    const QColor foreground = format.foreground().color();
    format.setFontWeight(blod ? QFont::Bold : QFont::Normal);
    format.setFontItalic(italic);
    format.setTextOutline(QPen(outline ? Qt::white : foreground, 1));
    format.setFontStrikeOut(strikeout);
    format.setFontUnderline(underline);
    m_edit->mergeCurrentCharFormat(format);

//    if (flages & TextFlag::TF_blod) {
//    } else if (flages & TextFlag::TF_italic) {
//    } else if (flages & TextFlag::TF_outline) {
//    } else if (flages & TextFlag::TF_strikeout) {
//    } else if (flages & TextFlag::TF_underline) {
//    }
}

void ScreenShot::onPointCtrlReleased(const int &id)
{
    int pointWidth = 2;
    if (id == 0) {
        pointWidth = 4;
    } else if (id == 1) {
        pointWidth = 6;
    } else if (id == 2) {
        pointWidth = 10;
    }

    m_paintNode.pen.setWidth(pointWidth);
    showPointTips(QString::number(pointWidth));
}

void ScreenShot::onMosaicSliderValueChanged(int id, int val)
{
    if (id == 0) {
        m_paintNode.pixelatedFuzzy = val;
    } else if (id == 1) {
        m_paintNode.smoothFuzzy = val;
    }
}

void ScreenShot::onHidePointTips()
{
    if (m_pointTips)
        m_pointTips->hide();
    m_timerPoint->stop();
}

void ScreenShot::onUpdateToolBarBlurPixmap()
{
    showCustomWidget(m_paintBar);
}

void ScreenShot::onPickedColor(const QColor &color)
{
    m_paintNode.pen.setColor(color);
    m_paintNode.brush.setColor(color);

    QTextCharFormat format = m_edit->currentCharFormat();
    format.setForeground(QBrush(color));
    m_edit->mergeCurrentCharFormat(format);

    CJ_CD.pen.setColor(color);
    CJ_CD.brush.setColor(color);
}

void ScreenShot::onTextFontFamilyChanged(const QFont &font)
{
    m_textFont.setFamily(font.family());
    m_edit->setFont(m_textFont);
    CJ_CD.font = font;  // 可能改动
}

void ScreenShot::onTextFontSizeChanged(const QString &fontSize)
{
    const int& width = fontSize.toInt();
    setTextFontSize(0, width, false);
    CJ_CD.font.setPointSize(width);   // 可能改动
}

void ScreenShot::onOcrTranslateCtrlIdReleased(const ImgTranslateData &data)
{
    hide();
    if (!m_networkOCR || !data.bTranslate) {
        m_ocrGeneratePix = QPixmap();
        update();
        return;
    }

    const QString dir = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first() + "/ocr_origin/";
    QDir directory(dir);
    if (!directory.exists() && !directory.mkpath(dir)) {
        qDebug() << "Failed to create directory: " << dir;
    }

    const QString& path = dir + QString("OCRTranslate_%1_%2.png").arg(XPROJECT_NAME).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    const bool ok = imageSave(path, false);


    QFile file(path);
    if (ok && file.exists()) {

        if (data.channel == "youdao")
            m_networkOCR->sendYouDaoImgTranslateRequest(data, path);
        else if (data.channel == "baidu")
            m_networkOCR->sendBaiDuImgTranslateRequest(data, path);

    } else {
        // 文件不存在
        qDebug() << "OCR origin image file does not exist. path:" << path;
    }
}

// 将图片粘贴在当前截图的窗口上
void ScreenShot::onOCRImageGenerateFinsh(const QSize &size, const QString &path)
{
    hide();
    QFile file(path);
    if (file.exists() && size.isValid()) {
        m_ocrGeneratePix = QPixmap(size);
        m_ocrGeneratePix.load(path);
        update();
    }

    // 显示 图片翻译 弹窗
    const QPixmap& pixmap = finishDrewPixmap(m_node.absoluteRect, false);
    m_imgTranslateDlg->setLeftPixmap(pixmap);
    m_imgTranslateDlg->setRightPixmap(size, path);
    const QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    if (screen) {
        const auto& rect = screen->geometry();
        const QPoint& center = rect.center(); // 获取屏幕的中心坐标
        m_imgTranslateDlg->resize(rect.width() * 0.64, rect.height() * 0.64);
        m_imgTranslateDlg->move(center - QPoint(m_imgTranslateDlg->width() / 2, m_imgTranslateDlg->height() / 2));
    }

    if (!m_imgTranslateDlg->isVisible()) m_imgTranslateDlg->show();
    m_imgTranslateDlg->activateWindow();
    close();
}

void ScreenShot::onOCRTextCtrlIdReleased(const OcrData &data)
{
    // if (data.operate == OcrTextOperate::OTO_is_allow_edit) {
    //     m_ocrTextEdit->setReadOnly(!data.allowWrite);
    //     return;

    // } else if (data.operate == OcrTextOperate::OTO_text_copy) {

    //     QClipboard* clipboard = QApplication::clipboard();
    //     clipboard->setText(m_ocrTextEdit->toPlainText());
    //     return;

    // } else if (data.operate == OcrTextOperate::OTO_update) {
    //     // 直接刷新
    // }  else {
    //     qDebug() << "ScreenShot::onOCRTextCtrlIdReleased is OcrData is empty!";
    // }

    const QString dir = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first() + "/ocr_origin/";
    QDir directory(dir);
    if (!directory.exists() && !directory.mkpath(dir)) {
        qDebug() << "Failed to create directory: " << dir;
    }

    const QString& path = dir + QString("OCRText_%1_%2.png").arg(XPROJECT_NAME).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    const bool ok = imageSave(path, false);


    QFile file(path);
    if (ok && file.exists()) {
        m_networkOCR->sendBaiDuOcrRequest(data, path);
    } else {
        // 文件不存在
        qDebug() << "OCR origin image file does not exist. path:" << path;
    }

}

void ScreenShot::onOCRTextGenerateFinsh(const QByteArray &response, const OcrData &ocrTextData)
{
    json j;
    try {
        j = json::parse(response.toStdString());
    } catch (const std::exception& e) {
        qDebug() << "Failed to parse JSON:" << e.what();
        return;
    }

    const QString& text = QString::fromStdString(j.dump());
   // qDebug().noquote() << "------>text:" << text;
    const bool& bValid = !j.empty() && j.contains("words_result") && j["words_result"].size() > 0;
    if (!bValid) { // 文字识别返回错误码以及原因： 如触发限制
        m_ocrDlg->setRightText(text);
    } else {
        if (ocrTextData.pipeline == OcrChannel::OCR_standard || ocrTextData.pipeline == OcrChannel::OCR_standard_location
            ||ocrTextData.pipeline == OcrChannel::OCR_high_precision || ocrTextData.pipeline == OcrChannel::OCR_high_precision_location) {
            // 遍历 "words_result" 数组并将 "words" 按照 "location" 字段的坐标插入
            for (const auto& item : j["words_result"]) {
                std::string words = item["words"];
                QString text = QString::fromStdString(words);

                const bool& containLocation = ocrTextData.pipeline == OcrChannel::OCR_standard_location || ocrTextData.pipeline == OcrChannel::OCR_high_precision_location;
                if (containLocation) {
                    json location = item["location"];
                    int left = location["left"];
                    int top = location["top"];
                    int right = left + location["width"].get<int>();
                    int bottom = top + location["height"].get<int>();
                    static int lastBottom = bottom;

                    const int range = CJ_GET("tokens.advanced_ocr_baidu_align_rang").get<int>();
                    if (qAbs(bottom - lastBottom) <= range)
                        m_ocrDlg->appendRightText(text + "    ");
                    else
                        m_ocrDlg->appendRightText("\n" + text + "    ");

                    lastBottom = bottom;
                } else {
                    m_ocrDlg->appendRightText(text + "\n");
                }

            }
        }
    }

    // m_ocrTextEdit->resize(m_node.absoluteRect.size());
    // m_ocrTextEdit->move(m_node.absoluteRect.topLeft());
    // m_ocrTextEdit->clear();
    // if (!m_ocrTextEdit->isVisible()) m_ocrTextEdit->show();

    // 显示 OCR 弹窗
    const QPixmap& pixmap = finishDrewPixmap(m_node.absoluteRect, true);
    m_ocrDlg->setLeftPixmap(pixmap);
    const QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    if (screen) {
        const auto& rect = screen->geometry();
        const QPoint& center = rect.center(); // 获取屏幕的中心坐标
        m_ocrDlg->resize(rect.width() * 0.64, rect.height() * 0.64);
        m_ocrDlg->move(center - QPoint(m_ocrDlg->width() / 2, m_ocrDlg->height() / 2));
    }


    if (!m_ocrDlg->isVisible()) m_ocrDlg->show();
    m_ocrDlg->activateWindow();

    close();
}

void ScreenShot::onOcrTranslateCtrlHide()
{
    m_ocrGeneratePix = QPixmap();
    update();
}

void ScreenShot::onOcrTextCtrlHide()
{
    m_ocrTextEdit->clear();
    m_ocrTextEdit->hide();
}

void ScreenShot::initUI()
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_vdRect = m_primaryScreen->virtualGeometry();
    m_paintBar->raise();
    m_pickedRectTips->raise();
    m_pointTips->raise();

    onPickedColor(CJ_CD.pen.color());
    m_paintNode.pen = CJ_CD.pen;
    m_paintNode.brush = CJ_CD.brush;

    m_ocrDlg->hide();
    m_imgTranslateDlg->hide();
    // 初始化上一次的效果
    QTextCharFormat format = m_edit->currentCharFormat();
    format.foreground().setColor(m_paintNode.pen.color());    // fix: 默认描边的颜色边框为黑色
    format.setTextOutline(QPen(format.foreground().color(), 1));
    m_edit->mergeCurrentCharFormat(format);

    setTextFontSize(0, CJ_CD.font.pointSize(), false, false);   // 可能改动
    TextFlags flags;
    CJ_CD.textBold ? flags |= TextFlag::TF_blod : flags &= ~TextFlags(TextFlag::TF_blod);
    CJ_CD.textItalic ? flags |= TextFlag::TF_italic : flags &= ~TextFlags(TextFlag::TF_italic);
    CJ_CD.textOutline ? flags |= TextFlag::TF_outline : flags &= ~TextFlags(TextFlag::TF_outline);
    CJ_CD.textStrikeout ? flags |= TextFlag::TF_strikeout : flags &= ~TextFlags(TextFlag::TF_strikeout);
    CJ_CD.textUnderline ? flags |= TextFlag::TF_underline : flags &= ~TextFlags(TextFlag::TF_underline);
    onTextCtrlToggled(flags);

    m_edit->hide();
    m_ocrTextEdit->setReadOnly(true);
    m_ocrTextEdit->hide();
    m_paintBar->show(); // fix: 初次 MouseRelease 时，通过宽度（此时为默认的）计算其位置是不正确（需要先 show 一下才会刷新真实的尺寸）
    m_paintBar->hide();
    m_pointTips->hide();
    m_pickedRectTips->hide();

    m_timerPoint->setInterval(5000);
    monitorsInfo();

#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);  // | Qt::WindowStaysOnTopHint
#ifdef HALF_SCRN_DEVELOP
    setWindowFlag(Qt::WindowStaysOnTopHint, false);
    if (m_screens.size() == 1) {
        m_vdRect.setWidth(m_vdRect.width() / 2);
    } else {
        for (const auto& it: m_screens) {
            if (currentScreen() == it) {
                m_vdRect = it->geometry();
                break;
            }
        }
    }
#endif

    setFixedSize(m_vdRect.size());  // fix: 在 Ubuntu 22.04 顶部状态栏和任务栏，全屏位置错误
    move(m_vdRect.topLeft());
#else // Q_OS_MAC
    m_vdRect = currentScreen()->geometry();
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#ifdef HALF_SCRN_DEVELOP
//    setWindowFlag(Qt::WindowStaysOnTopHint, false);
//    if (m_screens.size() == 1)
//        m_vdRect.setWidth(m_vdRect.width() / 2);
#endif

    setFixedSize(m_vdRect.size());   // resize() cannot get the desired effect
    move(m_vdRect.topLeft());
    qDebug() << "#2-->" << m_vdRect << "   " << this->rect();
#endif


    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S),  this, [this](){
        qDebug() << "Press CTRL + SHIFT + S";
        imageQuickSave();
    });
}

void ScreenShot::initConnect()
{
    connect(m_timerPoint, &QTimer::timeout, this, &ScreenShot::onHidePointTips);
    connect(&COMM, &Communication::sigOCRImageGenerateFinsh, this, &ScreenShot::onOCRImageGenerateFinsh);
    connect(&COMM, &Communication::sigOCRTextGenerateFinsh, this, &ScreenShot::onOCRTextGenerateFinsh);
    connect(&COMM, &Communication::sigImgTranslateCtrlHide, this, &ScreenShot::onOcrTranslateCtrlHide);
    connect(&COMM, &Communication::sigOcrTextCtrlHide, this, &ScreenShot::onOcrTextCtrlHide);
//    connect(&COMM, &Communication::sigWidgetResized, this, [this](){
//        QTimer::singleShot(50, this, [this](){ showCustomWidget(m_paintBar); }); // fix: 当 paintBtnsBar 快贴底部时候，此时点击绘画按钮，通过 sendEvent() 传递过来，再次进入此函数，需要等待 rect 刷新后，再次重新计算
//    });

    connect(m_paintBar, &PaintBar::sigPaintToolBtnsRelease, this, &ScreenShot::onPaintBtnRelease);
    connect(m_paintBar, &PaintBar::sigPaintCtrlIdReleased, this, &ScreenShot::onPaintCtrlIdReleased);
    connect(m_paintBar, &PaintBar::sigTextCtrlToggled, this, &ScreenShot::onTextCtrlToggled);
    connect(m_paintBar, &PaintBar::sigPointCtrlReleased, this, &ScreenShot::onPointCtrlReleased);
    connect(m_paintBar, &PaintBar::sigMosaicSliderValueChanged, this, &ScreenShot::onMosaicSliderValueChanged);
    connect(m_paintBar, &PaintBar::sigUpdatePaintBarBlurPixmap, this, &ScreenShot::onUpdateToolBarBlurPixmap);
    connect(m_paintBar, &PaintBar::sigPickedColor, this, &ScreenShot::onPickedColor);
    connect(m_paintBar, &PaintBar::sigTextFontFamilyChanged, this, &ScreenShot::onTextFontFamilyChanged);
    connect(m_paintBar, &PaintBar::sigTextFontSizeChanged, this, &ScreenShot::onTextFontSizeChanged);
    connect(m_paintBar, &PaintBar::sigImgTranslate, this, &ScreenShot::onOcrTranslateCtrlIdReleased);
    connect(m_paintBar, &PaintBar::sigOcr, this, &ScreenShot::onOCRTextCtrlIdReleased);


    connect(this, &ScreenShot::sigSetTextFontSizeComboBoxValue, m_paintBar, &PaintBar::sigSetTextFontSizeComboBoxValue);
    connect(this, &ScreenShot::sigAutoDisableUndoAndRedo, m_paintBar, &PaintBar::sigAutoDisableUndoAndRedo);
}

void ScreenShot::drawShadowOverlay(const QRect &fullRect, const QRect &pickedRect, QPainter& pa) const
{
    QPainterPath path;
    path.addRect(fullRect);
    path.addRect(pickedRect);
    path.setFillRule(Qt::OddEvenFill);
    pa.save();
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(0, 0, 0, 0.4 * 255));
    pa.drawPath(path);
    pa.restore();
}


// 只是将 准备 "素材原图 + 马赛克" 赋值 -> m_paintNode.pixmap
void ScreenShot::stashMosaicPixmap()
{
    if (m_paintNode.pst == PaintShapeType::PST_mosaic) {
        const auto& pickedrect = m_paintNode.node.absoluteRect;
        const bool bRealValid = pickedrect.isValid() && !pickedrect.isNull(); // 绘图片矩形必须超过一个点大小，避免内存浪费

        if (bRealValid) {
            const auto& p1 = m_paintNode.node.p1;
            const auto& p2 = m_paintNode.node.p2;
            const auto& rect = largestRect(p1, p2);

            if (m_mosaicPix.isNull()) return;
            QPixmap pix = m_mosaicPix.copy(rect);

            //            qDebug() << "----#3---->rect:" << rect << "&m_mosaicPix:" << m_mosaicPix << "m_mosaicPix:" << m_mosaicPix << "pix:" << pix;
            //            static int idx = 0;

            //            pix.save(QString("D:/pix_%1.png").arg(QString::number(idx)));
            if (m_paintNode.id == 0) pixelatedMosaic(pix, m_paintNode.pixelatedFuzzy);
            else if (m_paintNode.id == 1) smoothMosaic(pix, m_paintNode.smoothFuzzy);
            //            pix.save(QString("D:/pixMosaic_%1.png").arg(QString::number(idx++)));

            m_paintNode.pixmap = pix;
            //            qDebug() << "----#4---->rect:" << rect;
        }
    } /*else if (m_paintNode.pst == PaintShapeType::PST_mosaic) {

    }*/
}

void ScreenShot::originalPixmap()
{
    if (m_origPix.isNull()) {
        m_origPix =  m_primaryScreen->grabWindow(qApp->desktop()->winId(), m_vdRect.x(), m_vdRect.y(), m_vdRect.width(), m_vdRect.height());

        qDebug() << "originalPixmap()， &m_origPix:" << &m_origPix << "m_origPix:" << m_origPix;
    }
}

void ScreenShot::setMosaicPix()
{
    m_mosaicPix =  m_primaryScreen->grabWindow(qApp->desktop()->winId(), m_vdRect.x(), m_vdRect.y(), m_vdRect.width(), m_vdRect.height());
    qDebug() << "m_mosaicPix()， &m_mosaicPix:" << &m_mosaicPix << "m_mosaicPix:" << m_mosaicPix;
}

QString ScreenShot::imageSavePath(const ImageSaveType &types)
{
    QString path = "";
    const auto& formatName = formatToFileName(CJ_GET_QSTR("output.flie_name"));
    const QString& imageName = QDateTime::currentDateTime().toString(formatName);

    if (types == ImageSaveType::IST_manual_save) {
        const QString& dir = CJ_GET_QSTR("advanced.customize_ui_parameters.manual_save_image_dir");
        const QString& fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files(*.*)"));
        path = QFileDialog::getSaveFileName(this, tr("Save Files"), dir + "/" + imageName, fileter);

        if (!path.isEmpty()) {
            const QFileInfo fileInfo(path);
            CJ_SET("advanced.customize_ui_parameters.manual_save_image_dir", fileInfo.path().toStdString());
        }

    } else if (types == ImageSaveType::IST_quick_save) {
        if (CJ_GET("output.quick_save_enable").get<bool>())
            path = CJ_GET_QSTR("output.quick_save_path") + "/" + imageName;

    } else if (types == ImageSaveType::IST_auto_save) {
        if (CJ_GET("output.auto_save_enable").get<bool>())
            path = CJ_GET_QSTR("output.auto_save_path") + "/" + imageName;

    } else {
        qDebug() <<"error: types & ImageSaveType:: other !";
    }

    qDebug() <<"fileNmae path:" << path;
    return path;
}

bool ScreenShot::imageSave(const QString &path, const bool &bClose)
{
    const QPixmap& pixmap = finishDrewPixmap(m_node.absoluteRect, true);
    if (pixmap.isNull()) return false;

    QTime startTime = QTime::currentTime();
    const bool ret = pixmap.save(path, nullptr, CJ_GET("output.image_quailty").get<int>());  // 绘画在 m_savePix 中，若在 m_savePix 会有 selRect 的左上角的点的偏移
    QTime stopTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(stopTime);
    qInfo() << "btnSave() pixmap save time: " << elapsed << "ms" << pixmap.size() << "image path:" << path;


    if (CJ_GET("interface.auto_copy_to_clipbaoard").get<bool>()) {
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setPixmap(pixmap);  // 由于 Qt 库会在应用程序终止时释放剪贴板对象; 故此处内存增加是必然的，不用额外 delete 处理； 氪！排查这么久结论居然是这个
    }

    if (CJ_GET("output.auto_save_enable").get<bool>()) {
        QString dir = CJ_GET_QSTR("output.auto_save_path");
        const QString& autoSavePath = dir + path.right(path.count() - path.lastIndexOf('/'));
        pixmap.save(autoSavePath, nullptr, CJ_GET("output.image_quailty").get<int>());
    }

    if (bClose)
        close();

    return ret;
}

void ScreenShot::imageQuickSave()
{
    const auto& path = imageSavePath(ImageSaveType::IST_quick_save);
    const bool ok = imageSave(path);
    COMM.sigShowSystemMessagebox(ok ? tr("Success") : tr("Failed"), ok ? tr("Image save to ") + path : tr("Quick save feature is not enabled, please check."), 10000);
}

// bMouse true-鼠标滑轮， false-绘画工具栏下拉列表实现
void ScreenShot::setTextFontSize(const int& stepY, const int& width, const bool &bMouse, const bool &bShowPointTips)
{
    m_textFont.setPointSize(bMouse ? m_textFont.pointSize() + stepY : width);
    m_edit->setFont(m_textFont);
    const int& tWidth = m_textFont.pointSize();
    const QString& szWidth = QString::number(tWidth);
    if (bShowPointTips) showPointTips(szWidth + "pt");
    if (bMouse) emit sigSetTextFontSizeComboBoxValue(szWidth);  // 同步修改下拉列表的字体大小
}

void ScreenShot::autoDisableUndoAndRedo()
{
    const bool bUndoDisable = m_redo.size() > 0 ? false : true;
    const bool bRedoDisable = m_undo.size() > 0 ? false : true;
    emit sigAutoDisableUndoAndRedo(bUndoDisable, bRedoDisable);

    qDebug() << "----#c-->m_undo.size():" << m_undo.size() << "m_redo.size():" << m_redo.size() << bUndoDisable << bRedoDisable;
}

void ScreenShot::setCursorShape(const OrientationType &type, const QPoint &pt)
{
    if (m_actionType == ActionType::AT_wait) {
        CJ.m_cd.isShowCollimatorCursor = false;

        if (type == OrientationType::OT_internal) {
            setCursor(Qt::SizeAllCursor);
        } else if (type == OrientationType::OT_left || type == OrientationType::OT_right) {
            setCursor(Qt::SizeHorCursor);
        } else if (type == OrientationType::OT_top || type == OrientationType::OT_bottom) {
            setCursor(Qt::SizeVerCursor);
        } else if (type == OrientationType::OT_topLeft || type == OrientationType::OT_bottomRight) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (type == OrientationType::OT_topRight || type == OrientationType::OT_bottomLeft) {
            setCursor(Qt::SizeBDiagCursor);
        } else {
            qDebug() << "---->" << int(m_actionType);
            setCursor(Qt::WhatsThisCursor); // 一般都不会触发
        }
    } else if (m_actionType == ActionType::AT_drawing_shap) {
        // setCursor(Qt::CrossCursor);
        setCursor(Qt::BlankCursor);  // 隐藏光标
        CJ.m_cd.isShowCollimatorCursor = true;
    }
}

QScreen *ScreenShot::currentScreen(const QPoint &pos) const
{
    QScreen* scrn = qGuiApp->screenAt(pos);

#if defined(Q_OS_MACOS)
    // In macos, mouse position at the bottom or right edge of the screen will crash
    if (!scrn && (pos.x() >= m_vdRect.right() || pos.y() >= m_vdRect.bottom()))
        scrn = qGuiApp->screenAt(m_vdRect.bottomRight() - QPoint(1, 1));
#endif

    //if (!scrn) curScrn = qGuiApp->primaryScreen();

    if (!scrn) {
        QMessageBox::warning(nullptr, "ScreenShot::currentScreen", "Gets that the current screen is empty!");
        qDebug() << "Gets that the current screen is empty";
    }

    return scrn;
}

void ScreenShot::preDestruction()
{
    m_primaryScreen = nullptr;
    if (m_screens.size()) m_screens.clear();
    if (!m_origPix.isNull()) m_origPix = QPixmap();
    if (!m_mosaicPix.isNull()) m_mosaicPix = QPixmap();
    if (m_paintBar) m_paintBar->deleteLater();
    if (m_rectNodes.size()) m_rectNodes.clear();
}

void ScreenShot::monitorsInfo() const
{
    qInfo() << "---------------QApplication::desktop() Info BEGIN----------------";
    qInfo() << "所有可用区域 m_vdRect:" << m_vdRect << Qt::endl
        << "主屏可用区域 m_primaryScreen->geometry():" << m_primaryScreen->geometry()
        << "是否开启虚拟桌面 isVirtualDesktop:" << (m_primaryScreen->virtualSiblings().size() > 1 ? true : false) << Qt::endl;
    qInfo() << "---------------QApplication::desktop() Info END----------------";

    qInfo() << "---------------m_screens[] Info BEGIN----------------";
    for (const auto& it : m_screens) {
        qInfo() << "it:" << it
                 << "\n可用几何 availableGeometry:" << it->availableGeometry()
                 << "\n可用虚拟几何 availableVirtualSize:" << it->availableVirtualSize()
                 << "\n虚拟几何 virtualGeometry:" << it->virtualGeometry()
                 << "\n几何 geometry:" << it->geometry()
                 << "\n尺寸 size:" << it->size()
                 << "\n物理尺寸 physicalSize:" << it->physicalSize()
                 << "\n刷新率 refreshRate:" << it->refreshRate()
                 << "\n尺寸 size:" << it->size()
                 << "\n虚拟尺寸 virtualSize:" << it->virtualSize()
                 << "\n设备像素比 devicePixelRatio:" << it->devicePixelRatio()
                 << "\n逻辑DPI logicalDotsPerInch:" << int(it->logicalDotsPerInch()) << " DPIX:" << int(it->logicalDotsPerInchX()) << " DPIY:" << int(it->logicalDotsPerInchY())
                 << "\n物理DPI physicalDotsPerInch:" << int(it->physicalDotsPerInch()) << " DPIX:" << int(it->physicalDotsPerInchX()) << " DPIY:" << int(it->physicalDotsPerInchY());
//                 << "\n手算缩放比 getScale:" << GetScale(it)
//                 << "\n虚拟尺寸 m_captureScrnRt:" << m_captureScrnRt << "\n";
    }
    qInfo() << "---------------m_screens[] Info END----------------";
}

void ScreenShot::printfDevelopProjectInfo(QPainter& pa)
{
    pa.save();
    pa.setPen(QPen(Qt::yellow, 2));

    QScreen* scrn = m_primaryScreen;
    for (const auto& it : m_screens) {
        if (it != m_primaryScreen)
            scrn = it;
    }

    QPoint tTextPt(0, 1500);
    if (scrn) tTextPt = mapFromGlobal(scrn->geometry().topLeft());

    const int tTextX = tTextPt.x();
    const int tTextY = tTextPt.y() + 50;
    const int tAddHight = 20;
    int tCount = 1;

    const auto& currentScreenGeom = currentScreen()->geometry();
    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("pt:(%1, %2) m_vdRect:(%3, %4, %5 * %6) currentScreenGeom:(%7, %8, %9 * %10)")
                                                                   .arg(QCursor::pos().x()).arg(QCursor::pos().y())
                                                                   .arg(m_vdRect.x()).arg(m_vdRect.y()).arg(m_vdRect.width()).arg(m_vdRect.height())
                                                                   .arg(currentScreenGeom.x()).arg(currentScreenGeom.y()).arg(currentScreenGeom.width()).arg(currentScreenGeom.height()));

    pa.drawText(QPoint(tTextX, tTextY), QString("//[m_node]----------------------------------------------------"));
    QPoint tP1 = m_node.p1;
    QPoint tP2 = m_node.p2;
    QPoint tP3 = m_node.p3;
    QPoint tPt = m_node.pt;
    QRect tPickedRect = m_node.pickedRect;
    QRect absoluteRect = m_node.absoluteRect;

    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("hasMouseTracking:%1 ActionType:%2")
                                                                   .arg(hasMouseTracking() ? "true" : "false").arg(actionTypeToString(m_actionType)));
    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("p1:(%1, %2)  p2:(%3, %4) \n p3:(%5, %6)")
                                                                   .arg(tP1.x()).arg(tP1.y()).arg(tP2.x()).arg(tP2.y()).arg(tP3.x()).arg(tP3.y()));
    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("pt:(%1, %2) pickedRect:(%3, %4, %5 * %6) absoluteRect:(%7, %8, %9 * %10)")
                                                                   .arg(tPt.x()).arg(tPt.y())
                                                                   .arg(tPickedRect.x()).arg(tPickedRect.y()).arg(tPickedRect.width()).arg(tPickedRect.height())
                                                                   .arg(absoluteRect.x()).arg(absoluteRect.y()).arg(absoluteRect.width()).arg(absoluteRect.height()));

    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("//[m_paintNode]----------------------------------------------------"));
    tP1 = m_paintNode.node.p1;
    tP2 = m_paintNode.node.p2;
    tP3 = m_paintNode.node.p3;
    tPt = m_paintNode.node.pt;
    tPickedRect = m_paintNode.node.pickedRect;
    absoluteRect = m_node.absoluteRect;

    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("hasMouseTracking:%1 ActionType:%2")
                                                                   .arg(hasMouseTracking() ? "true" : "false").arg(actionTypeToString(m_actionType)));
    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("p1:(%1, %2)  p2:(%3, %4) \n p3:(%5, %6)")
                                                                   .arg(tP1.x()).arg(tP1.y()).arg(tP2.x()).arg(tP2.y()).arg(tP3.x()).arg(tP3.y()));
    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("pt:(%1, %2) pickedRect:(%3, %4, %5 * %6) absoluteRect:(%7, %8, %9 * %10)")
                                                                   .arg(tPt.x()).arg(tPt.y())
                                                                   .arg(tPickedRect.x()).arg(tPickedRect.y()).arg(tPickedRect.width()).arg(tPickedRect.height())
                                                                   .arg(absoluteRect.x()).arg(absoluteRect.y()).arg(absoluteRect.width()).arg(absoluteRect.height()));
    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("pst:%1 bShow:%2 id:%3 pixelatedFuzzy:%4 smoothFuzzy:%5 pen.width():%6")
                                                                   .arg(paintShapeTypeToString(m_paintNode.pst)).arg(m_paintNode.bShow ? "true" : "false").arg(m_paintNode.id)
                                                                   .arg(m_paintNode.pixelatedFuzzy).arg(m_paintNode.smoothFuzzy).arg(m_paintNode.pen.width()));
    pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("SerialNode-number:[%1] SerialNode-letter:%2 background:%3 pen:%4 brush:%5")
                                                                   .arg(m_paintNode.serialNode.number).arg(m_paintNode.serialNode.letter).arg(m_paintNode.serialNode.background.name())
                                                                   .arg(m_paintNode.pen.color().name()).arg(m_paintNode.brush.color().name()));


    int idx = 0;
    for (const auto& it : m_rectNodes) {
        const auto& rect = xrectToQRect(it.rect);
        const auto& relativelyRect = xrectToQRect(it.relativelyRect);
        pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("//idx:%1-------------------------\n").arg(idx++));
        pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++),  QString("it.rect(%1, %2, %3 * %4) it.relativelyRect(%5, %6, %7 * %8)")
                                                                       .arg(rect.x())
                                                                       .arg(rect.y())
                                                                       .arg(rect.width())
                                                                       .arg(rect.height())
                                                                       .arg(relativelyRect.x())
                                                                       .arg(relativelyRect.y())
                                                                       .arg(relativelyRect.width())
                                                                       .arg(relativelyRect.height()));


        pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString::fromStdWString(it.title));
        quintptr decimalValue = reinterpret_cast<quintptr>(it.ntHWnd);
        QString hexString = QString("0x%1").arg(decimalValue, 0, 16);
        pa.drawText(QPoint(tTextX, tTextY + tAddHight * tCount++), QString("hWnd:%1(10)  %2(16)").arg(decimalValue).arg(hexString));
    }
    pa.restore();
}

void ScreenShot::prinftWindowsRects(QPainter& pa)
{
    pa.save();
    QPen penBorder(QColor(255, 0, 0, 1 * 255), 2);
    QPen penText(QColor(0, 255, 0, 1 * 255), 2);

    if (m_rectNodes.size() <= 0) return;
    // const auto& it = m_rectNodes.at(0);
    const auto& it = m_rectNodes.at(m_rectNodes.size() > 1 ? m_rectNodes.size() - 1 : 0);
    const auto& rect = xrectToQRect(it.rect);
    const auto& relativelyRect = xrectToQRect(it.relativelyRect);

    const int fixLeft = 10;
    const int fixHeight = 22;
    int nCount = 1;

    pa.setPen(penBorder);
    pa.setBrush(Qt::NoBrush);
    pa.fillRect(QRect(relativelyRect.topLeft(), QSize(800, fixHeight * 8)), QColor(0, 0, 0, 0.8 * 255));
    pa.drawRect(relativelyRect);

    pa.setPen(penText);
    pa.drawText(relativelyRect.topLeft() + QPoint(fixLeft, fixHeight * nCount++),  QString("rect(%1, %2, %3 * %4)")
                                                              .arg(rect.x())
                                                              .arg(rect.y())
                                                              .arg(rect.width())
                                                              .arg(rect.height()));

    pa.drawText(relativelyRect.topLeft() + QPoint(fixLeft, fixHeight * nCount++),  QString("relativelyRect(%1, %2, %3 * %4)")
                                                              .arg(relativelyRect.x())
                                                              .arg(relativelyRect.y())
                                                              .arg(relativelyRect.width())
                                                              .arg(relativelyRect.height()));

    pa.drawText(relativelyRect.topLeft() + QPoint(fixLeft, fixHeight * nCount++), QString("title: %1").arg(QString::fromStdWString(it.title)));

    pa.drawText(relativelyRect.topLeft() + QPoint(fixLeft, fixHeight * nCount++),  QString("exeName: %1").arg(QString::fromStdWString(it.exeName)));
    pa.drawText(relativelyRect.topLeft() + QPoint(fixLeft, fixHeight * nCount++),  QString("procPath: %1").arg(QString::fromStdWString(it.procPath)));
    quintptr decimalValue;
    QString hexString = QString("0x%1").arg(it.ntPocessId, 0, 16);
    pa.drawText(relativelyRect.topLeft() + QPoint(fixLeft, fixHeight * nCount++), QString("ntPocessId: %1(Dec)  %2(Hex)").arg(it.ntPocessId).arg(hexString));

    // WND hwndDesktop = GetDesktopWindow();
    // std::wcout << L"hwndDesktop:" << hwndDesktop << L"  it.ntHWnd:" << it.ntHWnd;
    // std::wcout  << L"  it.ntHWnd:" << it.ntHWnd;
    decimalValue = reinterpret_cast<quintptr>(it.ntHWnd);
    hexString = QString("0x%1").arg(decimalValue, 0, 16);
    pa.drawText(relativelyRect.topLeft() + QPoint(fixLeft, fixHeight * nCount++), QString("hWnd: %1(10)  %2(16)").arg(decimalValue).arg(hexString));

    pa.restore();
}


void ScreenShot::rectNodesMapFromGlobal()
{
    for (auto& it : m_rectNodes) {
        const auto& rect = xrectToQRect(it.rect);
        const auto& topLeft = mapFromGlobal(rect.topLeft());
        it.relativelyRect.left = topLeft.x();
        it.relativelyRect.top = topLeft.y();
        it.relativelyRect.right = topLeft.x() + rect.width();
        it.relativelyRect.bottom = topLeft.y() + rect.height();
    }
}

void ScreenShot::firstRectNodesAssignmentNode()
{
    if (m_rectNodes.size() == 0) return;
    const auto& it = m_rectNodes.at(CJ_GET("advanced.customize_ui_parameters.auto_detection_windows_rect_top_level").get<bool>() ? 0 : m_rectNodes.size() - 1);
    const auto& relativelyRect = xrectToQRect(it.relativelyRect);

    m_node.p1 = relativelyRect.topLeft();
    m_node.p2 = relativelyRect.bottomRight();
    m_node.p3 = relativelyRect.bottomRight();
    m_node.pickedRect = relativelyRect;
}

// 返回的相对窗口的坐标
QPoint ScreenShot::customWidgetShowPositionRule(const CustomWidgetType &cwt)
{
    // 根据 input pt 坐标，获取其所在的屏幕的矩形，作为判定条件,返回相对于 Sunny 窗口的相对 rect 坐标
    auto currScrnRect = [this](const QPoint& pt) -> const QRect {
        const QScreen* screen = QGuiApplication::screenAt(pt);
        if (!screen) qDebug() << "customWidgetShowPositionRule is failed! screen is nullptr";
        const QRect rect = screen ? screen->geometry() : QRect();

        QRect rt(mapFromGlobal(rect.topLeft()), rect.size());
//        qDebug() << "#-->rect:" << rect << "rt:" << rt;
        return rt;
    };

    QPoint pt;
    const int space = 10; // 和 pickedRect 之间的间隔
    const QSize& size = m_paintBar->size();
    static bool prevBTranspose = false;
    if (cwt == CustomWidgetType::CWT_paint_btns_bar) {

        bool bTranspose = false;  // 适当时需要翻转对调两个 bar 的位置
        if (m_orie == Qt::Horizontal) {
            static int paintToolBarWidth = size.width(); // Horizontal 模式使用这个体验更佳
            if(m_node.absoluteRect.bottom() + m_paintBar->height() + space <= currScrnRect(mapToGlobal(m_node.absoluteRect.bottomRight())).bottom()) {
                pt = m_node.absoluteRect.bottomRight() + QPoint(-1 * paintToolBarWidth, space);
            } else if (m_node.absoluteRect.top() - m_paintBar->height() - space >= currScrnRect(mapToGlobal(m_node.absoluteRect.topRight())).top()) {
                pt = m_node.absoluteRect.topRight() - QPoint(paintToolBarWidth, space + size.height());
                bTranspose = true;
            } else {
                pt = m_node.absoluteRect.topRight() + QPoint(-1 * paintToolBarWidth, space);
            }

        } else if (m_orie == Qt::Vertical) {

            if(m_node.absoluteRect.right() + m_paintBar->width() <= currScrnRect(mapToGlobal(m_node.absoluteRect.topRight())).right()) {
                pt = m_node.absoluteRect.topRight() + QPoint(space, 0);
            } else if (m_node.absoluteRect.left() - m_paintBar->width() >= currScrnRect(mapToGlobal(m_node.absoluteRect.topLeft())).left()) {
                pt = m_node.absoluteRect.topLeft() + QPoint(-(space + size.width()), 0);
                bTranspose = true;
            } else {
                pt = m_node.absoluteRect.topLeft() + QPoint(space, 0);
            }
        }

        if (prevBTranspose != bTranspose) {
            m_paintBar->transposePaintBar(bTranspose);
            prevBTranspose = bTranspose;
        }

    } else if (cwt == CustomWidgetType::CWT_paint_btns_bar) {
    } else if (cwt == CustomWidgetType::CWT_picked_rect_tooptip) {

        if(m_node.absoluteRect.top() - m_pickedRectTips->height() - space >= currScrnRect(mapToGlobal(m_node.absoluteRect.topLeft())).top()) {
            pt = m_node.absoluteRect.topLeft() - QPoint(0, m_pickedRectTips->height() + space);
        } else if (m_node.absoluteRect.bottom() + m_pickedRectTips->height() + space <= currScrnRect(mapToGlobal(m_node.absoluteRect.bottomLeft())).bottom()) {
            pt = m_node.absoluteRect.bottomLeft() + QPoint(0, space);
        } else {
            pt = m_node.absoluteRect.topLeft() + QPoint(0, space);
        }

    } else if (cwt == CustomWidgetType::CWT_point_changed_tooptip) {
    } else {
        qDebug() << "unknow CustomWidgetType!";
    }

    return pt;
}

void ScreenShot::showPickedRectTips()
{
    const auto& rect = m_node.absoluteRect;
    QString tips = QString("%1, %2, %3 * %4")
                       .arg(rect.left()).arg(rect.top()).arg(rect.width()).arg(rect.height());


//    QString tips = QString("tp(%1, %2), br(%3, %4), %5 * %6")
//                       .arg(rect.left()).arg(rect.top()).arg(rect.right()).arg(rect.bottom()).arg(rect.width()).arg(rect.height());

//    const auto& barRect = m_paintBar->rect();
//    QString tips2 = QString(" -> tp(%1, %2), br(%3, %4), %5 * %6")
//                       .arg(barRect.left()).arg(barRect.top()).arg(barRect.right()).arg(barRect.bottom()).arg(barRect.width()).arg(barRect.height());
    m_pickedRectTips->setText(tips);
    showCustomWidget(m_pickedRectTips);
}

void ScreenShot::dealMousePressEvent(QMouseEvent *e)
{
    setMouseTracking(false);
    m_node.p1 = e->pos();
    m_node.p2 = e->pos();
    m_node.p3 = e->pos();
//    qDebug() << "MousePressEvent, m_node.p1:" << m_node.p1;

    if (m_actionType == ActionType::AT_wait) {
        const auto& orieType = containsForRect(m_node.pickedRect, m_node.p1);

        if (orieType == OrientationType::OT_internal) {
            m_actionType = ActionType::AT_move_picked_rect;
            m_node.pt = e->pos();
            setMouseTracking(true);
        } else {
            m_actionType = ActionType::AT_stretch_picked_rect;
            m_stretchPickedRectOrieType = orieType;
            m_node.pickedRect = largestRect(m_node.pickedRect, m_node.p1);
            setMouseTracking(true);
        }

    } else if (m_actionType == ActionType::AT_picking_custom_rect) {
        setMouseTracking(true);
        m_bFistPressed = true;

        if (m_HotKeyType == HotKeyType::HKT_custiom_capture) {
            const bool& customSizeEnable = CJ_GET("interface.custom_size_enable");
            const bool& topleftEnable = CJ_GET("interface.topleft_enable");
            const bool& sizeEnable = CJ_GET("interface.size_enable");

            if (customSizeEnable) {

                if (topleftEnable)
                    m_node.p1 = QPoint(CJ_GET("interface.custom_rect_left").get<int>(), CJ_GET("interface.custom_rect_top").get<int>());

                if (sizeEnable) {
                    m_node.pickedRect = QRect(m_node.p1, QSize(CJ_GET("interface.custom_rect_width").get<int>(), CJ_GET("interface.custom_rect_height").get<int>()));
                    m_node.absoluteRect = m_node.pickedRect;
                    m_node.p2 = m_node.p3 = m_node.pickedRect.bottomRight();
                }
            }
        }

    } else if (m_actionType == ActionType::AT_picking_detection_windows_rect) {
        m_node.pt = e->pos();
        setMouseTracking(true);
        m_bFistPressed = true;
    } else if (m_actionType == ActionType::AT_select_picked_rect) {
    } else if (m_actionType == ActionType::AT_select_drawn_shape) {
    } else if (m_actionType == ActionType::AT_drawing_shap) {
        setMouseTracking(false);
        m_node.trackPos.clear();
        m_node.trackPos.emplace_back(m_node.p3);
        m_paintNode.node = m_node;
        m_paintNode.bShow = true;

        // 序号自增加
        if (m_paintNode.pst == PaintShapeType::PST_serial) {
            if (m_paintNode.id == 0) {
                m_paintNode.serialNode.number++;
            } else if (m_paintNode.id == 1) {
                QChar serialChar = m_paintNode.serialNode.letter.unicode() + 1;
                m_paintNode.serialNode.letter = serialChar;
            }
        } else if (m_paintNode.pst == PaintShapeType::PST_mosaic) {
            setMosaicPix();  // 此刻需要准备好马赛克的原始素材，无论是 move/release 都是需要它
        } else if (m_paintNode.pst == PaintShapeType::PST_text) {

            if (m_paintNode.xTextEditType == XTextEditType::XTET_nullptr) { // 为 nullptr， 则初次创建
//                m_edit = new XTextEdit(this);

                qDebug() << "----#3.0----->m_edit:" << m_edit;
                m_edit->show();
                m_edit->move(m_node.p1);
                m_edit->setFocus();
                m_paintNode.xTextEditType = XTextEditType::XTET_generated;

                qDebug() << "----#3.1----->m_edit->hasFocus():" << m_edit->hasFocus();
            }
        }


    } else if (m_actionType == ActionType::AT_move_drawn_shape) {
    } else if (m_actionType == ActionType::AT_move_picked_rect) {
    } else if (m_actionType == ActionType::AT_stretch_drawn_shape) {
    } else if (m_actionType == ActionType::AT_stretch_picked_rect) {
    }

    m_node.absoluteRect = toAbsoluteRect(m_node.pickedRect);
}

void ScreenShot::dealMouseReleaseEvent(QMouseEvent *e)
{
    m_node.p2 = e->pos();
    m_node.p3 = e->pos();
//    qDebug() << "MouseReleaseEvent, m_node.p2:" << m_node.p2 << "m_node.pickedRect:" << m_node.pickedRect;

    if (m_actionType == ActionType::AT_wait) {
    } else if (m_actionType == ActionType::AT_picking_custom_rect) {
        m_node.pickedRect = CaptureHelper::rectFromTowPos(m_node.p1, m_node.p2);
        m_node.trackPos.emplace_back(m_node.p3);
        m_actionType = ActionType::AT_wait;
    } else if (m_actionType == ActionType::AT_picking_detection_windows_rect) {

        m_node.pickedRect = CaptureHelper::rectFromTowPos(m_node.p1, m_node.p2);
        // 此时已经滑动和已经被按下过了，所以一定是结束的标记
        if (allowableRangeErrorForPoint(m_node.p1, m_node.p2)) {
            firstRectNodesAssignmentNode();
        }

        m_actionType = ActionType::AT_wait;
    } else if (m_actionType == ActionType::AT_select_picked_rect) {
    } else if (m_actionType == ActionType::AT_select_drawn_shape) {
    } else if (m_actionType == ActionType::AT_drawing_shap) {
        m_node.trackPos.emplace_back(m_node.p3);
        // 点位确定了之后，再来 push_bach
        m_paintNode.node = m_node;
        m_paintNode.bShow = false;

        if (m_paintNode.pst == PaintShapeType::PST_mosaic) {
            stashMosaicPixmap();
        } else if (m_paintNode.pst == PaintShapeType::PST_text) {


            if (m_paintNode.xTextEditType == XTextEditType::XTET_generated) {
                m_node.pt = m_node.p2; // 保留之前的位置位置
                m_paintNode.xTextEditType = XTextEditType::XTET_editing;
            } else if (m_paintNode.xTextEditType == XTextEditType::XTET_editing) {

                if (!m_edit->rect().contains(m_node.p2)) { // 点击在其外面，则此是旧的已经完成;

                    m_paintNode.xTextEditType = XTextEditType::XTET_finish;
                    m_edit->clearFocus();
                    if (!m_edit->toPlainText().isEmpty()) {  // 不为空, 此时则直接入栈 push_back

                        m_paintNode.node.absoluteRect = QRect(m_node.pt, m_edit->rect().size());
                        m_paintNode.xTextEdit = showCreatorRichText(m_edit->document(), m_paintNode.node.absoluteRect, this);
                        qDebug() << "------$4--->m_edit->rect():" << m_edit->rect() << "m_paintNode.node.absoluteRect:" << m_paintNode.node.absoluteRect << "m_paintNode.xTextEdit:" << m_paintNode.xTextEdit;
                        m_edit->hide();
                        m_redo.push_back(m_paintNode);
                        autoDisableUndoAndRedo();
                    }

//                    m_edit->document()->clear();
                    m_edit->setText("");

                } else {
                    // 点击在其里面则继续，属无事发生; 继续编辑当前的编辑框的文本
                    m_edit->move(m_node.pt);
                }

            } else if (m_paintNode.xTextEditType == XTextEditType::XTET_finish) {
            } else {
                qDebug() << "m_paintNode.xTextEditType is XTET_nullptr or other?";
            }
        }


        m_paintNode.node = m_node;
        m_paintNode.bShow = false;
        if (m_paintNode.pst != PaintShapeType::PST_text) { // 特例, PST_text 在 MousePress 里面去 push_back
            m_redo.push_back(m_paintNode);
            autoDisableUndoAndRedo();
        }

        // 归零可能会干扰下次操作的一些参数
        m_node.trackPos.clear();
        if (m_paintNode.xTextEditType == XTextEditType::XTET_finish) m_paintNode.xTextEditType = XTextEditType::XTET_nullptr;
        m_paintNode.pixmap = QPixmap();    // fix: push_back 时候永远是最新的一个
        qDebug() << "m_redo:" << m_redo.size();



        setMouseTracking(m_paintNode.pst == PaintShapeType::PST_text ? false : true);
    } else if (m_actionType == ActionType::AT_move_drawn_shape) {
    } else if (m_actionType == ActionType::AT_move_picked_rect) {
        setMovePickedRect();
        setMouseTracking(true);
        m_actionType = ActionType::AT_wait;
    } else if (m_actionType == ActionType::AT_stretch_drawn_shape) {
    } else if (m_actionType == ActionType::AT_stretch_picked_rect) {
        m_node.p3 = e->pos();
        qDebug() << "----@MouseReleas0->" << m_node.p1 << m_node.p2 << m_node.p3 << m_node.pickedRect;
        stretchRect(m_node.pickedRect, m_node.p3, m_stretchPickedRectOrieType);
        ensurePositiveSize(m_node.pickedRect);
        m_node.p1 = m_node.pickedRect.topLeft();
        m_node.p2 = m_node.pickedRect.bottomRight();
        m_actionType = ActionType::AT_wait;
    }

    m_node.absoluteRect = toAbsoluteRect(m_node.pickedRect);
    m_node.p1 = m_node.p2 = QPoint(); // 完成一次操作后，就重置
}

void ScreenShot::dealMouseMoveEvent(QMouseEvent *e)
{
    m_node.p2 = e->pos();
    m_node.p3 = e->pos();
//    qDebug() << "MouseMoveEvent, m_node.p3:" << m_node.p3 << "m_node.pickedRect:" << m_node.pickedRect;

    if (m_actionType == ActionType::AT_wait) {
        const auto& orieType = containsForRect(m_node.pickedRect, m_node.p3);
        setCursorShape(orieType, m_node.p3);
    } else if (m_actionType == ActionType::AT_picking_custom_rect) {

        if (m_bFistPressed) {
            m_node.pickedRect = CaptureHelper::rectFromTowPos(m_node.p1, m_node.p3);
            m_node.trackPos.emplace_back(m_node.p3);
        } else {
            m_node.p1 = e->pos();
        }

    } else if (m_actionType == ActionType::AT_picking_detection_windows_rect) {
        if (m_bFistPressed) {
            if (!allowableRangeErrorForPoint(m_node.p3, m_node.pt)) {
                m_actionType = ActionType::AT_picking_custom_rect;
            }
        } else {

            // 获取截图主窗口的自身 hwnd
            CrossHwnd osHwnd;
            #if defined(Q_OS_WIN)
                HWND hWnd = reinterpret_cast<HWND>(winId());
                osHwnd.ntHWnd = hWnd;
            #elif defined(Q_OS_LINUX)
            #elif defined(Q_OS_MAC)
            #endif

            // 获取主窗口句柄

            enumWindowsRect(m_rectNodes, osHwnd);
            rectNodesMapFromGlobal();
            firstRectNodesAssignmentNode();
        }
    } else if (m_actionType == ActionType::AT_select_picked_rect) {
    } else if (m_actionType == ActionType::AT_select_drawn_shape) {
    } else if (m_actionType == ActionType::AT_drawing_shap) {
        if (m_paintNode.bShow)
            m_node.trackPos.emplace_back(m_node.p3);
        const auto& orieType = containsForRect(m_vdRect, m_node.p3);
        setCursorShape(orieType, m_node.p3);
        m_paintNode.node = m_node;

        if (m_paintNode.pst == PaintShapeType::PST_mosaic) {
            stashMosaicPixmap();
        } else if (m_paintNode.pst == PaintShapeType::PST_text) {

            if (!m_edit || !m_edit->isVisible()) return;
            if (m_paintNode.xTextEditType == XTextEditType::XTET_generated)
                m_edit->move(m_node.p3);
        }

        qDebug() << "----->m_node.trackPos:" << m_node.trackPos.size();
    } else if (m_actionType == ActionType::AT_move_drawn_shape) {
    } else if (m_actionType == ActionType::AT_move_picked_rect) {
        setMovePickedRect();
    } else if (m_actionType == ActionType::AT_stretch_drawn_shape) {
    } else if (m_actionType == ActionType::AT_stretch_picked_rect) {
        stretchRect(m_node.pickedRect, m_node.p3, m_stretchPickedRectOrieType);
        m_node.p1 = m_node.pickedRect.topLeft();
        m_node.p2 = m_node.pickedRect.bottomRight();
    }

    m_node.absoluteRect = toAbsoluteRect(m_node.pickedRect);
}

// Move or Stretch;
// Ctrl + towards: 外扩
// Shift + towards: 内缩
// towards: 水平移动
void ScreenShot::adjustPickedRect(QKeyEvent *e)
{
    const int pos = 1;
    int totalPos = pos;
    const int rate = 10;
    const bool& ctrl = e->modifiers() == Qt::ControlModifier;
    const bool& shift = e->modifiers() == Qt::ShiftModifier;
    const bool left(e->key() == Qt::Key_A | e->key() == Qt::Key_Left);
    const bool top(e->key() == Qt::Key_W | e->key() == Qt::Key_Up);
    const bool right(e->key() == Qt::Key_D | e->key() == Qt::Key_Right);
    const bool down(e->key() == Qt::Key_S | e->key() == Qt::Key_Down);

    if (shift | ctrl) totalPos = pos * rate;

    QRect& rt = m_node.absoluteRect;
    if (!rt.isValid()) return;

    if (m_actionType == ActionType::AT_wait) {
        if (ctrl | shift) m_actionType = ActionType::AT_stretch_picked_rect;
        else m_actionType = ActionType::AT_move_picked_rect;
    } else {
        return;
    }

    bool bUpdate = false;
    if (m_actionType == ActionType::AT_stretch_picked_rect) {
        if (ctrl) {
            if (left) rt.setLeft(rt.left() - totalPos);
            if (top) rt.setTop(rt.top() - totalPos);
            if (right) rt.setRight(rt.right() + totalPos);
            if (down) rt.setBottom(rt.bottom() + totalPos);
        } else if (shift) {
            if (left) rt.setLeft(rt.left() + totalPos);
            if (top) rt.setTop(rt.top() + totalPos);
            if (right) rt.setRight(rt.right() - totalPos);
            if (down) rt.setBottom(rt.bottom() - totalPos);
        } else {
            qDebug() << "m_actionType == ActionType::AT_stretch_picked_rect, but none Modifier!";
        }

        bUpdate = true;
    } else if (m_actionType == ActionType::AT_move_picked_rect) {
        if (left) rt.moveLeft(rt.left() - totalPos);
        if (top) rt.moveTop(rt.top() - totalPos);
        if (right) rt.moveRight(rt.right() + totalPos);
        if (down) rt.moveBottom(rt.bottom() + totalPos);

        bUpdate = true;
    }

    if (bUpdate) {
        m_node.pickedRect = m_node.absoluteRect;
        showCustomWidget(m_paintBar);
        showPickedRectTips();
        update();
    }

    m_actionType = ActionType::AT_wait;
}

// 对 m_node.pickedRect 进行偏移，且  pickedRect 同步给 p1，p2
void ScreenShot::setMovePickedRect()
{
    const auto& px = m_node.p3 - m_node.pt;
    m_node.pickedRect.translate(px);
    m_node.pt = m_node.p3;
    m_node.p1 = m_node.pickedRect.topLeft();
    m_node.p2 = m_node.pickedRect.bottomRight();
}

// 设置 w 的显示位置
void ScreenShot::showCustomWidget(QWidget *w)
{
    if (!w && m_node.pickedRect.isEmpty()) return;
    w->raise(); // fix: 本窗口无 Qt::WindowStaysOnTopHint 属性， paintBtnsBar 出现后，在此点击便会消失不见的 bug；

    const QRect& pickedRect(m_node.absoluteRect);
    const QRect& wRect(w->rect());
    QPoint pt;

    const bool& isShow = m_actionType != ActionType::AT_picking_custom_rect && m_actionType != ActionType::AT_picking_detection_windows_rect;
    if (w == m_paintBar) {

        pt = customWidgetShowPositionRule(CustomWidgetType::CWT_paint_btns_bar);
        if (isShow) {
            if (acrylicEffectEnable()) {
                const auto& t = finishDrewPixmap().copy(QRect(pt, m_paintBar->rect().size())); // fix: toolbar 覆盖已经绘画的位置，没有被包含进去
                const int& adius = CJ_GET("advanced.customize_ui_parameters.blur_effect_adius");
                m_paintBar->setLowerBlurEffect(t, adius);  // 此函数会照成主线程的绘画，函数卡顿
            } else {
                m_paintBar->disableBlurEffect();
            }
        }

        w->move(pt);
//        qDebug() << "m_actionType:" << actionTypeToString(m_actionType) << "pt" << pt << "pickedRect:" << pickedRect << "wRect:" << wRect;
        isShow ? w->show() : w->hide();
    } else if (w == m_pickedRectTips) {

        pt = customWidgetShowPositionRule(CustomWidgetType::CWT_picked_rect_tooptip);
        w->move(pt);
        pickedRect.isValid() && isShow ? w->show() : w->hide();
    }
}

void ScreenShot::showCrosshair(QPainter &pa, const QPoint &pt, const QRect &vdRt) const
{
    if (m_actionType == ActionType::AT_picking_detection_windows_rect || m_actionType == ActionType::AT_picking_custom_rect)
        drawCrosshair(pa, pt, vdRt);
}

void ScreenShot::showCollimatorCursor(QPainter &pa)
{
    pa.save();
    pa.setRenderHint(QPainter::Antialiasing);

    const QPoint& pt = QCursor::pos();
    const int& width = CJ.m_cd.pen.width();
    const QPen pen(CJ.m_cd.pen.color(), 1);
    pa.setPen(pen);
    // 绘制中心点
    pa.setBrush(CJ.m_cd.pen.color());
    const int& r = qMax(1, width / 2);
    pa.drawEllipse(pt, r, r);

    // 绘制十字准星光标
    const int& margin = 5 + r;
    const int& length = 10 + margin;
    QLine l1(QPoint(pt.x() - length, pt.y()), QPoint(pt.x() - margin, pt.y()));
    QLine l2(QPoint(pt.x() + margin, pt.y()), QPoint(pt.x() + length, pt.y()));
    QLine l3(QPoint(pt.x(), pt.y() - length), QPoint(pt.x(), pt.y() - margin));
    QLine l4(QPoint(pt.x(), pt.y() + margin), QPoint(pt.x(), pt.y() + length));
    QVector<QLine> lines;
    lines << l1 << l2 << l3 << l4;
    pa.drawLines(lines);

    pa.restore();
}

void ScreenShot::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) return;
    dealMousePressEvent(e);
    showCustomWidget(m_paintBar);
    showPickedRectTips();
    update();
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) return;
    dealMouseReleaseEvent(e);
    showCustomWidget(m_paintBar);   // 初次右下角的位置会有点错误，就很奇怪,因为初次show 时，其宽度不对，先show一下即可
    showPickedRectTips();
    update();

    qDebug() << "mouseReleaseEvent m_redo:" << m_redo.size();
}

void ScreenShot::mouseMoveEvent(QMouseEvent *e)
{
    dealMouseMoveEvent(e);
    showCustomWidget(m_paintBar);
    showPickedRectTips();
    update();
}

void ScreenShot::wheelEvent(QWheelEvent *e)
{
    // Note: On X11 this value is driver specific and unreliable, use angleDelta() instead
    // QPoint numPixels = event->pixelDelta();
    QPoint degrees = e->angleDelta() / 8;
    if (degrees.isNull()) return;
    QPoint step = degrees / 15;
    const int& stepY = step.y() > 0 ? 1 : -1;

    int width = -1;
    if (m_paintNode.pst == PaintShapeType::PST_mosaic) {
    } else if (m_paintNode.pst == PaintShapeType::PST_marker_pen) {
        m_paintNode.markerPenWidth += stepY;
        showPointTips(QString::number(m_paintNode.markerPenWidth));
    } else if (m_paintNode.pst == PaintShapeType::PST_text) {
        setTextFontSize(stepY, width, true);
    } else {
        const int min = 1;
        const int max = 200;
        width = m_paintNode.pen.widthF();
        width += stepY;
        width = qBound<int>(min, width, max);
        m_paintNode.pen.setWidthF(width);
        showPointTips(QString::number(width));

        CJ_CD.pen = m_paintNode.pen;

        update();
    }

    e->ignore();
}

void ScreenShot::keyReleaseEvent(QKeyEvent *e)
{
    int keyEnumValue = e->key();
    QMetaEnum keyEnum = QMetaEnum::fromType<Qt::Key>();
    QString keyName = keyEnum.valueToKey(keyEnumValue);

    qDebug() << "e->key():" << keyName << keyEnumValue << e->text();
    if (e->key() == Qt::Key_Escape) {
        preDestruction();
        close();
        e->accept();
        // hide() 和 close() 区别: https://stackoverflow.com/questions/39407564
        // 销毁再不会有问题,由单例改写为 new 形式了。排查：1. tray 有关，改用 qpushbutton 和 close即可； 2.单例有关，该市建议修改为 new 指针的比较合适
    } else if (e->key() == Qt::Key_QuoteLeft || e->key() == Qt::Key_Agrave) {  // 重音符号键 ` 或者 波浪键 ~
        const bool& detialInfo = CJ_GET("advanced.customize_ui_parameters.show_windows_detial_info");
        CJ_SET("advanced.customize_ui_parameters.show_windows_detial_info", !detialInfo);
    } else if (e->key() == Qt::Key_Tab) {
        const bool& bMinLevelDepth = CJ_GET("advanced.customize_ui_parameters.auto_detection_windows_rect_top_level");
        CJ_SET("advanced.customize_ui_parameters.auto_detection_windows_rect_top_level", !bMinLevelDepth);
        firstRectNodesAssignmentNode();
    }

    adjustPickedRect(e);
    QWidget::keyReleaseEvent(e);
    update();
}


void ScreenShot::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    const auto& pickedRect = m_node.pickedRect;
    QPainter pa(this);     // 始终从相对本窗口的坐标 (0, 0) 开始绘画
    pa.setBrush(Qt::NoBrush);
    pa.setPen(Qt::NoPen);

    if (!m_origPix.isNull()) {
        pa.drawPixmap(QPoint(0, 0), m_origPix);
    }

    for (const auto& it : m_redo) {
        it.printf();
        drawShape(it, pa);
    }

    if (m_paintNode.bShow)
        drawShape(m_paintNode, pa);


    if (!m_ocrGeneratePix.isNull())
        pa.drawPixmap(pickedRect, m_ocrGeneratePix);

//    pa.setPen(Qt::red);
//    pa.drawRect(m_paintNode.node.absoluteRect);
//    pa.drawRect(largestRect(m_paintNode.node.p1, m_paintNode.node.p2));
//    pa.setPen(Qt::NoPen);

    drawShadowOverlay(rect(), pickedRect, pa);
    drawBorder(pa, pickedRect);
    showCrosshair(pa, mapFromGlobal(QCursor::pos()), rect());

    // 以下部分都是 printf 一些调试参数的部分
    if (CJ_GET("advanced.customize_ui_parameters.show_windows_detial_info").get<bool>()) {
        prinftWindowsRects(pa);
        printfDevelopProjectInfo(pa);
    }

    if (CJ.m_cd.isShowCollimatorCursor)
        showCollimatorCursor(pa);
}

void ScreenShot::closeEvent(QCloseEvent *e)
{
    CJ.onSyncToFile();
    QWidget::closeEvent(e);
}

const QRect xrectToQRect(const XRECT &rect)
{
    return QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}


