#ifndef MYSUBTITLE_H
#define MYSUBTITLE_H

#include <QObject>
#include <QPointer>
#include <QFile>

class MYSubTitle : public QObject
{
    Q_OBJECT
public:
    MYSubTitle();

    //打开
    void Open(QString url);
    //读取字幕元素
    bool Read(int currentTime);

    //是否打开
    bool isOpen();
    //开始时间
    int nodeStartTime();
    //结束时间
    int nodeEndTime();
    //字幕内容
    QString text();

    enum SUBTITLE_TYPE{
        SUBTITLE_TYPE_UNKNOW = 0,
        SUBTITLE_TYPE_SRT,
        SUBTITLE_TYPPE_SSA      //SSA或ASS
        //...
    };
    Q_ENUM(SUBTITLE_TYPE)   //Qt的枚举类型需继承自QObject且用Q_ENUM声明

private:
    //int                                         playResX;
    //int                                         playResY;

    SUBTITLE_TYPE                                              m_mSubType{ SUBTITLE_TYPE_UNKNOW };

    QScopedPointer<QFile>                                      m_mSubTitleFile;

    int                                                        m_mNodeStartTime{ 0 };

    int                                                        m_mNodeEndTime{ 0 };

    QString                                                    m_mText{ "" };

    bool                                                       m_mIsOpen{ false };                        //是否打开
};

#endif // MYSUBTITLE_H
