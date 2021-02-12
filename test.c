#include <stdio.h>

extern void testExternalJSMethod();

int main(int argc, char ** argv) {
    printf("Hello World\n");
    testExternalJSMethod();
}
