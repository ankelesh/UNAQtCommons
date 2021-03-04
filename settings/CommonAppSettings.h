#pragma once
#include <QtCore/QString>
#include <QtCore/QTranslator>
#include <QByteArray>
#ifdef QT_VERSION5X
#include <QException>
#else
#include <exception>
#endif
#include <QSharedPointer>
#define CommonSettings CommonAppSettings::instanse()
extern QString rootFilePath;
class QSettings;
class SettingsInstanceException :
        public
        #ifdef QT_VERSION5X
        QException
        #else
        std::exception
        #endif
{
public:
    void raise() const
#ifdef QT_VERSION5X
    override
#endif
    { throw* this; }
    virtual const char * what() const
#ifndef QT_VERSION5X
    override
#endif
    {return "no settings object provided";}
    SettingsInstanceException* clone() const
#ifdef QT_VERSION5X
    override
#endif
    { return new SettingsInstanceException(*this); }
};

typedef QSharedPointer<QSettings> p_QSettings;

class CommonAppSettings	//	Holds main settings for data exchange. DO NOT MAKE MULTIPLE
{
private:
	static CommonAppSettings* _instanse;
	QByteArray netEncoding;
protected:
	QString ru_trans_path;
	QString ro_trans_path;
	QString en_trans_path;
	virtual p_QSettings _getSettingsFile();
	virtual void _save(p_QSettings);
	virtual void _load(p_QSettings);
	explicit CommonAppSettings(QString ru, QString ro, QString en);		//	Explicit default constructor. All data is obtained automatically via file
public:
	int scanPrefix;
	int scanSuffix;
	QString language;					//	language of the application
	QTranslator qt_translator;			//	global translator is stored here
	int	fontMinHeight;
	int fontMaxHeight;
	double fontPercent;

	// label printing
	QString labelPrinterName;
	QString lastPrinterBTMAC;
	QString lastPrinterBTUUID;

	unsigned int notificationsVolume;
	// network timeouts
	int timeoutInt;

	void setTranslator();				//	Sets translator. Can be used any time
	void save();						//	Forse save
	void load();
	void setAsCommon();
	const QByteArray& getNetworkEncoding();
	void setNetworkEncoding(const QString&);
	static CommonAppSettings* instanse();
};
