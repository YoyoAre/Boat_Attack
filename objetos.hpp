//#include "figuras.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "cargadorOBJ_2.cpp"
#include <string>
#include<time.h>
#include<math.h>
#include"algebra.hpp"
using namespace std;
enum sim{
		derecha = 0,
		izquierda = 1
	};

//materiales
void metal(float r,float g,float b){
    //Definición de las componentes ambiental, difusa, especular y brillo.
    GLfloat comp_amb[] = {0.25, 0.25, 0.25, 1.0};
    GLfloat comp_dif[] = {(r/256), (g/256), (b/256), 1.0};
    GLfloat comp_esp[] = {0.774597, 0.774597, 0.774597, 1.0};
    GLfloat shine = 76.8;
    //Aplicación de las componentes ambiental, difusa, especular y brillo.
    glMaterialfv(GL_FRONT, GL_AMBIENT, comp_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, comp_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, comp_esp);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shine);
}
void normal(float r = 0.8*256,float g = 0.8*256, float b = 0.8*256){
	GLfloat comp_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat comp_dif[] = {(r/256), (g/256), (b/256), 1.0};
    GLfloat comp_esp[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat shine = 0;
    //Aplicación de las componentes ambiental, difusa, especular y brillo.
    glMaterialfv(GL_FRONT, GL_AMBIENT, comp_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, comp_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, comp_esp);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shine);
}
float distancia(float x,float y){
	return sqrt(x*x + y*y);
}
//HUD

class Hud{ 
    public:

    bool tiene_tex;             //Indicador de si el modelo tiene textura o no.
    GLuint id_tex;              //ID de la textura
    unsigned char *tex = NULL;              //Imagen de la textura.
    int ancho_tex, alto_tex, num_comps_tex; //Propiedades de la textura.
    GLenum modelo_color = GL_RGB;           //Modelo de color de la textura.
    Hud(){
        //config_tex("sample.png");

    }
    ~Hud(){
        stbi_image_free(tex);
    }
    void config_tex(std::string nombre_archivo) { 
        stbi_set_flip_vertically_on_load(true);  
    tiene_tex = true;       
        //Se cargan las texturas desde las imágenes.
        tex = stbi_load(nombre_archivo.c_str(), &ancho_tex, &alto_tex, &num_comps_tex, 0);
        if (tex == NULL) {
            std::cout << "Hubo un error abriendo el archivo de textura. No se usaran texturas.";
            tiene_tex = false;
            stbi_image_free(tex);
            return;
        }
        
        //Se obtiene el modelo de color de la textura
        if (num_comps_tex == 3) modelo_color = GL_RGB;
        else if (num_comps_tex == 4) modelo_color = GL_RGBA;
        else { 
            std::cout << "La textura tiene un modelo de color diferente a RGB o RGBA. No se usaran texturas.\n";
            tiene_tex = false;
            stbi_image_free(tex);
            return;
        }
        
        //Se genera el id de la textura.
        glGenTextures(1, &id_tex);
    }
    void dibujar(){
        glEnable(GL_TEXTURE_2D);
        //Se habilita, configura y aplica la textura.
        glBindTexture(GL_TEXTURE_2D, id_tex);
        
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


        glTexImage2D(GL_TEXTURE_2D, 0, modelo_color, ancho_tex, alto_tex, 
                    0, modelo_color, GL_UNSIGNED_BYTE, tex);
        
        //nota: materiales, si puedes quitalos
        GLfloat comp_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat comp_dif[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat comp_esp[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat shine = 0;
    //Aplicación de las componentes ambiental, difusa, especular y brillo.
    glMaterialfv(GL_FRONT, GL_AMBIENT, comp_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, comp_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, comp_esp);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shine);
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0,1.0); glVertex2f(0.05,0.05);
        glTexCoord2f(1.0,1.0); glVertex2f(0.3,0.05);
        glTexCoord2f(1.0,0.0); glVertex2f(0.3,0.15);
        glTexCoord2f(0.0,0.0); glVertex2f(0.05,0.15);
        glEnd();


        //Se deshabilitan las texturas.
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);


    }

};


//cosas

class Hitbox
{
public:
	float x,y,r;
	Hitbox(){};
	Hitbox(float x0,float y0, float r0){
		x = x0;
		y = y0;
		r = r0;
	}
	Hitbox operator =(Hitbox a){
		this->x = a.x;
		this->y = a.y;
		this->r = a.r;
		return *this;
	}
	void dibujar(){
		glPushMatrix();
			glTranslatef(x,0,y);
			glutSolidSphere(r,10,10);
		glPopMatrix();
	}
};



bool choque(Hitbox a,Hitbox b){
	bool res = float(distancia(a.x - b.x, a.y-b.y)) < (a.r + b.r); //decia b.y
	return res;
	/*
	if(res){
		return true;
	}else{
		return false;
	}*/
}

