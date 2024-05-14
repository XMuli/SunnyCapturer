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

#ifdef Q_OS_WIN
    m_labFont = QFont("Courier New");
#elif defined(Q_OS_MAC)
    m_labFont = QFont("Menlo");
#else
    m_labFont = QFont("DejaVu Sans Mono");
#endif

    auto& lab = ui->labMonitor;
    // lab->setMaximumHeight(900);
    QPixmap pix = SYSINFO.renderMonitorToPixmap();
    lab->setFixedSize(pix.size());
    lab->setPixmap(pix);
    // lab->setScaledContents(true);

    auto& gridLayout = ui->gridLayout;
//    QLayoutItem* item;
//    while ((item = gridLayout->takeAt(0)) != nullptr) {
//        QWidget* widget = item->widget();
//        if (widget) {
//            delete widget;
//        };
//        delete item;
//    }

    // gridLayout->setColumnStretch(0, 1);
    // gridLayout->setColumnStretch(1, 2);

    ui->labSysteminfo->setFont(m_labFont);
    ui->labSysteminfo->setText(SYSINFO.windowsVersionInfo());

    // const auto& virGeomInfo = SYSINFO.virGeometryInfo();
    // if (!virGeomInfo.isEmpty()) ui->labSysteminfo->setText(virGeomInfo.at(0));

    insertLayout(SYSINFO.scrnsInfo());

    // connect(&DATAMAID, &DataMaid::sigLanguageChange, this, &AboutInfo::onLanguageChange);
    adjustSize();
    // onLanguageChange("");
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
    qApp->clipboard()->setText(detailedInfo());
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
