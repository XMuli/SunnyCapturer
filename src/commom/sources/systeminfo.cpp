#include "systeminfo.h"

#ifdef _WIN32
#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#elif __linux__
    #include <fstream>
    #include <sstream>
    #include <iostream>
    #include <unistd.h>
#endif

#include <QDebug>
#include <QSysInfo>
#include <QOperatingSystemVersion>
#include <QPen>
#include <QPainter>
#include <QStandardPaths>

#define FIX_FORMAT(str) \
    const QString& t(std); \
    t.leftJustified(20, ' ')

SystemInfo::SystemInfo(QObject *parent)
    : QObject(parent)
    , m_scrns(qGuiApp->screens())
    , m_priScrn(qGuiApp->primaryScreen())
{
}

const QStringList SystemInfo::detailedInfo() const
{
    QStringList lists("");
    if (!virGeometryInfo().isEmpty())
        lists += virGeometryInfo();

    lists += scrnsInfo();

    qDebug().noquote()<< scrnsInfo();
    qDebug().noquote()<< virGeometryInfo();
    return lists;
}


QString SystemInfo::windowsVersionInfo()
{
    QString ret;

    qInfo().noquote() << QString("bootUniqueId[%1]\n").arg(QSysInfo::bootUniqueId().data())
                      << QString("buildAbi[%1]\n").arg(QSysInfo::buildAbi())
                      << QString("buildCpuArchitecture[%1]\n").arg(QSysInfo::buildCpuArchitecture())
                      << QString("currentCpuArchitecture[%1]\n").arg(QSysInfo::currentCpuArchitecture())
                      << QString("kernelType[%1]\n").arg(QSysInfo::kernelType())
                      << QString("kernelVersion[%1]\n").arg(QSysInfo::kernelVersion())
                      << QString("machineHostName[%1]\n").arg(QSysInfo::machineHostName())
                      << QString("machineUniqueId[%1]\n").arg(QSysInfo::machineUniqueId().data())
                      << QString("prettyProductName[%1]\n").arg(QSysInfo::prettyProductName())
                      << QString("productType[%1]\n").arg(QSysInfo::productType())
                      << QString("productVersion[%1]\n").arg(QSysInfo::productVersion());

    QStringList lists;
    lists << XPROJECT_NAME << XPROJECT_VERSION
          << tr("Bulid Tits") << QString("%1 %2").arg(XCOMPILER_ID).arg(XARCH_BIT)
          << tr("Operating System") << QString("%1").arg(QSysInfo::prettyProductName())
          << tr("kernel") << QString("%1").arg(QSysInfo::kernelVersion());
    qDebug() << lists;

    const int fieldWidth = 20;
    const QChar fillChar = ' ';
    ret += QString(tr("Name: ")).leftJustified(fieldWidth, fillChar) + QString("%1 %2\n").arg(XPROJECT_NAME).arg(XPROJECT_VERSION);
    ret += QString(tr("Build Time: ")).leftJustified(fieldWidth, fillChar) + QString("%1\n").arg(XBUILD_TIME);
    ret += QString(tr("Build Kits: ")).leftJustified(fieldWidth, fillChar) + QString("%1 %2\n").arg(XCOMPILER_ID).arg(XCOMPILER);
    ret += QString(tr("Qt Version: ")).leftJustified(fieldWidth, fillChar) + QString("%1\n").arg(QT_VERSION_STR);

#if defined(Q_OS_WIN)
    // QString editionID = getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "EditionID");     // Edition ID: "Professional"
    QString edition = getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");     // Edition: "Windows 10 Pro"
    QString version = getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "DisplayVersion");  // Version: "22H2"
    QString currentBuild = getRegistryValue("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild");

    ret += QString(tr("Edition: ")).leftJustified(fieldWidth, fillChar)          + QString("%1\n").arg(edition);
    ret += QString(tr("Version: ")).leftJustified(fieldWidth, fillChar)          + QString("%1\n").arg(version + " " + currentBuild);
