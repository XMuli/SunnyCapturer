// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "screenshot.h"
#include <QScreen>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QIcon>
#include <QTime>
#include <QTimer>
#include <QClipboard>
#include <QFileDialog>
#include <QImage>
#include <QTextEdit>
#include <QLine>
#include <QToolButton>
#include <QPainterPath>
#include <QGuiApplication>
#include <QDir>
#include <QMouseEvent>
#include <QTimer>
#include <QWindow>
#include <QStandardPaths>
#include <QStringList>
#include <QTextCharFormat>
#include <QFont>
#include <QFontInfo>
#include <QDebug>
#include <QDir>
#include <QShortcut>
#include <QSystemTrayIcon>
#include <QDir>
#include <QMessageBox>
#include "../core/arrowline.h"
#include "../platform/wininfo.h"
#include "../tool/pin/pinwidget.h"
#include "rectcalcu.h"
#include "../screen/datamaid.h"
#include "tray.h"

namespace Util {
    bool getRectFromCurrentPoint(WinID winId, QRect &outRect)
    {
        const QRect rt = WinInfo::instance().targWinRect(winId, false);

        outRect.setLeft(rt.left());
        outRect.setTop(rt.top());
        outRect.setRight(rt.right());
        outRect.setBottom(rt.bottom());
        return true;
    }
}


XDrawStep ScreenShot::m_step = XDrawStep();

ScreenShot::ScreenShot(QWidget *parent)
	: QWidget(parent)
    , m_scal(DATAMAID->scale())
    , m_captureScrnRt()
    , m_curPix(nullptr)
    , m_rtCalcu(this)
    , m_bSmartWin(DATAMAID->paraValue("smartWindow").toBool())
    , m_bFirstSel(false)
    , m_bFirstPress(false)
    , m_pCurShape(nullptr)
    , m_scrns()
    , m_autoDetectRt(0, 0, 0, 0)
    , m_barOrien(Qt::Horizontal)
    , m_selBar(std::make_unique<SelectBar>(m_barOrien, this))
    , m_paraBar(std::make_unique<ParameterBar>(m_barOrien, this))
    , m_selSizeTip(std::make_unique<SelectSize>("", this))
    , m_widthTip(std::make_unique<SelectSize>("", this))
    , m_edit(std::make_unique<XTextWidget>(this))
{
    qInfo() << QString("bootUniqueId[%1]\n").arg(QSysInfo::bootUniqueId().data())
            << QString("buildAbi[%1]\n").arg(QSysInfo::buildAbi().toUtf8().data())
            << QString("buildCpuArchitecture[%1]\n").arg(QSysInfo::buildCpuArchitecture().toUtf8().data())
            << QString("currentCpuArchitecture[%1]\n").arg(QSysInfo::currentCpuArchitecture().toUtf8().data())
            << QString("kernelType[%1]\n").arg(QSysInfo::kernelType().toUtf8().data())
            << QString("kernelVersion[%1]\n").arg(QSysInfo::kernelVersion().toUtf8().data())
            << QString("machineHostName[%1]\n").arg(QSysInfo::machineHostName().toUtf8().data())
            << QString("machineUniqueId[%1]\n").arg(QSysInfo::machineUniqueId().data())
            << QString("prettyProductName[%1]\n").arg(QSysInfo::prettyProductName().toUtf8().data())
            << QString("productType[%1]\n").arg(QSysInfo::productType().toUtf8().data())
            << QString("productVersion[%1]\n").arg(QSysInfo::productVersion().toUtf8().data());


    setAttribute(Qt::WA_DeleteOnClose, true);
    //setAttribute(Qt::WA_QuitOnClose, false);
    m_edit->setTextColor(Qt::red);
    m_edit->setFont(m_step.font);
    m_edit->setVisible(false);

    const auto& scrns = qGuiApp->screens();
    for (const auto& it : scrns)
        m_scrns.emplace(std::pair(it, ScrnType::ST_Associated | ScrnType::ST_FullScreen));
    auto it = m_scrns.find(qGuiApp->primaryScreen());
    if (it != m_scrns.cend())
        it->second |= ScrnType::ST_Primary;

    m_captureScrnRt = priScrn()->virtualGeometry();
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | windowFlags());  // | Qt::WindowStaysOnTopHint
    #ifdef _MYDEBUG
        m_captureScrnRt = curScrn()->geometry();
        setWindowFlag(Qt::WindowStaysOnTopHint, false);
        if (m_scrns.size() == 1)
            m_captureScrnRt.setWidth(m_captureScrnRt.width() / 2);
    #endif

    setFixedSize(m_captureScrnRt.size());
    move(m_captureScrnRt.topLeft());
#else // Q_OS_MAC
    QRect geom = curScrn(QCursor::pos())->geometry();
    m_captureScrnRt = geom;
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);   // 窗口置顶 + 隐藏标题栏
    #ifdef _MYDEBUG
        if (m_scrns.size() == 1)
            geom.setWidth(geom.width() / 2);
    #endif

    setFixedSize(geom.size());   // resize() cannot get the desired effect
    move(geom.topLeft());
    qDebug() << "#2-->" << geom << "   " << this->rect();

#endif

    setMouseTracking(true);
    m_rtCalcu.scrnType = ScrnOperate::SO_Wait;

    // new refactor
    m_selSizeTip->setVisible(false);
    m_widthTip->setVisible(false);
    m_widthTip->setFixedSize(50, 50);
    m_widthTip->move(0, 0);
    m_selBar->setVisible(false);
    m_paraBar->setVisible(false);
    connect(m_selBar.get(), &SelectBar::sigEnableDraw, this, &ScreenShot::onEnableDraw);
    connect(m_selBar.get(), &SelectBar::sigSelShape, this, &ScreenShot::onSelShape);
    connect(m_selBar.get(), &SelectBar::sigRevocation, this, &ScreenShot::onRevocation);
    connect(m_selBar.get(), &SelectBar::sigRenewal, this, &ScreenShot::onRenewal);
    connect(m_selBar.get(), &SelectBar::sigPin, this, &ScreenShot::onPin);
    connect(m_selBar.get(), &SelectBar::sigSave, this, &ScreenShot::onSave);
    connect(m_selBar.get(), &SelectBar::sigCancel, this, &ScreenShot::onCancel);
    connect(m_selBar.get(), &SelectBar::sigFinish, this, &ScreenShot::onFinish);
    connect(m_selBar.get(), &SelectBar::sigInterruptEdit, this, &ScreenShot::onInterruptEdit);

    connect(m_paraBar.get(), &ParameterBar::sigParaBtnId, this, &ScreenShot::onParaBtnId);
    connect(m_paraBar.get(), &ParameterBar::sigSelColor, this, &ScreenShot::onSelColor);

    connect(m_selBar.get(), &SelectBar::sigEnableDraw, m_paraBar.get(), &ParameterBar::onEnableDraw);
    connect(m_selBar.get(), &SelectBar::sigSelShape, m_paraBar.get(), &ParameterBar::onSelShape);
    connect(this, &ScreenShot::sigClearScreen, this, &ScreenShot::onClearScreen);
    connect(this, &ScreenShot::sigLineWidthChange, this, &ScreenShot::onLineWidthChange);

    new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F4), this, SLOT(onQuickSave()));
    connect(this, &ScreenShot::sigNotificQuickSave, &(Tray::instance()), &Tray::onNotificQuickSave);
}

void ScreenShot::launchCapture(CaptureHelper::CaptureType type)
{
    if (type == CaptureHelper::SST_ScrnCapture) {
        windowCapture();
    } else if (type == CaptureHelper::SST_DelayCapture) {
        delayCapture();
    } else if (type == CaptureHelper::SST_FullScrnCapture) {
        fullScrnCapture();
    } else {
    }

    if (m_bSmartWin)
        updateAutoDetectRt();
    if (!isActiveWindow())
        activateWindow();     // fix: Initial use of global hotkeys to summon the screenshot window does not respond to Esc.  https://ifmet.cn/posts/44eabb4d

}

ScrnOperate ScreenShot::updateScrnType(const QPoint pos) const
{
	CursorArea cursArea = m_rtCalcu.getCursorArea(pos);

	// 后面加上绘画和选择
	if (cursArea == CursorArea::Internal) {
		return ScrnOperate::SO_Move;
	} else if (cursArea == CursorArea::External) {
		return ScrnOperate::SO_Wait;
	} else if (cursArea == CursorArea::Border) {
		return ScrnOperate::SO_Stretch;
    } else {
        return ScrnOperate::SO_Wait;
    }
}

void ScreenShot::updateCursorShape(const QPoint pos)
{
    if (m_rtCalcu.scrnType == ScrnOperate::SO_Draw)
        return;

    CursorArea cursArea = m_rtCalcu.getCursorArea(pos, true);
    if (m_rtCalcu.scrnType == ScrnOperate::SO_Move) {
    } else if (m_rtCalcu.scrnType == ScrnOperate::SO_Select) {
        setCursor(Qt::CrossCursor);
    } else if (m_rtCalcu.scrnType == ScrnOperate::SO_Stretch) {
    } else if (m_rtCalcu.scrnType == ScrnOperate::SO_Wait) {
        if (!DATAMAID->paraValue("smartWindow").toBool())
            return;

        if (cursArea == CursorArea::External) {

            (m_autoDetectRt.contains(pos, false) && !m_bFirstSel) || m_rtCalcu.getSelRect().contains(pos, true) ?
                setCursor(Qt::ArrowCursor) : setCursor(Qt::ForbiddenCursor);
        } else if (cursArea == CursorArea::Internal){
            setCursor(Qt::SizeAllCursor);
            // 已修复：光标移动到绘画工具栏时，光标显示异常，在里面的事件过滤器中修复此出逻辑纰漏。光标进入 tbBar 后，
            // 光标坐标也【不会开始改变】，至少在 ScreenShot。
        } else {
            updateBorderCursorShape(cursArea);
        }
    }
}

