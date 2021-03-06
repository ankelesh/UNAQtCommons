#pragma once
#include <QString>
#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromLatin1(""A"",sizeof(A)-1)
#endif
#include "AbsPrinterWrapper.h"

class QObject;
namespace PrinterWrapperFactory
{
	AbsPrinterWrapper* fabricate(QObject* parent, QString encoding = QStringLiteral("CP1251"));
}
