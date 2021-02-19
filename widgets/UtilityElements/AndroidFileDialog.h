#pragma once
#include <QString>
#include <QObject>
#ifdef Q_OS_ANDROID
#include <QDialog>
#include <QStyledItemDelegate>

// hi from commons
class QKeyEvent;
class ExtraSizeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    ExtraSizeDelegate(QObject* parent) : QStyledItemDelegate(parent){};
};

class QFileSystemModel;
class QTreeView;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class AndroidFileDialog : public QDialog
{
    Q_OBJECT
public:
    enum DialogTargets{File, Dir, FileAndDir};
protected:
    QVBoxLayout * mainLayout;
    QLabel* title;
    QHBoxLayout* pathLayout;
    QLineEdit* pathField;
    QPushButton* newDirButton;
    QTreeView* mainView;
    QFileSystemModel* model;
    QHBoxLayout* buttonLayout;
    QPushButton* backButton;
    QPushButton* okButton;
    int dialogTarget;
    void keyPressEvent(QKeyEvent* kev) override;
    void keyReleaseEvent(QKeyEvent* kev) override;
public:
    AndroidFileDialog(QString path, int target, QWidget* parent);
    static QString getDirName(QString default_path);
    static QString getFileName(QString default_path);
    QString getResult();
protected slots:
    void pathChanged(QString new_path);
    void directoryOpened(const QString& path);
    void onTreeClick(const QModelIndex& index);
    void onNewDir();
};
#else
class AndroidFileDialog : public QObject
{
    Q_OBJECT
public:
    static QString getDirName(QString default_path);
    static QString getFileName(QString default_path);
};
#endif