#else
    ret += QString(tr("Operating System: ")).leftJustified(fieldWidth, fillChar) + QString("%1\n").arg(QSysInfo::prettyProductName());
    ret += QString(tr("kernel: ")).leftJustified(fieldWidth, fillChar)           + QString("%1\n").arg(QSysInfo::kernelVersion());
#endif

    QString cpuInfo = getCPUInfo();          // CPU Info
    QString memoryInfo = getMemoryInfo();    // Memory Info
    ret += QString(tr("Memory: ")).leftJustified(fieldWidth, fillChar)           + QString("%1\n").arg(memoryInfo);
    ret += QString(tr("CPU: ")).leftJustified(fieldWidth, fillChar)              + QString("%1\n").arg(cpuInfo);

    qDebug().noquote() << "ret:" << ret;
    return ret;
}

QStringList SystemInfo::scrnsInfo() const
{
    int i = 1;
    QStringList lists;

    for (const auto& it : m_scrns) {
        QRect tGeometry(it->geometry());
        QString geometry = QString("(%1,%2 %3x%4)").arg(tGeometry.x()).arg(tGeometry.y()).arg(tGeometry.width()).arg(tGeometry.height());
        QSizeF tPhysicalSize(it->physicalSize());
        QString physicalSize = QString("%1x%2").arg(tPhysicalSize.width()).arg(tPhysicalSize.height());

        QString multiIdx = QString("%1 %2").arg(i++).arg(it == m_priScrn ? "[Primary]" : "");
        if (m_scrns.size() >= 1) lists << tr("Idx") << multiIdx;


        lists << tr("DevicePixelRatio") << QString::number(it->devicePixelRatio())
              << tr("Logical DPI") << QString("%1 (%2% scaling)").arg(it->logicalDotsPerInch()).arg(scaling(it) * 100)
              << tr("Physical DPI") << QString::number(it->physicalDotsPerInch())
              << tr("RefreshRate") << QString::number(it->refreshRate())
              << tr("Geometry") << geometry
              << tr("PhysicalSize") << physicalSize;
    }

    return lists;
}


QString SystemInfo::virGeometryInfo() const
{
    QString lists;
    if (m_scrns.size() > 1) {
        QRect tVirtualGeometry(m_priScrn->virtualGeometry());
        QString virtualGeometry = QString("(%1,%2 %3x%4)").arg(tVirtualGeometry.x()).arg(tVirtualGeometry.y()).arg(tVirtualGeometry.width()).arg(tVirtualGeometry.height());
        lists += tr("VirtualGeometry");
        lists +=  QString("%1").arg(virtualGeometry);
    }

    return lists;
}

