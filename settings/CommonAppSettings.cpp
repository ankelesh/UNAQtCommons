#include "CommonAppSettings.h"
#include <QApplication>
#include <QSettings>
#include <debug/debugtrace.h>
#include <QDir>
#include <QException>
QString rootFilePath =
#ifdef Q_OS_ANDROID
"/storage/emulated/0/";
#else
#ifdef Q_OS_WINCE
"/Documents/";
#else
QDir::currentPath();
#endif
#endif


p_QSettings CommonAppSettings::_getSettingsFile()
{
#ifdef DEBUG
	detrace_METHPERROR("CommonAppSettings::_getSettingsFile", "called non-overloaded call to settings file");
#endif
	return Q_NULLPTR;
}

void CommonAppSettings::_save(p_QSettings settings)
{
	if (settings.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("CommonAppSettings::_save", "NO SETTINGS OBJECT PROVIDED TO COMMON SETTINGS");
#endif
		return;
	}
#ifdef DEBUG
	detrace_METHCALL("dump common settings");
	detrace_METHEXPL("if errors: " << settings->status());
#endif
	settings->beginGroup(QStringLiteral("common"));
	settings->setValue("language", language);
	settings->setValue("scanPrefix", scanPrefix);
	settings->setValue("scanSuffix", scanSuffix);
	settings->setValue("fontMaxHeight", fontMaxHeight);
	settings->setValue("fontMinHeight", fontMinHeight);
	settings->setValue("fontPercent", fontPercent);
	settings->setValue("netEncoding", netEncoding);
	settings->endGroup();
	settings->beginGroup("label_printer");
	settings->setValue("labelPrinterName", labelPrinterName);
	settings->setValue("lastPrinterBTMAC", lastPrinterBTMAC);
	settings->setValue("lastPrinterBTUUID", lastPrinterBTUUID);
	settings->endGroup();
	settings->beginGroup("sound");
	settings->setValue("notificationsVolume", notificationsVolume);
	settings->endGroup();
	settings->beginGroup("network");
	settings->setValue("timeoutInt", timeoutInt);
	settings->endGroup();
}

void CommonAppSettings::_load(p_QSettings settings)
{
	if (settings.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("CommonAppSettings::_load", "NO SETTINGS OBJECT PROVIDED TO COMMON SETTINGS");
#endif
		return;
	}
#ifdef DEBUG
	detrace_METHCALL("load common part of AppSettings");
	detrace_METHEXPL("if errors: " << settings->status());
#endif
	settings->beginGroup(QStringLiteral("common"));
	language = settings->value("language", "English").toString();
	scanPrefix = settings->value("scanPrefix", QVariant(int('$'))).toInt();
	scanSuffix = settings->value("scanSuffix", QVariant(int('\n'))).toInt();
	fontMaxHeight = settings->value("fontMaxHeight", QVariant(30)).toInt();
	fontMinHeight = settings->value("fontMinHeight", QVariant(10)).toInt();
	fontPercent = settings->value("fontPercent", QVariant(0.03)).toDouble();
	netEncoding = settings->value("netEncoding", QVariant("CP1251")).toByteArray();
	settings->endGroup();
	settings->beginGroup("label_printer");
	labelPrinterName = settings->value("labelPrinterName", QVariant("zebra")).toString();
	lastPrinterBTMAC = settings->value("lastPrinterBTMAC", QVariant()).toString();
	lastPrinterBTUUID = settings->value("lastPrinterBTUUID", QVariant()).toString();
	settings->endGroup();
	settings->beginGroup("sound");
	notificationsVolume = settings->value("notificationsVolume", QVariant(100U)).toUInt();
	settings->endGroup();
	settings->beginGroup("network");
	settings->setValue("timeoutInt", timeoutInt);
	settings->endGroup();
	setTranslator();
}


CommonAppSettings::CommonAppSettings(QString ru, QString ro, QString en) :
	netEncoding(), 
	ru_trans_path(ru),
	ro_trans_path(ro),
	en_trans_path(en),
	scanPrefix(52), scanSuffix(10), language(),
	qt_translator(), fontMinHeight(10), fontMaxHeight(20),
	fontPercent(0.01), labelPrinterName(), lastPrinterBTMAC(), lastPrinterBTUUID(),
	notificationsVolume(), timeoutInt(10000)
{
}

void CommonAppSettings::setTranslator()
{
	if (language == "Russian")
	{
		qt_translator.load(ru_trans_path, ".");
	}
	else if (language == "Romanian")
	{
		qt_translator.load(ro_trans_path, ".");
	}
	else
	{
		qt_translator.load(en_trans_path, ".");
	}
	qApp->installTranslator(&qt_translator);
}

void CommonAppSettings::save()
{
	_save(_getSettingsFile());
}

void CommonAppSettings::load()
{
	_load(_getSettingsFile());
}



void CommonAppSettings::setAsCommon()
{
	_instanse = this;
}

const QByteArray& CommonAppSettings::getNetworkEncoding()
{
	return netEncoding;
}

void CommonAppSettings::setNetworkEncoding(const QString& enc)
{
	netEncoding = enc.toLocal8Bit();
}
CommonAppSettings* CommonAppSettings::_instanse;
CommonAppSettings* CommonAppSettings::instanse()
{
	if (_instanse == Q_NULLPTR)
		throw SettingsInstanceException();
	return _instanse;
}

