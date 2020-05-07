#ifndef MYVIDEOTHREAD_H
#define MYVIDEOTHREAD_H

//解码和显示视频
#include <list>
#include <mutex>
#include <QThread>
#include "IVideoCall.h"
#include "MYDecodeThread.h"


struct AVPacket;
struct AVCodecParameters;
class MYDecode;

class MYVideoThread :public MYDecodeThread
{
public:
    //解码pts，如果接受到的解码数据pts > seekpts 返回true  并且显示画面
    virtual bool RepaintPts(AVPacket *pkt, long long seekpts);
    //打开，不管成功与否都清理
    virtual bool Open(AVCodecParameters *para, IVideoCall *call, int width, int height);

    void run();

    MYVideoThread();
    virtual ~MYVideoThread();

    //同步时间，由外部传入
    long long synpts = 0;

    //暂停
    void SetPause(bool isPause);
    bool isPause = false;


protected:
    IVideoCall *call = 0;
    std::mutex vmux;


};

#endif // MYVIDEOTHREAD_H
