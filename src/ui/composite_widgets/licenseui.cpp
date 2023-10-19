#include "licenseui.h"
#include "ui_licenseui.h"
#include "communication.h"

LicenseUI::LicenseUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LicenseUI)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    readLicenseBlocks();
    displayLicenses();

    QString name = QString("<a href='%1' style='color: #008000;'>%2</a>").arg("https://sunny.xmuli.tech").arg(XPROJECT_NAME);
    QString author = QString("<a href='%1' style='color: #008000;'>%2</a>").arg("https://github.com/XMuli").arg("XMuli");
    QString copyright = QString(tr("Copyright (C) 2023 %1. All rights reserved.<br>The birth of this project is inseparable from these open source software")).arg(author);
    QString introduce = QString("<html><body><b>%1</b><br>%2<br></body></html>").arg(name).arg(copyright);
    ui->labIntroduce->setText(introduce);
    connect(&COMM, &Communication::sigLanguageChange, this, [this]() { ui->retranslateUi(this);});
    resize(900, 1200);
}

LicenseUI::~LicenseUI()
{
    delete ui;
}

void LicenseUI::readLicenseBlocks()
{
    QString readmePath = ":/resources/licenses/tripartite/README.txt";
    QFile readmeFile(readmePath);
    if (readmeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&readmeFile);
        QString block;

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();

            if (!line.isEmpty()) {
                block += line + "\n";
            } else {
                m_readmeBlocks.append(block.trimmed());
                block.clear();
            }
        }

        m_readmeBlocks.append(block.trimmed());

        readmeFile.close();
    }
}

void LicenseUI::displayLicenses()
{
    const QString& licensePath = ":/resources/licenses/tripartite";
    int idx = 1;

    qDebug() << "--->m_readmeBlocks:" << m_readmeBlocks.size();
    for (const QString &block : m_readmeBlocks) {
        QStringList lines = block.split('\n');

        if (lines.size() >= 4) {
            QString libraryName = lines[0];
            QString repositoryURL = lines[1];
            QString licenseName = lines[2];
            QString projectIntroduction = lines[3];

            // Find files with the repository name in the same directory
            QStringList matchingFiles = QDir(licensePath).entryList(QStringList() << "*" + libraryName + "*", QDir::Files);
            for (const QString &fileName : matchingFiles) {
                QFileInfo fileInfo(licensePath + QDir::separator() + fileName);

                QFile file(fileInfo.filePath());
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);
                    QString fileContent = in.readAll();

                    LicenseBlock licenseBlock;
                    licenseBlock.libraryName = libraryName;
                    licenseBlock.repositoryURL = repositoryURL;
                    licenseBlock.licenseName = licenseName;
                    licenseBlock.projectIntroduction = projectIntroduction;
                    licenseBlock.fileContent = fileContent;

                    m_licenseBlocks.append(licenseBlock);

                    file.close();
                }
            }
        }
    }

    // Display the license blocks
    auto& textBrowser = ui->textBrowser;
    for (auto it = m_licenseBlocks.begin(); it != m_licenseBlocks.end(); ++it) {
         const LicenseBlock &licenseBlock = *it;
        // Library Name
//        textBrowser->append(QString("<font size='+1'><b>%1</b></font>").arg(licenseBlock.libraryName));

        // Repository URL
        // 从右往左搜索第二个斜杠的索引
        const QString url(licenseBlock.repositoryURL);
        int lastSlashIndex = url.lastIndexOf('/');
        int secondLastSlashIndex = url.lastIndexOf('/', lastSlashIndex - 1);
        QString repositoryLink = QString("<a href='%1' style='color: #008000;'><font size='+1'><b>%2</b></font></a>")
                                     .arg(url)
                                     .arg("[" + QString::number(idx++) + "] " + url.mid(secondLastSlashIndex + 1));
        textBrowser->append(repositoryLink);

        // Project Introduction
        textBrowser->append(QString("%1").arg(licenseBlock.projectIntroduction));
        textBrowser->append("");

        // License Name
//        textBrowser->append(QString("%1").arg(licenseBlock.licenseName));

        // File Content
        textBrowser->append(licenseBlock.fileContent);
        textBrowser->append("");

        if (std::next(it) != m_licenseBlocks.end()) {
            // Separator
            textBrowser->append("--------------------------------------------------------------------------------");
            textBrowser->append("");
        }
    }

    textBrowser->moveCursor(QTextCursor::Start); // 滚动条置顶
}

void LicenseUI::on_pushButton_released()
{
    qApp->aboutQt();
}