// 功能函数，仅考虑边框状态
void ScreenShot::updateBorderCursorShape(const CursorArea & cursArea)
{
    if (cursArea == CursorArea::Top || cursArea == CursorArea::Bottom)
        setCursor(Qt::SizeVerCursor);
    else if (cursArea == CursorArea::Left || cursArea == CursorArea::Right)
        setCursor(Qt::SizeHorCursor);
    else if (cursArea == CursorArea::TopLeft || cursArea == CursorArea::BottomRight)
        setCursor(Qt::SizeFDiagCursor);
    else if (cursArea == CursorArea::TopRight || cursArea == CursorArea::BottomLeft)
        setCursor(Qt::SizeBDiagCursor);
}

// 清空截图内容（当关闭 Esc、或完成截图时）
void ScreenShot::onClearScreen()
{
    XDrawStep::serialText = "0_0_0_0";

    m_bFirstPress = false;
    m_scrnRts.clear();

	//m_screens、m_primaryScreen 还保留
	delete m_curPix;
    m_curPix = nullptr;
    m_pCurShape = nullptr;

	m_vDrawed.clear();
	m_vDrawUndo.clear();
	m_rtCalcu.clear();

    m_bFirstSel = false;
    m_selSizeTip->setVisible(false);
    m_widthTip->setVisible(false);

    m_selBar->setVisible(false);
    m_paraBar->setVisible(false);
    m_step.destroyClear();

    m_autoDetectRt = QRect();
    if (m_rtCalcu.scrnType == ScrnOperate::SO_Wait) // 状态重置
        setMouseTracking(true);
};

void ScreenShot::onEnableDraw(bool enable)
{
    if (enable) {
        m_rtCalcu.scrnType = ScrnOperate::SO_Draw;
        setMouseTracking(false);  // Fix: 鼠标移动中会被自动绘画矩形，副作用绘画状态的光标不完美了(选中框内外的光标被固定了)，严格不算 bug，一种外观特效
    //    qInfo()<<"--------------onDrawStart"<<m_rtCalcu.scrnType;
    } else {
        m_rtCalcu.scrnType = ScrnOperate::SO_Wait;
        setMouseTracking(true);  // 等待状态开启鼠标跟踪
    //    qInfo()<<"--------------onDrawEnd"<<m_rtCalcu.scrnType;
    }
}

void ScreenShot::onSelShape(DrawShape shape, bool checked)
{
    if (!checked)
        return;

    m_step.shape = shape;
    const bool bRect = shape == DrawShape::Rectangles;
    const bool bEllipses = shape == DrawShape::Ellipses;
    const bool bLineWidth = shape == DrawShape::LineWidth;
    const bool bCustomPath = shape == DrawShape::CustomPath;
    const bool bArrows = shape == DrawShape::Arrows;
    const bool bMosaics = shape == DrawShape::Mosaics;
    const bool bText = shape == DrawShape::Text;
    const bool bSeriNum = shape == DrawShape::SerialNumberShape;

    if (bRect || bEllipses || bArrows || bCustomPath || bMosaics || bText || bSeriNum) {
        m_step.shapePara = static_cast<ShapePara>((int)(property(std::to_string((int)shape).c_str()).value<int>()));

    } else if (bLineWidth) {
        //int idx = property(std::to_string((int)shape).c_str()).value<int>();
        //int penWidth = 0;

        //if (idx == 0)
        //    penWidth = 1;
        //else if (idx == 1)
        //    penWidth = 2;
        //else if (idx == 2)
        //    penWidth = 4;
    } else {
        //XLOG_INFO
    }
    //qDebug() << "--------@onDrawShape:" << int(m_step.shape);
}

void ScreenShot::onRevocation()
{
    if (m_vDrawed.empty())
        return;

    m_vDrawUndo.push_back(*(m_vDrawed.end() - 1));
    m_vDrawed.pop_back();
    qDebug() << "---->m_vDrawRevoke:" << m_vDrawUndo.size() << "    m_vDraw:" << m_vDrawed.size();
    update();
}

void ScreenShot::onRenewal()
{
    if (m_vDrawUndo.empty())
        return;

    m_vDrawed.push_back(*(m_vDrawUndo.end() - 1));
    m_vDrawUndo.pop_back();
    qDebug() << "---->m_vDrawRevoke:" << m_vDrawUndo.size() << "    m_vDraw:" << m_vDrawed.size();
    update();
}

void ScreenShot::onPin()
{
    if (m_savePix.isNull())
        return;

    #ifdef Q_OS_MAC
        setWindowFlags(Qt::Dialog);
        showNormal();
    #endif

    if (drawToCurPixmap()) {
        auto pin = new PinWidget(m_savePix, m_savePix.rect(), nullptr);   // 使用 nullptr，不会泄露
        pin->move(mapToGlobal(m_rtCalcu.getSelRect().topLeft()));
        pin->show();

        clearnAndClose();
    }
}

void ScreenShot::onSave()
{
    if (drawToCurPixmap()) {
        // Manual save
        const QString imageName = DATAMAID->formatToFileName(DATAMAID->paraValue(toFileName).toString());
        QString fileter(tr("Image Files(*.png);;Image Files(*.jpg);;All Files(*.*)"));
        QString fileNmae = QFileDialog::getSaveFileName(this, tr("Save Files"), imageName, fileter);
        if (fileNmae.isEmpty()) {
            return;
        } else {
            int ret = fileNmae.lastIndexOf('/');
            if (ret == -1)
                ret = fileNmae.lastIndexOf('\\');

            if (ret != -1) {
                const QString realFileName = fileNmae.right(fileNmae.count() - ret -1);
                DATAMAID->setParaValue("realFileName", realFileName);
            }
        }

        QTime startTime = QTime::currentTime();
        m_savePix.save(fileNmae, nullptr, DATAMAID->paraValue(toImageQuailty).toInt());  // 绘画在 m_savePix 中，若在 m_savePix 会有 selRect 的左上角的点的偏移
        QTime stopTime = QTime::currentTime();
        int elapsed = startTime.msecsTo(stopTime);
        qInfo() << "m_savePix save time: " << elapsed << " ms" << m_savePix.size();

        // save to clipboard
        if (DATAMAID->paraValue(tiAutoCopy2Clipboard).toBool())
            QApplication::clipboard()->setPixmap(m_savePix);

        imageAutoSave();
    }

    clearnAndClose();
}

void ScreenShot::clearnAndClose()
{
    emit sigClearScreen();
    close();
}

void ScreenShot::onCancel()
{
    clearnAndClose();
}

void ScreenShot::onFinish()  // 实际就是复制到剪切板
{
    if (drawToCurPixmap() && !m_savePix.isNull())
        QApplication::clipboard()->setPixmap(m_savePix);

    imageAutoSave();
    clearnAndClose();
}

// 响应快速保存的时候快捷键
void ScreenShot::onQuickSave()
{
    if (drawToCurPixmap()) {
        // Manual save
        const QString path = DATAMAID->paraValue(toQuickSavePath).toString();
        const QString name = DATAMAID->formatToFileName(DATAMAID->paraValue(toFileName).toString());
        QString pathName = path + "/" + name;


        QDir dir(path);
        if(!dir.exists()) {
             if (!dir.mkdir(path)) {
                 QMessageBox::critical(nullptr, tr("Fail"), tr("Failed to create folder %1, please try with administrator privileges").arg(path));
                 return;
             }
         }


        QTime startTime = QTime::currentTime();
        bool ret = m_savePix.save(pathName, nullptr, DATAMAID->paraValue(toImageQuailty).toInt());
        QTime stopTime = QTime::currentTime();
        int elapsed = startTime.msecsTo(stopTime);
        qInfo() << "m_savePix save time: " << elapsed << " ms" << m_savePix.size();

        emit sigNotificQuickSave(ret, pathName);

        if (DATAMAID->paraValue(tiAutoCopy2Clipboard).toBool())
            QApplication::clipboard()->setPixmap(m_savePix);

        imageAutoSave();
    }

    clearnAndClose();
}

