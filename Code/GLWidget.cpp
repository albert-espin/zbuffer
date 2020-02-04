#include <math.h>
#include <GLWidget.h>
#include <QtDebug>
#include <QString>

// índex dels programes de shader
#define SHADER_PROGRAM_INDEX_GOURAUD 2
#define SHADER_PROGRAM_INDEX_PHONG 1
#define SHADER_PROGRAM_INDEX_TOON 0


GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    setFocusPolicy( Qt::StrongFocus );
    scene = new Scene();
    camera = new Camera();


    isTextureEnabled = false;
    isBumpMappingEnabled = false;
    isEnvironmentalMappingEnabled = false;
}

GLWidget::~GLWidget() {
    makeCurrent();
    delete scene;
    for (QGLShaderProgram * program : programs){
        delete program;
    }

    isTextureEnabled = false;
    isBumpMappingEnabled = false;
    isEnvironmentalMappingEnabled = false;
}

// Metodes que es criden des dels menús

void GLWidget::activaToonShader() {
    //A implementar a la fase 1 de la practica 2
    cout<<"Estic a Toon"<<endl;

    changeToShaderWithIndex(SHADER_PROGRAM_INDEX_TOON);
}

void GLWidget::activaPhongShader() {
    //A implementar a la fase 1 de la practica 2
    cout<<"Estic a Phong"<<endl;

    changeToShaderWithIndex(SHADER_PROGRAM_INDEX_PHONG);
}

void GLWidget::activaGouraudShader() {
    //A implementar a la fase 1 de la practica 2
    cout<<"Estic a Gouraud"<<endl;

    changeToShaderWithIndex(SHADER_PROGRAM_INDEX_GOURAUD);
}



void GLWidget::changeToShaderWithIndex(int index, bool enableTexture){

    // en canviar de shader sempre desactivem environmental mapping,
    // que podrà ser reactivat manualment al menú si es desitja
    isEnvironmentalMappingEnabled = false;
    scene->isEnvironmentVisible = isEnvironmentalMappingEnabled;

    // configurem si la textura està activada: això és un booleà que es
    // passa als shaders per activar o no la textura sense haver de duplicar
    // shaders per Gouraud, Phong i Toon
    isTextureEnabled = enableTexture;

    // el programa de shaders actual passa a ser l'indicat per paràmetre
    program = programs[index];
    updateShader();

    // s'envien els components de l'escena a GPU
    scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

    // es refresca la imatge
    updateGL();
}

void GLWidget::activaPhongTex() {
    //A implementar a la fase 1 de la practica 2
    cout<<"Estic a Phong Tex"<<endl;

    this->updateShaderTexture();

    changeToShaderWithIndex(SHADER_PROGRAM_INDEX_PHONG, true);

}

void GLWidget::activaGouraudTex() {
    //A implementar a la fase 1 de la practica 2

    this->updateShaderTexture();

    changeToShaderWithIndex(SHADER_PROGRAM_INDEX_GOURAUD, true);

}

void GLWidget::activaToonTex() {

    this->updateShaderTexture();

    changeToShaderWithIndex(SHADER_PROGRAM_INDEX_TOON, true);

}

//Metode  per canviar de shaders.
void GLWidget::updateShader(){
    //A implementar a la fase 1 de la practica 2

    // el programa establert com a actual es munta i uneix al pipeline
    program->link();
    program->bind();
}

//Metode per canviar de shaders de textures
void GLWidget::updateShaderTexture(){
    //A implementar a la fase 1 de la practica 2

    // NOTA: No ha calgut implementar-lo perquè s'ha implementat
    // la textura als shaders originals i s'activa o es desactiva
    // segons convingui amb un booleà passat a GPU (aquest sistema
    // limita la repetició codi als shaders)
}
void GLWidget::ensenyaMenuLight0() {
    // Agafa la darrera llum creada. A CANVIAR si es vol tenir control de totes les llums.
    // De fet el metode showAuxWindowPuntualLight té com a parametre la llum a mostrar en el widget.
    // Es podria cridar diverses vegades.
    if (scene->lights.size()>0)
        showAuxWindowPuntualLight(scene->getLightActual());
}

