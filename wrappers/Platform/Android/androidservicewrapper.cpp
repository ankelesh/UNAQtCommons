#include "androidservicewrapper.h"
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniExceptionCleaner>
#include <QtAndroidExtras/QAndroidIntent>
#include <QtAndroid>
#include <QVariant>
#endif
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.h"
AndroidServiceWrapper::AndroidServiceWrapper()
{
#ifdef Q_OS_ANDROID
    available = true;
#else
    available = false;
#endif
}
#ifdef Q_OS_ANDROID
QAndroidJniObject AndroidServiceWrapper::_castStringListToArray(QStringList &l)
{
    jobjectArray adressesarray = instance().env->NewObjectArray(l.size(), instance().env.findClass("java/lang/String"), NULL);
    QAndroidJniObject converted;
    for (int i = 0; i < l.count(); ++i)
    {
        converted = QAndroidJniObject::fromString(l.at(i));
        instance().env->SetObjectArrayElement(adressesarray, i, converted.object());
    }
    return QAndroidJniObject::fromLocalRef(adressesarray);

}
#endif

AndroidServiceWrapper &AndroidServiceWrapper::instance()
{
    if (_instance == Q_NULLPTR)
    {
        _instance = new AndroidServiceWrapper();
    }
    return *_instance;
}
AndroidServiceWrapper * AndroidServiceWrapper::_instance = Q_NULLPTR;
void AndroidServiceWrapper::sendEmailIntent(QString subj, QString txt, QStringList emails)
{
    if (!available)
        return;
#ifdef Q_OS_ANDROID
     QAndroidIntent intent("android.intent.action.SEND");
     QAndroidJniObject type = QAndroidJniObject::fromString("message/rfc822");
     QAndroidJniObject JSubj = QAndroidJniObject::fromString(subj);
     QAndroidJniObject eSubj = QAndroidJniObject::fromString("android.intent.extra.SUBJECT");
     QAndroidJniObject JTxt = QAndroidJniObject::fromString(txt);
     QAndroidJniObject eTxt = QAndroidJniObject::fromString("android.intent.extra.TEXT");
     QAndroidJniObject eEmls = QAndroidJniObject::fromString("android.intent.extra.EMAIL");
     QAndroidJniObject JEmls = _castStringListToArray(emails);
     intent.handle().callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", type.object<jstring>());
     intent.handle().callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                                      eSubj.object<jstring>(), JSubj.object<jstring>());
     intent.handle().callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                                      eTxt.object<jstring>(), JTxt.object<jstring>());
     intent.handle().callObjectMethod("putExtra", "(Ljava/lang/String;[Ljava/lang/String;)Landroid/content/Intent;",
                                      eEmls.object<jstring>(), JEmls.object());

     QtAndroid::startActivity(intent.handle(), 10101);
     if (env->ExceptionCheck())
     {
         env->ExceptionClear();
         QAndroidJniExceptionCleaner jec(QAndroidJniExceptionCleaner::OutputMode::Silent);
         jec.clean();
     }


#endif
}

void AndroidServiceWrapper::sendMessageIntent(QString text)
{
    if (!available)
        return;
#ifdef Q_OS_ANDROID
     QAndroidIntent intent("android.intent.action.SEND");
     QAndroidJniObject type = QAndroidJniObject::fromString("text/plain");
     QAndroidJniObject JTxt = QAndroidJniObject::fromString(text);
     QAndroidJniObject eTxt = QAndroidJniObject::fromString("android.intent.extra.TEXT");
     intent.handle().callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", type.object<jstring>());
     intent.handle().callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                                      eTxt.object<jstring>(), JTxt.object<jstring>());
     QtAndroid::startActivity(intent.handle(), 202020);
     if (env->ExceptionCheck())
     {
         env->ExceptionClear();
         QAndroidJniExceptionCleaner jec(QAndroidJniExceptionCleaner::OutputMode::Silent);
         jec.clean();
     }

#endif
}