void ScreenShot::onInterruptEdit(const QPoint& pos)
{
    QMouseEvent mousePressEvent(QEvent::MouseButtonPress, mapFromGlobal(pos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &mousePressEvent);
}

void ScreenShot::onParaBtnId(DrawShape shape, QToolButton* tb)
{
    if (!tb) return;

    const int idx = tb->objectName().right(1).toInt();
    auto t = tb->objectName();
    const bool bRect = shape == DrawShape::Rectangles;
    const bool bEllipses = shape == DrawShape::Ellipses;
    const bool bMosaics = shape == DrawShape::Mosaics;
    const bool bCustomPath = shape == DrawShape::CustomPath;
    const bool bArrows = shape == DrawShape::Arrows;
    const bool bText = shape == DrawShape::Text;
    const bool bSeriNum = shape == DrawShape::SerialNumberShape;

    const bool bLineWidth = shape == DrawShape::LineWidth;
    const bool bSeriNumType = shape == DrawShape::SerialNumberType;

    if (bRect || bEllipses || bArrows || bMosaics) {
        setProperty(std::to_string((int)shape).c_str(), idx);
        m_step.shapePara = static_cast<ShapePara>(idx);
    } else if (bLineWidth) {
        int lineWidth[3] = { 1, 4, 10 };
        m_step.pen.setWidth(lineWidth[idx]);
        emit sigLineWidthChange(lineWidth[idx]);
    } else if (bCustomPath) {
    } else if (bText) {
        QPen tPen(m_step.pen);
        QBrush tBrush(m_step.brush);
        QFont tfont(m_step.font);
        const bool& checked = tb->isChecked();
        
        static QPen prevPen(m_step.pen);
        QTextCharFormat fmt;
        if (idx == 0) {
            m_step.textParas.setFlag(TextPara::TP_Bold, checked);
            fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
            tfont.setBold(checked);
        } else if (idx == 1) {
            m_step.textParas.setFlag(TextPara::TP_Italic, checked);
            fmt.setFontItalic(checked);
            tfont.setItalic(checked);
        } else if (idx == 2) {
            m_step.textParas.setFlag(TextPara::TP_Outline, checked);
            fmt.setTextOutline(checked ? easyRecognizeColorPen(m_step.brush.color(), true) : Qt::NoPen);
        }

        m_edit->mergeCurrentCharFormat(fmt);
        m_step.font = tfont;
    } else if (bSeriNum || bSeriNumType) {
        // SerialNumberShape: "serialType(0数字/1字母)_bool重置标志(0继续/1重置)_数字序号_字母序号"； 使用 _ 分割
        auto list = XDrawStep::serialText.split('_');
        if (bSeriNum) {
            m_step.shapePara = static_cast<ShapePara>(idx);
        } else if (bSeriNumType) {
            list[0] = QString(QString::number(idx));
            m_step.serialText = list.join("_");
        }
    } else {
    }
}

void ScreenShot::onLineWidthChange(int width)
{
    static QFont font;
    font.setFamily(m_step.font.family());
    font.setPointSize(14);
    m_widthTip->setFont(font);
    m_widthTip->setText(QString::number(width));
    m_widthTip->setAlignment(Qt::AlignCenter);
    m_widthTip->move(mapFromGlobal(curScrn()->geometry().topLeft()));
    m_widthTip->raise();
    m_widthTip->setVisible(true);

    static QTimer* timer = nullptr;
    if (!timer) {
        timer = new QTimer();
        connect(timer, &QTimer::timeout, this, [&]() { m_widthTip->setVisible(false); });
        timer->setSingleShot(true);
        timer->setInterval(4000);
    }

    timer->stop();
    timer->start();
}

void ScreenShot::onSelColor(QColor col)
{
    m_step.pen.setColor(col);
    m_step.brush.setColor(col);

    if (m_step.shape == DrawShape::Text)
        m_edit->setTextColor(col);
}

// 获取虚拟屏幕截图
QPixmap* ScreenShot::virtualScrnPixmap()
{
    if (!m_curPix)
        m_curPix = new QPixmap(priScrn()->grabWindow(qApp->desktop()->winId(), m_captureScrnRt.x(), m_captureScrnRt.y(), m_captureScrnRt.width(), m_captureScrnRt.height()));

//    m_curPix->save(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first() + "/m_currPixmap_123456.png");
    return m_curPix;
}

bool ScreenShot::drawToCurPixmap()
{
    if (!m_savePix || !m_curPix)
        return false;

    QPainter pa;
    pa.begin(m_curPix);
    for (XDrawStep& it : m_vDrawed)
        drawStep(pa, it);
    pa.end();

    QRect rtSel(m_rtCalcu.getSelRect());
    const QRect rtAppVirDesktop(QPoint(0, 0), m_captureScrnRt.size());
    m_rtCalcu.limitBound(rtSel, rtAppVirDesktop);
    const double DPI = GetDevicePixelRatio();
    if (rtSel.isValid())
        m_savePix = m_curPix->copy(QRect(rtSel.topLeft() * DPI, rtSel.size() * DPI));

    return (!m_savePix.isNull() && m_curPix);
}

void ScreenShot::drawBorderBlackWhite (QPainter & pa, const QRect& rt, int num, bool isRound) const
{
	if (num == 0)
		return;

	pa.save();
	pa.setRenderHint(QPainter::Antialiasing, false);
	pa.setBrush(Qt::NoBrush);
	QPen penWhite(QColor(255, 255, 255, 1 * 255), 1);
	penWhite.setStyle(Qt::CustomDashLine);
	penWhite.setDashOffset(0);
    penWhite.setDashPattern(QVector<qreal>() << 4 * ScreenShot::GetScale() << 4 * ScreenShot::GetScale());
	penWhite.setCapStyle(Qt::FlatCap);
	pa.setPen(penWhite);
	pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.right(), rt.top()));
	pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.left(), rt.bottom()));
	pa.drawLine(QPoint(rt.left(), rt.bottom()), QPoint(rt.right(), rt.bottom()));
	pa.drawLine(QPoint(rt.right(), rt.top()), QPoint(rt.right(), rt.bottom()));

	QPen penBlack(penWhite);
	penBlack.setColor(QColor(0, 0, 0, 1 * 255));
	penBlack.setDashOffset(4);
	pa.setPen(penBlack);
	pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.right(), rt.top()));
	pa.drawLine(QPoint(rt.left(), rt.top()), QPoint(rt.left(), rt.bottom()));
	pa.drawLine(QPoint(rt.left(), rt.bottom()), QPoint(rt.right(), rt.bottom()));
	pa.drawLine(QPoint(rt.right(), rt.top()), QPoint(rt.right(), rt.bottom()));

	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	rt.getCoords(&x1, &y1, &x2, &y2);

	QVector<QPoint> ver = { QPoint(x1, y1),
							QPoint(x2, y1),
							QPoint(x1, y2),
							QPoint(x2, y2),
							QPoint((x1 + x2) / 2.0, y1),
							QPoint((x1 + x2) / 2.0, y2),
							QPoint(x1, (y1 + y2) / 2.0),
							QPoint(x2, (y1 + y2) / 2.0) };

	pa.setPen(QPen(Qt::white, 1.5));
	pa.setBrush(QColor(146, 146, 146, 1 * 255));
    QPoint offsetPos(HAIF_R_BORDER_MARK * ScreenShot::GetScale(), HAIF_R_BORDER_MARK * ScreenShot::GetScale());
	pa.setRenderHint(QPainter::Antialiasing, true);

	for (int i = 0; i < num; ++i)
		pa.drawEllipse(ver[i], offsetPos.x(), offsetPos.y());

    pa.restore();
}

void ScreenShot::drawBorderFlipped(QPainter& pa, const QRect& rt, bool isRound) const
{
    pa.save();
    pa.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setWidth(DATAMAID->paraValue("borderWidth").toInt() * m_scal + SELECT_ASSIST_RECT_PEN_WIDTH);
    pen.setColor(DATAMAID->paraValue("borderColor").toString());  //
    pa.setPen(pen);
    pa.setBrush(Qt::NoBrush);

    int x1 = rt.left();
    int y1 = rt.top();
    int x2 = rt.right();
    int y2 = rt.bottom();

    const int penWidth = pen.width();
    const int penAssWidth = SELECT_ASSIST_RECT_WIDTH * m_scal;
    const int penAssWidthDouble = 2 * penAssWidth;

    if (rt.width() >= penAssWidthDouble && rt.height() >= penAssWidthDouble) {
        // hor 且补齐交叉角落的空缺的那一块
        QLine l1(QPoint(x1 - penWidth / 2, y1), QPoint(x1 + penAssWidth, y1));
        QLine l2(QPoint(x1 - penWidth / 2, y2), QPoint(x1 + penAssWidth, y2));
        QLine l3(QPoint(x2 + penWidth / 2, y1), QPoint(x2 - penAssWidth, y1));
        QLine l4(QPoint(x2 + penWidth / 2, y2), QPoint(x2 - penAssWidth, y2));

        // ver
        QLine l5(QPoint(x1, y1), QPoint(x1, y1 + penAssWidth));
        QLine l6(QPoint(x1, y2), QPoint(x1, y2 - penAssWidth));
        QLine l7(QPoint(x2, y1), QPoint(x2, y1 + penAssWidth));
        QLine l8(QPoint(x2, y2), QPoint(x2, y2 - penAssWidth));

        pa.drawLine(l1.translated(QPoint(0, -penWidth / 2)));
        pa.drawLine(l2.translated(QPoint(0, penWidth / 2)));
        pa.drawLine(l3.translated(QPoint(0, -penWidth / 2)));
        pa.drawLine(l4.translated(QPoint(0, penWidth / 2)));
        pa.drawLine(l5.translated(QPoint(-penWidth / 2, 0)));
        pa.drawLine(l6.translated(QPoint(-penWidth / 2, 0)));
        pa.drawLine(l7.translated(QPoint(penWidth / 2, 0)));
        pa.drawLine(l8.translated(QPoint(penWidth / 2, 0)));
    }

    pen.setWidth(DATAMAID->paraValue("borderWidth").toInt() * m_scal);
    pa.setPen(pen);
    pa.drawRect(rt);
    pa.restore();
}

