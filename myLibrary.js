if (typeof mergeInto !== 'undefined') mergeInto(LibraryManager.library, {
    testExternalJSMethod: function() {
        alert("Hello world");
        let deviceFilter = { vendorId: 0x1532, productId: 0x0c00 };
        let requestParams = { filters: [deviceFilter] };
        navigator.hid.requestDevice(requestParams).then((devices) => {
            console.log(devices);
            if (devices.length == 0) return;
        });
    }
});
