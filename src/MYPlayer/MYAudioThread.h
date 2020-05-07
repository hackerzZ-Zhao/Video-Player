#ifndef MYAUDIOPLAYTHREAD_H
#define MYAUDIOPLAYTHREAD_H

#include <mutex>
#include <list>
#include <QThread>
#include "MYDecodeThread.h"


struct AVCodecParameters;
class MYAudioPlay;
class MYResample;

class MYAudioThread : public MYDecodeThread
{
public:
    //当前音频播放的pts
    long long pts = 0;
    //打开，不管成功与否都清理
    virtual bool Open(AVCodecParameters *para, int sampleRate, int channels);

    //停止线程，清理资源
    virtual void Close();

    virtual void Clear();
    void run();
    MYAudioThread();
    virtual ~MYAudioThread();

    //暂停
    void SetPause(bool isPause);
    bool isPause = false;
    //改变音量
    void SetVolume(double newVolume);

protected:
    std::mutex amux;
    MYAudioPlay *ap = 0;
    MYResample *res = 0;

};

#endif // MYAUDIOPLAYTHREAD_H