// 此函数内禁止使用 m_step，极其容易导致错误
void ScreenShot::drawStep(QPainter& pa, const XDrawStep& step)
{
    if (DrawShape::NoDraw == step.shape) return;
    if (m_rtCalcu.scrnType == ScrnOperate::SO_Move && m_pCurShape == &step) return;  // 选中图形的处于移动状态，跳过这不绘画
    
    pa.save();
    //pa.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //pa.setRenderHint(QPainter::HighQualityAntialiasing, true);
    pa.setRenderHint(QPainter::Antialiasing, true);
    pa.setFont(step.font);
    pa.setPen(step.pen);
    pa.setBrush(step.brush);

    if (step.shape == DrawShape::Rectangles
        || step.shape == DrawShape::Ellipses
        || step.shape == DrawShape::Arrows
        || step.shape == DrawShape::CustomPath
        || step.shape == DrawShape::Mosaics) {
        if (step.shapePara == ShapePara::SP_0)
            pa.setBrush(Qt::NoBrush);
    }

    switch (step.shape) {
    case DrawShape::Rectangles: {
        if (!step.rt.isEmpty())
            pa.drawRect(step.rt);
        break;
    }
    case DrawShape::Ellipses: {
        if (!step.rt.isEmpty())
            pa.drawEllipse(step.rt.center(), step.rt.width() / 2, step.rt.height() / 2);
        break;
    }
    case DrawShape::Arrows: {
        QPainterPath path;
        ArrowLine arrowLine(step.pen.widthF(), step.p1, step.p2);
        if (step.shapePara == ShapePara::SP_0) {
            path = arrowLine.arrowLine();
            pa.setBrush(step.pen.color());
        } else if (step.shapePara == ShapePara::SP_1) {
            path = arrowLine.arrowLineOpen();
            pa.setBrush(Qt::NoBrush);
        } else if (step.shapePara == ShapePara::SP_2) {
            path = arrowLine.circleLine();
        } else if (step.shapePara == ShapePara::SP_3) {
            path = arrowLine.line();
        }

        pa.drawPath(path);
        break;
    }
    case DrawShape::CustomPath: {
		pa.drawPolyline(step.custPath.data(), step.custPath.size());
		break;
	}
    case DrawShape::Mosaics: {
		if (!m_curPix || step.rt.isEmpty()) break;  // 优化，删除就很明显

        const double DPI = GetDevicePixelRatio();
        QPixmap mosaicPixmap = m_curPix->copy(QRect(step.rt.topLeft() * DPI, step.rt.size() * DPI));
        const int mscPx = step.pen.width();
        if (step.shapePara == ShapePara::SP_0) {
            const QPixmap* pix = DATAMAID->smoothMosaic(&mosaicPixmap, mscPx);
            pa.drawPixmap(step.rt, *pix);
		} else if (step.shapePara == ShapePara::SP_1) {
            const QImage img = DATAMAID->pixlelatedMosaic(&mosaicPixmap, mscPx);
            pa.drawImage(step.rt, img);
        }

        break;
    }
    case DrawShape::Text: {
        // Ref: http://qtdebug.com/qtbook-paint-text  https://doc.qt.io/qt-5/qpainter.html#drawText-5
        // pa.drawPath: 遇到 \n 不会换行，自行分割解决; 可以实现描边效果 -> QPen 是描边颜色， Brush 是字体颜色；【采用此方案】
        // pa.drawText: 遇到 \n  会换行;  无描边效果 -> QPen 是字体颜色；其字体大小由 pa.font.pointSize 控制,而非 pa.pen.width
        if (!step.text.isEmpty() && m_edit) {
            QPen tPen(step.pen);
            tPen.setWidthF(2.0);
            pa.setPen(tPen);

            pa.setFont(step.font);
            const QFontMetrics fm(pa.fontMetrics());
            auto l = step.text.split(QChar('\n'));
            int flags = Qt::TextWrapAnywhere;
            QRect textBoundingRect = fm.boundingRect(QRect(0, 0, m_captureScrnRt.width(), 0), flags, step.text);
            textBoundingRect.moveTopLeft(step.rt.topLeft());
            const int val = 5;
            auto topLeft = textBoundingRect.topLeft() + QPoint(val, fm.ascent() + val);

            QPen outlinePen(easyRecognizeColorPen(step.brush.color()));
            outlinePen.setWidthF(1.2); // 轮廓描边
            pa.setPen((step.textParas & TextPara::TP_Outline) ? outlinePen : Qt::NoPen);
            
//            step.showDebug();
            QPainterPath path;
            for (const auto &it : l) {
                path.addText(topLeft, pa.font(), it);
                topLeft += QPoint(0, fm.ascent() + fm.leading());
            }
            pa.drawPath(path);
        }
        break;
    }
    case DrawShape::SerialNumberShape:
    case DrawShape::SerialNumberType: {
        if (step.text.isEmpty())
            break;
        // SerialNumberShape: "serialType(0数字/1字母)_bool重置标志(0继续/1重置)_数字序号_字母序号"； 使用 _ 分割        
        QString str;
        const auto& list = step.text.split('_');
        str = (list[0].toInt() == 0) ? list[2] : QString('A' + list[3].toInt() - 1);

        const QFontMetrics fm(pa.fontMetrics());
        QRect textBoundingRect = fm.boundingRect(QRect(0, 0, m_captureScrnRt.width(), 0), Qt::TextWrapAnywhere, str);
        const int width = qMax<int>(textBoundingRect.width(), textBoundingRect.height());
        textBoundingRect.setSize(QSize(width, width));
        textBoundingRect.moveCenter(step.p1);
        const int margin = qMax<int>(width / 8, step.pen.width());
        const QRect adjustRt = textBoundingRect.adjusted(-margin, -margin, margin, margin);

        pa.setPen(QPen(easyRecognizeColorPen(step.pen.color()).color(), qBound<double>(1.2, step.pen.widthF() / 10.0, 5)));
        step.shapePara == ShapePara::SP_0 ? pa.drawRoundedRect(adjustRt, 8, 8) : pa.drawEllipse(adjustRt);
        pa.setPen(QPen(easyRecognizeColorPen(step.brush.color()).color(), step.pen.widthF()));
        pa.setBrush(Qt::NoBrush);

        QFont font;
        font.setFamily(m_step.font.family());
        font.setPointSizeF(qMax<double>(SN_Min, step.pen.widthF()));
        pa.setFont(font);

        pa.drawText(adjustRt, Qt::AlignCenter, str);    // No outline effect
        break;
    }
    default:
        break;
    }

    pa.restore();
}

bool ScreenShot::isDrawShape(XDrawStep& step) const
{
    bool bSerial = m_step.shape == DrawShape::SerialNumberShape || m_step.shape == DrawShape::SerialNumberType;
    if (bSerial && !step.text.isEmpty())
        return true;

    if (step.p1 == step.p2)
        return false;

    return true;
}

const QVector<QPoint> ScreenShot::drawBarPosition(Qt::Orientation orien /*= Qt:: Horizonta*/, ToolBarOffset offset /*= ToolBarOffset::TBO_Middle*/) const
{
    QVector<QPoint> vec;
    if (!m_selBar || !m_paraBar)
        return vec;

    const int space = B_SPACEING;
    int selBarHeight = m_selBar->height();
    int paraBarHeight = m_paraBar->height();

    if (orien == Qt::Vertical) {
        selBarHeight = m_selBar->width();
        paraBarHeight = m_paraBar->width();
    }

    const int sBarHeight = space + selBarHeight;
    const int pBarHeight = space + paraBarHeight;
    const QRect rtSel(m_rtCalcu.getSelRect());
    const int barMaxTop = rtSel.top() - sBarHeight;
    const int barMaxBottom = rtSel.bottom() + sBarHeight;
    const int barMaxLeft = rtSel.left() - sBarHeight;
    const int barMaxRight = rtSel.right() + sBarHeight;

    QScreen* scrn = curScrn(rtSel.bottomRight());
    if (!scrn) scrn = qGuiApp->screenAt(rtSel.topRight());
    if (!scrn) scrn = qGuiApp->screenAt(QCursor::pos());
    QRect rtScrn = scrn->geometry();
    QPoint p1;  // selBar
    QPoint p2;  // paraBar
    if (orien == Qt::Horizontal) {
        p1 = QPoint(rtSel.center().x() - m_selBar->width() / 2, rtSel.bottom() + space); // 默认底部中间
        p2 = QPoint(p1.x(), p1.y() + space + selBarHeight);

        if (offset == ToolBarOffset::TBO_Left)
            p1.setX(rtSel.left());
        else if (offset == ToolBarOffset::TBO_Right)
            p1.setX(rtSel.right() - m_selBar->width());

        int topLimit = qMax(m_captureScrnRt.top(), rtScrn.top());
        int bottomLimit = qMin(m_captureScrnRt.bottom(), rtScrn.bottom());

        if (barMaxTop > topLimit) { // 选中框，上未触顶
            if (barMaxBottom > bottomLimit) { // 底部触顶
                p1.setY(rtSel.top() - sBarHeight);
                p2.setY(p1.y() - pBarHeight);
            }

            if (m_paraBar->isVisible()) {
                if (barMaxBottom + pBarHeight > bottomLimit) {

                    if (barMaxTop - pBarHeight < topLimit) {
                        p1.setY(rtSel.top() + space);
                        p2.setY(rtSel.top() + sBarHeight + space);
                    } else {
                        p1.setY(rtSel.top() - sBarHeight);
                        p2.setY(p1.y() - pBarHeight);
                    }
                } else {
                    p1.setY(rtSel.bottom() + space);
                }
            }

        } else {
            if (barMaxBottom > bottomLimit
                || (m_paraBar->isVisible() && barMaxBottom + pBarHeight > bottomLimit)) { // 底部触顶
                p1.setY(rtSel.top() + space);
                p2.setY(rtSel.top() + sBarHeight + space);
            }
        }

    } else {

        p1 = QPoint(rtSel.left() - (selBarHeight + space), rtSel.center().y() - m_selBar->height() / 2);         // 默认左侧中间
        p2 = QPoint(p1.x() - space * 2 - selBarHeight, p1.y());
        if (offset == ToolBarOffset::TBO_Top)
            p1.setY(rtSel.top());
        else if (offset == ToolBarOffset::TBO_Bottom)
            p1.setY(rtSel.bottom() - m_selBar->height());

        int leftLimit = qMax(m_captureScrnRt.left(), rtScrn.left());
        int rightLimit = qMin(m_captureScrnRt.right(), rtScrn.right());

        if (barMaxLeft > leftLimit) { // 选中框左边未触顶
            //if (barMaxRight > rightLimit) // 右侧触顶
            //    basePos.setY(rtSel.bottom() - barHeight);
        } else {
            p1.setX(rtSel.right() + space);
            p2.setX(p1.x() + space + selBarHeight);

            if (barMaxRight > rightLimit) { // 右侧触顶
                p1.setX(rtSel.left() + space);
                p2.setX(p1.x() + space + selBarHeight);
            }
        }

        p2.setY(p1.y());
    }

    vec << p1 << p2;
    return vec;
}

const QPoint ScreenShot::drawSelSizeTip(const QRect& rt) const
{
    const QPoint pos = mapToGlobal(rt.topLeft());
    QString str = QString("[%1, %2, %3 * %4]").arg(pos.x()).arg(pos.y()).arg(rt.width()).arg(rt.height());
    if (str.compare(m_selSizeTip->text()) != 0) {
        emit m_selSizeTip->sigTextChanged(str);
        m_selSizeTip->setText(str);
    }
    
    return QPoint(rt.left(), rt.top() - m_selSizeTip->height() - SS_SPACE_TO_SELECTRECT);
}

