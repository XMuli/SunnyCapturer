#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QPointer>
#include <QWidget>
#include "qhotkey.h"
#include "../../data/configmanager.h"

#define COMM Communication::instance()

enum class HotKeyType{
    HKT_capture,           // 正常的立刻截图，不会有任何预设
    HKT_delay_capture,     // 延迟截图: 计时器 + HKT_capture 截图效果   （会受 custom_size_enable & custom_dealy 影响， [会无视] topleft_enable size_enable 的状态）
    HKT_custiom_capture    // 自定义截图: 计时器 + 预设的窗口等大小 截图  （会受 custom_size_enable & custom_dealy & topleft_enable & size_enable 的影响）
};


QString hotKeyTypeToString(const HotKeyType& hotKeyType);

class Communication : public QObject
{
    Q_OBJECT

public:
    static Communication& instance(); // signle
    void init();
    bool resetShortcut(const QKeySequence &keySequence, const HotKeyType& type);
    bool shortcutStatus(const HotKeyType& type) const ;
    void setAppFont(const QString& font);
    void loadTranslation(const QString& language);
    void loadCustomQss(const QString &path);

signals:
    void sigLanguageChange(const QString qmFile);
    void sigShowSystemMessagebox(const QString &title, const QString &msg, const int& msecs); // 系统消息通知

private:
    QString toLocaleName(const QString& language);

    explicit Communication(QObject *parent = nullptr);
    virtual ~Communication() = default;
    Communication(const Communication&) = delete;
    Communication& operator=(const Communication&) = delete;
    Communication(Communication&&) = delete;
    Communication& operator=(Communication&&) = delete;

private:
    QHotkey* m_hkCapture;
    QHotkey* m_hkDelayCapture;
    QHotkey* m_hkCustiomCapture;
};


std::map<QString, QString> languageMap();
std::map<const QString, const bool> themesMap();
QStringList getQSSFileNames(const QString& path = qApp->applicationDirPath() + "/resources/qss");


#endif // COMMUNICATION_H
