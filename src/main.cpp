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
void controller(unsigned char key, int x, int y);
void translation(int key, int x, int y);
void changeIntensity();

// Textures and Light Functions
void setLights(glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

// Model Building Functions
void drawPlaneWindow(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPlaneWindowInverse(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFloor(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFloorReverse(glm::mat4 P, glm::mat4 V, glm::mat4 M);
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

// Texture variables
    Texture imgNoEmissive;          //img1
    Texture imgLightingSpecular;    //img2
    Texture imgBlack_w_Cubes;       //img3
    Texture imgWhiteMarble;         //img4
    Texture imgGravel;              //img5
    Texture imgSquared;             //img6
    Texture imgSquaredNormal;       //img7

// Lights and Materials
    // Lights
        #define   NLD 1
        #define   NLP 1
        #define   NLF 1
        Light     lightG;
        Light     lightD[NLD];
        Light     lightP[NLP];
        Light     lightF[NLF];
    // Materials
        // Materials
            Material  gold;
            Material  ruby;
            Material  polished_bronze;
            Material  cyan;
            Material  emerald;

        // Textures
            Textures  texFloor;
            Textures  texBase;
            Textures  texLights;
            Textures texFloorInverse;


// Viewport
    int w = 500;
    int h = 500;

// Animation Variables
    float desZ            = 0.0;
    float alphaX          = 40.0;
    float alphaY          = 25.0;
    GLint speed           = 20;  // 20 ms
    bool timerIsActivated = true;
    float rotY            = 0.0;
    float zoom            = 50.0;
    float rotY_b_t        = 0.0;
    float rotArms         = 0.0;
    float movTop          = 1.3;
    float movX            = 0.0;
    float movZ            = 0.0;
    float intensity       = 0.7;
    float rotateLight     = 180.0;
    bool lightOn          = true;
    Light savedLight;

int main(int argc, char** argv) {

    // GLUT Initialization
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Practica 3");

    // GLEW Initialization
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

    // Specific Initializations
    funInit();

    // CallBacks Configuration
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutSpecialFunc(translation);
    glutKeyboardFunc(controller);
    glutMouseFunc(cameraZoom);
    glutMotionFunc(cameraMovement);
    glutTimerFunc(speed, helixAutoRotation, 0);

    // Main Loop
    glutMainLoop();

    return 0;

}

void funInit() {

    // Transparencies
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Depth Test
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

    // Global Ambient Light
    lightG.ambient        = glm::vec3(0.25, 0.25, 0.25);

    // Directional Lights
    lightD[0].direction   = glm::vec3(0.0, -1.0, 0.0);
    lightD[0].ambient     = glm::vec3( 0.2, 0.2, 0.2);
    lightD[0].diffuse     = glm::vec3( 0.7, 0.7, 0.7);
    lightD[0].specular    = glm::vec3( 0.7, 0.7, 0.7);

    // Positional Lights
    lightP[0].position    = glm::vec3(-1.5,0.2,0.0);
    lightP[0].ambient     = glm::vec3(0.7, 0.7, 0.7);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

    // Focal Lights
    lightF[0].position    = glm::vec3(3.0,  3.0,  4.0);
    lightF[0].direction   = glm::vec3(-3.0, -3.0, -4.0);
    lightF[0].ambient     = glm::vec3(0.7,  0.7,  0.7);
    lightF[0].diffuse     = glm::vec3(1.25,  1.25,  1.25);
    lightF[0].specular    = glm::vec3(0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 7.5;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 1.0;
    lightF[0].c0          = 1.000;  //1.000
    lightF[0].c1          = 0.090;  //0.090
    lightF[0].c2          = 0.032;  //0.032

    // Materials
    gold.ambient   = glm::vec4(0.24725f, 0.1995f, 0.0745f, 1.0f);
    gold.diffuse   = glm::vec4(0.75164f, 0.60648f, 0.22648f, 1.0f);
    gold.specular  = glm::vec4(0.628281f, 0.555802f, 0.366065f, 1.0f );
    gold.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    gold.shininess = 51.2f;

    ruby.ambient   = glm::vec4(0.1745f, 0.01175f, 0.01175f, 1.0f);
    ruby.diffuse   = glm::vec4(0.61424f, 0.04136f, 0.04136f, 1.0f);
    ruby.specular  = glm::vec4(0.727811f, 0.626959f, 0.626959f, 1.0f);
    ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    ruby.shininess = 76.8f;

    polished_bronze.ambient   = glm::vec4(0.25f, 0.148f, 0.06475f, 1.0f);
    polished_bronze.diffuse   = glm::vec4(0.4f, 0.2368f, 0.1036f, 1.0f);
    polished_bronze.specular  = glm::vec4(0.774597f, 0.458561f, 0.200621f, 1.0f);
    polished_bronze.emissive  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    polished_bronze.shininess = 76.8f;

    cyan.ambient   = glm::vec4(0.0f,0.1f,0.06f ,1.0f);
    cyan.diffuse   = glm::vec4(0.0f,0.50980392f,0.50980392f,1.0f);
    cyan.specular  = glm::vec4(0.50196078f,0.50196078f,0.50196078f,1.0f);
    cyan.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    cyan.shininess = 32.0f;

    emerald.ambient   = glm::vec4(0.0215f, 0.1745f, 0.0215f, 0.55f);
    emerald.diffuse   = glm::vec4(0.07568f, 0.61424f, 0.07568f, 0.55f);
    emerald.specular  = glm::vec4(0.633f, 0.727811f, 0.633f, 0.55f);
    emerald.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 0.55f);
    emerald.shininess = 76.8f;

    texFloor.diffuse   = imgGravel.getTexture();
    texFloor.specular  = imgBlack_w_Cubes.getTexture();
    texFloor.emissive  = imgLightingSpecular.getTexture();
    texFloor.normal    = 0;
    texFloor.shininess = 10.0;

    texFloorInverse.diffuse   = imgGravel.getTexture();
    texFloorInverse.specular  = imgNoEmissive.getTexture();
    texFloorInverse.emissive  = imgLightingSpecular.getTexture();
    texFloorInverse.normal    = 0;
    texFloorInverse.shininess = 10.0;

    texBase.diffuse   = imgSquared.getTexture();
    texBase.specular  = imgLightingSpecular.getTexture();
    texBase.emissive  = imgNoEmissive.getTexture();
    texBase.normal    = imgSquaredNormal.getTexture();
    texBase.shininess = 10.0;

    texLights.diffuse   = imgWhiteMarble.getTexture();
    texLights.specular  = imgWhiteMarble.getTexture();
    texLights.emissive  = imgNoEmissive.getTexture();
    texLights.normal    = 0;
    texLights.shininess = 10.0;

}

void funReshape(int wnew, int hnew) {

    // Viewport Configuration
    glViewport(0, 0, wnew, hnew);

    // w & h Update
    w = wnew;
    h = hnew;

}

void funDisplay() {

    // Color Buffer Delete
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Used Shaders
    shaders.useShaders();

    // P Matrix
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P  = glm::perspective(glm::radians(zoom), aspect, nplane, fplane);

    // V Matrix
    float x = 5.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 5.0f*glm::sin(glm::radians(alphaY));
    float z = 5.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 pos   (x, y, z);
    glm::vec3 lookat(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0, 0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);
    shaders.setVec3("ucpos",pos);

    // Set Lights
    setLights(P,V);

    // Draw Scene

    glm::mat4 T = glm::translate(I, glm::vec3(movX, 0.0, movZ));
    glm::mat4 T_sphere = glm::translate(I, glm::vec3(movX, movTop, movZ));
    glm::mat4 R_sphere = glm::rotate(I, glm::radians(rotY_b_t), glm::vec3(0, 1, 0));
    glm::mat4 R = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));

    glEnable(GL_CULL_FACE);
        drawModel(P,V,I*T);
        drawFloor(P,V,I);

        glDepthMask(GL_FALSE);
            drawSphere(P,V,I*T_sphere*R_sphere);
            drawPlaneWindow(P,V,I);

                glDisable(GL_DEPTH_TEST);
                drawFloorReverse(P,V,I);
                glEnable(GL_DEPTH_TEST);
            drawPlaneWindowInverse(P,V,I);
        glDepthMask(GL_TRUE);


    // Buffer Swap
    glutSwapBuffers();

}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for (int i = 0; i < NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for (int i = 0; i < NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for (int i = 0; i < NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for (int i = 0; i < NLP; i++) {
        lightP[i].position = glm::vec3(1.5*glm::cos(glm::radians(rotateLight)) , 0.2, 1.5*glm::sin(glm::radians(rotateLight)) );
        glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.025));
        drawObjectTex(sphere,texLights,P,V,M);
    }

    for (int i = 0; i < NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025));
        drawObjectTex(sphere,texLights,P,V,M);
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

// ----------------------------------------------      Drawing Models Functions     ----------------------------------------------

void drawPlaneWindow(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R90 = glm::rotate(I, glm::radians(-90.0f), glm::vec3(0, 0, 1));
    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    glm::mat4 T = glm::translate(I, glm::vec3(-2.0, 0.0, 0.0));
    drawObjectMat(plane, emerald, P, V, M * T * R90 * S);

}

void drawPlaneWindowInverse(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R90 = glm::rotate(I, glm::radians(-90.0f), glm::vec3(0, 0, 1));
    glm::mat4 R180 = glm::rotate(I, glm::radians(180.0f), glm::vec3(1, 0, 0));
    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    glm::mat4 T = glm::translate(I, glm::vec3(-2.0, 0.0, 0.0));
    drawObjectMat(plane, emerald, P, V, M * T * R90 * S*R180);

}


void drawFloor(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    drawObjectTex(plane, texFloor, P, V, M * S);

}

void drawFloorReverse(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    glm::mat4 R = glm::rotate(I, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
    drawObjectTex(plane, texFloorInverse, P, V, M * R * S);

}

void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.6, 0.05, 0.6));
    drawObjectTex(cylinder, texBase, P, V, M * S * T);

}

