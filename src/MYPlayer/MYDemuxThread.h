#ifndef MYDEMUXTHREAD_H
#define MYDEMUXTHREAD_H

#include <mutex>
#include <QThread>
#include "IVideoCall.h"


class MYDemux;
class MYVideoThread;
class MYAudioThread;

class MYDemuxThread :public QThread
{
public:
    //创建对象并打开
    virtual bool Open(const char *url, IVideoCall *call);
    //启动所有线程
    virtual void Start();

    //关闭线程清理资源
    virtual void Close();
    virtual void Clear();

    virtual void Seek(double pos);

    void run();
    MYDemuxThread();
    virtual ~MYDemuxThread();
    bool isExit = false;
    long long pts = 0;
    long long totalMs = 0;
    //暂停
    void SetPause(bool isPause);
    bool isPause = false;

    //改变音量
    void SetVolume(double newVolume);

    //打开文件是否成功
    void setOpenSuccess(bool value);
    bool getOpenSuccess();
protected:
    std::mutex mux;
    MYDemux *demux = 0;
    MYVideoThread *vt = 0;
    MYAudioThread *at = 0;
    bool openSuccess = false;
};

#endif // MYDEMUXTHREAD_H