// Per a les llums: com afegir-les
void GLWidget::changePositionLight() {
    // TO DO: cal modificar en la fase 1 de la practica 2
    // Des d'quest mètode s'AFEGEIX una nova llum
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum a l'escena

    // si la llum actual ja es puntual no es fa res, en cas contrari es crea una llum
    // puntual reaprofitant les components possibles de la llum actual
    if (scene->getLightActual()->getTipusLight() != Puntual){

        Light * old = scene->getLightActual();

        // s'extreu la llum antiga
        scene->removeCurrentLight();

        // inicialització arbitrària
        vec3 pos(1,2,4);
        vec3 attenuationCoefficients(1,0.1,0.01);

        // si es de focus podem reaprofitar la posició
        if (dynamic_cast<SpotLight*>(old)){
           pos = vec3(old->getLightPosition()[0], old->getLightPosition()[1], old->getLightPosition()[2]);
           attenuationCoefficients = vec3(old->getCoeficients()[0], old->getCoeficients()[1], old->getCoeficients()[2]);
        }

        // es crea la nova llum
        scene->addLight(new PointLight(old->getDiffuseIntensity(), old->getIa(), old->getIs(), attenuationCoefficients, pos));

        // s'elimina la llum antiga
        delete old;

        // s'envien els components de l'escena a GPU
        scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

        // es refresca la imatge
        updateGL();
    }

}
void GLWidget::changeDirectionalLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum a l'escena.
    // TO DO: cal modificar en la fase 1 de la practica 2
    // Des d'quest mètode s'AFEGEIX una nova llum


    // si la llum actual ja es direccional no es fa res, en cas contrari es crea una llum
    // direccional reaprofitant les components possibles de la llum actual
    if (scene->getLightActual()->getTipusLight() != Direccional){

        Light * old = scene->getLightActual();

        // s'extreu la llum antiga
        scene->removeCurrentLight();

        // inicialització arbitrària (a menys que l'antiga sigui llum de focus, llavors es reaprofita)
        vec3 dir(3,2,-10);
        if (dynamic_cast<SpotLight*>(old)){
            dir = ((SpotLight*)old)->getDirection();
        }

        // es crea la nova llum
        scene->addLight(new DirectionalLight(old->getDiffuseIntensity(), old->getIa(), old->getIs(), dir));

        // s'elimina la llum antiga
        delete old;

        // s'envien els components de l'escena a GPU
        scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

        // es refresca la imatge
        updateGL();
    }

}
void GLWidget::changeSpotLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum a l'escena.
    // TO DO: cal modificar en la fase 1 de la practica 2
    // Des d'quest mètode s'AFEGEIX una nova llum

    // si la llum actual ja es de focus no es fa res, en cas contrari es crea una llum
    // de focus reaprofitant les components possibles de la llum actual
    if (scene->getLightActual()->getTipusLight() != Spot){

        Light * old = scene->getLightActual();

        // s'extreu la llum antiga
        scene->removeCurrentLight();

        // inicialització arbitrària
        vec3 pos(-1,2,2);
        vec3 attenuationCoefficients(1,0.1,0.01);

        // si es de focus podem reaprofitar la posició i els coeficients
        if (dynamic_cast<PointLight*>(old)){
            attenuationCoefficients = vec3(old->getCoeficients()[0], old->getCoeficients()[1], old->getCoeficients()[2]);
            pos = vec3(old->getLightPosition()[0], old->getLightPosition()[1], old->getLightPosition()[2]);
        }

        // inicialització arbitrària (a menys que l'antiga sigui direccional, llavors es reaprofita)
        vec3 dir(1,-1,-10);
        if (dynamic_cast<DirectionalLight*>(old)){
            dir = ((DirectionalLight*)old)->getDirection();
        }

        // inicialització arbitrària
        float angle = 10.f;

        // es crea la nova llum
        scene->addLight(new SpotLight(old->getDiffuseIntensity(), old->getIa(), old->getIs(), attenuationCoefficients, pos, dir, angle));

        // s'elimina la llum antiga
        delete old;

        // s'envien els components de l'escena a GPU
        scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

        // es refresca la imatge
        updateGL();
    }

}
void GLWidget::updateXPositionLight(int xposition) {
    // S'ha de modificar la posicio x de la llum activa
    vec4 v = scene->getLightActual()->getLightPosition();
    v[0] = (float)xposition;
    scene->getLightActual()->setLightPosition(v);

    // s'envien els components de l'escena a GPU
    scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

    // es refresca la imatge
    updateGL();
}

void GLWidget::updateYPositionLight(int yposition) {
    // S'ha de modificar la posicio y de la llum activa
    vec4 v = scene->getLightActual()->getLightPosition();
    v[1] = (float)yposition;
    scene->getLightActual()->setLightPosition(v);


    // s'envien els components de l'escena a GPU
    scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

    // es refresca la imatge
    updateGL();
}

