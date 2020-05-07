#include "MYDecode.h"
#include "MYDecodeThread.h"

//清理资源，停止线程
void MYDecodeThread::Close()
{
    Clear();

    //等待线程结束
    isExit = true;
    wait();
    decode->Close();

    mux.lock();
    delete decode;
    decode = NULL;
    mux.unlock();
}

//清理队列
void MYDecodeThread::Clear()
{
    mux.lock();
    decode->Clear();
    while (!packs.empty())
    {
        AVPacket *pkt = packs.front();
        MYFreePacket(&pkt);
        packs.pop_front();
    }
    mux.unlock();
}


//取出一帧数据，并出栈，如果没有返回NULL
AVPacket *MYDecodeThread::Pop()
{
    mux.lock();
    if (packs.empty())
    {
        mux.unlock();
        return NULL;
    }
    AVPacket *pkt = packs.front();
    packs.pop_front();
    mux.unlock();
    return pkt;
}
void MYDecodeThread::Push(AVPacket *pkt)
{
    if (!pkt)return;
    //进行阻塞——读取比解码快
    while (!isExit)
    {
        mux.lock();
        if (packs.size() < maxList)
        {
            packs.push_back(pkt);
            mux.unlock();
            break;
        }
        mux.unlock();
        msleep(1);
    }
}


MYDecodeThread::MYDecodeThread()
{
    //打开解码器
    if (!decode) decode = new MYDecode();
}


MYDecodeThread::~MYDecodeThread()
{	//等待线程退出
    isExit = true;
    wait();
}
