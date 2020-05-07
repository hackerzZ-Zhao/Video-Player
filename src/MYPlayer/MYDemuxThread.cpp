#include <iostream>
extern "C"
{
    #include <libavformat/avformat.h>
}
#include "MYDemux.h"
#include "MYVideoThread.h"
#include "MYAudioThread.h"
#include "MYDecode.h"
#include "MYDemuxThread.h"

#include <QDebug>


using namespace std;

void MYDemuxThread::Clear()
{
    mux.lock();
    if (demux)demux->Clear();
    if (vt)vt->Clear();
    if (at)at->Clear();
    mux.unlock();
}

void MYDemuxThread::Seek(double pos)
{
    //清理缓冲
    Clear();
    mux.lock();
    bool status = this->isPause;
    mux.unlock();

    //暂停
    SetPause(true);
    mux.lock();
    if (demux)
        demux->Seek(pos);
    mux.unlock();
    //实际要显示的位置pts
    long long seekPts = pos*demux->totalMs;
    while (!isExit)
    {
        AVPacket *pkt = demux->ReadVideo();
        if (!pkt) break;
        //如果解码到seekPts
        if (vt->RepaintPts(pkt, seekPts))
        {
            this->pts = seekPts;
            break;
        }
        //bool re = vt->decode->Send(pkt);
        //if (!re)break;
        //AVFrame *frame= vt->decode->Recv();
        //if (!frame)continue;
        ////到达位置
        //if (frame->pts >= seekPts)
        //{
        //	this->pts = frame->pts;
        //	vt->call->Repaint(frame);
        //	break;
        //}
        //av_frame_free(&frame);

    }

    //seek是非暂停状态
    if (!status)
        SetPause(false);
}

//暂停
void MYDemuxThread::SetPause(bool isPause)
{
    mux.lock();
    this->isPause = isPause;
    if (at)at->SetPause(isPause);
    if (vt)vt->SetPause(isPause);
    mux.unlock();
}

//改变音量
void  MYDemuxThread::SetVolume(double newVolume)
{
    //mux.lock();
    if(!at)
    {
        //mux.unlock();
        return;
    }
    //mux.unlock();
    qDebug()<<newVolume;
    at->SetVolume(newVolume);
}

//打开文件是否成功
void MYDemuxThread::setOpenSuccess(bool value)
{
    openSuccess = value;
}
bool MYDemuxThread::getOpenSuccess()
{
    return openSuccess;
}

void MYDemuxThread::run()
{
    while (!isExit)
    {
        mux.lock();
        if (isPause)
        {
            mux.unlock();
            msleep(5);
            continue;
        }
        if (!demux)
        {
            mux.unlock();
            msleep(5);
            continue;
        }

        //音视频同步
        if (vt && at)
        {
            pts = at->pts;
            vt->synpts = at->pts;
        }

        AVPacket *pkt = demux->Read();
        if (!pkt)
        {
            mux.unlock();
            msleep(5);
            continue;
        }
        //判断数据是音频
        if (demux->IsAudio(pkt))
        {
            if (at)at->Push(pkt);
        }
        else //视频
        {
            if (vt)vt->Push(pkt);
        }
        mux.unlock();
        msleep(1);
    }
}

//创建对象并打开
bool MYDemuxThread::Open(const char *url, IVideoCall *call)
{
    if (url == 0 || url[0] == '\0')
        return false;

    mux.lock();
    if (!demux) demux = new MYDemux();
    if (!vt) vt = new MYVideoThread();
    if (!at) at = new MYAudioThread();

    //打开解封装
    bool re = demux->Open(url);
    if (!re)
    {
        mux.unlock();
        cout << "demux->Open(url) failed!" << endl;
        return false;
    }
    setOpenSuccess(true);    //打开成功
    //打开视频解码器和处理线程
    if (!vt->Open(demux->CopyVPara(), call, demux->width, demux->height))
    {
        re = false;
        cout << "vt->Open failed!" << endl;
    }
    //打开音频解码器和处理线程
    if (!at->Open(demux->CopyAPara(), demux->sampleRate, demux->channels))
    {
        re = false;
        cout << "at->Open failed!" << endl;
    }
    totalMs = demux->totalMs;
    mux.unlock();
    cout << "MYDemuxThread::Open " << re << endl;
    return re;
}

//关闭线程清理资源
void MYDemuxThread::Close()
{
    isExit = true;
    wait();
    if (demux) demux->Close();
    if (vt) vt->Close();
    if (at) at->Close();
    mux.lock();
    QThread::quit();
    delete demux;
    delete vt;
    delete at;
    demux = NULL;
    vt = NULL;
    at = NULL;
    openSuccess = false;
    mux.unlock();
}

//启动所有线程
void MYDemuxThread::Start()
{
    mux.lock();
    if (!demux) demux = new MYDemux();
    if (!vt) vt = new MYVideoThread();
    if (!at) at = new MYAudioThread();
    //启动当前线程
    QThread::start();
    if (vt)vt->start();
    if (at)at->start();
    mux.unlock();
}
MYDemuxThread::MYDemuxThread()
{
}


MYDemuxThread::~MYDemuxThread()
{
    isExit = true;
    wait();
}
