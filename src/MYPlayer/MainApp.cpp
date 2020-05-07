#include <QDir>
#include <QFont>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QStandardPaths>
#include <QSettings>
#include <QDebug>
#include "MainApp.h"
#include "MYPlay.h"
#include "MYVideoOutput.h"

#define APP_NAME                                "MYPlayer"
#define APP_DISPLAYNAME                         "MYPlayer"
#define APP_ICON_PATH                           ":/image/AppIcon.ico"
#define UI_MAIN_PATH                            "qrc:/qml/main.qml"
#define UI_TRANSLATION_PATH                     ":/translation/MYPlayer_zh_CN.qm"
#define IMAGEDIR_RELATIVE_PATH                  "./resource/ui/image"
#define DEFAULT_FONT_NAME                       "\xE5\xBE\xAE\xE8\xBD\xAF\xE9\x9B\x85\xE9\xBB\x91"// 微软雅黑


MainApp* g_pApp = nullptr;

MainApp* GetMainApp()
{
    return g_pApp;
}

QObject* MainApp::GetInstance(QQmlEngine* /*engine*/, QJSEngine* /*scriptEngine*/)
{
    QObject* pObj = GetMainApp();
    QQmlEngine::setObjectOwnership(pObj, QQmlEngine::CppOwnership);
    return pObj;
}

MainApp::MainApp(int &argc, char **argv)
       : QApplication(argc, argv)
       , m_strAppRootPath()
       , m_strImageRootPath()
{
}

MainApp::~MainApp() = default;

int MainApp::Main(int /*argc*/, char* /*argv*/[])
{
	// 初始化失败后，反初始化
    if (!Initialize())
    {
        Uninitialize();
        return EXIT_FAILURE;
    }
	// 执行程序
    int nRet = exec();

	// 反初始化
    Uninitialize();

    return nRet;
}

void MainApp::testFunc()
{
    setDemoNum(demoNum() + 1);
}

void MainApp::changeLanuage(QString language)
{
    if(language == MainApp::language())
        return;
    qDebug()<<language<<endl;
    m_pTranslator.reset(new QTranslator);
    QSettings *settingIni = new QSettings("setting.ini",QSettings::IniFormat);
    settingIni->setValue("Config/Language",language);
    if(language == "Chinese")
    {
        if (!m_pTranslator->load(QStringLiteral(UI_TRANSLATION_PATH)))
            {return;}
        // Add translator
        installTranslator(m_pTranslator.data());
    }
    if(language == "English")
        installTranslator(m_pTranslator.data());
    setLanguage(language);
    delete settingIni;
}

// demoNum
int MainApp::demoNum() const
{
    return m_mDemoNum;
}

void MainApp::setDemoNum(int newValue)
{
    if (m_mDemoNum != newValue)
    {
        m_mDemoNum = newValue;
        emit demoNumChanged(m_mDemoNum);
    }
}

QString MainApp::language() const
{
    return m_mLanguage;
}
void MainApp::setLanguage(QString newValue)
{
    if (m_mLanguage != newValue)
    {
        m_mLanguage = newValue;
        emit languageChanged(m_mLanguage);
    }
}

bool MainApp::Initialize()
{
    // 设置应用程序属性
    setApplicationName(QStringLiteral(APP_NAME));
    setWindowIcon(QIcon(QStringLiteral(APP_ICON_PATH)));
    setApplicationDisplayName(tr(APP_DISPLAYNAME));
//    setOrganizationName();
//    setOrganizationDomain();
//    setApplicationVersion();

    // Initialize Path
    InitializePath();

    // Initialize config
    InitializeCfg();

    // Initialize translator
    if (!InitializeTranslator())
        {return false;}

    // Initialize QML engine
    if (!InitializeQmlEngine())
        {return false;}

    // Initialize UI
    if (!InitializeUI(m_pQmlEngine.data()))
        {return false;}

    InitializeOthers();

    return true;
}

bool MainApp::Uninitialize()
{
    UninitializeOthers();

    // Uninitialize UI
    UninitializeUI(m_pQmlEngine.data());

    // Uninitialize Qml Engine
    UninitializeQmlEngine();

    // Uninitialize translator
    UninitializeTranslator();

    // Uninitialize config
    UninitializeCfg();

    // Uninitialize Path
    UninitializePath();

    return true;
}

