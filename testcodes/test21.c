struct A {
  int ax;
  float ay[2];
} temp;

int main (struct A t);

int main (struct A t) {
  return 2;
}

int test() {
  float f = 0;
  int n = 0;
  f = main (temp);
  n = main(temp, 1.0);
}
