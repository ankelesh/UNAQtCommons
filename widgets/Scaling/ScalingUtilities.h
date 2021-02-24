#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/QApplication>
#include <QtWidgets/qdesktopwidget.h>
#define GEOMETRY_SOURCE qApp->screens().first()
#else
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#define GEOMETRY_SOURCE qApp->desktop()
#endif
#include <QtGui/QScreen>
#include <QString>

#define GENERAL_FONT *FontAdapter::general()
#define AppFonts FontAdapter::instanse()

//These functions are used to calculate size in percent of all window
inline int calculateAdaptiveHeight(double percent = 0.125)
//Calculates height for buttons, by default is giving 12.5% of screen height
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	percent /= 2;
#endif
#endif
	return int(GEOMETRY_SOURCE->availableGeometry().height() * percent);
}

inline int calculateAdaptiveWidth(double percent = 0.5)
//Calculates width, by default is giving 50% of screen width
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	return int(GEOMETRY_SOURCE->availableGeometry().height() * (percent * 0.66 * 0.66));
#endif
#endif
	return int(GEOMETRY_SOURCE->availableGeometry().width() * percent);
}
inline QSize calculateAdaptiveSize(double percent = 0.3)
//Calculates square size, by default is giving 30% of screen dimensions
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	percent /= 2;
#endif
#endif
	return QSize(
		int(GEOMETRY_SOURCE->availableGeometry().width() * percent),
		int(GEOMETRY_SOURCE->availableGeometry().height() * percent)
	);
}
inline QSize calculateAdaptiveSize(double Hpercent, double Wpercent)
//Calculates more adaptive size, allowing to scale both dimensions. No defaults.
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	Hpercent /= 2;
	Wpercent /= 4;
#endif
#endif
	return QSize(
		int(GEOMETRY_SOURCE->availableGeometry().width() * Wpercent),
		int(GEOMETRY_SOURCE->availableGeometry().height() * Hpercent)
	);
}

inline QSize imitatePhoneSize(double HPercent)
// sets window size in vertical projection using screen height
{
	return QSize(
		int(GEOMETRY_SOURCE->availableGeometry().height() * (HPercent * 0.66)),
		int(GEOMETRY_SOURCE->availableGeometry().height() * HPercent)
	);
}

// These functions are used to shorten line if it is too long
QString& normalizeLine(QString& line);
QString normalizeLine(const QString line);

class FontAdapter
	// creates scaled fonts
{
	int minheight;		//	minimum height of letter
	int maxheight;		//	maximum height of letter
	double minimumFontPercent;	// default font percent, which used to create original scaling
	int averageCharPerWidth;
	static FontAdapter* _instanse;
	static QFont* _generalFont;

	void _setCharPerWidth();
public:
	FontAdapter(int minheight, int maxheight, double minimumFontPercent);
	void reset(int mh, int Mh, double mfp);
	int howMuchCharacterFitsIntoScreen();

	static FontAdapter* instanse();
	static const QFont* general();
	// creates scaled font by using adapter. Extra percents are relational to calculated minheight
	static QFont makeFont(double extrapercents);
	static QFont makeCustomFont(double onlyPercents);
	QFont makeIndependentFont(int min, int max, int perc);
	static QString breakStringToFitScreen(QString src, const QFontMetrics& metrics, double percent = 1.0, int* breaks = Q_NULLPTR);
};
