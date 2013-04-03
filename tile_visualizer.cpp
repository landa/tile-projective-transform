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
#include <unistd.h>

using namespace std;

GLuint textureId;

int look_at = 0;
double move_forward = 0.0f;
double move_right = 0.0f;
double angle = 0.0f;

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
  return 1.0f - 0.1f*rand()/(float)RAND_MAX;
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
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3fv(&tile_definition[3][0]);
  glVertex3fv(&tile_definition[2][0]);
  glVertex3fv(&tile_definition[1][0]);
  glVertex3fv(&tile_definition[0][0]);
  glEnd();
/*
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
*/
}

GLuint loadTexture() {
  textureId = SOIL_load_OGL_texture(
    "roll.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
      SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
  );
}

void draw() {

  srand(0);

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
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );
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
  gluPerspective(49.35, 4.0f/3.0f, 0.1f, 100.0f);

  // move the projector to the camera pose
  glRotatef(1.6f, 0.0f, 0.0f, 1.0f);
  glRotatef(-camera_angle*180/M_PI, 0.0f, 1.0f, 0.0f);
  glTranslatef(-camera_origin[0], -camera_origin[1], -camera_origin[2]);



  // Set up the virtual camera

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glPushMatrix();
  gluPerspective(53.13f, 1.0f, 0.1f, 100.0f);
  fprintf(stderr, "moving camera right by %f, up by %f, back by %f\n",
                  camera_origin[0], camera_origin[1], camera_origin[2]);
  glRotatef(camera_angle, 0.0f, 1.0f, 0.0f);
  glTranslatef(camera_origin[0], camera_origin[1], camera_origin[2]);

  GLfloat tile_0_camera[3];
  GLfloat tile_0_offset[3];
  vec_scalar_mult(tile_0_normal, 1.0f, tile_0_offset);
  vec_add(tile_0_origin, tile_0_offset, tile_0_camera);

  GLfloat tile_1_camera[3];
  GLfloat tile_1_offset[3];
  vec_scalar_mult(tile_1_normal, 1.0f, tile_1_offset);
  vec_add(tile_1_origin, tile_1_offset, tile_1_camera);

  GLfloat tile_17_camera[3];
  GLfloat tile_17_offset[3];
  vec_scalar_mult(tile_17_normal, 1.0f, tile_17_offset);
  vec_add(tile_17_origin, tile_17_offset, tile_17_camera);

  GLfloat tile_18_camera[3];
  GLfloat tile_18_offset[3];
  vec_scalar_mult(tile_18_normal, 1.0f, tile_18_offset);
  vec_add(tile_18_origin, tile_18_offset, tile_18_camera);

  GLfloat tile_19_camera[3];
  GLfloat tile_19_offset[3];
  vec_scalar_mult(tile_19_normal, 1.0f, tile_19_offset);
  vec_add(tile_19_origin, tile_19_offset, tile_19_camera);

  GLfloat tile_20_camera[3];
  GLfloat tile_20_offset[3];
  vec_scalar_mult(tile_20_normal, 1.0f, tile_20_offset);
  vec_add(tile_20_origin, tile_20_offset, tile_20_camera);

  GLfloat tile_21_camera[3];
  GLfloat tile_21_offset[3];
  vec_scalar_mult(tile_21_normal, 1.0f, tile_21_offset);
  vec_add(tile_21_origin, tile_21_offset, tile_21_camera);

  GLfloat tile_22_camera[3];
  GLfloat tile_22_offset[3];
  vec_scalar_mult(tile_22_normal, 1.0f, tile_22_offset);
  vec_add(tile_22_origin, tile_22_offset, tile_22_camera);

  GLfloat tile_23_camera[3];
  GLfloat tile_23_offset[3];
  vec_scalar_mult(tile_23_normal, 1.0f, tile_23_offset);
  vec_add(tile_23_origin, tile_23_offset, tile_23_camera);

  glTranslatef(0.0f-move_right, 0.0f, 0.0f+move_forward);
  glRotatef(angle, 0.0f, 1.0f, 0.0f);
  if (look_at == 1) {
    gluLookAt(tile_1_camera[0], tile_1_camera[1]-2.0f, tile_1_camera[2], tile_1_origin[0], tile_1_origin[1]-2.0f, tile_1_origin[2], 0.0f, 1.0f, 0.0f);
  } else if (look_at == 17) {
    gluLookAt(tile_17_camera[0], tile_17_camera[1]-2.0f, tile_17_camera[2], tile_17_origin[0], tile_17_origin[1]-2.0f, tile_17_origin[2], 0.0f, 1.0f, 0.0f);
  } else if (look_at == 18) {
    gluLookAt(tile_18_camera[0], tile_18_camera[1]-2.0f, tile_18_camera[2], tile_18_origin[0], tile_18_origin[1]-2.0f, tile_18_origin[2], 0.0f, 1.0f, 0.0f);
  } else if (look_at == 19) {
    gluLookAt(tile_19_camera[0], tile_19_camera[1]-2.0f, tile_19_camera[2], tile_19_origin[0], tile_19_origin[1]-2.0f, tile_19_origin[2], 0.0f, 1.0f, 0.0f);
  }


/*
  // draw floor and ceiling
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f(-100.0f, 5.0f, 100.0f);
  glVertex3f(-100.0f, 5.0f, -100.0f);
  glVertex3f(100.0f, 5.0f, -100.0f);
  glVertex3f(100.0f, 5.0f, 100.0f);
  glVertex3f(-100.0f, 0.0f, 100.0f);
  glVertex3f(-100.0f, 0.0f, -100.0f);
  glVertex3f(100.0f, 0.0f, -100.0f);
  glVertex3f(100.0f, 0.0f, 100.0f);
  glEnd();
*/

  // Draw all of the tiles

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();
  glLoadIdentity();
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
drawTileOutline(tile_31_corners);
drawTileOutline(tile_32_corners);
drawTileOutline(tile_33_corners);
drawTileOutline(tile_34_corners);
drawTileOutline(tile_35_corners);
drawTileOutline(tile_36_corners);
drawTileOutline(tile_37_corners);
drawTileOutline(tile_38_corners);
drawTileOutline(tile_39_corners);
drawTileOutline(tile_40_corners);
drawTileOutline(tile_41_corners);
drawTileOutline(tile_42_corners);
drawTileOutline(tile_43_corners);
drawTileOutline(tile_44_corners);
drawTileOutline(tile_45_corners);
drawTileOutline(tile_46_corners);
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

void keyHandler(unsigned char key, int x, int y) {
  if (key == '1') look_at = 1;
  if (key == 'e') look_at = 18;
  if (key == 'w') look_at = 19;
  if (key == 'q') look_at = 20;
  if (key == 'l') move_right += 0.5f;
  if (key == 'h') move_right -= 0.5f;
  if (key == 'j') move_forward += 0.5f;
  if (key == 'k') move_forward -= 0.5f;
  if (key == 'a') angle -= 2.0f;
  if (key == 's') angle += 2.0f;
  draw();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Tile Localization");
  glViewport(0,0,800,800);

  glutDisplayFunc(&draw);
  glutKeyboardFunc(&keyHandler);
  glutMainLoop();
  return 0;
}