class Bala{
	public:
	bool activa;
	float x,y,z; int angulo;
	Hitbox caja;
	Bala(){
		activa = false;
	}
	Bala(float x0, float y0, float z0, int theta){
		x = x0; y = y0; z = z0;
		caja = Hitbox(x,z,0.4);
		angulo = theta;
		activa = true;
	}
	Bala operator =(Bala a){
		this->x = a.x;
		this->activa = a.activa;
		this->z = a.z;
		this->angulo = a.angulo;

		return *this;
	}
	void desactiva(){
		activa = false;
	}
	void mover(float dtime){
		if(activa){
			float velocidad = 0.1;

			x = x + cos(radian(angulo)) * (velocidad) * dtime;
			z = z - sin(radian(angulo)) * (velocidad) * dtime;

			caja = Hitbox(x,z,0.4);
		}
	}
	void dibujar(){
		if(activa){
			metal(0,0,0);
			caja.dibujar();
		}
	}
};

class Bote{
public:
	int ataque,vida,direccion;
	bool rotando, enemigo;
	int angulo; int disparos;
	float x,y,z,escala,velocidad;
	int destino;
	int cooldown;
	ModeloOBJ bote,bandera,arma;
	Hitbox caja;
	Bala bala[3];
	Bote(float x0 = 0.0, float z0 = 0.0, int theta = 0,
	 bool enemy = true, int vid = 0, int disp = 1, int daño = 1,
	 float speed = 0.01){
		velocidad = speed; ataque = daño; vida = vid; direccion = 1;
		x= x0;y = 0; z = z0; angulo = theta;
		rotando = false; enemigo = enemy; disparos = disp;
		escala = 0.45;
		destino = angulo;
		cooldown = 300;
		caja = Hitbox(x,z,1*escala);
		bote.cargarOBJ("./bote/cuerpo.obj");
		bote.config_tex("./bote/textura.jpg");

		arma.cargarOBJ("./bote/cañon.obj");

		bandera.cargarOBJ("./bote/bandera.obj");
		int i;
		for(i = 0;i<disparos;i++){
			bala[i] = Bala();
		}
	}
	void dibujar(){
		//if(true){
		if(vida > 0){
			normal();
			glPushMatrix();
				glTranslatef(x,y,z);
				
				glRotatef(angulo,0,1,0);
				
				glScalef(escala,escala,escala);
				normal();
				bote.dibujar();

				metal(227,228,229);
				arma.dibujar_sin_tex();
				//si no es enemigo es azul, si es enemigo es rojo
				!enemigo ? normal(0,80,220) : normal(200,0,0);
				glPushMatrix();
					glRotatef(90,0,1,0);
					bandera.dibujar_sin_tex();
				glPopMatrix();
			glPopMatrix();
			int i;
			for(i = 0;i<disparos;i++){
				bala[i].dibujar();
			}
		}
	}
	void revivir(){
		if(vida <= 0){
			cooldown -= 1;
		}
		if(cooldown <= 0){
			vida = 2;
			//x = -15; z = 0; // randomizar
			cooldown = 200 + (rand()%300); //randomizar

			int ote = rand()%4;
			switch (ote){
			case 0:
				x = -15;
				z = 0;
				angulo = 0;
				break;
			case 1:
				x = 15;
				z = 0;
				angulo = 180;
				break;
			case 2:
				x = 0;
				z = 15;
				angulo = 90;
				break;
			case 3:
				x = 0;
				z = -15;
				angulo = 270;
				break;
			}
			
		}
	}
	void animar(float dtime = 1){
		if(rotando){
			angulo += velocidad * direccion * dtime;
		
			if(angulo % 90 < velocidad * dtime){
				rotando = false;
				angulo = destino;
				angulo = angulo % 360;
				//angulo = int(angulo / 90) * 90;
			}
		}
		desarmar();
		revivir();
		
	}
	void mover(float dtime = 0.04){
		if(vida > 0){
			if(enemigo){
				float min = 7;
				x = x + cos(radian(angulo)) * (velocidad) * dtime;
				z = z - sin(radian(angulo)) * (velocidad) * dtime;
				caja = Hitbox(x,z,1*escala);

				if(float(distancia(x , z)) < (min)){
					disparar();
				}
			}
			int i;
			for(i = 0;i<disparos;i++){
				if(bala[i].activa){
					bala[i].mover(dtime);
				}
			}
		}
	}
	void disparar(){
		int i; bool puede = true;
		float x0,z0;
		for(i = 0;i<disparos;i++){
			if(!bala[i].activa && puede){
				puede = false;
				x0 = x + cos(radian(angulo)) * (0.5);
				z0 = z - sin(radian(angulo)) * (0.5);
				bala[i] = Bala(x0,y,z0,angulo);
				//printf("Bala disparada\n");
			}
		}
	}
	void desarmar(){
		int i,max = 10;
		for(i = 0;i<disparos;i++){
			if(bala[i].activa){
				if(float(distancia(x - bala[i].x, z-bala[i].z)) > (max)){
					bala[i].desactiva();
				}
			}
		}
	}
};
void ataque(Bote *atacante,Bote *receptor){
	int i;
	if(receptor->vida > 0 && atacante->vida > 0){
		for(i = 0;i < atacante->disparos;i++){
			if(atacante->bala[i].activa /*&& receptor->vida > 0*/){
					if(choque(atacante->bala[i].caja,receptor->caja)){
						receptor->vida -= atacante->ataque;
						atacante->bala[i].desactiva();
					}
				}
		}
	}
}
void empate(Bote *atacante,Bote *receptor){
	int i,j;
	if(receptor->vida > 0 && atacante->vida > 0){
		for(i = 0;i < atacante->disparos;i++){
			for(j = 0;j < receptor->disparos;j++){
				if(atacante->bala[i].activa && receptor->bala[j].activa){
						if(choque(atacante->bala[i].caja,receptor->bala[j].caja)){
							atacante->bala[i].desactiva();
							receptor->bala[j].desactiva();
						}
					}
			}
		}
	}	
}
void status(Bote *b1,Bote *b2){
	ataque(b1,b2);
	ataque(b2,b1);
	empate(b1,b2);
}

