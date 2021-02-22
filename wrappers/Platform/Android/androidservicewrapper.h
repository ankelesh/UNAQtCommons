#pragma once
#ifndef ANDROIDSERVICEWRAPPER_H
#define ANDROIDSERVICEWRAPPER_H

#include <QObject>
#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#endif
class AndroidServiceWrapper : public QObject
{
    Q_OBJECT
protected:
    bool available;
#ifdef Q_OS_ANDROID
    QAndroidJniEnvironment env;
#endif
    AndroidServiceWrapper();
    static AndroidServiceWrapper * _instance;
#ifdef Q_OS_ANDROID
    static QAndroidJniObject _castStringListToArray(QStringList& l);
#endif
public:
    static AndroidServiceWrapper& instance();
    bool isAvailable() const {return available;};
    void sendEmailIntent(QString subj, QString text, QStringList emails);
    void sendMessageIntent(QString text);
};

#endif // ANDROIDSERVICEWRAPPER_H