void GLWidget::updateZPositionLight(int zposition) {
    // S'ha de modificar la posicio z de la llum activa
    vec4 v = scene->getLightActual()->getLightPosition();
    v[2] = (float)zposition;
    scene->getLightActual()->setLightPosition(v);


    // s'envien els components de l'escena a GPU
    scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

    // es refresca la imatge
    updateGL();
}

void GLWidget::updateLightIntensity(int intens) {
    // S'ha de modificar la intensitat de la llum 0. es podria canviar per la llum actual
    vec3 intensitat;
    intensitat =  scene->getLightActual()->getDiffuseIntensity();
    intensitat[0] = intens/200.0;
    intensitat[1] = intens/200.0;
    intensitat[2] = intens/200.0; // el 200 es l'escala del scrollbar

    scene->getLightActual()->setDiffuseIntensity(intensitat);

    // s'envien els components de l'escena a GPU
    scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

    // es refresca la imatge
    updateGL();
}

void GLWidget::activateLight(){
    scene->getLightActual()->switchOnOff();
}

void GLWidget::activaBumpMapping() {
    //TO DO: a implementar a la fase 2 de la practica 2

    // s'activa el bump mapping si està desactivat; es desactiva en cas contrari
    isBumpMappingEnabled = !isBumpMappingEnabled;

    // s'envien els components de l'escena a GPU
    scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

    // es refresca la imatge
    updateGL();
}

void GLWidget::activaEnvMapping() {
    //To DO: a implementar a la fase 2 de la practica 2


    // s'activa l'environmental mapping si està desactivat; es desactiva en cas contrari
    isEnvironmentalMappingEnabled = !isEnvironmentalMappingEnabled;
    scene->isEnvironmentVisible = isEnvironmentalMappingEnabled;

    // s'envien els components de l'escena a GPU
    scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

    // es refresca la imatge
    updateGL();

    // si cal, es carrega, posteriorment a la càrrega de l'escena en si,
    // un fons per decorar-la
    if (isEnvironmentalMappingEnabled){

        // es monta el programa per l'environmental mapping
        environmentProgram->link();
        environmentProgram->bind();

        // s'envien les dades del fons a GPU
        environmentToGPU();

        // es refresca la imatge
        updateGL();
    }

}


void GLWidget::environmentToGPU(){

    // s'envia a GPU tant la càmera com l'objecte del fons,
    // per a ser tractats pel shader especial d'environmental
    // mapping
    camera->toGPU(environmentProgram);
    scene->environmentObject->toGPU(environmentProgram);
}


// Mètodes d'interacció amb el ratolí

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    std::cout<<"Press button"<<std::endl;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
        std::cout<<"M button"<<std::endl;
    if (event->buttons() & Qt::LeftButton)
    {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
           setXRotation(dy);
           setYRotation(dx);
        } else if(lastPos.y()!= event->y()) {// rotar la camera
            setXRotation(dy);
        } else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }

    } else if (event->buttons() & Qt::RightButton) {
       // Zoom: canviar la mida de la window
        if(lastPos.y()> event->y())
             Zoom(-1);
        else
             Zoom(1);
    }

    lastPos = event->pos();
}


void GLWidget::setXRotation(int angle)
{
    if (angle >0) {
        camera->angX += 5;
    } else if (angle<0)
        camera->angX -= 5;
    qNormalizeAngle(camera->angX);

    camera->rotaCamera(scene->capsaMinima);
    updateGL();
}

void GLWidget::setYRotation(int angle)
{
    if (angle >0) {
        camera->angY += 5;
    } else if (angle<0)
        camera->angY-= 5;
    qNormalizeAngle(camera->angY);

    camera->rotaCamera(scene->capsaMinima);
    updateGL();

}

void GLWidget::qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360*16;
    while (angle > 360)
        angle -= 360*16;
}

void GLWidget::Zoom (int positiu) {
    camera->AmpliaWindow(positiu*(0.005));

    updateGL();

}

void GLWidget::initShader(const char* vShaderFile, const char* fShaderFile){
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    vshader->compileSourceFile(vShaderFile);
    fshader->compileSourceFile(fShaderFile);

    program  = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->link();
    program->bind();
}


void GLWidget::initEnvironmentalShader(){
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    vshader->compileSourceFile("://resources/vshaderEnvironmental.glsl");
    fshader->compileSourceFile("://resources/fshaderEnvironmental.glsl");

    environmentProgram  = new QGLShaderProgram(this);
    environmentProgram->addShader(vshader);
    environmentProgram->addShader(fshader);
    environmentProgram->link();
    environmentProgram->bind();
}