class Mar{
public:
ModeloOBJ awa;
Mar(){
	awa.config_tex("./mar/textura.jpg");
}
void dibujar(){
	glPushMatrix();
	glRotatef(45,0,1,0);
	glEnable(GL_TEXTURE_2D);
        //Se habilita, configura y aplica la textura.
        glBindTexture(GL_TEXTURE_2D, awa.id_tex);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


        glTexImage2D(GL_TEXTURE_2D, 0, awa.modelo_color, awa.ancho_tex, awa.alto_tex, 
                    0, awa.modelo_color, GL_UNSIGNED_BYTE, awa.tex);
        //glGenerateMipmap(GL_TEXTURE_2D);

		normal(0,80,200);
        glColor3f(1,1,1);
		glBegin(GL_QUADS);
		float l = 20, h = 0.0;
        glTexCoord2f(0.0,1.0); glVertex3f(-l,h,-l);
        glTexCoord2f(1.0,1.0); glVertex3f( l,h,-l);
        glTexCoord2f(1.0,0.0); glVertex3f( l,h, l);
        glTexCoord2f(0.0,0.0); glVertex3f(-l,h, l);
        glEnd();
        //Se deshabilitan las texturas.
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
};

// luces 

enum tipo_luz{
	direccional = 0,
	puntual = 1
};


void luz_amb(GLenum  light = GL_LIGHT0,float r = 0.0,float g = 0.0, float b = 0.0, float alfa = 1.0){
	GLfloat comp[] = {abs(r)<=1?r:r/256, abs(g)<=1?g:g/256, abs(b)<=1?b:b/256, alfa};
	glLightfv(light, GL_AMBIENT, comp);
}
void luz_dif(GLenum  light = GL_LIGHT0,float r = 1.0,float g = 1.0, float b = 1.0, float alfa = 1.0){
	GLfloat comp[] = {abs(r)<=1?r:r/256, abs(g)<=1?g:g/256, abs(b)<=1?b:b/256, alfa};
	glLightfv(light, GL_DIFFUSE, comp);
}
void luz_esp(GLenum  light = GL_LIGHT0,float r = 1.0,float g = 1.0, float b = 1.0, float alfa = 1.0){
	GLfloat comp[] = {abs(r)<=1?r:r/256, abs(g)<=1?g:g/256, abs(b)<=1?b:b/256, alfa};
	glLightfv(light, GL_SPECULAR, comp);
}
void luz_pos(GLenum  light = GL_LIGHT0,Punto v = Punto(0,0,1),int tipo = 0){
	GLfloat comp[] = {v.x, v.y, v.z, float(tipo)};
	glLightfv(light, GL_POSITION, comp);
}
void luz_dir(GLenum  light = GL_LIGHT0,Punto v = Punto(0,0,1)) {
	GLfloat comp[] = {v.x, v.y, v.z};
	glLightfv(light, GL_SPOT_DIRECTION, comp);
}
void Luz(bool dia,float x, float y){

	float l;
	l = dia?0:150;

	luz_amb(GL_LIGHT0,l,l,l);
	luz_dif(GL_LIGHT0,0.9,0.9,0.9);
	luz_esp(GL_LIGHT0,0.9,0.9,0.9);
	luz_pos(GL_LIGHT0,Vector(x,y,10),puntual);
	//luz_dir(GL_LIGHT0,normalizar(Vector(x,y,0)));
}