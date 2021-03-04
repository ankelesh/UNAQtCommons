#include "DatetimeConstants.h"
#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromLatin1("" A "" , sizeof(A)-1)
#endif
const QString DATETIME_ENCODING_FORMAT = "dd.MM.yyyy hh.mm.ss";
const QString DATE_ENCODING_FORMAT(QStringLiteral("dd.MM.yyyy"));
const QString TIME_ENCODING_FORMAT(QStringLiteral("hh.mm.ss"));
