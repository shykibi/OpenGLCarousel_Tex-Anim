#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

#include <sstream>
std::string toString(const int &i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

#define I glm::mat4(1.0)

void funInit();
void funReshape(int w, int h);
void funDisplay();
void helixAutoRotation(int value);
void cameraZoom(int key, int status, int x, int y);
void cameraMovement(int x, int y);
void moveModel(unsigned char key, int x, int y);
void traslation(int key, int x, int y);

//Funciones de luces y texturas
void setLights(glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

//Funciones para construir el modelo
void drawPlane(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelix(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawLittleCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArticulation(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArm(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTop(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBody(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawModel(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBodyAndTop(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawExtreme(glm::mat4 P, glm::mat4 V, glm::mat4 M);


// Shaders variables
    Shaders shaders;

// Model variables
    Model plane;     // 2*2*2
    Model cone;      // 5.37*4.85*5.37
    Model cylinder;  // 2*2*2
    Model sphere;    // 2*2*2 -->   4*4*4

// Textures variables


    Texture imgNoEmissive;          //img1
    Texture imgLightingSpecular;    //img2
    Texture imgBlack_w_Cubes;       //img3
    Texture imgWhiteMarble;         //img4
    Texture imgGravel;              //img5
    Texture imgSquared;             //img6
    Texture imgSquaredNormal;       //img7


// Lights and materials
    // Lights
        #define   NLD 1
        #define   NLP 1
        #define   NLF 2
        Light     lightG;
        Light     lightD[NLD];
        Light     lightP[NLP];
        Light     lightF[NLF];
    // Materials
        // Material
            Material  mluz;
            Material  gold;
            Material  ruby;
        // Textures materials
            Textures  texFloor; //la difusa es imgGravel, la specular(brillos) es la imgBlack_w_Cubes, ¿LA EMISIVA ES IMGlIGHTINGSPECULAR?
            Textures  texBase;  //difusa imgSquares, normal imgSquaresNormal
            Textures  texLights;    //imgMarble


// Viewport
    int w = 500;
    int h = 500;

// Animations variables
    float desZ   =  0.0;
    float alphaX =  40.0;
    float alphaY =  25.0;

    GLint speed = 20;  // 20 ms
    float rotY = 0.0;
    float zoom = 50.0;
    float rotY_b_t = 0.0;
    float rotArms = 0.0;
    float movTop = 1.3;
    float movX= 0.0;
    float movZ= 0.0;

int main(int argc, char** argv) {

    // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Sesion 7");

    // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

    // Inicializaciones específicas
    funInit();

    // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutSpecialFunc(traslation);
    glutKeyboardFunc(moveModel);
    glutMouseFunc(cameraZoom);
    glutMotionFunc(cameraMovement);


    glutTimerFunc(speed, helixAutoRotation, 0);

    // Bucle principal
    glutMainLoop();

    return 0;
}

void funInit() {
    // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Test de profundidad
    glEnable(GL_DEPTH_TEST);

    // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

    // Models
    plane.initModel("resources/models/plane.obj");
    sphere.initModel("resources/models/sphere.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    cone.initModel("resources/models/cone.obj");


    // Textures (images)
    imgNoEmissive.initTexture("resources/textures/img1.png");
    imgLightingSpecular.initTexture("resources/textures/img2.png");
    imgBlack_w_Cubes.initTexture("resources/textures/img3.png");
    imgWhiteMarble.initTexture("resources/textures/img4.png");
    imgGravel.initTexture("resources/textures/img5.png");
    imgSquared.initTexture("resources/textures/img6.png");
    imgSquaredNormal.initTexture("resources/textures/img7.png");



    // Global ambiental Light
    lightG.ambient        = glm::vec3(0.1, 0.1, 0.1);

    // Directional Lights
    lightD[0].direction   = glm::vec3(-1.0, 0.0, 0.0);
    lightD[0].ambient     = glm::vec3( 0.1, 0.1, 0.1);
    lightD[0].diffuse     = glm::vec3( 0.7, 0.7, 0.7);
    lightD[0].specular    = glm::vec3( 0.7, 0.7, 0.7);

    // Positional Lights
    lightP[0].position    = glm::vec3(0.0, 3.0, 3.0);
    lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

    // Focal Lights
    lightF[0].position    = glm::vec3(-2.0,  2.0,  5.0);
    lightF[0].direction   = glm::vec3( 2.0, -2.0, -5.0);
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;
    lightF[1].position    = glm::vec3( 2.0,  2.0,  5.0);
    lightF[1].direction   = glm::vec3(-2.0, -2.0, -5.0);
    lightF[1].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[1].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].innerCutOff = 5.0;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.090;
    lightF[1].c2          = 0.032;

    // Materiales
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

    ruby.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
    ruby.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
    ruby.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
    ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    ruby.shininess = 76.8;

    gold.ambient   = glm::vec4(0.247250, 0.199500, 0.074500, 1.00);
    gold.diffuse   = glm::vec4(0.751640, 0.606480, 0.226480, 1.00);
    gold.specular  = glm::vec4(0.628281, 0.555802, 0.366065, 1.00);
    gold.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    gold.shininess = 51.2;

    /*texRuby.diffuse   = imgRuby.getTexture();
    texRuby.specular  = imgRuby.getTexture();
    texRuby.emissive  = imgNoEmissive.getTexture();
    texRuby.normal    = 0;
    texRuby.shininess = 10.0.8;

    texGold.diffuse   = imgGold.getTexture();
    texGold.specular  = imgGold.getTexture();
    texGold.emissive  = imgNoEmissive.getTexture();
    texGold.normal    = 0;
    texGold.shininess = 10.0;

    texEarth.diffuse   = imgEarth.getTexture();
    texEarth.specular  = imgEarth.getTexture();
    texEarth.emissive  = imgNoEmissive.getTexture();
    texEarth.normal    = 0;
    texEarth.shininess = 10.0;

    texChess.diffuse   = imgChess.getTexture();
    texChess.specular  = imgChess.getTexture();
    texChess.emissive  = imgNoEmissive.getTexture();
    texChess.normal    = 0;
    texChess.shininess = 10.0;

    texCube.diffuse= imgCubeDiffuse.getTexture();
    texCube.specular= imgCubeSpecular.getTexture();
    texCube.emissive= imgNoEmissive.getTexture();
    texCube.normal=0;
    texCube.shininess= 10.0;

    texWindow.diffuse   = imgWindow.getTexture();
    texWindow.specular  = imgWindow.getTexture();
    texWindow.emissive  = imgWindow.getTexture();
    texWindow.normal    = 0;
    texWindow.shininess = 10.0;

    texPlane.diffuse   = imgGold.getTexture();
    texPlane.emissive  = imgNoEmissive.getTexture();
    texPlane.specular  = imgGold.getTexture();
    texPlane.normal    = imgWallNormal.getTexture();
    texPlane.shininess = 10.0;
    */

}

void funReshape(int wnew, int hnew) {

    // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

    // Actualización de w y h
    w = wnew;
    h = hnew;

}

void funDisplay() {

    // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Indicamos los shaders a utilizar
    shaders.useShaders();

    // Matriz P
    float fovy=zoom;
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P  = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

    // Matriz V
    float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 10.0f*glm::sin(glm::radians(alphaY));
    float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 pos   (  x,   y,   z);
    glm::vec3 lookat(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0, 0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);
    shaders.setVec3("ucpos",pos);

    // Fijamos las luces
    setLights(P,V);

    // Dibujamos la escena
    drawPlane(P,V,I);
    glm::mat4 T = glm::translate(I, glm::vec3(movX, 0.0, movZ));
    drawModel(P,V,I*T);

    //ESTO DE AQUÍ ABAJO EVITARÁ EL Z FIGHTING Y QUE SE NOS REFLEJE LA LUZ DEBAJO DEL PLANO!!!
    /*      glm::mat4 S = glm::scale    (I, glm::vec3(4.0f, 1.0f, 4.0f));
            glm::mat4 T = glm::translate(I, glm::vec3(0.0f,-3.0f, 0.0f));
            glm::mat4 R = glm::rotate   (I, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));

     * glEnable(GL_CULL_FACE);
        drawObjectTex(plane,texChess,P,V,T*S);
        drawObjectTex(plane,texChess,P,V,T*S*R);
    glDisable(GL_CULL_FACE);*/


    //CÓDIGO ÚTIL PARA LAS TRANSPARENCIAS   --> SE DESABILITA LA MÁSCARA DE PROFUNDIDAD PARA EJECUTAR
    /*glm::mat4 Rv = glm::rotate   (I, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 Tv = glm::translate(I, glm::vec3(0.0, 0.0, 3.0));
    glDepthMask(GL_FALSE);
        drawObjectTex(plane, texWindow, P, V, Tv*Rv);
    glDepthMask(GL_TRUE);*/



    // Intercambiamos los buffers
    glutSwapBuffers();

}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1));
        //SE LE DA UNA "FORMA A LAS LUCES PARA QUE TENGAN UN ASPECTO VISIBLE"
        //drawObjectMat(sphere,mluz,P,V,M);
        //drawObjectTex(sphere,texEarth,P,V,M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025));
        //drawObjectTex(sphere,texEarth,P,V,M);
    }

}

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials", false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);

}

void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uPVM",P*V*M);

    glEnable(GL_POLYGON_OFFSET_FILL);
    shaders.setVec3("uColor",color);
    model.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    shaders.setVec3("uColor",glm::vec3(0.75*color[0], 0.75*color[1], 0.75*color[2]));
    model.renderModel(GL_LINE);

}

// ----------------------------------------------      Drawing Models Functions     ----------------------------------------------

void drawPlane(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    drawObject(plane,glm::vec3(0.5, 0.5, 0.5),P,V,M*S);

}

void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 1, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.6, 0.05, 0.6));
    drawObject(cylinder,glm::vec3(1.0, 1.0, 0.0),P,V,M*S*T);

}

void drawBody(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 2.1, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.094, 0.268, 0.094));
    drawObject(cone,glm::vec3(0.9, 0.4, 0.0),P,V,M*S*T);

}