/**
 * @brief GLWidget::initShadersGPU
 */
void GLWidget::initShadersGPU(){

    // s'inicialitza i es manté preparat el shader program de decoració del fons
    initEnvironmentalShader();
    makeCurrent();
    scene->createEnvironmentObject(environmentProgram);

    // s'inicialitza i es manté preparat el shader program de Toon
    initShader("://resources/vshaderToon.glsl", "://resources/fshaderToon.glsl");
    programs.push_back(program);

    // s'inicialitza i es manté preparat el shader program de Phong
    initShader("://resources/vshaderPhong.glsl", "://resources/fshaderPhong.glsl");
    programs.push_back(program);

    // s'inicialitza i es manté preparat el shader program de Gouraud
    initShader("://resources/vshaderGouraud.glsl", "://resources/fshaderGouraud.glsl");
    programs.push_back(program);

    // per defecte es fa que el shader program actual sigui Gouraud
    program = programs[SHADER_PROGRAM_INDEX_GOURAUD];
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

void GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);

    // desactivació del cull faces
    glDisable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE);

    // activació d'alfa per transparències
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_RGBA);
    glEnable(GL_DOUBLE);

    initShadersGPU();

    // Creació de llums (amb valors arbitraris)


    // llum direccional
    Light * dirLight0 = new DirectionalLight(vec3(.2,.8,.2), vec3(.2f), vec3(.2f), vec3(-5,-10,-10)/*vec3(-.5,0,-.5)*/);
    scene->addLight(dirLight0);

    // llum de focus
    Light * spotLight0 = new SpotLight(vec3(.1,.2,1), vec3(.2f), vec3(.2f), vec3(1,.01,.004), vec3(0, 0, 2), vec3(0,0,-1), 10.f);
    scene->addLight(spotLight0);

    // llum puntual
    Light * pointLight0 = new PointLight(vec3(.7f,.3f,.4f), vec3(.3f, 0.2f, 0.2f), vec3(.8f), vec3(1,0.1,0.03), vec3(2.1,2.2,2.3));
    scene->addLight(pointLight0);







    camera->init(this->size().width(), this->size().height(), scene->capsaMinima);
    glViewport(camera->vp.pmin[0], camera->vp.pmin[1], camera->vp.a, camera->vp.h);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::paintGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    camera->toGPU(program);
    scene->draw();
}


void GLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    camera->vp.pmin[0] = (width - side) / 2;
    camera->vp.pmin[1] = (height - side) / 2;
    camera->vp.a = side;
    camera->vp.h = side;
    updateGL();
}

void GLWidget::newObj(QString fichero){
    qDebug() << fichero;
    //Object * obj = new Object(100000, fichero);

    // creació de l'objecte imposant un material arbitrari
    Object * obj = new Object(100000, fichero, new Material(vec3(1,.5,.4), vec3(0.3), vec3(0.9), 10.f, 1.));

    // afegiment de l'objecte a l'escena
    scene->addObject(obj);

    // s'envien els components de l'escena a GPU
    scene->toGPU(program, isTextureEnabled, isBumpMappingEnabled);

    camera->actualitzaCamera(scene->capsaMinima);
    updateGL();
}

