#pragma once
#include <QLineEdit>
#include <QInputMethod>
#include <QVector>


class SwitchableIMDecorator : public QObject
{
    Q_OBJECT
protected:
    QVector<Qt::InputMethodHints> hintArray;
    int currentIndex;
    bool reopen;
    QLineEdit* target;
    void _setupIMH();
public:
    explicit SwitchableIMDecorator(QVector<Qt::InputMethodHints> hints, bool autoReopen, QLineEdit* parent);
    explicit SwitchableIMDecorator(QLineEdit* parent);
    void initiate(QVector<Qt::InputMethodHints> hints, bool autoReopen);
public slots:
    void nextIM();
    void prevIM();
    void setIM(int index);
    void dropIM();
};
