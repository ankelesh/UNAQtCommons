#include "SwitchableIMDecorator.h"
#include <QApplication>
void SwitchableIMDecorator::_setupIMH()
{
    if (target == Q_NULLPTR)
        return;
    if (reopen)
        qApp->inputMethod()->hide();
    target->setInputMethodHints(hintArray.at(currentIndex));
    target->setFocus();
    if (reopen)
        qApp->inputMethod()->show();
}

SwitchableIMDecorator::SwitchableIMDecorator(QVector<Qt::InputMethodHints> hints, bool autoReopen, QLineEdit *parent)
    : QObject(parent), hintArray(hints), currentIndex(0), reopen(autoReopen), target(parent)
{
    if (hintArray.isEmpty())
    {
        hintArray.push_back(Qt::InputMethodHint::ImhNone);
    }
}

SwitchableIMDecorator::SwitchableIMDecorator(QLineEdit *parent)
    : QObject(parent), hintArray(), currentIndex(0), reopen(true), target(parent)
{
    hintArray.push_back(Qt::InputMethodHint::ImhNone);
}

void SwitchableIMDecorator::initiate(QVector<Qt::InputMethodHints> hints, bool autoReopen)
{
    hintArray = hints;
    reopen = autoReopen;
    if (target != Q_NULLPTR)
        target->setInputMethodHints(hints.first());
}

void SwitchableIMDecorator::nextIM()
{
    if (hintArray.count() > 1)
    {
        ++currentIndex;
        if (currentIndex == hintArray.count())
            currentIndex = 0;
        _setupIMH();
    }
}

void SwitchableIMDecorator::prevIM()
{
    if (hintArray.count() > 1)
    {
        --currentIndex;
        if (currentIndex < 0)
            currentIndex = hintArray.count() - 1;
        _setupIMH();
    }
}

void SwitchableIMDecorator::setIM(int index)
{
    if (index >= 0 && index < hintArray.count())
    {
        currentIndex = index;
        _setupIMH();
    }
}

void SwitchableIMDecorator::dropIM()
{
    currentIndex = 0;
    _setupIMH();
}
