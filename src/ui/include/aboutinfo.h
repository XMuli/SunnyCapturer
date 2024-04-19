#ifndef ABOUTINFO_H
#define ABOUTINFO_H

#include <QKeyEvent>
#include <QWidget>

namespace Ui {
class AboutInfo;
}

class AboutInfo : public QWidget
{
    Q_OBJECT

public:
    explicit AboutInfo(QWidget *parent = nullptr);
    ~AboutInfo();

private slots:
    void on_pushButton_released();
    void onLanguageChange(const QString &language);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void init();
    const QString detailedInfo() const;
    void insertLayout(const QStringList &lists);

private:
    Ui::AboutInfo *ui;
};

#endif // ABOUTINFO_H
