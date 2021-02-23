#include "abs_control.h"
#include "ControlTranslator.h"
//#define DEBUG
#ifdef DEBUG
#include <debug/debugtrace.h>
#endif
#include <QWidget>

double abs_control::_getEnumerableValue() const
{
	return 0.0;
}

void abs_control::_setEnumerableValue(double)
{
}

void abs_control::_setEnumerableMinimum(double) 
{
}

abs_control::abs_control(QWidget* parent, ControlTypes Type, QString cname)
    : QObject(parent), type(Type), parent(parent), label(makeControlTranslation(cname)),name(cname)
{
#ifdef DEBUG
	detrace_DCONSTR("control-like of type " << type);
#endif
}

QString abs_control::getValue() const
{
	return prepareAndReturnValue();
}

QWidget* abs_control::myWidget() const
{
	return getInnerWidget();
}

abs_control::ControlTypes abs_control::myType()
{
	return type;
}

double abs_control::getEnumerableValue() const
{
	return _getEnumerableValue();
}

bool abs_control::setValue(QString val)
{
	return parseAndSetValue(val);
}

void abs_control::reset()
{
	clear();
}

abs_control::~abs_control()
{
}

bool abs_control::canGiveValue() const
{
	return valueAvailable();
}

void abs_control::connectAsNext(abs_control* ctrl)
{
	if (ctrl != Q_NULLPTR)
	{
#ifdef QT_VERSION5X
		QObject::connect(ctrl, &abs_control::editingFinished, this, &abs_control::setFocus);
#else
		QObject::connect(ctrl, SIGNAL(editingFinished()), this, SLOT(setFocus()));
#endif
	}

}

void abs_control::setEnumerableMinimum(double val)
{
	_setEnumerableMinimum(val);
}

void abs_control::setEnumerableValue(double v)
{
	_setEnumerableValue(v);
}

void abs_control::subcontrolEditingFinished()
{
	if (valueAvailable())
	{
		emit editingFinished();
		emit valueReceived(prepareAndReturnValue());
	}
	else
	{
		emit editingFinished();
	}
}


QString chopZeros(QString val, bool cdot)
// removes .0000 from end of the line to avoid cursor-to-end behaviour
{
	val.replace(',', '.');
	if (cdot)
	{
		while (val.endsWith('0'))
		{
			val.chop(1);
		}
	}
	else
	{
		while (val.endsWith('0') || val.endsWith('.'))
		{
			val.chop(1);
		}
	}
	return val;
}
