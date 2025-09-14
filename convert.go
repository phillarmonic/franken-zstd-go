package franken_zstd

//#include <zend.h>
import "C"
import (
	"unsafe"
)

// zendStringToGoString converts a zend_string to a Go string
func zendStringToGoString(zendStr *C.zend_string) string {
	if zendStr == nil {
		return ""
	}
	return C.GoStringN((*C.char)(unsafe.Pointer(&zendStr.val)), C.int(zendStr.len))
}

// zendStringToGoBytes converts a zend_string to a Go byte slice
func zendStringToGoBytes(zendStr *C.zend_string) []byte {
	if zendStr == nil {
		return nil
	}
	return C.GoBytes(unsafe.Pointer(&zendStr.val), C.int(zendStr.len))
}

// goStringToZendString converts a Go string to a zend_string
// Note: The caller is responsible for freeing the returned zend_string
func goStringToZendString(str string) *C.zend_string {
	if str == "" {
		return nil
	}
	cStr := C.CString(str)
	defer C.free(unsafe.Pointer(cStr))

	// Create a new zend_string with the content
	return C.zend_string_init(cStr, C.size_t(len(str)), C._Bool(false))
}

// goBytesToZendString converts a Go byte slice to a zend_string
// Note: The caller is responsible for freeing the returned zend_string
func goBytesToZendString(data []byte) *C.zend_string {
	if len(data) == 0 {
		return nil
	}

	// Create a new zend_string with the binary data
	return C.zend_string_init((*C.char)(unsafe.Pointer(&data[0])), C.size_t(len(data)), C._Bool(false))
}

// External C functions that we need to declare
//extern zend_string *zend_string_init(const char *str, size_t len, int persistent);
//extern void free(void *ptr);
