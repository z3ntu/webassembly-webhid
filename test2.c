#include <emscripten.h>
#include <stdio.h>

EM_JS(void, do_fetch, (), {
  Asyncify.handleAsync(async () => {
    out("waiting for a fetch");
    const response = await fetch("hello.html");
    out("got the fetch response");
    // (normally you would do something with the fetch here)
  });
});

EM_JS(void, js_get_devices, (), {
  Asyncify.handleAsync(async () => {
    out("waiting for a fetch");
    const response = await navigator.hid.getDevices();
    console.log(response);
    out("got the fetch response");
    // (normally you would do something with the fetch here)
  });
});

int main(int argc, char ** argv) {
    printf("Hello World\n");
    //do_fetch();
    js_get_devices();
}
