#ifndef MYAUDIOPLAY_H
#define MYAUDIOPLAY_H

class MYAudioPlay
{
public:
    int sampleRate = 44100;
    int sampleSize = 16;
    int channels = 2;

    //打开音频播放
    virtual bool Open() = 0;
    virtual void Close() = 0;
    virtual void Clear() = 0;
    //返回缓冲中未播放的时间（ms）
    virtual long long GetNoPlayMs() = 0;
    //播放音频
    virtual bool Write(const unsigned char *data, int datasize) = 0;
    virtual int GetFree() = 0;

    //暂停
    virtual void SetPause(bool isPause) = 0;

    //改变音量
    virtual void SetVolume(double newVolume) = 0;

    static MYAudioPlay *Get();
    MYAudioPlay();
    virtual ~MYAudioPlay();
};

#endif // MYAUDIOPLAY_H