void drawBody(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 2.1, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.094, 0.268, 0.094));
    drawObjectMat(cone, polished_bronze, P, V, M * S * T);

}

void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M) {  //Aspas

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -2.8, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.012, 0.062, 0.038));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObjectMat(cone, ruby, P, V, M * R90 * S * T);

}

void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -1.0, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.05, 0.5, 0.05));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObjectMat(cylinder, gold, P, V, M * R90 * S * T);

}

void drawLittleCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -0.15, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.025, 0.15, 0.025));
    drawObjectMat(cylinder, cyan, P, V, M * T * S);

}

void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.15, 0.15, 0.15));
    drawObjectMat(sphere, emerald, P, V, M * S);

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
    drawObjectMat(sphere, ruby, P, V, M * S);

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

    if (timerIsActivated) {
        rotY -= 2.5;
        glutPostRedisplay();
        glutTimerFunc(speed, helixAutoRotation, 0);
    }

}

void controller(unsigned char key, int x, int y) {

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
        case 's':
            if (timerIsActivated)
                timerIsActivated = false;
            else {
                glutTimerFunc(speed, helixAutoRotation, 0);
                timerIsActivated = true;
            }
            break;
        case 'd':
            if(intensity > 0.0) {
                intensity -= 0.1;
                changeIntensity();
            }
            break;
        case 'D':
            if (intensity < 0.7) {
                intensity += 0.1;
                changeIntensity();
            }
            break;
        case 'p':
            rotateLight += 5.0;
            break;
        case 'f':
            if (lightOn) {
                savedLight = lightF[0];
                lightF[0].ambient = glm::vec3(0.0, 0.0, 0.0);
                lightF[0].diffuse = glm::vec3(0.0, 0.0, 0.0);
                lightF[0].specular = glm::vec3(0.0, 0.0, 0.0);
                lightOn = false;
            } else {
                lightF[0] = savedLight;
                lightOn = true;
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();

}

void changeIntensity() {
    lightD[0].specular = glm::vec3(intensity, intensity, intensity);
    lightD[0].diffuse = glm::vec3(intensity, intensity, intensity);
    lightD[0].ambient = glm::vec3(intensity, intensity, intensity);
}

void translation(int key, int x, int y) {

    switch (key) {
        case GLUT_KEY_LEFT:
            if(movX > -1.4) movX -= 0.1;
            break;
        case GLUT_KEY_RIGHT:
            if(movX < 1.4) movX += 0.1;
            break;
        case GLUT_KEY_UP:
            if(movZ > -1.4) movZ -= 0.1;
            break;
        case GLUT_KEY_DOWN:
            if(movZ < 1.4) movZ += 0.1;
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