void ScreenShot::selectedShapeMove(QPainter& pa) const
{
    QRect rtCurMove;
    if (m_pCurShape && (m_rtCalcu.scrnType == ScrnOperate::SO_Wait || m_rtCalcu.scrnType == ScrnOperate::SO_Move)) {
        pa.save();

        rtCurMove = m_pCurShape->rt.translated(m_rtCalcu.pos2 - m_rtCalcu.pos1);
        pa.setRenderHint(QPainter::Antialiasing, false);
        pa.setBrush(Qt::NoBrush);
        QPen penWhite(QColor(255, 255, 255, 1 * 255), 1);
        penWhite.setStyle(Qt::CustomDashLine);
        penWhite.setDashOffset(0);
        penWhite.setDashPattern(QVector<qreal>() << 4 * ScreenShot::GetScale() << 4 * ScreenShot::GetScale());
        penWhite.setCapStyle(Qt::FlatCap);
        pa.setPen(penWhite);

        const int r = 3;
        QPen penBlack(penWhite);
        penBlack.setColor(QColor(0, 0, 0, 1 * 255));
        penBlack.setDashOffset(4);

        if (m_pCurShape->shape == DrawShape::Rectangles) {
            pa.setBrush(m_pCurShape->brush);
            pa.setPen(m_pCurShape->pen);
            pa.drawRect(rtCurMove);

            pa.setBrush(Qt::NoBrush);
            pa.setPen(penWhite);
            pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));
            pa.setPen(penBlack);
            pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));
        } else if (m_pCurShape->shape == DrawShape::Ellipses) {
            pa.setBrush(m_pCurShape->brush);
            pa.setPen(m_pCurShape->pen);
            pa.drawEllipse(rtCurMove.center(), rtCurMove.width() / 2, rtCurMove.height() / 2);

            pa.setBrush(Qt::NoBrush);
            pa.setPen(penWhite);
            pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));
            pa.setPen(penBlack);
            pa.drawRect(rtCurMove.adjusted(-r, -r, r, r));

            //pa.setBrush(Qt::NoBrush);
            //pa.setPen(penWhite);

            //// 生成可填充的轮廓
            //QPainterPathStroker stroker;
            //stroker.setCapStyle(Qt::RoundCap);      // 端点风格
            //stroker.setJoinStyle(Qt::RoundJoin);    // 连接样式
            //stroker.setDashPattern(Qt::DashLine);   // 虚线图案
            //stroker.setWidth(penWhite.width());     // 宽度

            //// 生成一个新路径（可填充区域），表示原始路径 path 的轮廓
            //QPainterPath outlinePath = stroker.createStroke(path);


            //pa.drawEllipse(rtCurMove.center(), rtCurMove.width() / 2, rtCurMove.height() / 2);
            //pa.setPen(penBlack);
            //pa.drawEllipse(rtCurMove.center(), rtCurMove.width() / 2, rtCurMove.height() / 2);
        }

        pa.restore();
    }
}
void ScreenShot::whichShape()
{
    bool bChecked = false; // 点击是否选中
    const int r = 4;
    QPoint pos(QCursor::pos());
    for (auto it = m_vDrawed.rbegin(); it != m_vDrawed.rend(); ++it) {
        const auto& rt = it->rt;
        const QRect& rtOut = rt.adjusted(-r, -r, r, r);
        const QRect& rtIn = rt.adjusted(r, r, -r, -r);

        QPainterPath path;
        path.setFillRule(Qt::OddEvenFill);
        if (it->shape == DrawShape::Rectangles) {
            path.addEllipse(rtOut);
            if (it->shapePara == ShapePara::SP_0)
                path.addEllipse(rtIn);
        } else if (it->shape == DrawShape::Ellipses) {

            path.addEllipse(rtOut.center(), rtOut.width() / 2, rtOut.height() / 2);
            if (it->shapePara == ShapePara::SP_0)
                path.addEllipse(rtIn.center(), rtIn.width() / 2, rtIn.height() / 2);
        } else if (it->shape == DrawShape::Arrows) {
            // TODO 2022.07.12: 倾斜的矩形 + 三角形； 旋转坐标轴后绘画？
        }

        if (path.contains(pos)) {
            m_pCurShape = &(*it);
            bChecked = true;
            return;
        }
    }

    m_pCurShape = nullptr;  // 没有选中任何一个
}

void ScreenShot::imageQuickSave()
{
    const bool enable = DATAMAID->paraValue(toQuickSave).toBool();
    if (!enable) return;

    const QString path = DATAMAID->formatToFileName(DATAMAID->paraValue(toQuickSavePath).toString());
    const QString imageName = DATAMAID->paraValue("realFileName").toString();
    QDir dir(path);
    if (dir.exists() && !m_savePix.isNull())
        m_savePix.save(path + QDir::separator() + imageName);
}

void ScreenShot::imageAutoSave()
{
    const bool enable = DATAMAID->paraValue(toAutoSave).toBool();
    if (!enable) return;

    const QString path = DATAMAID->formatToFileName(DATAMAID->paraValue(toAutoSavePath).toString());
    const QString imageName = DATAMAID->paraValue("realFileName").toString();
    QDir dir(path);
    if (dir.exists() && !m_savePix.isNull())
        m_savePix.save(path + QDir::separator() + imageName);
}

// 样式一: 浅蓝色
void ScreenShot::drawBorderLightBlue(QPainter& pa, const QRect& rt, int num, bool isRound) const
{
    if (num == 0)
        return;

    pa.setPen(Qt::NoPen);
    pa.setBrush(Qt::NoBrush);

    QIcon icon(":/resources/border_style/light_blue/boardPoint.svg");
    QPixmap pixmap = icon.pixmap(QSize(HAIF_R_BORDER_MARK, HAIF_R_BORDER_MARK) * 4 * ScreenShot::GetScale());
    pixmap.setDevicePixelRatio(GetDevicePixelRatio());

    QPoint offsetPos(HAIF_R_BORDER_MARK * 2 * ScreenShot::GetScale(), HAIF_R_BORDER_MARK * 2 * ScreenShot::GetScale()) ;
    pa.drawPixmap(rt.topLeft() - offsetPos, pixmap);
    pa.drawPixmap(rt.topRight() - offsetPos, pixmap);
    pa.drawPixmap(rt.bottomLeft() - offsetPos, pixmap);
    pa.drawPixmap(rt.bottomRight() - offsetPos, pixmap);

    if (num <= 8) {
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;

        rt.getCoords(&x1, &y1, &x2, &y2);
        pa.drawPixmap(QPoint((x1 + x2) / 2, y1) - offsetPos, pixmap);
        pa.drawPixmap(QPoint((x1 + x2) / 2, y2) - offsetPos, pixmap);
        pa.drawPixmap(QPoint(x1, (y1 + y2) / 2) - offsetPos, pixmap);
        pa.drawPixmap(QPoint(x2, (y1 + y2) / 2) - offsetPos, pixmap);
	}
}

void ScreenShot::showAllDrawedShape(QPainter& pa)
{
    QPoint posText(0, 100);
    const int space = 15;
//    QRect m_rtCalcu_selRect(m_rtCalcu.getSelRect());

    int i = 0;
    pa.drawText(posText + QPoint(0, space * i), QString("m_vDrawed:%0").arg(m_vDrawed.size()));

    //for (const auto& it : m_vDrawed){
    //    pa.drawText(posText + QPoint(0, space * i), QString("m_vDrawed[%0]:[%1]")
    //        .arg(i++).arg(it.shape));
    //}

    //pa.drawText(posText + QPoint(0, space * 1), QString("pos1: (%1, %2)  pos2: (%3, %4)")
    //    .arg(m_rtCalcu.pos1.x()).arg(m_rtCalcu.pos1.y()).arg(m_rtCalcu.pos2.x()).arg(m_rtCalcu.pos2.y()));
    //pa.drawText(posText + QPoint(0, space * 2), QString("pos2 - pos1:(%1, %2)")
    //    .arg(m_rtCalcu.pos2.x() - m_rtCalcu.pos1.x()).arg(m_rtCalcu.pos2.y() - m_rtCalcu.pos1.y()));
    //pa.drawText(posText + QPoint(0, space * 3), QString("m_rtCalcu.getSelRect(): (%1, %2, %3 * %4)")
    //    .arg(m_rtCalcu_selRect.x()).arg(m_rtCalcu_selRect.y()).arg(m_rtCalcu_selRect.width()).arg(m_rtCalcu_selRect.height()));
    //pa.drawText(posText + QPoint(0, space * 4), QString("rtSel: (%1, %2, %3 * %4)")
    //    .arg(rtSel.x()).arg(rtSel.y()).arg(rtSel.width()).arg(rtSel.height()));
    //pa.drawText(posText + QPoint(0, space * 5), QString("pos(): (%1, %2)")
    //    .arg(pos().x()).arg(pos().y()));
    //pa.drawText(posText + QPoint(0, space * 6), QString("m_rtAtuoMonitor: (%1, %2), rtAtuoMonitor: (%3, %4)")
    //    .arg(m_rtAtuoMonitor.x()).arg(m_rtAtuoMonitor.y()).arg(rtAtuoMonitor.x()).arg(rtAtuoMonitor.y()));
    //pa.drawText(posText + QPoint(0, space * 7), QString("m_rtCalcu.bSmartMonitor: %1")
    //    .arg(m_rtCalcu.bSmartMonitor));
}

