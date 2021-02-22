#pragma once
#include <QObject>
#ifdef Q_OS_ANDROID
#include <QString>
#include <QHash>
#include <QVector>
#include <QMetaType>
#include <QPair>


typedef QVector<QPair<QString, QString> > PairedStringList;
class QBroadcastCatcher : public QObject
{
    Q_OBJECT
private:
    QBroadcastCatcher();
    static QBroadcastCatcher* _instanse;
    void registerBroadcastReceiver(QString & action, QString& target, QString& category);
public:
    static QBroadcastCatcher* instanse();
    static void listenForBroadcast(QString action, QString target = QString(), QString category = QString());
    void handleTargetedIntent(QString intentName, QString intentContents);
    void handleFullIntent(QString intentName, QString intentContents);
signals:
    void intentCaptured(uint ihash, QString intentName, QString intentContents);
    void intentExtracted(uint ihash, QString intentName, PairedStringList intentContents);

};
#endif