// 1. 计算所有显示器矩形的边界盒：我们将遍历所有显示器，计算它们的矩形，然后得到一个包含所有这些矩形的最小矩形。
// 2. 调整此边界盒到maxRt的中心：基于maxRt的中心和边界盒的中心，计算一个偏移量，并将所有显示器矩形按此偏移量进行平移。
// 3. 绘制调整后的矩形：使用更新后的位置绘制每个显示器的矩形。
QPixmap SystemInfo::renderMonitorToPixmap()
{
    m_scrns = qGuiApp->screens();
    m_priScrn = qGuiApp->primaryScreen();

    qreal scaleFactor = 0.08;
    const int margin = 10;
    const int textMargin = 20;
    const QSize virSize = m_priScrn->virtualGeometry().size() * scaleFactor + QSize(textMargin, textMargin) * 2;
    const QRect maxRt(QPoint(margin, margin), virSize);
    const QSize& clientSize = virSize + QSize(margin, margin) * 2;

    QPixmap pixmap(clientSize);
    pixmap.fill(Qt::transparent);
    QPainter pa(&pixmap);

#ifdef QT_DEBUG
    pa.setPen(QPen(Qt::blue, 1));
    pa.setBrush(Qt::NoBrush);
    pa.drawRect(maxRt);
#endif

    QPoint center = maxRt.center();
    QRect allMonitorsRect; // 初始为空，将根据内容扩展
    QVector<QRect> monitorRects;
    QVector<QSize> originalSizes; // 添加原始尺寸的存储

    for (auto& screen : m_scrns) {
        QSize originalSize = screen->geometry().size();
        QRect rt = screen->geometry();
        QSize newSize(rt.size() * scaleFactor);
        QPoint newCenter = center + (rt.center() - m_priScrn->virtualGeometry().center()) * scaleFactor;
        QPoint newTopLeft = newCenter - QPoint(newSize.width() / 2, newSize.height() / 2);
        QRect newRect(newTopLeft, newSize);

        originalSizes.append(originalSize); // 存储原始尺寸
        if (allMonitorsRect.isNull())
            allMonitorsRect = newRect;
        else
            allMonitorsRect = allMonitorsRect.united(newRect);

        monitorRects.append(newRect);
    }

    // 计算所有显示器矩形的中心与maxRt中心的偏移，然后将所有矩形平移到maxRt中心
    QPoint offset = center - allMonitorsRect.center();

    for (int i = 0; i < m_scrns.size(); ++i) {
        QRect adjustedRect = monitorRects[i].translated(offset);

        pa.setPen(QPen(Qt::black, 1));
        pa.drawRect(adjustedRect);

        // 绘制高度和宽度的文字
        pa.setPen(QPen(Qt::red, 1));
        QString heightText = QString::number(originalSizes[i].height());
        QString widthText = QString::number(originalSizes[i].width());
        pa.save();
        pa.translate(adjustedRect.topLeft() + QPoint(-5, adjustedRect.height() / 2));
        pa.rotate(-90);
        pa.drawText(QPoint(-10, 0), heightText);
        pa.restore();
        pa.drawText(QPoint(adjustedRect.center().x() - 10, adjustedRect.top() - 5), widthText);

        // 绘制序号和主屏标记
        QString text = QString("序: %1").arg(i+1);
        if (m_scrns.at(i) == m_priScrn)
            text += " (Primary)";
        pa.drawText(adjustedRect.bottomLeft() + QPoint(5, -5), text);
    }

#ifdef QT_DEBUG
    pa.setPen(QPen(Qt::green, 2));
    pa.drawRect(0, 0, clientSize.width(), clientSize.height());
#endif
    // QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    // QString filePath = desktopPath + "/monitor_image.png";
    // pixmap.save(filePath);

    return pixmap;
}


double SystemInfo::scaling(const QScreen *screen) const
{
    double scale = 0;
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)          // or defined(Q_WS_WIN) || defined(Q_WS_X11)
    scale = screen->logicalDotsPerInch() / 96.0;
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
    scale = screen->logicalDotsPerInch() / 72.0;
    return scale;
#endif
}


#if defined(Q_OS_WIN)
QString SystemInfo::getRegistryValue(const QString &keyPath, const QString &valueName)
{
    QString value;

    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, reinterpret_cast<LPCWSTR>(keyPath.utf16()), 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        qDebug() << "Failed to open registry key";
        return value;
    }

    WCHAR buffer[255];
    DWORD bufferSize = sizeof(buffer);
    DWORD valueType = REG_SZ;
    result = RegQueryValueEx(hKey, reinterpret_cast<LPCWSTR>(valueName.utf16()), NULL, &valueType, reinterpret_cast<LPBYTE>(buffer), &bufferSize);
    if (result == ERROR_SUCCESS) {
        value = QString::fromWCharArray(buffer);
    } else {
        qDebug() << "Failed to get registry value";
    }

    RegCloseKey(hKey);
    return value;
    return QString();
}
#endif

