#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>

#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readyRead();
    void on_pushButton_deploy_clicked();

    void on_pushButton_locateBinDir_clicked();

    void on_pushButton_locateEXEDir_clicked();

private:
    Ui::MainWindow *ui;
    void loadSettings();
    void saveSettings();
    QString m_sSettingsFile;

    QProcess r;
    std::vector<std::string> cmd;
    QString binDir, appDir;
    //int count = 0;

};

#endif // MAINWINDOW_H
