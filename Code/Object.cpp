#include <Object.h>
#include <readfile.h>

/**
 * @brief Object::Object
 * @param npoints
 * @param parent
 */
Object::Object(int npoints, QObject *parent) : QObject(parent){
    numPoints = npoints;
    points = new point4[numPoints];
    normals = new point4[numPoints];

    // comentat ja que un cop passem el material no té sentit un vector de colors
    //colors = new point4[numPoints];
 }


/**
 * @brief Object::Object
 * @param npoints
 * @param n
 */
Object::Object(int npoints, QString n) : numPoints(npoints){
    points = new point4[numPoints];
    normals = new point4[numPoints];

    // comentat ja que un cop passem el material no té sentit un vector de colors
    //colors = new point4[numPoints];

    readObj(n);
    make();
}

Object::Object(const int npoints, QString n, Material *material) : Object(npoints, n)
{
    this->material = material;
}


/**
 * @brief Object::~Object
 */
Object::~Object(){
    delete points;
    delete normals;
    delete material;

    // comentat ja que un cop passem el material no té sentit un vector de colors
    //delete colors;
}

/**
 * @brief Object::toGPU
 * @param pr
 */
void Object::toGPU(QGLShaderProgram *pr) {
    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal passar les normals

    program = pr;

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );


    // el doble sizeof respon al fet que passem, per cada vèrtex, posició i normal
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index, NULL, GL_STATIC_DRAW );
    glEnable( GL_DEPTH_TEST );


    // crida al pas a GPU per al material
    this->material->toGPU(program);

    // es passen les dades de textura a GPU
    toGPUTexture(program);

}


/**
 * Pintat en la GPU.
 * @brief Object::draw
 */
void Object::draw(){
    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal passar les normals a la GPU

    // Aqui es torna a repetir el pas de dades a la GPU per si hi ha més d'un objecte
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, &points[0] );

    // passarem les normals
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, &normals[0] );

    int vertexLocation = program->attributeLocation("vPosition");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);

    // passem a GPU la normal del vèrtex
    int normalLocation = program->attributeLocation("vNormal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer("vNormal", GL_FLOAT, sizeof(point4)*Index, 4);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );
}

/**
 * @brief Object::make
 */
void Object::make(){

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal calcular la normal a cada vertex a la CPU

    // comentat ja que un cop passem el material no té sentit un vector de colors
    /*static vec3  base_colors[] = {
        vec3( 1.0, 0.0, 0.0 ),
        vec3( 0.0, 1.0, 0.0 ),
        vec3( 0.0, 0.0, 1.0 ),
        vec3( 1.0, 1.0, 0.0 )
    };*/

    Index = 0;
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]];

            // comentat ja que un cop passem el material no té sentit un vector de colors
            //colors[Index] = vec4(base_colors[j%4], 1.0);

            Index++;
        }
    }

    // càlcul de les normals a cada vèrtex
    calculateVertexNormals();


    // inicialització de la textura
    initTextura();
}




/**
 * @brief Object::
 *
 *
 * Texture
 * @param pr
 */
void Object::toGPUTexture(QGLShaderProgram *pr) {

    // TO DO: Cal implementar en la fase 2 de la practica 2
    // S'ha d'activar la textura i es passa a la GPU

    // pas dels mapes de textura a GPU
    program = pr;
    diffuseTexture->bind(0);
    program->setUniformValue("texMap", 0);
    normalTexture->bind(1);
    program->setUniformValue("bumpMap", 1);
    environmentTexture->bind(2);
    program->setUniformValue("envTexMap", 2);

}


/**
 * Pintat en la GPU.
 * @brief Object::drawTexture
 */
void Object::drawTexture(){

    // TO DO: Cal implementar en la fase 2 de la practica 2
    // S'ha d'activar la textura i es passa a la GPU

    // activació de la textura per GL
    glEnable(GL_TEXTURE_2D);
}



/**
 * @brief Object::initTextura
 */
