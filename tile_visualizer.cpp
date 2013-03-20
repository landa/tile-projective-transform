#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include "SOIL.h"
#include "tile_definitions.hpp"
#include "colors.hpp"
#include "tile_math.hpp"
#include <ctime>
#include <iostream>

using namespace std;

GLuint textureId;

//  The points below use the scanmatcher coordinate system.
//  To convert to the OpenGL coordinate system, I used the
//  following transformation:
//    x' = -y
//    y' = z
//    z' = -x
void convertCoordinates(GLfloat* point) {
  GLfloat cpy[3] = { point[0], point[1], point[2] };
  point[0] = -cpy[1];
  point[1] = cpy[2];
  point[2] = -cpy[0];
}

float rf() {
  return (float)rand()/(float)RAND_MAX;
}

void drawTile(GLfloat tile_definition[4][3]) {
  glBegin(GL_QUADS);
  glColor3f(rf(), rf(), rf());
  glVertex3fv(&tile_definition[0][0]);
  glVertex3fv(&tile_definition[1][0]);
  glVertex3fv(&tile_definition[2][0]);
  glVertex3fv(&tile_definition[3][0]);
  glEnd();
}

GLuint loadTexture() {
  textureId = SOIL_load_OGL_texture(
    "scene_image.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
      SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );
}

void draw() {

  long start_time = clock();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  


  // enable and prime our loaded texture
  loadTexture();
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_GEN_S); 
  glEnable(GL_TEXTURE_GEN_T); 
  glEnable(GL_TEXTURE_GEN_R); 
  glEnable(GL_TEXTURE_GEN_Q); 
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, (int) GL_EYE_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, (int) GL_EYE_LINEAR);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, (int) GL_EYE_LINEAR);
  glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, (int) GL_EYE_LINEAR);
  // These calls initialize the TEXTURE_MAPPING function to identity. We will be
  // using the Texture matrix stack to establish this mapping indirectly.
  float eyePlaneS[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
  float eyePlaneT[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
  float eyePlaneR[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
  float eyePlaneQ[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  glTexGenfv(GL_S, GL_EYE_PLANE, eyePlaneS);
  glTexGenfv(GL_T, GL_EYE_PLANE, eyePlaneT);
  glTexGenfv(GL_R, GL_EYE_PLANE, eyePlaneR);
  glTexGenfv(GL_Q, GL_EYE_PLANE, eyePlaneQ);
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  glTranslated(0.5, 0.5, 0.5); // Scale and bias the [-1,1] NDC values
  glScaled(0.5, 0.5, 0.5); // to the [0,1] range of the texture map
  gluPerspective(40.0f, 1024/768.0f, 0.1f, 100.0f);

  // move the projector to the camera pose
  glTranslatef(camera_origin[0], camera_origin[1], camera_origin[2]);
  glRotatef(camera_angle, 0.0f, 1.0f, 0.0f);

  // gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);



  // Set up the camera

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glPushMatrix();
  gluPerspective(40.0f, 1024/768.0f, 0.1f, 100.0f);
  fprintf(stderr, "moving camera right by %f, up by %f, back by %f\n",
                  camera_origin[0], camera_origin[1], camera_origin[2]);
  glTranslatef(camera_origin[0], camera_origin[1], camera_origin[2]);
  glRotatef(camera_angle, 0.0f, 1.0f, 0.0f);
  GLfloat tile_19_camera[3];
  GLfloat tile_19_offset[3];
  vec_scalar_mult(tile_19_normal, 1.0f, tile_19_offset);
  vec_add(tile_19_origin, tile_19_offset, tile_19_camera);
  // gluLookAt(0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
  // gluLookAt(tile_19_camera[0], tile_19_camera[1]-2.0f, tile_19_camera[2], tile_19_origin[0], tile_19_origin[1]-2.0f, tile_19_origin[2], 0.0f, 1.0f, 0.0f);


/*
  // draw floor and ceiling
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f(-100.0f, 0.0f, 100.0f);
  glVertex3f(-100.0f, 0.0f, -100.0f);
  glVertex3f(100.0f, 0.0f, -100.0f);
  glVertex3f(100.0f, 0.0f, 100.0f);
  glColor3f(0.5f, 0.5f, 0.5f);
  glVertex3f(-100.0f, 2.0f, 100.0f);
  glVertex3f(-100.0f, 2.0f, -100.0f);
  glVertex3f(100.0f, 2.0f, -100.0f);
  glVertex3f(100.0f, 2.0f, 100.0f);
  glEnd();
*/

  // Draw all of the tiles

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();
  glLoadIdentity();
drawTile(tile_0_corners);
drawTile(tile_1_corners);
drawTile(tile_2_corners);
drawTile(tile_3_corners);
drawTile(tile_4_corners);
drawTile(tile_5_corners);
drawTile(tile_6_corners);
drawTile(tile_7_corners);
drawTile(tile_8_corners);
drawTile(tile_9_corners);
drawTile(tile_10_corners);
drawTile(tile_11_corners);
drawTile(tile_12_corners);
drawTile(tile_13_corners);
drawTile(tile_14_corners);
drawTile(tile_15_corners);
drawTile(tile_16_corners);
drawTile(tile_17_corners);
drawTile(tile_18_corners);
drawTile(tile_19_corners);
drawTile(tile_20_corners);
drawTile(tile_21_corners);
drawTile(tile_22_corners);
drawTile(tile_23_corners);
drawTile(tile_24_corners);
drawTile(tile_25_corners);
drawTile(tile_26_corners);
drawTile(tile_27_corners);
drawTile(tile_28_corners);
drawTile(tile_29_corners);
drawTile(tile_30_corners);
drawTile(tile_31_corners);
drawTile(tile_32_corners);
drawTile(tile_33_corners);
drawTile(tile_34_corners);
drawTile(tile_35_corners);
drawTile(tile_36_corners);
drawTile(tile_37_corners);
drawTile(tile_38_corners);
drawTile(tile_39_corners);
drawTile(tile_40_corners);
drawTile(tile_41_corners);
drawTile(tile_42_corners);
drawTile(tile_43_corners);
drawTile(tile_44_corners);
drawTile(tile_45_corners);
drawTile(tile_46_corners);
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_GEN_S); 
  glDisable(GL_TEXTURE_GEN_T); 
  glDisable(GL_TEXTURE_GEN_R); 
  glDisable(GL_TEXTURE_GEN_Q); 
  glutSwapBuffers();
  long end_time = clock();
  double seconds = (double)(end_time - start_time)/CLOCKS_PER_SEC;
  cerr << "Running time: " << seconds << endl;
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Tile Localization");
  glViewport(0,0,800,800);

  glutDisplayFunc(&draw);
  glutMainLoop();
  return 0;
}
