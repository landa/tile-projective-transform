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

void drawTileOutline(GLfloat tile_definition[4][3]) {
  glLineWidth(10.0f);
  glBegin(GL_LINE_LOOP);
  glColor3f(1.0, 1.0, 1.0);
  glVertex3fv(&tile_definition[3][0]);
  glVertex3fv(&tile_definition[2][0]);
  glVertex3fv(&tile_definition[1][0]);
  glVertex3fv(&tile_definition[0][0]);
  glEnd();
  glBegin(GL_LINES);
  for (int ii = 0; ii < 20; ii++) {                                    
    glVertex3f(cv::Point(0, (int) ii*spacing),
    glVertex3f(cv::Point(old.cols, (int) ii*spacing));    

    glVertex3f(0, (int) ii*spacing),
    glVertex3f(old.cols, (int) ii*spacing));    
  }                                                             
  spacing = old.cols/20.0;                               
  for (int ii = 0; ii < 20; ii++) {                                             
    line(old, cv::Point((int) ii*spacing, 0), cv::Point((int) ii*spacing, old.rows), cv::Scalar(0, 255, 0), 3);    
  }
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
  gluPerspective(40.0f, 3/2.0f, 0.1f, 100.0f);

  // move the projector to the camera pose
  glMatrixMode(GL_MODELVIEW);
  glRotatef(camera_angle, 0.0f, 1.0f, 0.0f);
  glTranslatef(camera_origin[0], camera_origin[1], camera_origin[2]);

  // gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);



  // Set up the virtual camera

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glPushMatrix();
  gluPerspective(53.13f, 1.0f, 0.1f, 100.0f);
  fprintf(stderr, "moving camera right by %f, up by %f, back by %f\n",
                  camera_origin[0], camera_origin[1], camera_origin[2]);
  // glRotatef(camera_angle, 0.0f, 1.0f, 0.0f);
  // glTranslatef(camera_origin[0], camera_origin[1], camera_origin[2]);
  GLfloat tile_17_camera[3];
  GLfloat tile_17_offset[3];
  vec_scalar_mult(tile_17_normal, 1.1f, tile_17_offset);
  vec_add(tile_17_origin, tile_17_offset, tile_17_camera);
  // gluLookAt(0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
  gluLookAt(tile_17_camera[0], tile_17_camera[1]-2.0f, tile_17_camera[2], tile_17_origin[0], tile_17_origin[1]-2.0f, tile_17_origin[2], 0.0f, 1.0f, 0.0f);


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

drawTileOutline(tile_0_corners);
drawTileOutline(tile_1_corners);
drawTileOutline(tile_2_corners);
drawTileOutline(tile_3_corners);
drawTileOutline(tile_4_corners);
drawTileOutline(tile_5_corners);
drawTileOutline(tile_6_corners);
drawTileOutline(tile_7_corners);
drawTileOutline(tile_8_corners);
drawTileOutline(tile_9_corners);
drawTileOutline(tile_10_corners);
drawTileOutline(tile_11_corners);
drawTileOutline(tile_12_corners);
drawTileOutline(tile_13_corners);
drawTileOutline(tile_14_corners);
drawTileOutline(tile_15_corners);
drawTileOutline(tile_16_corners);
drawTileOutline(tile_17_corners);
drawTileOutline(tile_18_corners);
drawTileOutline(tile_19_corners);
drawTileOutline(tile_20_corners);
drawTileOutline(tile_21_corners);
drawTileOutline(tile_22_corners);
drawTileOutline(tile_23_corners);
drawTileOutline(tile_24_corners);
drawTileOutline(tile_25_corners);
drawTileOutline(tile_26_corners);
drawTileOutline(tile_27_corners);
drawTileOutline(tile_28_corners);
drawTileOutline(tile_29_corners);
drawTileOutline(tile_30_corners);
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
