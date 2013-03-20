void vec_cross_product(GLfloat vec1[3], GLfloat vec2[3], GLfloat res[3]) {
  res[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
  res[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
  res[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
}

void vec_scalar_mult(GLfloat vec[3], GLfloat k, GLfloat res[3]) {
  res[0] = k * vec[0];
  res[1] = k * vec[1];
  res[2] = k * vec[2];
}

void vec_add(GLfloat vec1[3], GLfloat vec2[3], GLfloat res[3]) {
  res[0] = vec1[0] + vec2[0];
  res[1] = vec1[1] + vec2[1];
  res[2] = vec1[2] + vec2[2];
}
