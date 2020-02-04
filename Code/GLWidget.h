#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <math.h>
#include <Scene.h>
#include <Camera.h>
#include <QtWidgets>
#include <QGLWidget>
#include <QTime>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1

class QGLShaderProgram;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void newObj(QString fichero);

    // es carrega el shader program associat a l'índex passat, configurant
    // si s'activa o no la textura
    void changeToShaderWithIndex(int index, bool enableTexture = false);

    // s'envia a GPU el conjunt de dades necessàries per decorar el fons
    void environmentToGPU();


public slots:

    void activaToonShader();
    void activaPhongShader();
    void activaGouraudShader();
    void activaPhongTex();
    void activaGouraudTex();
    void activaToonTex();
    void activaBumpMapping();
    void activaEnvMapping();

    void ensenyaMenuLight0();
    void changePositionLight();
    void changeDirectionalLight();
    void changeSpotLight();
    void activateLight();
    void updateXPositionLight(int xposition);
    void updateYPositionLight(int yposition);
    void updateZPositionLight(int zposition);
    void updateLightIntensity(int intensity);
    void showAuxWindowPuntualLight(Light *light);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void setXRotation(int angle);
    void setYRotation(int angle);

    void qNormalizeAngle(double &angle);

    void Zoom (int positiu);
private:
    QWidget* auxWidget; // window for parameters

    Scene *scene;      // escena que conté els objectes i les llums
    Camera *camera;

    QPoint lastPos;   // per interactuar amb la camera

    // punter al shader program actual
    QGLShaderProgram *program;

    // conjunt dels principals shader programs disponibles
    vector<QGLShaderProgram*> programs;

    // punter al shader program especial que fa possible l'environmental mapping
    QGLShaderProgram * environmentProgram;

    // booleà que es passa a la GPU per activar o no la textura sense
    // haver de duplicar shaders per Gouraud, Phong i Toon
    bool isTextureEnabled;

    // booleà que es passa a la GPU per activar o no el bump mapping
    bool isBumpMappingEnabled;

    // indicador de si s'ha de visualitzar o no la decoració de fons
    bool isEnvironmentalMappingEnabled;

    void initShader(const char* vertexShaderFile, const char* fragmentShaderFile);
    void initShadersGPU();
    void updateShader();
    void updateShaderTexture();

    // inicialitza el shader d'environmental mapping
    void initEnvironmentalShader();

};

#endif // GLWIDGET_H