QString SystemInfo::getCPUInfo()
{
#if defined(_MSC_VER)
    QString cpuInfo;

    // Initialize COM
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    IWbemLocator* pLoc = NULL;
    IWbemServices* pSvc = NULL;
    HRESULT hres;

    // Connect to the root\cimv2 namespace with the current user and obtain pointer pSvc to make IWbemServices calls.
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hres)) {
        qDebug() << "Failed to create IWbemLocator object. Err code =" << hres;
        CoUninitialize();
        return cpuInfo;
    }

    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
    if (FAILED(hres)) {
        qDebug() << "Could not connect. Error code =" << hres;
        pLoc->Release();
        CoUninitialize();
        return cpuInfo;
    }

    // Set security levels on the proxy
    hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hres)) {
        qDebug() << "Could not set proxy blanket. Error code =" << hres;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return cpuInfo;
    }

    // Use the IWbemServices pointer to make requests of WMI
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator
        );
    if (FAILED(hres)) {
        qDebug() << "Query for Win32_Processor failed. Error code =" << hres;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return cpuInfo;
    }

    // Get the data from the query
    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if (0 == uReturn)
            break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        if (FAILED(hr)) {
            qDebug() << "Failed to get CPU name property. Error code =" << hr;
            pclsObj->Release();
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return cpuInfo;
        }

        cpuInfo += QString::fromWCharArray(vtProp.bstrVal);
        VariantClear(&vtProp);

        hr = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
        if (FAILED(hr)) {
            qDebug() << "Failed to get CPU core count property. Error code =" << hr;
            pclsObj->Release();
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return cpuInfo;
        }

        cpuInfo += QString(" (") + QString::number(vtProp.uiVal) + QString(" Cores, ");

        hr = pclsObj->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0);
        if (FAILED(hr)) {
            qDebug() << "Failed to get CPU logical processor count property. Error code =" << hr;
            pclsObj->Release();
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return cpuInfo;
        }

        cpuInfo += QString::number(vtProp.uiVal) + QString(" Threads)");
        VariantClear(&vtProp);

        pclsObj->Release();
    }

    // Cleanup
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    return cpuInfo;
#elif defined(Q_OS_LINUX)
    QString cpuModel;
    int cores = 0;
    long numThreads = sysconf(_SC_NPROCESSORS_CONF); // 获取逻辑数量
    std::ifstream cpuinfo("/proc/cpuinfo");

    if (cpuinfo.is_open()) {
        std::string line;
        while (std::getline(cpuinfo, line)) {
            std::istringstream iss(line);
            std::string key, value;

            if (std::getline(iss, key, ':') && std::getline(iss, value)) {
                if (key.find("model name") != std::string::npos) {
                    // Trim leading and trailing whitespace
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);

                    cpuModel = QString::fromStdString(value);
                } else if (key.find("cpu cores") != std::string::npos) {
                    cores = std::stoi(value);
                }
            }
        }
        cpuinfo.close();
    }

    cpuModel += QString(" (") + QString::number(cores) + QString(" Cores, ");
    cpuModel += QString::number(numThreads) + QString(" Threads)");

    return cpuModel;
#elif defined(__clang__)
#endif
}

QString SystemInfo::getMemoryInfo()
{
#if defined(_MSC_VER)
    QString memoryInfo;
    // Get total physical memory using Windows API
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    double memoryInGB = memStatus.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);  // 先计算以GB为单位的内存大小，使用double保证小数部分被保留
    double roundedMemory = std::ceil(memoryInGB * 10.0) / 10.0;  // 使用 std::ceil 来向上取整到最近的0.1GB
    memoryInfo = QString::fromLatin1("%1 GB").arg(roundedMemory, 0, 'f', 1); // 格式化输出

    return memoryInfo;
#elif defined(Q_OS_LINUX)
    QString memoryInfo;
    std::ifstream meminfo("/proc/meminfo");

    if (meminfo.is_open()) {
        std::string line;
        while (std::getline(meminfo, line)) {
            std::istringstream iss(line);
            std::string key, value;

            if (std::getline(iss, key, ':') && std::getline(iss, value)) {
                if (key.find("MemTotal") != std::string::npos) {
                    std::istringstream valueStream(value);
                    unsigned long long memSizeKB;
                    valueStream >> memSizeKB;
                    double memSizeGB = static_cast<double>(memSizeKB) / (1024 * 1024);
                    memoryInfo = QString::number(memSizeGB, 'f', 1) + " GB";
                    break;
                }
            }
        }
        meminfo.close();
    }

    return memoryInfo;
#elif defined(__clang__)
#endif



}

#if defined(_MSC_VER)
#elif defined(__GNUC__)
#elif defined(__clang__)
#endif
