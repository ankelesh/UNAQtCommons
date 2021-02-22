#include "QBroadcastCatcher.h"
#ifdef Q_OS_ANDROID
#include "jni.h"
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroid>
#include <QMetaType>



Q_DECLARE_METATYPE(PairedStringList);

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
  Java_una_unamd_qbroadcastcatcher_QBroadcastCatcher_handleTargetedExtra(JNIEnv *env,
                                        jobject obj,
                                        jstring sender_name,
                                        jstring data)
{
    Q_UNUSED (obj);
    const char *b_data = env->GetStringUTFChars(data, Q_NULLPTR);
    const char * b_sendName = env->GetStringUTFChars(sender_name, Q_NULLPTR);
    QString sendName = QString::fromUtf8(b_sendName, strlen(b_sendName));
    QString icontent = QString::fromUtf8(b_data,strlen(b_data));
    QBroadcastCatcher::instanse()->handleTargetedIntent(sendName, icontent);
}

JNIEXPORT void JNICALL
    Java_una_unamd_qbroadcastcatcher_QBroadcastCatcher_handleIntentContents(JNIEnv *env,
                                      jobject obj,
                                      jstring sender_name,
                                      jstring icontent_serialization)
{
    Q_UNUSED (obj);
    const char * b_sendName = env->GetStringUTFChars(sender_name, NULL);
    const char * b_icontent = env->GetStringUTFChars(icontent_serialization, NULL);
    QString sendName = QString::fromUtf8(b_sendName, strlen(b_sendName));
    QString iContent = QString::fromUtf8(b_icontent, strlen(b_icontent));
    QBroadcastCatcher::instanse()->handleFullIntent(sendName, iContent);
}

#ifdef __cplusplus
}
#endif



QBroadcastCatcher::QBroadcastCatcher()
{
    qRegisterMetaType<PairedStringList>("PairedStringList");
    QAndroidJniEnvironment env;
    jclass javaClass = env.findClass("una/unamd/qbroadcastcatcher/QBroadcastCatcher");
    QAndroidJniObject classObject(javaClass);
    classObject.callMethod<void>("registerDefaultServiceBroadcastReceiver","(Landroid/content/Context;)V",
                                 QtAndroid::androidContext().object());
}

void QBroadcastCatcher::registerBroadcastReceiver(QString &action, QString &target, QString &category)
{
    QAndroidJniEnvironment env;
    jclass javaClass = env.findClass("una/unamd/qbroadcastcatcher/QBroadcastCatcher");
    QAndroidJniObject classObject(javaClass);
    QAndroidJniObject joiact = QAndroidJniObject::fromString(action);
    QAndroidJniObject joicat = QAndroidJniObject::fromString(category);
    QAndroidJniObject joiext = QAndroidJniObject::fromString(target);
    jstring iact = joiact.object<jstring>();
    jstring icat = joicat.object<jstring>();
    jstring iext = joiext.object<jstring>();

    classObject.callMethod<void>("registerNewServiceBroadcastReceiver",
                                 "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
                                 QtAndroid::androidContext().object(),
                                 iact,
                                 icat,
                                 iext
                                 );
}

QBroadcastCatcher* QBroadcastCatcher::_instanse = Q_NULLPTR;
QBroadcastCatcher *QBroadcastCatcher::instanse()
{
    if (_instanse == Q_NULLPTR)
       _instanse = new QBroadcastCatcher();
    return _instanse;
}

void QBroadcastCatcher::listenForBroadcast(QString action, QString target, QString category)
{
    instanse()->registerBroadcastReceiver(action,target,category);
}

void QBroadcastCatcher::handleTargetedIntent(QString intentName, QString intentContents)
{
    emit intentCaptured(qHash(intentName), intentName,intentContents);
}

void QBroadcastCatcher::handleFullIntent(QString intentName, QString intentContents)
{
    QVector<QPair<QString, QString> > result;
    QStringList raw_pairs = intentContents.split(" __; ");
    for (QStringList::iterator raw_pair = raw_pairs.begin(); raw_pair != raw_pairs.end(); ++raw_pair)
    {
        QStringList extracted_pair = raw_pair->split(" _:_ ");
        if (extracted_pair.count() == 2)
        {
            result.push_back(QPair<QString,QString>(extracted_pair.at(0), extracted_pair.at(1)));
        }
    }
    emit intentExtracted(qHash(intentName), intentName, result);
}
#endif
