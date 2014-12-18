#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!QFile("settings.ini").exists())
    {
        m_sSettingsFile = "settings.ini";
        ui->lineEdit_qtbinDirPosition->setText("C:/Qt");
        saveSettings();
    }
    else
    {
        m_sSettingsFile = "settings.ini";
        loadSettings();
    }

    cmd.resize(4);
    cmd[0] = "setlocal";
    cmd[3] = "exit";
}

MainWindow::~MainWindow()
{
    if(!ui->lineEdit_qtbinDirPosition->text().isEmpty())
        saveSettings();
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    QString sText = settings.value("binDir","").toString();
    ui->lineEdit_qtbinDirPosition->setText(sText);
    sText = settings.value("exeDir","").toString();
    ui->lineEdit_exePosition->setText(sText);
}

void MainWindow::saveSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::IniFormat);
    QString sText = ui->lineEdit_qtbinDirPosition->text();
    settings.setValue("binDir", sText);
    sText = ui->lineEdit_exePosition->text();
    settings.setValue("exeDir", sText);
    settings.sync();

    ui->statusBar->showMessage("Save settings successfully!!");
}

void MainWindow::on_pushButton_deploy_clicked()
{
    binDir = ui->lineEdit_qtbinDirPosition->text();
    appDir = ui->lineEdit_exePosition->text();
    if(!QFile(QString(binDir + "/windeployqt.exe")).exists())
    {
        QMessageBox::critical(this,"Error","Qt bin files not located or Qt version too old.\n(Requires Qt 5.2 or above.)");
        ui->statusBar->showMessage("Error, Qt bin files not located or Qt version too old. Requires Qt 5.2 or above.)");
        return;
    }
    if(appDir.isEmpty())
    {
        QMessageBox::critical(this,"Error","Executable cannot be empty.");
        ui->statusBar->showMessage("Error, Executable cannot be empty.");
        return;
    }
    cmd[1] = QString("set PATH=" + binDir).toStdString();
    cmd[2] = QString("windeployqt " + appDir).toStdString();

    connect(&r, SIGNAL(readyReadStandardOutput()), this, SLOT(readyRead()));

    r.start("cmd");
    if (!r.waitForStarted())
    {
        QMessageBox::critical(this,"Error","Unable to use command prompt.");
        return ;
    }
    r.waitForReadyRead();
    QByteArray result = r.readAll();
#ifdef _DEBUG
    std::cout << result.data();
#endif

    //count = 0;
    for(int i = 0; i < cmd.size(); i++)
    {
        r.write(cmd[i].c_str());
        r.write("\n\r");
    }
    r.waitForFinished(1000);

    ui->statusBar->showMessage("Done.");
    disconnect(&r, SIGNAL(readyReadStandardOutput()), this, SLOT(readyRead()));
    QDesktopServices::openUrl(QUrl(appDir));
}

void MainWindow::readyRead()
{
    QByteArray result = r.readAllStandardOutput();
#ifdef _DEBUG
        std::cout << result.data();
#endif
    ui->textBrowser_showMessage->append(QString(result));
    //count++;
}

void MainWindow::on_pushButton_locateBinDir_clicked()
{
    QString&& tmp = QFileDialog::getExistingDirectory(this, "Bin Directory Location", "C:/Qt");
    if(!tmp.isEmpty())
    {
        ui->lineEdit_qtbinDirPosition->setText(tmp);
    }
}

void MainWindow::on_pushButton_locateEXEDir_clicked()
{
    QString&& tmp = QFileDialog::getExistingDirectory(this, "Executable Location", ui->lineEdit_exePosition->text());
    if(!tmp.isEmpty())
    {
        ui->lineEdit_exePosition->setText(tmp);
    }
}