// 效果：绘画的顺序重要
void ScreenShot::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);
    if (m_rtCalcu.scrnType == ScrnOperate::SO_Draw)
        setCursor(Qt::CrossCursor);

    // 原始图案
    QPainter pa(this); // 改为始终从 (0, 0) 开始绘画
    pa.setBrush(Qt::NoBrush);
    pa.setPen(Qt::NoPen);
    if (m_curPix)
        pa.drawPixmap(QPoint(0, 0), *m_curPix);

    // 选中矩形图片
    QRect rtSel(m_rtCalcu.getSelRect());   // 移动选中矩形
    if (m_bSmartWin)
        rtSel = m_autoDetectRt;

    const QRect shotGeom = QRect(mapFromGlobal(m_captureScrnRt.topLeft()), m_captureScrnRt.size()); // 修复为相对窗口的
    m_rtCalcu.limitBound(rtSel, shotGeom); // 修复边界时图片被拉伸
    if (rtSel.width() > 0 && rtSel.height() > 0) {
        double devPixRatio = GetDevicePixelRatio(nullptr); // TODO 2022.01.14: QScreen* curScrn() 截图时光标所在的屏幕
        m_savePix = m_curPix->copy(QRect(rtSel.topLeft() * devPixRatio, rtSel.size() * devPixRatio));  // NOTE: devicePixelRatio（macox = 2）
        pa.drawPixmap(rtSel, m_savePix);

        // 放大镜实现
        //QSize tSize(100, 100);
        //auto& mousePos = QCursor::pos();
        ////QRect rtMagnifying(QPoint(mousePos.x() - tSize.width() / 2, mousePos.y() - tSize.height() / 2), tSize);
        //QRect rtPick(mousePos * getDevicePixelRatio(), tSize * getDevicePixelRatio());
        //pa.drawPixmap(mousePos + QPoint(100, 100), m_curPix->copy(rtPick).scaled(tSize * 4, Qt::KeepAspectRatio)); // 放大 4 倍

        // m_savePix 和 m_curPix 的地址没有改变，但前者的 cacheKey 总在变化???
    }

    // 画家准备
    pa.setRenderHint(QPainter::Antialiasing, true);
    const int penWidth = HAIF_INTERVAL;  // 画笔宽度
    QPen pen(QColor("#01bdff"));
    pen.setWidth(penWidth);
    pa.setPen(pen);
    pa.setOpacity(1);
    pa.setBrush(Qt::transparent);

	// 绘画图案
    for (XDrawStep& it : m_vDrawed)
        drawStep(pa, it);

    // 绘画当前步
    pen.setWidth(penWidth / 2);
	pen.setColor(Qt::green);
	pa.setPen(pen);
    bool bDrawSerial = m_step.shape == DrawShape::SerialNumberShape || m_step.shape == DrawShape::SerialNumberType;
    if (!bDrawSerial)
        drawStep(pa, m_step);

    // drawWinInfo(pa);
    selectedShapeMove(pa);
    drawMaskLayer(shotGeom, rtSel, pa);
    drawBorder(rtSel, pa);
    drawCrosshair(pa);
    drawToolBar();

    pen.setColor(Qt::white);
    pa.setPen(pen);
    showDebugInfo(pa, rtSel);
}

void ScreenShot::showDebugInfo(QPainter& pa, QRect& rtSel)
{
#ifdef _MYDEBUG
    // 调试的实时数据
    QFont font;//(font());
    font.setPointSize(12);  // 默认大小为 9
    pa.setFont(font);
    const int space = font.pointSize() * 2.5;

    QPoint tTopLeft;
    tTopLeft.setX(priScrn()->geometry().x());
    tTopLeft.setY(priScrn()->size().height());

    QPoint tPosText(tTopLeft.x(), tTopLeft.y() - 5 * space);
    QRect m_rtCalcu_selRect(m_rtCalcu.getSelRect());

    pa.drawText(tPosText - QPoint(0, space * -1), QString("Wait(0) Select(1) Move(2) Draw(3) Stretch(4)"));
    pa.drawText(tPosText - QPoint(0, space * 0), QString("m_rtCalcu.scrnType: %1")
        .arg(int(m_rtCalcu.scrnType)));
    pa.drawText(tPosText - QPoint(0, space * 1), QString("pos1: (%1, %2)  pos2: (%3, %4)")
        .arg(m_rtCalcu.pos1.x()).arg(m_rtCalcu.pos1.y()).arg(m_rtCalcu.pos2.x()).arg(m_rtCalcu.pos2.y()));
    pa.drawText(tPosText - QPoint(0, space * 2), QString("pos2 - pos1:(%1, %2)")
        .arg(m_rtCalcu.pos2.x() - m_rtCalcu.pos1.x()).arg(m_rtCalcu.pos2.y() - m_rtCalcu.pos1.y()));
    pa.drawText(tPosText - QPoint(0, space * 3), QString("m_rtCalcu.getSelRect(): (%1, %2, %3 * %4)")
        .arg(m_rtCalcu_selRect.x()).arg(m_rtCalcu_selRect.y()).arg(m_rtCalcu_selRect.width()).arg(m_rtCalcu_selRect.height()));
    pa.drawText(tPosText - QPoint(0, space * 4), QString("rtSel: (%1, %2, %3 * %4)")
        .arg(rtSel.x()).arg(rtSel.y()).arg(rtSel.width()).arg(rtSel.height()));
    pa.drawText(tPosText - QPoint(0, space * 5), QString("pos(): (%1, %2)")
        .arg(pos().x()).arg(pos().y()));
    pa.drawText(tPosText - QPoint(0, space * 6), QString("m_rtAtuoMonitor: (%1, %2, %3 * %4)")
        .arg(m_autoDetectRt.x()).arg(m_autoDetectRt.y()).arg(m_autoDetectRt.width()).arg(m_autoDetectRt.height()));
    pa.drawText(tPosText - QPoint(0, space * 7), QString("XHelper::instance().smartWindow(): %1")
        .arg(DATAMAID->paraValue("smartWindow").toBool()));
    pa.drawText(tPosText - QPoint(0, space * 8), QString("m_vDrawed:%1").arg(m_vDrawed.size()));
    if (m_pCurShape) {
        QRect rtCurMove = m_pCurShape->rt.translated(m_rtCalcu.pos2 - m_rtCalcu.pos1);
        pa.drawText(tPosText - QPoint(0, space * 9), QString("rtMoveTest(%1, %2, %3 * %4)").arg(rtCurMove.x()).arg(rtCurMove.y())
            .arg(rtCurMove.width()).arg(rtCurMove.height()));
    }
    pa.drawText(tPosText - QPoint(0, space * 10), QString("m_step=>pos1(%1, %2)  pos2(%3 * %4)  rt(%5, %6, %7 * %8)  text:%9")
        .arg(m_step.p1.x()).arg(m_step.p1.y()).arg(m_step.p2.x()).arg(m_step.p2.y())
        .arg(m_step.rt.x()).arg(m_step.rt.y()).arg(m_step.rt.width()).arg(m_step.rt.height())
        .arg(m_step.text));

    const int tSpace = 10;
    const int barHeight = m_selBar->height();
    QPoint topLeft(rtSel.right() - m_selBar->width(), rtSel.bottom() + tSpace);
    const int barMaxTop = rtSel.top() - tSpace - barHeight;
    const int barMaxBottom = rtSel.bottom() + tSpace + barHeight;

    //qDebug() << rtSel.left() << "  " << rtSel.top() << "  " << rtSel.right() << "  " << rtSel.bottom()
    //    << "[width]  " << rtSel.width() << "  " << rtSel.height()
    //    << "[topLeft]  " << rtSel.topLeft() << "  " << rtSel.bottomRight();
    if (rtSel.width() > 0 && rtSel.height() > 0 ) {
        const auto selScrn = curScrn(rtSel.bottomRight());
        QRect rtScrn;
        if (selScrn) rtScrn = selScrn->geometry();
        int topLimit = qMax(m_captureScrnRt.top(), rtScrn.top());
        int bottomLimit = qMin(m_captureScrnRt.bottom(), rtScrn.bottom());

        pa.drawText(tPosText - QPoint(0, space * 11), QString("barMaxTop:%1   barMaxBottom:%2  m_rtVirDesktop 左上右下(%3, %4, %5 * %6)")
            .arg(barMaxTop).arg(barMaxBottom).arg(m_captureScrnRt.left()).arg(m_captureScrnRt.top()).arg(m_captureScrnRt.right()).arg(m_captureScrnRt.bottom()));
        pa.drawText(tPosText - QPoint(0, space * 12), QString("rtScrn 左上右下(%1, %2, %3 * %4) topLimit:%5  bottomLimit:%6")
            .arg(rtScrn.left()).arg(rtScrn.top()).arg(rtScrn.right()).arg(rtScrn.bottom()).arg(topLimit).arg(bottomLimit));
        pa.drawText(tPosText - QPoint(0, space * 13), QString("XDrawStep::serialText:%1").arg(XDrawStep::serialText));
    }

#endif

#if 0
    QRect rtOuter = m_rtCalcu.getExteRect(rtSel);
    QRect rtInner = m_rtCalcu.getInteRect(rtSel);
    int interval = (rtOuter.height() - rtInner.height()) / 2;

    QRect rtLeft(rtOuter.left(), rtInner.top(), interval, rtInner.height());
    QRect rtTop(rtInner.left(), rtOuter.top(), rtInner.width(), interval);
    QRect rtRight(rtInner.right(), rtInner.top(), interval, rtInner.height());
    QRect rtBottom(rtInner.left(), rtInner.bottom(), rtInner.width(), interval);
    QRect rtTopLeft(rtOuter.left(), rtOuter.top(), interval, interval);
    QRect rtTopRight(rtInner.right(), rtOuter.top(), interval, interval);
    QRect rtBottomLeft(rtOuter.left(), rtInner.bottom(), interval, interval);
    QRect rtBottomRight(rtInner.right(), rtInner.bottom(), interval, interval);

    //pa.setBrush(Qt::blue);

    pa.setBrush(Qt::NoBrush);
    pa.setPen(Qt::blue);
    pa.drawRect(rtLeft);
    pa.drawRect(rtTop);
    pa.drawRect(rtRight);
    pa.drawRect(rtBottom);
    //pa.setBrush(Qt::yellow);
    pa.setPen(Qt::white);
    pa.drawRect(rtTopLeft);
    pa.drawRect(rtTopRight);
    pa.drawRect(rtBottomLeft);
    pa.drawRect(rtBottomRight);

    /*qDebug() << "【paintEvent】  :" << m_rtCalcu.scrnType << m_rtCalcu.getSelRect() << rtSel << m_rtCalcu.getSelRect() << "   " << m_rtCalcu.selEndPos << "  " << m_basePixmap << "  " << QRect();*/
    //<< "外部矩形：" << rtOuter << "内部矩形：" << rtInner;
#endif // 1
}

QScreen* ScreenShot::priScrn() const
{
    QScreen* priScrn  = qGuiApp->primaryScreen();
    return priScrn;
}

