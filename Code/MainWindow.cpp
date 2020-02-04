
#include <QtWidgets>
#include "ui_mainwindow.h"

#include "GLWidget.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // s'afegeix l'opció de Toon amb textura al menú
    action_toon_tex = new QAction(this);
    action_toon_tex->setObjectName(QStringLiteral("action_toon_tex"));
    ui->menuTextures->addAction(action_toon_tex);
    action_toon_tex->setText(QApplication::translate("MainWindow", "ToonTex", Q_NULLPTR));
    action_toon_tex->setShortcut(QApplication::translate("MainWindow", "Ctrl+0", Q_NULLPTR));


    setWindowTitle(tr("Practica 2: GiVD 2016-2017"));
    resize(640, 640);


    glWidget = new GLWidget;


    setCentralWidget(glWidget);

    // connexio dels menus

    connect(this->ui->action_obre_fitxer, SIGNAL(triggered()), this, SLOT(newObjFromFile()));
    connect(this->ui->action_gouraud, SIGNAL(triggered()), glWidget, SLOT(activaGouraudShader()));
    connect(this->ui->action_phong, SIGNAL(triggered()), glWidget, SLOT(activaPhongShader()));
    connect(this->ui->action_toon, SIGNAL(triggered()), glWidget, SLOT(activaToonShader()));
    connect(this->ui->action_gouraud_tex, SIGNAL(triggered()), glWidget, SLOT(activaGouraudTex()));
    connect(this->ui->action_phong_tex, SIGNAL(triggered()), glWidget, SLOT(activaPhongTex()));
    connect(this->action_toon_tex, SIGNAL(triggered()), glWidget, SLOT(activaToonTex()));
    connect(this->ui->action_bump_mapping, SIGNAL(triggered()), glWidget, SLOT(activaBumpMapping()));
    connect(this->ui->action_environmental_mapping, SIGNAL(triggered()), glWidget, SLOT(activaEnvMapping()));
    connect(this->ui->action_afegeix_llum_puntual, SIGNAL(triggered()), glWidget, SLOT(ensenyaMenuLight0()));    

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newObjFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isNull())
        this->glWidget->newObj(fileName);

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{

    if (e->key() == Qt::Key_Escape)
        close();

    else{

        if (e->modifiers() == Qt::ControlModifier){

            if (e->key() == Qt::Key_F){
                newObjFromFile();
            }

            if (e->key() == Qt::Key_1){
                glWidget->activaGouraudTex();
            }

            else if (e->key() == Qt::Key_2){
                glWidget->activaPhongTex();
            }

            else if (e->key() == Qt::Key_0){
                glWidget->activaToonTex();
            }

            else if (e->key() == Qt::Key_3){
                glWidget->activaBumpMapping();
            }


            else if (e->key() == Qt::Key_4){
                glWidget->activaEnvMapping();
            }
        }

        else if (e->modifiers() == Qt::AltModifier){

            if (e->key() == Qt::Key_1){
                glWidget->activaGouraudShader();
            }

            else if (e->key() == Qt::Key_2){
                glWidget->activaPhongShader();
            }

            else if (e->key() == Qt::Key_3){
                glWidget->activaToonShader();
            }
        }

        QWidget::keyPressEvent(e);
    }
}