void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -2.8, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.012, 0.062, 0.038));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObject(cone,glm::vec3(1.0, 0.0, 0.0),P,V,M*R90*S*T);

}

void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -1.0, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.05, 0.5, 0.05));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObject(cylinder,glm::vec3(0.0, 0.0, 1.0),P,V,M*R90*S*T);

}

void drawLittleCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -0.15, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.025, 0.15, 0.025));
    drawObject(cylinder,glm::vec3(0.0, 1.0, 1.0),P,V,M*T*S);

}

void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.15, 0.15, 0.15));
    drawObject(sphere,glm::vec3(0.0, 1.0, 0.0),P,V,M*S);

}


void drawHelix(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::mat4 R_auto = glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0));
    drawCone(P,V,M*R_auto);
    drawCone(P,V,M*R_auto*R90);
    drawCone(P,V,M*R_auto*R90*R90);
    drawCone(P,V,M*R_auto*R90*R90*R90);

}

void drawArticulation(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.0375, 0.0375, 0.0375));
    drawObject(sphere,glm::vec3(1.0, 0.0, 1.0),P,V,M*S);

}

void drawExtreme(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Ty_helix = glm::translate(I, glm::vec3(0.0, -0.3, 0.0));
    glm::mat4 R45 = glm::rotate(I, glm::radians(45.0f), glm::vec3(0, 1, 0));
    drawLittleCylinder(P,V,M);
    drawHelix(P,V,M*Ty_helix*R45);

}