void Object::initTextura()
 {
    // TO DO: A implementar a la fase 2 de la practica 2
    // Cal inicialitzar la textura de l'objecte: veure l'exemple del CubGPUTextura
    qDebug() << "Initializing textures...";

    // Càrrega la textura difusa
    glActiveTexture(GL_TEXTURE0);
    // imatges de bricks amb llicència CC BY 3.0 té per autor JosipKladaric (https://opengameart.org/content/brick-wall)
    diffuseTexture = new QOpenGLTexture(QImage(":/resources/textures/brick_wall2-diff-2048.png"));
    //diffuseTexture = new QOpenGLTexture(QImage(":/resources/textures/2k_earth_daymap.jpg"));
    //diffuseTexture = new QOpenGLTexture(QImage(":/resources/textures/2k_jupiter.jpg"));
    //diffuseTexture = new QOpenGLTexture(QImage(":/resources/textures/2k_stars+milky_way.jpg"));
    if (diffuseTexture){
        diffuseTexture->setMinificationFilter(QOpenGLTexture::Nearest); // Nearest elimina el marge entre inici i final de textura
        diffuseTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    }

    // Càrrega la textura normal
    glActiveTexture(GL_TEXTURE1);
    // imatges de bricks amb llicència CC BY 3.0 té per autor JosipKladaric (https://opengameart.org/content/brick-wall)
    normalTexture = new QOpenGLTexture(QImage(":/resources/textures/brick_wall2-nor-2048.tif"));
    //normalTexture = new QOpenGLTexture(QImage(":/resources/textures/2k_earth_normal_map.tif"));
    if (normalTexture){
        normalTexture->setMinificationFilter(QOpenGLTexture::Nearest); // Nearest elimina el marge entre inici i final de textura
        normalTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    }

    // Càrrega de la textura del fons
    glActiveTexture(GL_TEXTURE2);
    //environmentTexture = new QOpenGLTexture(QImage(":/resources/textures/2k_stars+milky_way.jpg"));
    //environmentTexture = new QOpenGLTexture(QImage(":/resources/textures/2k_jupiter.jpg"));
    environmentTexture = new QOpenGLTexture(QImage(":/resources/textures/Earth-from-Space.jpg"));
    if (environmentTexture){
        environmentTexture->setMinificationFilter(QOpenGLTexture::Nearest); // Nearest elimina el marge entre inici i final de textura
        environmentTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    }
}



// Llegeix un fitxer .obj
//  Si el fitxer referencia fitxers de materials (.mtl), encara no es llegeixen
//  Tots els elements del fitxer es llegeixen com a un unic Object.
void Object::readObj(QString filename){

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        while (true)
        {
            char *comment_ptr = ReadFile::fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */
                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = ReadFile::fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = ReadFile::words[0];

            if (!strcmp (first_word, "v"))
            {
                if (nwords < 4) {
                    fprintf (stderr, "Too few coordinates: '%s'", ReadFile::str_orig);
                    exit (-1);
                }

                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();

                if (nwords == 5) {
                    QString sw(ReadFile::words[4]);
                    double w = sw.toDouble();
                    x/=w;
                    y/=w;
                    z/=w;
                }
                // S'afegeix el vertex a l'Object
                vertexs.push_back(point4(x, y, z, 1));

            }
            else if (!strcmp (first_word, "vn")) {

            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'Object
                // A modificar si es vol carregar mes de un Object
                construeix_cara (&ReadFile::words[1], nwords-1);
            }
            // added
            else if (!strcmp (first_word, "mtllib")) {
                //read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                //int size = strlen(words[1])-1;
                //while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                //currentMaterial = matlib.index(words[1]);
            }
            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }
        }
    }
    // Calcul de les normals a cada cara
    for(unsigned int i=0; i<cares.size(); i++){
        cares[i].calculaNormal(vertexs);
    }
}

