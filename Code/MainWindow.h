#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <GLWidget.h>

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE
//! [0]
//!
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;


    QAction *action_toon_tex;

public slots:
    void newObjFromFile();

};

#endif // MAINWINDOW_H
