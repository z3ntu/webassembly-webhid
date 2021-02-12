#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <stdio.h>
#include "hidapi.h"

emscripten::val *nav_hid = NULL;

int hid_init(void)
{
    emscripten::val Navigator = emscripten::val::global("navigator");
    if (!Navigator.as<bool>()) {
        printf("No global AudioContext, trying webkitAudioContext\n");
        return -1;
    }

    *nav_hid = Navigator["hid"];
    if (!nav_hid->as<bool>()) {
        printf("DBG2 No global AudioContext, trying webkitAudioContext\n");
        nav_hid = NULL;
        return -1;
    }

    return 0;
}

hid_device * hid_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number)
{
    emscripten::val filters = emscripten::val::array();

    emscripten::val options = emscripten::val::object();
    options.set("filters", filters);

    emscripten::val devices = nav_hid->call<emscripten::val>("requestDevice", options).await();

    // TODO

    return NULL;
}

int main(int argc, char ** argv) {
    printf("Hello World\n");

    hid_init();

    //hid_open(0x1532, 0x0c00, NULL);

    emscripten::val devices = nav_hid->call<emscripten::val>("getDevices").await();
    std::vector<emscripten::val> devices2 = emscripten::vecFromJSArray<emscripten::val>(devices);

    struct hid_device_info *root = NULL; /* return object */
    struct hid_device_info *cur_dev = NULL;
    struct hid_device_info *prev_dev = NULL; /* previous device */

    for (emscripten::val &device : devices2) {
        printf("Got device: %s\n", device["productName"].as<std::string>().c_str());

        struct hid_device_info *tmp;

        /* VID/PID match. Create the record. */
        tmp = (struct hid_device_info*) calloc(1, sizeof(struct hid_device_info));
        if (cur_dev) {
            cur_dev->next = tmp;
        }
        else {
            root = tmp;
        }
        prev_dev = cur_dev;
        cur_dev = tmp;

        /* Fill out the record */
        cur_dev->next = NULL;
        //cur_dev->path = dev_path? strdup(dev_path): NULL;

        /* VID/PID */
        cur_dev->vendor_id = device["vendorId"].as<unsigned short>();
        cur_dev->product_id = device["productId"].as<unsigned short>();

        /* Serial Number */
//        cur_dev->serial_number = utf8_to_wchar_t(serial_number_utf8);

        /* Release Number */
//        cur_dev->release_number = 0x0;

        /* Interface Number */
//        cur_dev->interface_number = -1;

    }
    printf("Still alive!\n");
}

int hidtest()
{
    int res;
	unsigned char buf[256];
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	struct hid_device_info *devs, *cur_dev;

	if (hid_init())
		return -1;

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("  Usage (page): 0x%hx (0x%hx)\n", cur_dev->usage, cur_dev->usage_page);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

    return 0;
}
