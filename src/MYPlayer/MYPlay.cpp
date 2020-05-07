#include "MYPlay.h"
#include <QDebug>
#include <QSettings>
#include <QFileInfo>
#include <QPointer>
#include "MYDemuxThread.h"
#include "MYVideoOutput.h"
#include "MYSubTitle.h"

static QScopedPointer<MYDemuxThread> dt;
static QScopedPointer<MYSubTitle> st;

MYPlay::MYPlay()
{
    dt.reset(new MYDemuxThread);
    st.reset(new MYSubTitle);
    dt->Start();
    startTimer(40);
    setBackGround(NULL);
    setVideoPath(NULL);
    setSubTitlePath(NULL);
}

MYPlay::~MYPlay()
{
    qDebug()<<"close";
    if(m_mOpenSuccess)
    {
        QSettings *settingIni = new QSettings("setting.ini",QSettings::IniFormat);
        qDebug()<<m_strTheVideoPath;
        settingIni->setValue("Progress/" + m_strTheVideoPath,dt->pts);
        delete settingIni;
    }
    dt->Close();
}

void MYPlay::testFunc()
{
    setDemoNum(demoNum() + 1);
}

//打开线程，传输文件路径
void MYPlay::urlPass(QString url)
{
    MYVideoOutput* video =qobject_cast<MYVideoOutput*>(source());
    qDebug() << (video == nullptr) <<endl;
    m_strTheVideoPath = url;
    if(video != nullptr)
    {
        QFileInfo fileInfo(url);
        QString subTitleP = url.left(url.length() - fileInfo.suffix().length());
        QSettings *settingIni = new QSettings("setting.ini",QSettings::IniFormat);
        QString saveProgress = settingIni->value("Progress/" + url).toString();
        qDebug()<<saveProgress;
        delete settingIni;
        //qDebug()<<subTitleP;
        fileInfo = QFileInfo(subTitleP+"srt");     //打开视频时自动查找视频所在文件夹有对应名称的无字幕文件，有则打开
        if(fileInfo.exists())
        {
            st->Open(fileInfo.absoluteFilePath());
        }
        else
            qDebug()<<"there is no srt file";
        //if(fileInfo.exists())
            //st->Open(fileInfo);
        if(!dt->Open(url.toLocal8Bit(),video))
            return;
        if(saveProgress != "")
            dt->Seek(saveProgress.toDouble() / (double)dt->totalMs);
    }
    qDebug() << url;
    setIsPlay(!dt->isPause);
    setOpenSuccess(dt->getOpenSuccess());
}

//播放进度条
void MYPlay::posFind(double value)
{
    double pos = ((double)(int)(((double)dt->pts / (double)dt->totalMs) * 1000)) / 1000;
    double newValue = ((double)(int)(value * 1000)) / 1000;
    qDebug() << "pos value = " << pos;
    dt->Seek(newValue);
}

void MYPlay::timerEvent(QTimerEvent *e)
{
    //if (isSliderPress)return;
    setTestNum(m_mTestNum + 1);
    long long total = dt->totalMs;
    //qDebug()<<dt->pts;
    int currentTime = (int)dt->pts;
    //qDebug()<<st->isOpen();
    if(st->isOpen())
    {
        if(!(st->nodeStartTime() <= currentTime && st->nodeEndTime() >= currentTime) || (st->nodeEndTime() == st->nodeStartTime()))   //如果已经读取到现在的节点则没必要在read，或者处于刚开始时需要执行read
            if(st->Read(currentTime))
                setSubTitleText(st->text());
            else
                setSubTitleText("");
    }
    if (total > 0)
    {
        double pos = ((double)(int)(((double)dt->pts / (double)total) * 1000)) / 1000;     //保留三位小数，后面置0，方便比较
        setPosNum(pos);
    }
}
//播放暂停
void MYPlay::playOrPause()
{
    bool isPause = !dt->isPause;
    setIsPlay(!isPause);
    dt->SetPause(isPause);
}
//快进
void MYPlay::goOn()
{
    double pos = ((double)(int)(((double)dt->pts / (double)dt->totalMs) * 1000)) / 1000 + 0.02;   //依照需求调整快进的程度
    dt->Seek(pos);
}
//快退
void MYPlay::goBack()
{
    double pos = ((double)(int)(((double)dt->pts / (double)dt->totalMs) * 1000)) / 1000 - 0.02;   //依照需求调整快退的程度
    dt->Seek(pos);
}
//改变并储存背景
void MYPlay::changeBackground(QString url)
{
    setBackGround(url);
}
//改变视频选择路径
void MYPlay::changeVideoPath(QString url)
{
    setVideoPath(url);
}
//改变字幕选择路径
void MYPlay::changeSubTitlePath(QString url)
{
    setSubTitlePath(url);
}
//改变音量
void MYPlay::setVolume(double newVolume)
{
    dt->SetVolume(((double)(int)(newVolume*100))/100);
}
//停止
void MYPlay::setStop()
{
    if(m_mOpenSuccess)
    {
        dt->Close();
        dt.reset(new MYDemuxThread);
        dt->Start();
        if(st->isOpen())
        {
            st.reset(new MYSubTitle);
            setSubTitleText("");
        }
        setOpenSuccess(false);
    }
}
//加载字幕
void MYPlay::loadSubTitle(QString url)
{
    if(m_mOpenSuccess)
    {
        st.reset(new MYSubTitle);
        st->Open(url);
    }
}
// demoNum
int MYPlay::demoNum() const
{
    return m_mDemoNum;
}

