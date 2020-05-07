#ifndef MYVIDEOOUTPUT_H
#define MYVIDEOOUTPUT_H

#include <QQuickPaintedItem>
#include <QImage>
#include <memory>
#include <mutex>
#include "IVideoCall.h"


struct SwsContext;

class MYVideoOutput : public QQuickPaintedItem, public IVideoCall
{
    Q_OBJECT

public:
    virtual void Init(int width, int height);

    //不管成功与否都释放frame空间
    virtual void Repaint(AVFrame *frame);

    explicit MYVideoOutput(void);
    ~MYVideoOutput();

signals:
    void requestUpdate();

public slots:
    void procUpdate();

protected:
    virtual void paint(QPainter *pPainter);

private:
    std::mutex                                  mux;
    QImage                                      m_oVideoFrame;
    SwsContext*                                 m_pSwsContext{nullptr};
};

#endif // MYVIDEOOUTPUT_H
