#include "ScalingUtilities.h"
#include "widgets/utils/GlobalAppSettings.h"
#include <cmath>

QString& normalizeLine(QString& line)
{
#ifdef Q_OS_WINCE
	int inserts = line.length() / 22;
	if (inserts <= 0)
		return line;
	for (int i = 1; i <= inserts; ++i)
	{
		line.insert(i * 24 + i - 1, "\n");
	}
#endif
	return line;
}
QString normalizeLine(const QString line)
{
	QString nline = line;
#ifdef Q_OS_WINCE
	int inserts = nline.length() / 22;
	if (inserts <= 0)
		return nline;
	for (int i = 1; i <= inserts; ++i)
	{
		nline.insert(i * 24 + i - 1, "\n");
	}
#endif
	return nline;
}

const QFont& makeFont(double /*perc*/)
{
	return *(FontAdapter::general());
}

void FontAdapter::_setCharPerWidth()
{
	QFontMetrics fm(*general());
	int width = calculateAdaptiveWidth(0.95);
	if (fm.averageCharWidth() == 0 || width == 0)
		averageCharPerWidth = 10;
	averageCharPerWidth = int(width / fm.averageCharWidth());
}

FontAdapter::FontAdapter(int mh, int mah, double mfp)
	: minheight(mh), maxheight(mah), minimumFontPercent(mfp)
{
#ifdef  Q_OS_WIN
	minimumFontPercent = mfp * 0.6;
#endif //  Q_OS_WIN
}
void FontAdapter::reset(int mh, int Mh, double mfp)
{
	minheight = mh;
	maxheight = Mh;
	minimumFontPercent = mfp;
	*_generalFont = QFont(makeFont(1.0));
}
int FontAdapter::howMuchCharacterFitsIntoScreen()
{
	return averageCharPerWidth;
}
FontAdapter* FontAdapter::_instanse = Q_NULLPTR;
QFont* FontAdapter::_generalFont = Q_NULLPTR;
FontAdapter* FontAdapter::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new FontAdapter(AppSettings->fontMinHeight, AppSettings->fontMaxHeight,
			AppSettings->fontPercent);
	}
	return _instanse;
}

const QFont* FontAdapter::general()
{
	if (_generalFont == Q_NULLPTR)
	{
		_generalFont = new QFont(FontAdapter::makeFont(1.0));
		FontAdapter::instanse()->_setCharPerWidth();
	}
	return _generalFont;
}

QFont FontAdapter::makeFont(double extrapercents)
{
	double currentHeight = GEOMETRY_SOURCE->availableGeometry().height();
	currentHeight *= FontAdapter::instanse()->minimumFontPercent;
	currentHeight *= extrapercents;
	if (currentHeight < _instanse->minheight)
		currentHeight = _instanse->minheight;
	else
		if (currentHeight > _instanse->maxheight)
			currentHeight = _instanse->maxheight;
	return QFont("Times new Roman", int(currentHeight));
}
QFont FontAdapter::makeCustomFont(double onlyPercents)
{
	double currentHeight = GEOMETRY_SOURCE->availableGeometry().height();
	currentHeight *= onlyPercents;
	if (currentHeight < _instanse->minheight)
		currentHeight = _instanse->minheight;
	else
		if (currentHeight > _instanse->maxheight)
			currentHeight = _instanse->maxheight;
	return QFont("Times new Roman", int(currentHeight));
}
QFont FontAdapter::makeIndependentFont(int min, int max, int perc)
{
	double currentHeight = GEOMETRY_SOURCE->availableGeometry().height();
	currentHeight *= minimumFontPercent;
	if (currentHeight < minheight)
		currentHeight = minheight;
	else
		if (currentHeight > maxheight)
			currentHeight = maxheight;
	return QFont("Times new Roman", int(currentHeight));
}
QString FontAdapter::breakStringToFitScreen(QString src, const QFontMetrics& metrics, double percent, int* breaks)
{
	double totalWidthAllowed = double(calculateAdaptiveWidth(percent)) - metrics.averageCharWidth();
	double tobreak = double(src.count()) * metrics.averageCharWidth() / totalWidthAllowed;
	if (tobreak <= 1.0)
	{
		if (breaks != Q_NULLPTR)
			*breaks = int(std::ceil(tobreak));
		return src;
	}
	QString out;
	out.reserve(src.count() + tobreak + 1);
	int wsum = 0;
	QString::iterator pos = src.begin();
	while (pos != src.end())
	{
		while (wsum < totalWidthAllowed && pos != src.end())
		{
			out += *pos;
			wsum += metrics.width(*pos);
			++pos;

		}
		wsum = 0;
		out += "\n";
	}
	if (breaks != Q_NULLPTR)
	{
		*breaks = int(std::ceil(tobreak));
	}
	return out;
}