void MYPlay::setDemoNum(int newValue)
{
    if (m_mDemoNum != newValue)
    {
        m_mDemoNum = newValue;
        emit demoNumChanged(m_mDemoNum);
    }
}

int MYPlay::testNum() const
{
    return m_mTestNum;
}

void MYPlay::setTestNum(int newValue)
{
    if (m_mTestNum != newValue)
    {
        m_mTestNum = newValue;
        emit testNumChanged(m_mTestNum);
    }
}

double MYPlay::posNum() const
{
    return m_mPosNum;
}

void MYPlay::setPosNum(double newValue)
{
    if (m_mPosNum != newValue)
    {
        m_mPosNum = newValue;
        emit posNumChanged(m_mPosNum);
    }
}

bool MYPlay::isPlay() const
{
    return m_mIsPlaying;
}

void MYPlay::setIsPlay(bool isPlaying)
{
    if(m_mIsPlaying != isPlaying)
    {
        m_mIsPlaying = isPlaying;
        emit isPlayingChanged(m_mIsPlaying);
    }
}

bool MYPlay::openSuccess() const
{
    return m_mOpenSuccess;
}

void MYPlay::setOpenSuccess(bool openSuccess)
{
    if(m_mOpenSuccess != openSuccess)
    {
        m_mOpenSuccess = openSuccess;
        emit openSuccessChanged(m_mOpenSuccess);
    }
}

QObject* MYPlay::source() const
{
    return m_mSource;

}
void MYPlay::setSource(QObject *source)
{
    if(m_mSource != source)
    {
        m_mSource = source;
        emit sourceChanged(m_mSource);
    }
}
//背景图片路径
QString MYPlay::backGround() const
{
    return m_strBackgroundPath;
}
void MYPlay::setBackGround(QString url)
{
    QSettings *settingIni = new QSettings("setting.ini",QSettings::IniFormat);
    QString backGroundpath = settingIni->value("Path/Background").toString();
    if(url == NULL)
    {
        if(backGroundpath == NULL)
        {
            delete settingIni;
            return;
        }
        else
        {
            m_strBackgroundPath = backGroundpath;
            QFileInfo fileInfo(backGroundpath);
            m_strBackGroundChoosePath = fileInfo.path();
            qDebug()<<m_strBackGroundChoosePath<<endl;
            delete settingIni;
            emit backGroundChanged(m_strBackgroundPath);
            emit backGroundChooseChanged(m_strBackGroundChoosePath);
            return;
        }
    }
    if(m_strBackgroundPath != url)
    {
        settingIni->setValue("Path/Background",url);
        m_strBackgroundPath = url;
        QFileInfo fileInfo(url);
        //qDebug()<<fileInfo.suffix();
        m_strBackGroundChoosePath = fileInfo.path();
        emit backGroundChanged(m_strBackgroundPath);
        emit backGroundChooseChanged(m_strBackGroundChoosePath);
    }
    delete settingIni;
}
//选择背景路径
QString MYPlay::backGroundChoose() const
{
    return m_strBackGroundChoosePath;
}

//选择视频路径
QString MYPlay::videoPath() const
{
    return m_strVideoPath;
}
void MYPlay::setVideoPath(QString url)
{
    QSettings *settingIni = new QSettings("setting.ini",QSettings::IniFormat);
    QString videoPath = settingIni->value("Path/VideoChoose").toString();
    if(url == NULL)
    {
        if(videoPath == NULL)
        {
            delete settingIni;
            return;
        }
        else
        {
            m_strVideoPath = videoPath;
            delete settingIni;
            emit videoPathChanged(m_strVideoPath);
            return;
        }
    }
    if(m_strVideoPath != url)
    {
        QFileInfo fileInfo(url);
        m_strVideoPath = fileInfo.path();
        settingIni->setValue("Path/VideoChoose",m_strVideoPath);
        emit videoPathChanged(m_strVideoPath);
    }
    delete settingIni;
}
//选择字幕路径
QString MYPlay::subTitlePath() const
{
    return m_strSubTitlePath;
}
void MYPlay::setSubTitlePath(QString url)
{
    QSettings *settingIni = new QSettings("setting.ini",QSettings::IniFormat);
    QString subTitlePath = settingIni->value("Path/SubTitleChoose").toString();
    if(url == NULL)
    {
        if(subTitlePath == NULL)
        {
            delete settingIni;
            return;
        }
        else
        {
            m_strSubTitlePath = subTitlePath;
            delete settingIni;
            emit subTitlePathChanged(m_strSubTitlePath);
            return;
        }
    }
    if(m_strSubTitlePath != url)
    {
        QFileInfo fileInfo(url);
        m_strSubTitlePath = fileInfo.path();
        settingIni->setValue("Path/SubTitleChoose",m_strSubTitlePath);
        emit subTitlePathChanged(m_strSubTitlePath);
    }
    delete settingIni;
}

//字幕文本
QString MYPlay::subTitleText() const
{
    return m_mSubTitleText;
}
void MYPlay::setSubTitleText(QString text)
{
    if(m_mSubTitleText != text)
    {
        m_mSubTitleText = text;
        emit subTitleTextChanged(m_mSubTitleText);
    }
}
