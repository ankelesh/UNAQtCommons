#include "PrinterWrapperFactory.h"
#include "FTRCOMPrinterWrapper.h"
#include "NullPrinterWrapper.h"
#include "AndroidBluetoothPrinterWrapper.h"
#include <settings/CommonAppSettings.h>
#include "widgets/utils/GlobalAppSettings.h"
AbsPrinterWrapper* PrinterWrapperFactory::fabricate(QObject* parent, QString encoding)
{
#ifdef FTR_COM
    return new FTRCOMPrinterWrapper(AppSettings->printerPortDesignation, AppSettings->printerPort, parent, encoding);
#endif
#ifdef Q_OS_ANDROID
    return new AndroidBluetoothPrinterWrapper(CommonSettings->labelPrinterName, 
		CommonSettings->lastPrinterBTMAC, CommonSettings->lastPrinterBTUUID, parent, encoding);
#endif

	return new NullPrinterWrapper("D:/print.txt", parent, encoding);
}
