#include "AndroidFileDialog.h"

#ifdef Q_OS_ANDROID
#include <QScroller>
#include <QInputDialog>
#include <QFileSystemModel>
#include <QTreeView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>

void AndroidFileDialog::pathChanged(QString new_path)
{
    mainView->setCurrentIndex(model->index(new_path));
    model->setRootPath(new_path);
    mainView->setExpanded(model->index(new_path),true);
}

void AndroidFileDialog::directoryOpened(const QString &path)
{
    pathField->setText(path);
}

void AndroidFileDialog::onTreeClick(const QModelIndex &index)
{
    directoryOpened(model->filePath(index));
}

void AndroidFileDialog::onNewDir()
{
    QString newFName = QInputDialog::getText(
                this,
                tr("Enter new directory name"),
                tr("Enter new directory name"),
                QLineEdit::Normal,
                "new_folder"
                );
    QModelIndex new_dir =  model->mkdir(mainView->currentIndex(), newFName);
    if (new_dir.isValid())
    {
         onTreeClick(new_dir);
         mainView->setCurrentIndex(new_dir);
         mainView->setExpanded(new_dir,true);
    }
}

void AndroidFileDialog::keyPressEvent(QKeyEvent *kev)
{
    QWidget::keyPressEvent(kev);
}

void AndroidFileDialog::keyReleaseEvent(QKeyEvent *kev)
{
    QWidget::keyReleaseEvent(kev);
}

AndroidFileDialog::AndroidFileDialog(QString path, int target, QWidget *parent)
    : QDialog(parent), mainLayout(new QVBoxLayout(this)),
      title(new QLabel(this)),
      pathLayout(new QHBoxLayout(this)),
      pathField(new QLineEdit(this)),
      newDirButton(new QPushButton(tr("new"),this)),
      mainView(new QTreeView(this)),
      model(new QFileSystemModel(this)),
      buttonLayout(new QHBoxLayout(this)),
      backButton(new QPushButton(tr("BACK"), this)),
      okButton(new QPushButton(tr("OK"),this)),
      dialogTarget(target)
{
    mainLayout->addWidget(title);
    mainLayout->addLayout(pathLayout);
    pathLayout->addWidget(pathField);
    pathLayout->addWidget(newDirButton);
    mainLayout->addWidget(mainView);
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(okButton);
    mainView->setModel(model);
    title->setAlignment(Qt::AlignCenter);
    switch (dialogTarget)
    {
    default:
    case FileAndDir:
        title->setText(tr("Select file or directory"));
        break;
    case Dir:
        model->setFilter(QDir::Filter::Dirs | QDir::NoDotAndDotDot);
        title->setText(tr("Select directory"));
        break;
    case File:
        title->setText(tr("Select file"));
        newDirButton->hide();
        break;
    }
    pathField->setText(path);
    mainView->setCurrentIndex(model->index(path));
    model->setRootPath(path);
    mainView->setExpanded(model->index(path),true);
    backButton->setDefault(false);
    okButton->setDefault(false);
    setMinimumSize(QSize(900,900));
    mainView->hideColumn(1);
    mainView->hideColumn(2);
    mainView->hideColumn(3);
    mainView->setItemDelegate(new ExtraSizeDelegate(this));
    mainView->setVerticalScrollMode(QTreeView::ScrollPerPixel);
    QScroller::grabGesture(mainView, QScroller::LeftMouseButtonGesture);
    QObject::connect(pathField, &QLineEdit::textEdited, this, &AndroidFileDialog::pathChanged);
    QObject::connect(model, &QFileSystemModel::rootPathChanged, this, &AndroidFileDialog::directoryOpened);
    QObject::connect(mainView, &QTreeView::clicked, this, &AndroidFileDialog::onTreeClick);
    QObject::connect(backButton, &QPushButton::clicked, this, &AndroidFileDialog::reject);
    QObject::connect(okButton, &QPushButton::clicked, this, &AndroidFileDialog::accept);
    QObject::connect(newDirButton,&QPushButton::clicked, this, &AndroidFileDialog::onNewDir);
}
QString AndroidFileDialog::getDirName(QString default_path)
{
    AndroidFileDialog* afd = new AndroidFileDialog(default_path, AndroidFileDialog::Dir, Q_NULLPTR);
    QString path;
    if (afd->exec())
    {
         path = afd->getResult();
    }
    afd->deleteLater();
    return path;
}

QString AndroidFileDialog::getFileName(QString default_path)
{
    AndroidFileDialog* afd = new AndroidFileDialog(default_path, AndroidFileDialog::File, Q_NULLPTR);
    QString path;
    if (afd->exec())
    {
        path = afd->getResult();
    }
    afd->deleteLater();
    return path;
}

QString AndroidFileDialog::getResult()
{
    QModelIndex index = mainView->currentIndex();
    if (!index.isValid())
        return QString();
    switch (dialogTarget)
    {
        default:
        case FileAndDir:
            return model->filePath(index);
        case File:
            if (model->isDir(index))
                return QString();
            else
                return model->filePath(index);
        case Dir:
            if (model->isDir(index))
                return model->filePath(index);
            else
                return QString();
    }
}

void ExtraSizeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
       QStyledItemDelegate::paint(painter, option, index);
}


QSize ExtraSizeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.rheight() *=2;
    return size;

}
#else
#include <QFileDialog>

QString AndroidFileDialog::getDirName(QString default_path)
{
    QString path = QFileDialog::getExistingDirectory(Q_NULLPTR, tr("Select directory"), default_path);
    return path;
}

QString AndroidFileDialog::getFileName(QString default_path)
{
    QString path = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Select directory"), default_path);
    return path;
}
#endif