Capsa3D Object::calculCapsa3D()
{
    vec3    pmin, pmax;
    int     i;
    Capsa3D capsa;

    pmin.x = points[0].x;
    pmin.y = points[0].y;
    pmin.z = points[0].z;
    pmax = pmin;
    for(i=1; i<Index; i++) {
        if(points[i].x <pmin[0])
            pmin[0] = points[i].x;
        if(points[i].y <pmin[1])
            pmin[1] = points[i].y;
        if(points[i].z <pmin[2])
            pmin[2] = points[i].z;

        if(points[i].x >pmax[0])
            pmax[0] = points[i].x;
        if(points[i].y >pmax[1])
            pmax[1] = points[i].y;
        if(points[i].z >pmax[2])
            pmax[2] = points[i].z;
    }
    capsa.pmin = pmin;
    capsa.a = pmax[0]-pmin[0];
    capsa.h = pmax[1]-pmin[1];
    capsa.p = pmax[2]-pmin[2];
    return capsa;
}


void Object::calculateVertexNormals()
{

    // per calcular les normals dels vèrtexs com a normal promig de les
    // de les cares on apareix sense haver de recórrer totes les cares pels
    // N vèrtexs sinó un únic cop, muntem una taula hash amb els vèrtexs com
    // claus i les normals de les cares on apareixen en recórrer el conjunt de
    // cares; posteriorment es calcula el promig per a cada vèrtex

    std::map<vec4_comparable, vector<vec3>> vertexFaceNormalsMap;

    for (Cara face : cares){

        for (int vertexIndex : face.idxVertices){

            point4 vertex = vertexs[vertexIndex];
            vec4_comparable comparableVertex = vec4_comparable(vertex);

            if (vertexFaceNormalsMap.find(comparableVertex) == vertexFaceNormalsMap.end()){
                vertexFaceNormalsMap[comparableVertex] = vector<vec3>();
            }

            vertexFaceNormalsMap[comparableVertex].push_back(normalize(face.normal));
        }
    }


    // calculem i guardem el promig per cada vèrtex

    for (std::pair<vec4_comparable, vector<vec3>> mapPair : vertexFaceNormalsMap){

        vec4_comparable comparableVertex = mapPair.first;
        vector<vec3> normals = mapPair.second;

        vec3 averageNormal = 0;

        for (vec3 normal : normals){
            averageNormal += normal;
        }

        // es guarda la normal promig del vèrtex ja normalitzada en un atribut especial
        vertexNormalsMap[comparableVertex] = normalize(averageNormal);
    }

    // finalment, per adaptar-se al pas de normals a la GPU, com tenim dos
    // punters paral·lels "points" i "normals" per les coordenades i normals
    // de cada punt, guardem a cada posició del vector de normals que
    // s'inicia a l'adreça de "normals" la normal del vèrtex de mateix índex
    // que a "points"
    for (int i = 0; i < Index; i++){
        normals[i] = vec4(vertexNormalsMap[vec4_comparable(points[i])], 0);
    }

}



void Object::construeix_cara ( char **words, int nwords) {
    Cara face;

    for (int i = 0; i < nwords; i++) {
        int vindex;
        int nindex;
        int tindex;

        if ((words[i][0]>='0')&&(words[i][0]<='9')) {
            ReadFile::get_indices (words[i], &vindex, &tindex, &nindex);

#if 0
            printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
#endif

            /* store the vertex index */

            if (vindex > 0)       /* indices are from one, not zero */
                face.idxVertices.push_back(vindex - 1);
            else if (vindex < 0)  /* negative indices mean count backwards */
                face.idxVertices.push_back(this->vertexs.size() + vindex);
            else {
                fprintf (stderr, "Zero indices not allowed: '%s'\n", ReadFile::str_orig);
                exit (-1);
            }
        }
    }
    face.color = vec4(1.0, 0.0, 0.0, 1.0);
    this->cares.push_back(face);
}

vec4_comparable::vec4_comparable(vec4 vector)
{
    this->vector = vector;
}

vec4_comparable::~vec4_comparable()
{
}
