#include "FTRCOMPrinterWrapper.h"
#ifdef FTR_COM
#include "tty/tty.h"
#endif
#ifdef DEBUG
#include <debug/debugtrace.h>
#endif
#include <QTimer>
bool FTRCOMPrinterWrapper::_isValid() const
{
#ifdef FTR_COM
	return true;
#else
	return false;
#endif
}

bool FTRCOMPrinterWrapper::_isReady() const
{
#ifdef FTR_COM
	return printerSocket->isConnected();
#else
	return false;
#endif
}

void FTRCOMPrinterWrapper::_retryOpeningConnection()
{
#ifdef FTR_COM
	printerSocket->connect(socket, baudRate);
	if (printerSocket->isConnected())
		emit connected();
	else
	{
        errorOutput += printerSocket->getErrors();emit error(errorOutput);
	}
#endif
}

QString FTRCOMPrinterWrapper::_errors() const
{
	return errorOutput;
}


#ifdef FTR_COM
void FTRCOMPrinterWrapper::_print(QString data)
{
	if (!printerSocket->isConnected())
	{
		_retryOpeningConnection();
	}
	if (!printerSocket->isConnected())
	{
#ifdef DEBUG
		detrace_METHEXPL("fail: " << printerSocket->getErrors());
#endif
		return;
	}
	if (!printerSocket->write(encoder.fromUnicode(data)))
	{
		errorOutput = tr("printing failed: ") + printerSocket->getErrors();
		emit error(errorOutput);
#ifdef DEBUG
		detrace_METHEXPL("fail: " << printerSocket->getErrors());
#endif
	}
#else
void FTRCOMPrinterWrapper::_print(QString /*data*/)
{
#endif
}

FTRCOMPrinterWrapper::FTRCOMPrinterWrapper(QString /*port*/, int brate, QObject* parent, QString enc)
	:AbsPrinterWrapper(CEComport, parent, enc), 
#ifdef FTR_COM
	printerSocket(new TTY()),
#endif
	baudRate(brate)
{

}

FTRCOMPrinterWrapper::~FTRCOMPrinterWrapper()
{
#ifdef FTR_COM
	printerSocket->disconnect();
	delete printerSocket;
#endif
}


void FTRCOMPrinterWrapper::_establishConnection()
{
#ifdef FTR_COM
	printerSocket->connect(socket, baudRate);
	if (printerSocket->isConnected())
		emit connected();
	else
	{
		errorOutput += printerSocket->getErrors();
		emit error(errorOutput);
	}
#endif
}
