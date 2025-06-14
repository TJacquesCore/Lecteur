#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtMultimedia/QtMultimedia>
#include<filesystem>
#include<fstream>
#include<QtMultimedia/QAudioOutput>
#include<QFileDialog>
#include<QStandardPaths>
#include<QListWidgetItem>
#include<unordered_set>
#include<chrono>
#include<thread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:

    QMediaPlayer* player = new QMediaPlayer;
    QAudioOutput* output = new QAudioOutput;
    std::string reload_location();
    std::string location;

    void refresh();
    void play_music_by_button();



private slots:
   void play_music_by_list(QListWidgetItem* item);





private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