bool MainApp::InitializePath()
{
    // Application directory path
    m_strAppRootPath = qApp->applicationDirPath();
    QDir dirApp(m_strAppRootPath);
    if (!dirApp.exists())
        {return false;}

    // Image root directory path
    m_strImageRootPath = m_strAppRootPath;
    m_strImageRootPath.append(QStringLiteral("/")).append(QStringLiteral(IMAGEDIR_RELATIVE_PATH));
    QDir dirImg(m_strImageRootPath);
    if (!dirImg.exists())
    {
    }
    m_strImageRootPath = dirImg.canonicalPath();

    return true;
}

bool MainApp::UninitializePath()
{
    // Image root directory path
    m_strImageRootPath.clear();

    // Application directory path
    m_strAppRootPath.clear();

    return true;
}

bool MainApp::InitializeCfg()
{
    return true;
}

bool MainApp::UninitializeCfg()
{
    return true;
}

bool MainApp::InitializeTranslator()
{
    if (m_pTranslator != nullptr)
        {return true;}

    // Load translation from resource
    m_pTranslator.reset(new QTranslator);
    QSettings *settingIni = new QSettings("setting.ini",QSettings::IniFormat);

    QString Language = settingIni->value("Config/Language").toString();
    qDebug()<<Language<<endl;
    if(Language == NULL)
    {
        settingIni->setValue("Config/Language",QString("Chinese"));
        Language = settingIni->value("Config/Language").toString();
    }
    setLanguage(Language);
    if(Language == "Chinese")
    {
        if (!m_pTranslator->load(QStringLiteral(UI_TRANSLATION_PATH)))
            {return false;}
        // Add translator
        installTranslator(m_pTranslator.data());
    }
    if(Language == "English")
    {

    }
    delete settingIni;
    return true;
}

bool MainApp::UninitializeTranslator()
{
    if (!m_pTranslator)
        {return true;}

    // Remove translator
    removeTranslator(m_pTranslator.data());

    // Free translator instance
    m_pTranslator.reset();

    return true;
}

bool MainApp::InitializeQmlEngine()
{
    if (m_pQmlEngine != nullptr)
        {return true;}

    // Create QML engine instance
    m_pQmlEngine.reset(new QQmlApplicationEngine);

    return (nullptr != m_pQmlEngine);
}

bool MainApp::UninitializeQmlEngine()
{
    if (!m_pQmlEngine)
        {return true;}

    // Free QML engine instance
    m_pQmlEngine.reset();

    return true;
}

bool MainApp::InitializeUI(QQmlApplicationEngine* pQmlEngine)
{
    if (pQmlEngine == nullptr)
        {return false;}

    // 设置程序默认字体
    setFont(QFont(DEFAULT_FONT_NAME, 9));

    // Register Qml Types
    qmlRegisterSingletonType<MainApp>("com.imooc.myplayer", 1, 0, "MainApp",
                                      &MainApp::GetInstance);
    qmlRegisterType<MYPlay>("com.imooc.myplayer", 1, 0, "MYPlay");
    qmlRegisterType<MYVideoOutput>("com.imooc.myplayer", 1, 0, "MYVideoOutput");
//    qmlRegisterType<MYQmlVideoOutput>("com.imooc.myplayer", 1, 0, "MYQmlVideoOutput");

    // Set context property
//    pQmlEngine->rootContext()->setContextProperty("appCfg", m_pCfg.data());

    // Add image provider
//    pQmlEngine->addImageProvider(LMCBtnImageProvider::s_strProviderId,
//                            new LMCBtnImageProvider(m_strImageRootPath));
//    pQmlEngine->addImageProvider(LMCImageProvider::s_strProviderId,
//                            new LMCImageProvider(m_strImageRootPath));

    // Load main UI qml from resource
    pQmlEngine->load(QUrl(QStringLiteral(UI_MAIN_PATH)));
    QList<QObject*> rootObjs(pQmlEngine->rootObjects());
    return !rootObjs.isEmpty();;
}

bool MainApp::UninitializeUI(QQmlApplicationEngine* pQmlEngine)
{
    if (pQmlEngine == nullptr)
        {return true;}

    return true;
}

bool MainApp::InitializeOthers()
{
    // 注册类型
    //qRegisterMetaType<QSharedPointer<WorkTask>>();

    return true;
}

bool MainApp::UninitializeOthers()
{
    return true;
}
