#include <ctype.h>
#include"objetos.hpp"

/*
Autor:
Arellano Bautista Arturo 
*/

/*
se requiere instalar opengl para que compile 

compilar en windows:
g++ main.cpp -lfreeglut -lopengl32 -lglu32

compilar en ubuntu:
g++ main.cpp -o botes -lglut -lGLU -lGL
*/


/*
nota: checar los valores globales que no me sirvan
*/

//Variables de traslación en X y Y.
GLfloat tx = 0;
GLfloat ty = 0;
//Variables de rotación en X y Y.
GLfloat rx = 0;
GLfloat ry = 0;
//Variables de escala en todos los ejes.
GLfloat es = 0.8;
GLboolean proy_orto = GL_FALSE;

float t = 0,FPS = 180;
int speed = 5;


bool reproduciendo = true, hitbox = true;
unsigned int tiempo = 1100*50;

Hud hud;
Bote player(0,0,0,false,5,3,1,5);
Mar mar;
//Bote malo(-15,0,0,true,2,1,1,0.01);
//Bote malo(0,-15,270,true,2,1,1,0.01);
int n = 3;
Bote malo[3];

void restart(){
    player.vida = 5;
    int i;
    for(i = 0;i<n;i++){
        malo[i].vida = 0;   
    }
}

void animacion(int l){
    bool fini = false;
    time_t deltaTime;
    time_t currentFrame = time(0); //glfwGetTime();
    deltaTime = currentFrame - (l);
    float lastFrame = currentFrame;
    if (reproduciendo) {
        tiempo += speed;
        player.animar(1000/FPS);
        //printf("%f -- \n",deltaTime);
        player.mover(1000/FPS);

        int i;
        for(i = 0;i<n;i++){
            malo[i].animar(1000/FPS);
            malo[i].mover (1000/FPS);
            status(&player,&malo[i]);

        }

        


        //Se solicita el redibujo.
        glutPostRedisplay();
    }else{
        //dino.score(90);
    }

    //Se manda a llamar nuevamente esta función dentro de 1000/FPS milisegundos.
    glutTimerFunc(1000/FPS, animacion, int(lastFrame));
    
}

void dibuja_hud(void){
    //cambio a 2d
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //nota, revisa 
    gluOrtho2D(0.0,1.0,1.0,0.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //dibujar el hud
    hud.dibujar();

    //regreso a 3D
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void dibujar(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPushMatrix();                 //Empieza el nodo raíz.
        /*
        orden preferte: 
        rotar, transladar, escalar
        
        glRotatef(10, 1, 0, 0);
        //glTranslatef(tx,ty,0);
        glTranslatef(2,-5,0);
       	glScalef(es,es,es);
        */
       
        glRotatef(rx, 1, 0, 0);
        glRotatef(ry, 0, 1, 0);

        mar.dibujar();
        player.dibujar();
        
        int i;
        for(i = 0;i<n;i++){
            malo[i].dibujar();
        }
    glPopMatrix();

    //dibuja_hud();

	glutSwapBuffers();
}

void camara (void) {
    float ancho = GLUT_WINDOW_WIDTH;
    float alto = GLUT_WINDOW_HEIGHT;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (proy_orto) {
        glOrtho(-5, 5, -5, 5, -5, 5);    
    }
    else {
        
        gluPerspective(45, ancho/alto, 0.1, 1000);
        gluLookAt(20, 10, 20, 0, 0, 0, 0, 1, 0);
        // de donde (x,y,z) apuntando a (x,y,z), ultimos 3 no recuerdo xd
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
enum especial{ //creo que no las terminas usando
	//para las teclas especiales, las que no son letras
    /*	
    up = 72,
	left = 75,
	derecha = 77,
	down = 80,
    */
	esc = 27,
};
bool luz = true;
void teclado (unsigned char key, int x, int y) {
    key = toupper(key);
    //Es mas facil todas las letras mayusculas xD
    switch (key) {
        case 'P':
            //proy_orto = !proy_orto;
            //camara();
            break;
        /*
        case 'O': reproduciendo = !reproduciendo; break;
        */
        case ' ': player.disparar(); break;
        //TRASLACIÓN
        case 'W': ty += 0.1; break;
        case 'S': ty -= 0.1; break;
        case 'A': tx -= 0.1; break;
        case 'D': tx += 0.1; break;

        case 'R': restart(); break;
        

        case 'C': t+=10; break;



        //ROTACIÓN
        case 'I': case 'i': rx -= 5; break;
        case 'K': case 'k': rx += 5; break;
        case 'J': case 'j': ry += 5; t+=0; break;
        case 'L': case 'l': ry -= 5; t+=0; break;
  
        //ESCALA
        case '+': es += 0.1; break;
        case '-': es -= 0.1; break;
        
   		//Salida
        case esc: exit(0); break;
    }
    glutPostRedisplay();
}
void especiales(int key,int x,int y){
	switch(key){
		case GLUT_KEY_UP:

		break;
		case GLUT_KEY_DOWN:
            //(dino.dir != -1)?dino.dir = -1: pass();
            //if(dino.dir != -1){dino.dir = -1;}
		break;
		case GLUT_KEY_LEFT:
            if(!player.rotando){
            player.rotando = true;
            player.direccion = 1;
            player.destino += 90;
            }
		break;
		case GLUT_KEY_RIGHT:
            if(!player.rotando){
            player.rotando = true;
            player.direccion = -1;
            player.destino -= 90;
            }
		break;
	}
	glutPostRedisplay();
}

void redimensionar (int ancho, int alto) {
    /*
    if (alto == 0) alto = 1;
    int lado = ancho < alto ? ancho : alto;
    glViewport((ancho - lado) / 2, (alto - lado) / 2, lado, lado);*/
        float w = ancho, h = alto;
        glViewport(0,0,w,h); // Reajusta el tamaño del objeto con forme se avanza
        glMatrixMode(GL_PROJECTION); // Matriz de proyección, no afecta el objeto
        glLoadIdentity(); // Para eliminar los valores de la matriz que ya tenpia guardada.
        gluPerspective(60.0, (double)w / (double)h, 0.1, 30.0);
        gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
        glMatrixMode(GL_MODELVIEW); // Matriz que modela el objeto.
        glLoadIdentity(); // Para eliminar los valores de la matriz que ya tenpia guardada.
    camara();
}

void config_GLUT(void) {
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Botes");

    //glutFullScreen();

    glutDisplayFunc(dibujar);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(especiales);
    glutReshapeFunc(redimensionar);
}

void config_OGL(void) {
    glClearColor(0.0, 0.6, 0.9, 1.0);
    Luz(true,10,10);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    camara();
    glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    animacion(time(0));
	
}
int main(int argc, char** argv) {
    for(int i=0;i<10;i++){
        //numeros[i].recarga(i);
    }
    srand(time(NULL));
    glutInit(&argc, argv);
    config_GLUT();
    config_OGL();
    glutMainLoop();
    return 10;
}