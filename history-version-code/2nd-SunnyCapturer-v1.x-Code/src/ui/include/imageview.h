#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QLabel>

class ImageView : public QWidget
{
    Q_OBJECT

public:
    ImageView(QWidget *parent = nullptr);
    ~ImageView();
    void setPixmap(const QPixmap &newPix);

private:
    void initUI();
    void updateZoomLabel();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onLoadImage(void);
    void onZoomInImage(void);
    void onZoomOutImage(void);
    void onPresetImage(void);

private:
    QPixmap        m_pix;
    qreal          m_ZoomValue = 1.0;
    int            m_XPtInterval = 0;
    int            m_YPtInterval = 0;
    QPoint         m_OldPos;
    bool           m_Pressed = false;
    QLabel*        m_labZoom;
    QTimer*        m_timerLabZoom;
};

#endif // IMAGEVIEW_H
