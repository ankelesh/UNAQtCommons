#include "submodules/UNAQtCommons/widgets/ControlsWidgets/QuantityControl.h"
#include "widgets/utils/ElementsStyles.h"
//#define DEBUG
#ifdef Q_OS_ANDROID
#include <QInputMethod>
#endif
#ifdef DEBUG
#include <debug/debugtrace.h>
#endif
#include <widgets/UtilityElements/BigButtonsSpinbox.h>

void QuantityControl::_setEnumerableMinimum(double val) 
{
	innerSpinbox->setMinimum(val);

}
void QuantityControl::_setEnumerableValue(double v)
{
	innerSpinbox->setDValue(v);
}
QString QuantityControl::prepareAndReturnValue() const
{
    return QString::number(innerSpinbox->dvalue());
}

bool QuantityControl::parseAndSetValue(QString str)
{
	bool ok;
	double toSet = str.toDouble(&ok);
	if (ok)
	{
		innerSpinbox->setDValue(toSet);
		return true;
	}
	return false;
}

void QuantityControl::clear()
{
	innerSpinbox->setValue(0);
	innerSpinbox->update();
}

bool QuantityControl::valueAvailable() const
{
	return innerSpinbox->dvalue() > 0;
}


QuantityControl::QuantityControl(bool isInt, const QString& cname, QWidget* Parent)
    :
    abs_control(Parent, ((isInt)?    Int : Float), cname),
	innerSpinbox(new BigButtonsSpinbox(
	((isInt)? BigButtonsSpinbox::intspin : BigButtonsSpinbox::floatspin), parent))
{
	innerSpinbox->setMinimum(0);
	innerSpinbox->setMaximum(10000);
	innerSpinbox->setValue(0);
	innerSpinbox->setInfo(label);
	innerSpinbox->setStyleSheet(FOCUSED_SPINBOX_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(innerSpinbox, &BigButtonsSpinbox::returnPressed, this, &QuantityControl::subcontrolEditingFinished);
#else
	QObject::connect(innerSpinbox, SIGNAL(returnPressed()), this, SLOT(subcontrolEditingFinished()));
#endif
	hide();
}

QuantityControl::QuantityControl(int precision, const QString& cname, QWidget* parent)
	: abs_control(parent, Float, cname),
	innerSpinbox(new BigButtonsSpinbox(
		BigButtonsSpinbox::floatspin, parent))
{
	innerSpinbox->setMinimum(0);
	innerSpinbox->setMaximum(10000);
	innerSpinbox->setPrecision(precision);
	innerSpinbox->setValue(0);
	innerSpinbox->setInfo(label);
	innerSpinbox->setStyleSheet(FOCUSED_SPINBOX_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(innerSpinbox, &BigButtonsSpinbox::returnPressed, this, &QuantityControl::subcontrolEditingFinished);
#else
	QObject::connect(innerSpinbox, SIGNAL(returnPressed()), this, SLOT(subcontrolEditingFinished()));
#endif
	hide();
}



QuantityControl::~QuantityControl()
{
	innerSpinbox->hide();
	innerSpinbox->blockSignals(true);
	innerSpinbox->deleteLater();
}

QWidget* QuantityControl::getInnerWidget() const
{
	return innerSpinbox;
}

double QuantityControl::_getEnumerableValue() const
{
	return innerSpinbox->dvalue();
}

void QuantityControl::setFocus() const
{
	innerSpinbox->setFocus();
	innerSpinbox->selectAll();
#ifdef Q_OS_ANDROID
    qApp->inputMethod()->show();
#endif
}

void QuantityControl::setMinimum(double min)
{
	innerSpinbox->setMinimum(min);
}

void QuantityControl::setMaximum(double max)
{
	innerSpinbox->setMaximum(max);
}

double QuantityControl::getPureValue()const
{
	return innerSpinbox->dvalue();
}

void QuantityControl::renameControl(QString newName)
{
	label = newName;
	innerSpinbox->setInfo(label);
}

void QuantityControl::show()
{
	innerSpinbox->show();
}

void QuantityControl::hide()
{
	innerSpinbox->hide();
}

