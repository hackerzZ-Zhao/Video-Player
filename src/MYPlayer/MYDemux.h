#ifndef MYDEMUX_H
#define MYDEMUX_H

#include <mutex>

struct AVFormatContext;
struct AVPacket;
struct AVCodecParameters;

class MYDemux
{
public:

    //打开媒体文件，或者流媒体 rtmp http rstp
    virtual bool Open(const char *url);

    //空间需要调用者释放，释放AVPacket对象空间，和数据空间 av_packet_free
    virtual AVPacket *Read();

    //只读视频,音频丢弃——用于解耦合
    virtual AVPacket *ReadVideo();

    virtual bool IsAudio(AVPacket *pkt);

    //获取视频参数 返回的空间需要清理 avcodec_parameters_free
    virtual AVCodecParameters *CopyVPara();

    //获取音频参数 返回的空间需要清理 avcodec_parameters_free
    virtual AVCodecParameters *CopyAPara();

    //seek 位置 pos 0.0~1.0
    virtual bool Seek(double pos);

    //清空读取缓存
    virtual void Clear();
    virtual void Close();


    MYDemux();
    virtual ~MYDemux();

    //媒体总时长（毫秒）
    int totalMs = 0;
    int width = 0;
    int height = 0;
    int sampleRate = 0;
    int channels = 0;



protected:
    std::mutex mux;
    //解封装上下文
    AVFormatContext *ic = NULL;
    //音视频索引，读取时区分音视频
    int videoStream = 0;
    int audioStream = 1;


};

#endif // MYDEMUX_H
