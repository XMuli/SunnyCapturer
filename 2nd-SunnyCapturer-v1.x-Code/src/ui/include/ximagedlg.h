#ifndef XIMAGEDLG_H
#define XIMAGEDLG_H

#include <QWidget>

namespace Ui {
class XImageDlg;
}

#include <QLabel>
#include <QPixmap>
#include <QScrollArea>

class XImageDlg : public QWidget
{
    Q_OBJECT

public:
    explicit XImageDlg(QWidget *parent = nullptr);
    ~XImageDlg();

    void setPixmap(const QPixmap &pix);
    QLabel* label();
    QScrollArea* scrollArea();


private:
    Ui::XImageDlg *ui;
};

#endif // XIMAGEDLG_H
