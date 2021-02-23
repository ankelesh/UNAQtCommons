#pragma once
#include "abs_control.h"

/*
	This widget provides numeric Spinbox wrapped in abs_control interface.
	You can provide isInt parameter to define which precision should be used
	in spinbox.  Do not forget
	to provide control name in original notation - you should not translate it.

*/
class BigButtonsSpinbox;
class QuantityControl :public abs_control
{
protected:
	BigButtonsSpinbox* innerSpinbox;

	// Inherited via abs_control
	virtual void _setEnumerableMinimum(double) override;
	virtual void _setEnumerableValue(double) override;
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;
	virtual QWidget* getInnerWidget() const override;
	virtual double _getEnumerableValue() const override;
public:
    explicit QuantityControl(bool isInt, const QString& cname, QWidget* parent);
	explicit QuantityControl(int precision, const QString& cname, QWidget* parent);
	virtual void show() override;
	virtual void hide() override;
	virtual void setFocus() const override;
	void setMinimum(double min);
	void  setMaximum(double max);
	double getPureValue()const;
	void renameControl(QString newName);
    virtual ~QuantityControl()
#ifdef QT_VERSION5X
    override
#endif
    ;
};
