#ifndef XOCRWIDGET_H
#define XOCRWIDGET_H

#include <QWidget>

namespace Ui {
class XOcrWidget;
}

class XOcrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XOcrWidget(QWidget *parent = nullptr);
    ~XOcrWidget();

    void setLeftPixmap(const QPixmap& pix);
    void setRightText(const QString& text);
    void appendRightText(const QString& text);

private slots:
    void on_splitter_splitterMoved(int pos, int index);

private:
    Ui::XOcrWidget *ui;
};

#endif // XOCRWIDGET_H
