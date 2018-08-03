#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class DiskCheck;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_genButton_clicked();
    void onNotifyCheck(int flag, const QString& tag, int percent);
    void onNotifyGenerate(int flag, const QString& tag);

    void on_md5SelectButton_clicked();

    void on_udiskSelectButton_clicked();

    void on_chkButton_clicked();

private:
    Ui::MainWindow *ui;
    DiskCheck* check;
    QLabel* statusLabel;

};

#endif // MAINWINDOW_H