// Finestra per a introduir una Light puntual o modificar-ne el seu contingut
void GLWidget::showAuxWindowPuntualLight(Light *light)
{
    auxWidget = new QWidget;
    auxWidget->move(0,0);

    QVBoxLayout *outer = new QVBoxLayout;
    QHBoxLayout *buttons = new QHBoxLayout;

    QGroupBox *groupBox = new QGroupBox("Light type");
    QRadioButton *radio1 = new QRadioButton("Position light");
    connect(radio1, SIGNAL(clicked()), this, SLOT(changePositionLight()));
    radio1->setChecked(true);

    QRadioButton *radio2 = new QRadioButton("Directional light");
    connect(radio2, SIGNAL(clicked()), this, SLOT(changeDirectionalLight()));
    radio2->setChecked(false); // Quan es clica es modifica el tipus de Light

    QRadioButton *radio3 = new QRadioButton("Spot light");
    connect(radio3, SIGNAL(clicked()), this, SLOT(changeSpotLight()));
    radio3->setChecked(false); // Sols quan es clica es modifica el tipus de  Light

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);

    groupBox->setLayout(vbox);
    buttons->addWidget(groupBox);

    QGroupBox *groupBox2 = new QGroupBox("Selection");

    QRadioButton *transparent1 = new QRadioButton("&On");
    transparent1->setChecked(true);
    connect(transparent1, SIGNAL(clicked()), this, SLOT(activateLight()));

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(transparent1);

    groupBox2->setLayout(vbox2);
    buttons->addWidget(groupBox2);
    outer->addLayout(buttons);

    // light source intensity
    QSlider* lightSlider = new QSlider(Qt::Horizontal);
    lightSlider->setTickPosition(QSlider::TicksBelow);
    lightSlider->setMinimum(0);
    lightSlider->setMaximum(200);
    if (light == NULL)
        lightSlider->setSliderPosition(100);
    else
        lightSlider->setSliderPosition(100*length(light->getDiffuseIntensity()));
    connect(lightSlider,SIGNAL(valueChanged(int)),this,SLOT(updateLightIntensity(int)));
    QLabel* lightLabel = new QLabel("Light intensity = ");
    QLabel* lightLabelValue = new QLabel();
    lightLabelValue->setNum(100 * light->getDiffuseIntensity()[0]);
    connect(lightSlider,SIGNAL(valueChanged(int)),lightLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxLight = new QHBoxLayout;
    hboxLight->addWidget(lightLabel);
    hboxLight->addWidget(lightLabelValue);
    outer->addLayout(hboxLight);
    outer->addWidget(lightSlider);

    // X slider
    QSlider* XSlider = new QSlider(Qt::Horizontal);
    XSlider->setTickPosition(QSlider::TicksBelow);
    XSlider->setMinimum(-100);
    XSlider->setMaximum(100);
    if (light == NULL)
        XSlider->setSliderPosition(100);
    else
        XSlider->setSliderPosition(light->getLightPosition()[0]);
    connect(XSlider,SIGNAL(valueChanged(int)),this,SLOT(updateXPositionLight(int)));
    QLabel* XLabel = new QLabel("X Position = ");
    QLabel* XLabelValue = new QLabel();
    XLabelValue->setNum(light->getLightPosition()[0]);
    connect(XSlider,SIGNAL(valueChanged(int)),XLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxX = new QHBoxLayout;
    hboxX->addWidget(XLabel);
    hboxX->addWidget(XLabelValue);
    outer->addLayout(hboxX);
    outer->addWidget(XSlider);

    // Y slider
    QSlider* YSlider = new QSlider(Qt::Horizontal);
    YSlider->setTickPosition(QSlider::TicksBelow);
    YSlider->setMinimum(-100);
    YSlider->setMaximum(100);
    if (light == NULL)
        YSlider->setSliderPosition(100);
    else
        YSlider->setSliderPosition(light->getLightPosition()[1]);

    connect(YSlider,SIGNAL(valueChanged(int)),this,SLOT(updateYPositionLight(int)));
    QLabel* YLabel = new QLabel("Y Position = ");
    QLabel* YLabelValue = new QLabel();
    YLabelValue->setNum(light->getLightPosition()[1]);
    connect(YSlider,SIGNAL(valueChanged(int)),YLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxY = new QHBoxLayout;
    hboxY->addWidget(YLabel);
    hboxY->addWidget(YLabelValue);
    outer->addLayout(hboxY);
    outer->addWidget(YSlider);

    // Z Slider
    QSlider* ZSlider = new QSlider(Qt::Horizontal);
    ZSlider->setTickPosition(QSlider::TicksBelow);
    ZSlider->setMinimum(-100);
    ZSlider->setMaximum(100);
    if (light == NULL)
        ZSlider->setSliderPosition(100);
    else
        ZSlider->setSliderPosition(light->getLightPosition()[2]);
    connect(ZSlider,SIGNAL(valueChanged(int)),this,SLOT(updateZPositionLight(int)));
    QLabel* ZLabel = new QLabel("Z Position = ");
    QLabel* ZLabelValue = new QLabel();
    ZLabelValue->setNum(light->getLightPosition()[2]);
    connect(ZSlider,SIGNAL(valueChanged(int)),ZLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxZ = new QHBoxLayout;
    hboxZ->addWidget(ZLabel);
    hboxZ->addWidget(ZLabelValue);
    outer->addLayout(hboxZ);
    outer->addWidget(ZSlider);

    // Es poden afegir més camps a explorar amb el widget
    // Afegiu-los aqui sota

    auxWidget->setLayout(outer);
    auxWidget->show();
}