void ScreenShot::adjustSelectedRect(QKeyEvent *e)
{
    const int pos = 1;
    int totalPos = pos;
    const int rate = 10;
    const bool shift = e->modifiers() == Qt::ShiftModifier;   // speed up 
    const bool ctrl = e->modifiers() == Qt::ControlModifier;  // expansion
    const bool alt = e->modifiers() == Qt::AltModifier;       // shrink
    const bool shiftAndCtrl(e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier));
    const bool shiftAndAlt(e->modifiers() == (Qt::ShiftModifier | Qt::AltModifier));
    const bool left(e->key() == Qt::Key_A | e->key() == Qt::Key_Left);
    const bool top(e->key() == Qt::Key_W | e->key() == Qt::Key_Up);
    const bool right(e->key() == Qt::Key_D | e->key() == Qt::Key_Right);
    const bool down(e->key() == Qt::Key_S | e->key() == Qt::Key_Down);

    QRect rt = m_rtCalcu.getSelRect();
    if (shift | shiftAndCtrl | shiftAndAlt) totalPos = pos * rate;
    if (rt.isValid() && m_rtCalcu.scrnType == ScrnOperate::SO_Wait) {
        if (ctrl | alt | shiftAndCtrl | shiftAndAlt)
            m_rtCalcu.scrnType = ScrnOperate::SO_Stretch;
        else if (left | top | right | down)
            m_rtCalcu.scrnType = ScrnOperate::SO_Move;
    }

    if (m_rtCalcu.scrnType == ScrnOperate::SO_Move) {
        if (left) rt.moveLeft(rt.left() - totalPos);
        if (top) rt.moveTop(rt.top() - totalPos);
        if (right) rt.moveRight(rt.right() + totalPos);
        if (down) rt.moveBottom(rt.bottom() + totalPos);
        
    } else if (m_rtCalcu.scrnType == ScrnOperate::SO_Stretch) {
        if (ctrl | shiftAndCtrl) {
            if (left) rt.setLeft(rt.left() - totalPos);
            if (top) rt.setTop(rt.top() - totalPos);
            if (right) rt.setRight(rt.right() + totalPos);
            if (down) rt.setBottom(rt.bottom() + totalPos);
        }

        if (alt | shiftAndAlt) {
            if (left) rt.setLeft(rt.left() + totalPos);
            if (top) rt.setTop(rt.top() + totalPos);
            if (right) rt.setRight(rt.right() - totalPos);
            if (down) rt.setBottom(rt.bottom() - totalPos);
        }
    }

    m_rtCalcu.setRtSel(rt);
    m_rtCalcu.scrnType = ScrnOperate::SO_Wait;
    update();
}

void ScreenShot::drawWinInfo(QPainter& pa) const
{
    int i = 0;
    pa.save();
    pa.setPen(QPen(Qt::black, 12));
    pa.setBrush(Qt::NoBrush);
    for (const WinData& it : IWinInfo::m_vWinData) {
        if (!it.bFilter) {
            const QRect& rt = it.rect;
            pa.drawRect(rt);
            pa.drawText(QPoint(rt.topLeft()) + QPoint(0, 30), it.path);
            pa.drawText(QPoint(rt.topLeft()) + QPoint(0, 10), it.title + QString("[%1]").arg(i++));
        }
    }

    pa.restore();
}

void ScreenShot::drawMaskLayer(const QRect& virGeom, const QRect& rtSel, QPainter& pa) const
{
    QPainterPath path;
    path.addRect(virGeom);
    path.addRect(rtSel);
    path.setFillRule(Qt::OddEvenFill);
    pa.save();
    pa.setPen(Qt::NoPen);
    pa.setBrush(QColor(0, 0, 0, 0.5 * 255));
    pa.drawPath(path);
    pa.restore();
}

void ScreenShot::drawBorder(QRect& rtSel, QPainter& pa) const
{
    if (rtSel.width() < 0 || rtSel.height() < 0)
        return;
    pa.save();
    const QPen pen(QPen(QColor(DATAMAID->paraValue("borderColor").toString()), DATAMAID->paraValue("borderWidth").toInt()));
    pa.setPen(pen);
    pa.setBrush(Qt::NoBrush);
    m_selSizeTip->move(drawSelSizeTip(rtSel));

    QRect rt(rtSel);
    for (auto it = m_scrnRts.cbegin(); it != m_scrnRts.cend(); ++it) {
        if (*it == rtSel) {
            const int offset = pen.width();
            rt.adjust(offset, offset, -offset, -offset);
            break;
        }
    }

    const QString style = DATAMAID->paraValue(tiBorderStyle).toString();
    if (style == "Flipped") {
        drawBorderFlipped(pa, rt);
    } else if (style == "BalckAndWhite") {
        drawBorderBlackWhite (pa, rt);
    } else if (style == "LightBlue") {
        pa.drawRect(rt);
        drawBorderLightBlue(pa, rt);
    }

    pa.restore();
}

void ScreenShot::drawCrosshair(QPainter& pa) const
{
    if (DATAMAID->paraValue("crosshair").toBool() && !m_bFirstPress) {
        pa.save();
        pa.setPen(QPen(QColor(DATAMAID->paraValue("crosshairColor").toString()), DATAMAID->paraValue("crosshairWidth").toInt()));
        pa.setBrush(Qt::NoBrush);

        QPoint p(QCursor::pos());
        QLine l1(QPoint(m_captureScrnRt.left(), p.y()), QPoint(m_captureScrnRt.right(), p.y()));
        QLine l2(QPoint(p.x(), m_captureScrnRt.top()), QPoint(p.x(), m_captureScrnRt.bottom()));
        pa.drawLine(l1);
        pa.drawLine(l2);
        pa.restore();
    }
}

void ScreenShot::drawToolBar() const
{
    if (isVisible() && m_selBar && m_bFirstSel) {
        const auto v = drawBarPosition(m_barOrien, ToolBarOffset::TBO_Middle);

        if (v.size() == 2) {
            m_selBar->move(v.at(0));
            m_paraBar->move(v.at(1));
        }

        // Cross-platform blur transparency effect
        const int blurRadius = 10;
        const double dpr = GetDevicePixelRatio();
        auto t1 = m_curPix->copy(QRect(v[0] * dpr, m_selBar->rect().size() * dpr));
        m_selBar->setBlurBackground(t1, blurRadius);
        auto t2 = m_curPix->copy(QRect(v[1] * dpr, m_paraBar->rect().size() * dpr));
        m_paraBar->setBlurBackground(t2, blurRadius);
    }
}

void ScreenShot::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
		qDebug() << "Key_Escape";
		emit sigClearScreen();
        // hide() 和 close() 区别: https://stackoverflow.com/questions/39407564
        //hide();
        close();   // // 销毁再不会有问题,由单例改写为 new 形式了。排查：1. tray 有关，改用 qpushbutton 和 close即可； 2.单例有关，该市建议修改为 new 指针的比较合适
        //deleteLater();
    }

    adjustSelectedRect(e);
}

// 注意: 1. 按下、松开时候会切换状态；点击绘画按钮也会切换状态
//      2. 开启鼠标跟踪时机；点击绘画按钮也会相应开启/关闭
//      3. mousePressEvent、mouseMoveEvent、mouseReleaseEvent 合成整体来看；以及不忘记绘画按钮的槽函数
void ScreenShot::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		return;

    m_bFirstPress = true;
	setMouseTracking(false);
	if (m_rtCalcu.getSelRect().isEmpty() && m_rtCalcu.scrnType == ScrnOperate::SO_Wait) {
		//m_rtCalcu.clear();

		m_rtCalcu.scrnType = ScrnOperate::SO_Select;
        m_rtCalcu.pos1 = e->pos();
        m_rtCalcu.pos2 = m_rtCalcu.pos1;
	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Draw) {

        if (m_step.shape == DrawShape::Text) {
            m_step.p2 = m_step.p1;   // p2 as perviousPos
            m_step.p1 = e->pos();
        } else {
            m_step.p2 = m_step.p1 = e->pos();
        }
        
        if (m_step.shape == DrawShape::Text) {
            if (m_selBar->isVisible() && QRect(m_selBar->mapToGlobal(QPoint(0, 0)), m_selBar->size()).contains(m_step.p1))
                return;

            m_edit->move(m_step.p1);
            const QRect rtEdit(mapFromGlobal(m_edit->mapToGlobal(QPoint(0, 0))), m_edit->size());
            if (!rtEdit.contains(m_step.p1, true)) {  // 编辑完成 || 初次编辑

                if (!m_edit->isVisible() && m_step.text.isEmpty()) {
                    m_edit->setVisible(true);
                    m_edit->setFocus();
                    
                } else {       // 显示中
                    m_step.text = m_edit->toPlainText();
                    m_step.rt.setTopLeft(m_step.p2);

                    if (!m_step.text.isEmpty())
                        m_vDrawed.push_back(m_step);  // 暂时特例：绘画文字为单独处理
                    m_edit->clear();
                    m_step.text.clear();
                    m_edit->setVisible(false);
                }
            }

            m_step.p2 = m_step.p1;
        } else if (m_step.shape == DrawShape::SerialNumberShape || m_step.shape == DrawShape::SerialNumberType) {
            if (m_step.serialText.isEmpty())
                return;
            // SerialNumberShape: "serialType(0数字/1字母)_bool重置标志(0继续/1重置)_数字序号_字母序号"； 使用 _ 分割
            auto list = m_step.serialText.split('_');

            if (list[0].toInt() == 0) {
                list[2] = QString::number(list[2].toInt() + 1);
            } else if (list[0].toInt() == 1) {
                list[3] = QString::number(list[3].toInt() + 1);
            }

            m_step.serialText = list.join("_");
            m_step.text = m_step.serialText;
        }

	} else {  // 则可能为移动、拉伸、等待状态
		m_rtCalcu.scrnType = updateScrnType(e->pos());
    }

	if (m_rtCalcu.scrnType == ScrnOperate::SO_Move) {
		m_rtCalcu.pos1 = e->pos();
        m_rtCalcu.pos2 = m_rtCalcu.pos1;

        whichShape(); // 判定移动选中的已绘图形
	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Stretch) {
		m_rtCalcu.pos1 = e->pos();
        m_rtCalcu.pos2 = m_rtCalcu.pos1;
	}

    update();
}

QPen ScreenShot::easyRecognizeColorPen(const QColor& color, const bool bDefaultPenWidth) const
{
    //调色盘的都并非是真的纯黑色或者纯白色，调试半小时。氪
    QPen tPen(m_step.pen);
    const QColor white("#FBFBFB");
    const QColor black("#323232");
    const bool isWhite = (color == Qt::white || color == white);
    const bool isBlack = (color == Qt::black || color == black);
    tPen.setColor(isWhite ? black : white);

    if (bDefaultPenWidth) tPen.setWidthF(1.2);
    return tPen;
}

