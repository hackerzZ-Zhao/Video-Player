#ifndef MYDECODETHREAD_H
#define MYDECODETHREAD_H

//解码和显示视频
#include <list>
#include <mutex>
#include <QThread>

struct AVPacket;
class MYDecode;

class MYDecodeThread :public QThread
{
public:
    MYDecodeThread();
    virtual ~MYDecodeThread();
    virtual void Push(AVPacket *pkt);
    //清理队列
    virtual void Clear();
    //清理资源，停止线程
    virtual void Close();

    //取出一帧数据，并出栈，如果没有返回NULL
    virtual AVPacket *Pop();
    //最大队列（最大缓冲近两秒）
    int maxList = 100;
    bool isExit = false;

protected:
    MYDecode *decode = 0;
    std::list <AVPacket *> packs;
    std::mutex mux;
};

#endif // MYDECODETHREAD_H
