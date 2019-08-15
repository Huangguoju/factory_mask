#ifndef ABOUTUSDIALOG_H
#define ABOUTUSDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class AboutUsDialog;
}

class AboutUsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutUsDialog(QWidget *parent = 0);
    ~AboutUsDialog();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_joinUsURl_linkActivated(const QString &link);

private:
    Ui::AboutUsDialog *ui;
    int m_charWidth;
    int m_curIndex;
    QString m_showText;
};

#endif // ABOUTUSDIALOG_H
