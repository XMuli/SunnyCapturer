// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2023-2024 XMuli
// SPDX-GitHub: https://github.com/XMuli/Sunny
// SPDX-Author: XMuli <xmulitech@gmail.com>

#include "aboutinfo.h"
#include "ui_aboutinfo.h"
#include <QFont>
#include <QLabel>
#include <QDebug>
#include <QSpacerItem>
#include <QStringList>
#include <QFont>
#include <QLayoutItem>
#include <QApplication>
#include <QClipboard>
#include <QFrame>
#include "systeminfo.h"
#include "verspacerline.h"
// #include "../config/datamaid.h"

void AboutInfo::insertLayout(const QStringList& lists)
{
    const int& rowCount = 7;                        // 固定为 7 行
    const int& monitorCount = lists.size() / (2 * rowCount);   // 显示器个数
    auto& gridLayout = ui->gridLayout;

    for (int j = 0; j < monitorCount; ++j) {
        for (int i = 0; i < rowCount; i++) {
            QLabel* lab1 = new QLabel(lists.at(j*rowCount*2 + i * 2));
            QLabel* lab2 = new QLabel(lists.at(j*rowCount*2 + i * 2 + 1));
            // lab1->setFont(m_labFont);
            // lab2->setFont(m_labFont);
            lab2->setStyleSheet("QLabel { color : gray; }");

            QFont font(this->font());
            // font->setp
            // font.setPointSize(font.pointSize() + 1);
            lab2->setFont(font);

            gridLayout->addWidget(lab1, i, j*3);
            gridLayout->addWidget(lab2, i, j*3 + 1);
        }

        // QFrame* frame = new QFrame(nullptr);
        // frame->setFrameShape(QFrame::VLine);
        // frame->setFrameShadow(QFrame::Sunken);
        // frame->setLineWidth(1);
        // frame->setMidLineWidth(2);
        // frame->setFixedHeight(200);

        if (j < monitorCount - 1)
            gridLayout->addWidget(new VerSpacerLine(160, this), 0, j*3+2, rowCount, 1, Qt::AlignCenter);

    }

    // gridLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Preferred, QSizePolicy::Fixed), 0, gridLayout->columnCount());


    // 插入固定的垂直间距
    // QSpacerItem * spacer = new QSpacerItem(1, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
    // gridLayout->addItem(spacer, gridLayout->rowCount(), 0);
}

AboutInfo::AboutInfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AboutInfo)
    , m_labFont("Courier New")
{
    ui->setupUi(this);

    init();
}

AboutInfo::~AboutInfo()
{
    delete ui;
}

void AboutInfo::init()
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    auto& lab = ui->labMonitor;
    // lab->setMaximumHeight(900);
    QPixmap pix = SYSINFO.renderMonitorToPixmap();
    lab->setFixedSize(pix.size());
    lab->setPixmap(pix);
    // lab->setScaledContents(true);

    QString szEdition;
    QString szVersion;
#if defined(Q_OS_WIN)
    QString edition = SYSINFO.getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");     // Edition: "Windows 10 Pro"
    QString version = SYSINFO.getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "DisplayVersion");  // Version: "22H2"
    QString currentBuild = SYSINFO.getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild");

    szEdition = edition;
    szVersion = version + " " + currentBuild;
#else
    szEdition = QSysInfo::prettyProductName()
    szVersion = QSysInfo::kernelVersion();
#endif

    QString grayCss = "QLabel { color : gray; }";
    ui->labProject->setStyleSheet(grayCss);
    ui->labBuildTime->setStyleSheet(grayCss);
    ui->labBuildKits->setStyleSheet(grayCss);
    ui->labQtVersion->setStyleSheet(grayCss);
    ui->labEdition->setStyleSheet(grayCss);
    ui->labVersion->setStyleSheet(grayCss);
    ui->labMemory->setStyleSheet(grayCss);
    ui->labCPU->setStyleSheet(grayCss);

    ui->labProject->setText(QString("%1 %2").arg(XPROJECT_NAME).arg(XPROJECT_VERSION));
    ui->labBuildTime->setText(XBUILD_TIME);
    ui->labBuildKits->setText(QString("%1 %2").arg(XCOMPILER_ID).arg(XCOMPILER));
    ui->labQtVersion->setText(QT_VERSION_STR);
    ui->labEdition->setText(szEdition);
    ui->labVersion->setText(szVersion);
    ui->labMemory->setText(SYSINFO.getMemoryInfo());
    ui->labCPU->setText(SYSINFO.getCPUInfo());

    insertLayout(SYSINFO.scrnsInfo());

    // connect(&DATAMAID, &DataMaid::sigLanguageChange, this, &AboutInfo::onLanguageChange);
    adjustSize();
    // onLanguageChange("");
}

const QString AboutInfo::getSystemInfo()
{
    QWidget* widgetSystemInfo = ui->widgetSystemInfo;
    QString result;
    QGridLayout* layout = qobject_cast<QGridLayout*>(widgetSystemInfo->layout());

    if (!layout) {
        return result;  // 如果不是 QGridLayout，则返回空字符串
    }

    int rows = layout->rowCount();
    int cols = layout->columnCount();

    for (int row = 0; row < rows; ++row) {
        QStringList rowText;
        for (int col = 0; col < cols; ++col) {
            QLayoutItem* item = layout->itemAtPosition(row, col);
            if (item) {
                QLabel* label = qobject_cast<QLabel*>(item->widget());
                if (label) {
                    rowText << label->text();
                }
            }
        }
        if (!rowText.isEmpty()) {
            result += rowText.join(" ") + "\n";
        }
    }

    return result;
}

const QString AboutInfo::detailedInfo() const
{
    const auto& gridLayout = ui->gridLayout;
    QString output;

    int rowCount = gridLayout->rowCount();
    int columnCount = gridLayout->columnCount();

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnCount; ++col) {
            const bool bFirst = col % 3 == 0 ? true : false;
            QLayoutItem * item = gridLayout->itemAtPosition(row, col);
            if (item) {
                QLabel* label = qobject_cast<QLabel*>(item->widget());
                if (label) {
                    const int& n = 22;
                    const QChar fill = QLatin1Char(' ');
                    if (bFirst ) output += QString("%1 ").arg(label->text().leftJustified(n-5, fill));
                    else output += QString("%1 ").arg(label->text().leftJustified(n, fill));
                }
            }
        }
        // 添加换行符
        output += "\n";
    }

    qDebug().noquote() << output.trimmed();
    return output.trimmed();
}

void AboutInfo::on_pushButton_released()
{
    const QString& text = getSystemInfo() +  "\n\n" + detailedInfo();
    qApp->clipboard()->setText(text);
    close();
}

void AboutInfo::onLanguageChange(const QString &language)
{
    ui->retranslateUi(this);
    setWindowTitle(XPROJECT_NAME + tr(" Detailed"));
}

void AboutInfo::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        event->accept();
        close();
    }

    AboutInfo::keyPressEvent(event);
}
