#ifndef MAINAPP_H
#define MAINAPP_H


#include <QApplication>
#include <QPointer>


class QJSEngine;
class QQmlEngine;
class QQmlApplicationEngine;
class QTranslator;

class MainApp : public QApplication
{
    Q_OBJECT

    // For QML property(-ies)
    // e.g.
    Q_PROPERTY(int demoNum READ demoNum WRITE setDemoNum NOTIFY demoNumChanged)

    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)

public:
    // The singleton type provider function (callback). µ¥Àý
    static QObject* GetInstance(QQmlEngine* engine, QJSEngine* scriptEngine);

public:
    MainApp(int &argc, char **argv);
    ~MainApp() override;

    int Main(int argc, char* argv[]);

public:
    // For QML invokable function(s)
    Q_INVOKABLE void testFunc();

    Q_INVOKABLE void changeLanuage(QString language);

    // For QML property(-ies)
    // e.g.
    // demoNum
    int demoNum() const;
    void setDemoNum(int newValue);

    QString language() const;
    void setLanguage(QString newValue);


signals:
    void demoNumChanged(int newValue);

    void languageChanged(QString newValue);

public slots:

private slots:

private:
    bool Initialize();
    bool Uninitialize();

    bool InitializePath();
    bool UninitializePath();

    bool InitializeCfg();
    bool UninitializeCfg();

    bool InitializeTranslator();
    bool UninitializeTranslator();

    bool InitializeQmlEngine();
    bool UninitializeQmlEngine();

    bool InitializeUI(QQmlApplicationEngine* pQmlEngine);
    bool UninitializeUI(QQmlApplicationEngine* pQmlEngine);

    bool InitializeOthers();
    bool UninitializeOthers();

private:
    QString                                     m_strAppRootPath;
    QString                                     m_strImageRootPath;

    QScopedPointer<QTranslator>                 m_pTranslator;
    QScopedPointer<QQmlApplicationEngine>       m_pQmlEngine;

    int                                         m_mDemoNum{0};

    QString                                     m_mLanguage{""};
};


MainApp* GetMainApp();


#endif // MAINAPP_H