void drawArm(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 R_antiR = glm::rotate(I, glm::radians(rotArms), glm::vec3(0, 0, 1));
    drawCylinder(P,V,M);
    drawArticulation(P,V,M*T);
    drawExtreme(P,V,M*T*R_antiR);

}

void drawTop(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R72 = glm::rotate(I, glm::radians(72.0f), glm::vec3(0, 1, 0));
    glm::mat4 R = glm::rotate(I, glm::radians(-rotArms), glm::vec3(0, 0, 1));
    drawSphere(P,V,M);
    drawArm(P,V,M*R);
    drawArm(P,V,M*R72*R);
    drawArm(P,V,M*R72*R72*R);
    drawArm(P,V,M*R72*R72*R72*R);
    drawArm(P,V,M*R72*R72*R72*R72*R);

}

void drawBodyAndTop(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, movTop, 0.0));
    drawBody(P,V,M);
    drawTop(P,V,M*T);

}

void drawModel(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R = glm::rotate(I, glm::radians(rotY_b_t), glm::vec3(0, 1, 0));
    drawBase(P,V,M);
    drawBodyAndTop(P,V,M*R);

}
// ----------------------------------------------      Animation Functions     ----------------------------------------------

void helixAutoRotation(int value) {

    rotY -= 2.5;
    glutPostRedisplay();
    glutTimerFunc(speed, helixAutoRotation ,0);

}

void moveModel(unsigned char key, int x, int y) {

    switch (key) {
        case 'r':
            rotY_b_t += 5;
            break;
        case 'R':
            rotY_b_t -= 5;
            break;
        case 'a':
            if (rotArms < 45 && rotArms >= 0) rotArms += 3;
            break;
        case 'A':
            if (rotArms <= 45 && rotArms > 0) rotArms -= 3;
            break;
        case 'y':
            if (movTop > 0.35) movTop -= 0.01;
            break;
        case 'Y':
            if (movTop < 1.3) movTop += 0.01;
            break;

        //SESION :
        case 'e':   desZ -= 0.1f;  break;
        case 'd':   desZ += 0.1f;  break;
        default:    desZ  = 0.0f;  break;
    }
    glutPostRedisplay();

}

void traslation(int key, int x, int y){

    switch (key) {
        case GLUT_KEY_LEFT:  if(movX > -1.4) movX -= 0.1;
            break;
        case GLUT_KEY_RIGHT: if(movX < 1.4) movX += 0.1;
            break;
        case GLUT_KEY_UP: if(movZ > -1.4) movZ -= 0.1;
            break;
        case GLUT_KEY_DOWN: if(movZ < 1.4) movZ += 0.1;
            break;
        default:
            break;
    }
    glutPostRedisplay();

}

void cameraZoom(int key, int status, int x, int y) {

    switch (key) {
        case 4:
            if (zoom < 90) zoom += 1;
            break;
        case 3:
            if (zoom > 10) zoom -= 1;
            break;
        default:
            break;
    }
    glutPostRedisplay();

}

void cameraMovement(int x, int y) {

    int ax = x - w / 2;
    int ay = y - h / 2;

    int px2degX = ax * 180 / w;
    int px2degY = ay * 90 / (h/2);

    if(px2degX < 180 && px2degX > -180) alphaX = px2degX;
    if(px2degY < 90 && px2degY > -90) alphaY = -px2degY;
    glutPostRedisplay();

}