void ScreenShot::mouseMoveEvent(QMouseEvent *e)
{
//    if (event->button() != Qt::LeftButton)
//        return;

	// 此时为 Qt::NoButton
	if (m_rtCalcu.scrnType == ScrnOperate::SO_Wait) {
        if (m_bSmartWin)
            updateAutoDetectRt();
	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Select) {
        m_rtCalcu.pos2 = e->pos();
        m_bSmartWin = false;
        //if (m_rtCalcu.pos1 != m_rtCalcu.pos2)
        //  不显示 TODO: 2022.02.10 再添加一个变量即可
	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Move) {
		m_rtCalcu.pos2 = e->pos();
	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Draw) {
        m_step.p2 = e->pos();

        if (m_step.shape == DrawShape::CustomPath) {
            m_step.custPath.append(e->pos());
        } else if (m_step.shape == DrawShape::Text) {
            m_step.p1 = m_step.p2;
            m_edit->move(m_step.p1);
        }

        m_step.rt = RectCalcu::getRect(m_step.p1, m_step.p2);
	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Stretch) {
		m_rtCalcu.pos2 = e->pos();
	}

    updateCursorShape(e->pos());
    update();
}

void ScreenShot::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
        return;

    if (m_rtCalcu.scrnType == ScrnOperate::SO_Wait) {
	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Select) {
		m_rtCalcu.pos2 = e->pos();

        if (m_rtCalcu.pos1 == m_rtCalcu.pos2) {  // 点击到一个点，视作智能检测窗口； 否则就是手动选择走下面逻辑
            m_rtCalcu.setRtSel(m_autoDetectRt);
        }

        m_bSmartWin = false; // 自动选择也结束

	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Move) {
		m_rtCalcu.pos2 = e->pos();

        // 移动选中的图形
        if (m_pCurShape) {
            m_pCurShape->rt.translate(m_rtCalcu.pos2 - m_rtCalcu.pos1);
        }

	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Draw) {
        
        m_step.p2 = e->pos();
        if (m_step.shape == DrawShape::CustomPath)
            m_step.custPath.append(e->pos());

        m_step.rt = RectCalcu::getRect(m_step.p1, m_step.p2);

        // DrawShape::Text  在按下时候单独处理 m_vDrawed.push_back
        if (m_step.shape != DrawShape::Text
                && m_step.shape != DrawShape::NoDraw) {
            if (isDrawShape(m_step)) {
                m_vDrawed.push_back(m_step); // TODO 2022.01.16 优化:  不必每次(无效得)点击，也都记录一次
                bool bSerial = m_step.shape == DrawShape::SerialNumberShape || m_step.shape == DrawShape::SerialNumberType;
                if (!bSerial)
                    m_step.partClear();

                //{
                //    int i = 0;
                //    for (auto& it : m_vDrawed) {
                //        qDebug() << "----------->i:" << i++;
                //        it.showDebug();
                //    }

                //    qDebug() << "----------->m_step:";
                //    m_step.showDebug();
                //}
            }
        }

	} else if (m_rtCalcu.scrnType == ScrnOperate::SO_Stretch) {
		m_rtCalcu.pos2 = e->pos();
	}

    if (!m_rtCalcu.calcurRsultOnce().isEmpty()) {  // 计算一次结果
        m_bFirstSel = true;
        if (!m_selBar->isVisible())
            m_selBar->setVisible(true);

        if (!m_selSizeTip->isVisible())
            m_selSizeTip->setVisible(true);
    }

	if (m_rtCalcu.scrnType != ScrnOperate::SO_Draw) {
		m_rtCalcu.scrnType = ScrnOperate::SO_Wait;
		setMouseTracking(true);
	}

    update();
}

void ScreenShot::wheelEvent(QWheelEvent* e)
{
    // Note: On X11 this value is driver specific and unreliable, use angleDelta() instead
    // QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = e->angleDelta() / 8;
    QPoint numSteps = numDegrees / 15;
    if (numDegrees.isNull())
        return;

    int penWidth = 0;
    if (m_step.shape == DrawShape::Text) {
        m_step.font.setPointSize(m_step.font.pointSize() + numSteps.y());
        m_edit->setFont(m_step.font);
        penWidth = m_step.font.pointSize();
    } else {
        const int min = 1;
        const int max = 100;
        penWidth = m_step.pen.widthF();

        const bool bSerialNumber = m_step.shape == DrawShape::SerialNumberShape || m_step.shape == DrawShape::SerialNumberType;
        if (bSerialNumber) {
            penWidth -= penWidth % 5;
            penWidth += (numSteps.y() > 0 ? 5 : -5);
        } else {
            penWidth += numSteps.y();
        }

        penWidth = qBound<int>(min, penWidth, max);
        m_step.pen.setWidthF(penWidth);
    }

//    qDebug() << "ScreenShot::wheelEvent:" << m_step.font.pointSizeF() << "   tLineWidth:" << penWidth << "  m_step.pen.widthF():" << m_step.pen.widthF() << "  " << m_edit->font().pointSizeF();
    emit sigLineWidthChange(penWidth);
    e->accept();
}

void ScreenShot::scrnsCapture()
{
    m_scrnRts.clear();
    m_scrnRts.insert(m_captureScrnRt);
    for (const auto& it : m_scrns)
        m_scrnRts.insert(it.first->geometry());

    scrnsInfo();
    virtualScrnPixmap();

#ifdef Q_OS_WIN
    show();
#else
    showFullScreen();         // Linux supports virtual multi-screen, Mac only one screen
#endif
}

// 屏幕详细参数
void ScreenShot::scrnsInfo() const
{
    qInfo() << "---------------QApplication::desktop() Info BEGIN----------------";
    qInfo() << "所有可用区域 m_virtualGeometry:" << m_captureScrnRt << Qt::endl
        << "主屏可用区域 priScrn()->geometry():" << priScrn()->geometry() << Qt::endl
        << "是否开启虚拟桌面 isVirtualDesktop:" << (priScrn()->virtualSiblings().size() > 1 ? true : false) << Qt::endl;
    qInfo() << "---------------QApplication::desktop() Info END----------------";

    qInfo() << "---------------m_screens[] Info BEGIN----------------";
    for (const auto& scrn : m_scrns) {
        const auto it = scrn.first;
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
                 << "\n物理DPI physicalDotsPerInch:" << int(it->physicalDotsPerInch()) << " DPIX:" << int(it->physicalDotsPerInchX()) << " DPIY:" << int(it->physicalDotsPerInchY())
                 << "\n手算缩放比 getScale:" << GetScale(it)
                 << "\n虚拟尺寸 m_captureScrnRt:" << m_captureScrnRt << "\n";
    }

    qInfo() << "m_virtualGeometry:" << m_captureScrnRt;
    qInfo() << "---------------m_screens[] Info END----------------";
}

double ScreenShot::GetDevicePixelRatio(const QScreen* scrn)
{
    double DPI = 1;
#ifdef Q_OS_MAC
    DPI = 2;
#endif

    return scrn ? scrn->devicePixelRatio() : DPI;
}

// 随着光标移动，更新获取桌面所有窗口信息
void ScreenShot::updateAutoDetectRt()
{
    WinID winId;

#ifdef Q_OS_WIN
    winId._hWnd = (void *)QWidget::winId();

#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
    winId._xWindow = (unsigned long)0;
#endif

    Util::getRectFromCurrentPoint(winId, m_autoDetectRt);
    m_autoDetectRt = QRect(mapFromGlobal(m_autoDetectRt.topLeft()), m_autoDetectRt.size());
    //m_autoDetectRt = m_rtCalcu.limitBound(m_autoDetectRt, m_captureScrnRt);
}

double ScreenShot::GetScale(const QScreen* scrn)
{
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)             // or defined(Q_WS_WIN) || defined(Q_WS_X11)
	double scale = scrn->logicalDotsPerInch() / 96.0;
	if (scale < 1.25)
		return 1;
	else if (1.25 <= scale && scale < 1.5)
		return 1.25;
	else if (1.5 <= scale && scale < 1.75)
		return 1.5;
	else if (1.75 <= scale && scale < 2)
		return 1.75;
	else if (2 <= scale && scale < 2.25)
		return 2;
	else if (2.25 <= scale && scale < 2.5)
		return 2.25;
    else if (2.5 <= scale && scale < 3)
		return 2.5;
	else if (3 <= scale && scale < 3.5)
		return 3;
	else if (3.5 <= scale && scale < 4)
		return 3.5;
	else
		return scale;
#elif  defined(Q_OS_MAC)
    double scale = scrn->logicalDotsPerInch() / 72.0;
    return scale;
#else
    return scrn->logicalDotsPerInch() / 96.0;
#endif
}

bool ScreenShot::isSelBorder()
{
    return !m_pCurShape;
}

const Qt::Orientation ScreenShot::getBarOrien() const
{
    return m_barOrien;
}

void ScreenShot::setBarOrien(Qt::Orientation val)
{
    m_barOrien = val;
}

void ScreenShot::windowCapture()
{
    scrnsCapture();
    m_bSmartWin = true;
}

void ScreenShot::delayCapture(int ms)
{
    QTimer::singleShot(ms, this, [this](){scrnsCapture();});
}

void ScreenShot::fullScrnCapture()
{
    scrnsCapture();
    m_bSmartWin = false;
    m_rtCalcu.setRtSel(curScrn()->geometry());
}

QScreen *ScreenShot::curScrn(const QPoint& pos) const
{
    QScreen* scrn = qGuiApp->screenAt(pos);

#if defined(Q_OS_MACOS)
    // In macos, mouse position at the bottom or right edge of the screen will crash
    if (!scrn && (pos.x() >= m_captureScrnRt.right() || pos.y() >= m_captureScrnRt.bottom()))
        scrn = qGuiApp->screenAt(m_captureScrnRt.bottomRight() - QPoint(1, 1));
#endif

    //if (!scrn)
    //    curScrn = qGuiApp->primaryScreen();

    if (!scrn)
        qDebug() << "Gets that the current screen is empty";

    return scrn;
}

