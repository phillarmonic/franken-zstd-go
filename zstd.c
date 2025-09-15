#include <php.h>
#include <zend_exceptions.h>
#include <ext/standard/info.h>
#include "zstd.h"
#include "zstd_arginfo.h"

// Contains symbols exported by Go
#include "_cgo_export.h"

// Exception class entry
zend_class_entry *compression_exception_ce;

// Compressor class entry
zend_class_entry *compressor_ce;

// Object structure for Compressor instances
typedef struct {
    zend_string *name;
    int level;
    int window_size;
    zend_object std;
} compressor_object;

static inline compressor_object *compressor_from_obj(zend_object *obj) {
    return (compressor_object*)((char*)(obj) - XtOffsetOf(compressor_object, std));
}

#define Z_COMPRESSOR_OBJ_P(zv) compressor_from_obj(Z_OBJ_P(zv))

// Compressor::create
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, create)
{
    zend_long level = 3;
    zend_long window_size = 22;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(level)
        Z_PARAM_LONG(window_size)
    ZEND_PARSE_PARAMETERS_END();

    // Validate parameters
    if (level < 1 || level > 22) {
        zend_throw_exception_ex(compression_exception_ce, 0, "Invalid compression level: " ZEND_LONG_FMT " (must be 1-22)", level);
        RETURN_THROWS();
    }
    
    if (window_size < 10 || window_size > 31) {
        zend_throw_exception_ex(compression_exception_ce, 0, "Invalid window size: " ZEND_LONG_FMT " (must be 10-31)", window_size);
        RETURN_THROWS();
    }

    // Create new compressor object
    object_init_ex(return_value, compressor_ce);
    compressor_object *obj = Z_COMPRESSOR_OBJ_P(return_value);
    
    // Generate unique name for this compressor instance
    char name_buf[64];
    snprintf(name_buf, sizeof(name_buf), "compressor_%p", (void*)obj);
    obj->name = zend_string_init(name_buf, strlen(name_buf), 0);
    obj->level = (int)level;
    obj->window_size = (int)window_size;

    // Create the Go compressor
    char *error = go_compressor_create(obj->name, level, window_size);
    if (error) {
        zend_throw_exception_ex(compression_exception_ce, 0, "Failed to create compressor: %s", error);
        free(error);
        RETURN_THROWS();
    }
}

// Compressor::compress
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, compress)
{
    zend_string *data;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(data)
    ZEND_PARSE_PARAMETERS_END();

    compressor_object *obj = Z_COMPRESSOR_OBJ_P(ZEND_THIS);
    
    char *result, *error;
    int result_len;
    result = NULL;
    error = NULL;
    
    go_compressor_compress(obj->name, data, &result, &result_len, &error);
    
    if (error) {
        zend_throw_exception_ex(compression_exception_ce, 0, "Compression failed: %s", error);
        free(error);
        if (result) free(result);
        RETURN_THROWS();
    }
    
    if (result) {
        RETVAL_STRINGL(result, result_len);
        free(result);
    } else {
        RETURN_EMPTY_STRING();
    }
}

// Compressor::decompress
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, decompress)
{
    zend_string *compressed_data;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(compressed_data)
    ZEND_PARSE_PARAMETERS_END();

    compressor_object *obj = Z_COMPRESSOR_OBJ_P(ZEND_THIS);
    
    char *result, *error;
    int result_len;
    result = NULL;
    error = NULL;
    
    go_compressor_decompress(obj->name, compressed_data, &result, &result_len, &error);
    
    if (error) {
        zend_throw_exception_ex(compression_exception_ce, 0, "Decompression failed: %s", error);
        free(error);
        if (result) free(result);
        RETURN_THROWS();
    }
    
    if (result) {
        RETVAL_STRINGL(result, result_len);
        free(result);
    } else {
        RETURN_EMPTY_STRING();
    }
}

// Compressor::getStats
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, getStats)
{
    ZEND_PARSE_PARAMETERS_NONE();

    compressor_object *obj = Z_COMPRESSOR_OBJ_P(ZEND_THIS);
    
    zend_long original_size, compressed_size, compression_time;
    double compression_ratio;
    char *error = NULL;
    
    go_compressor_get_stats(obj->name, &original_size, &compressed_size, &compression_ratio, &compression_time, &error);
    
    if (error) {
        zend_throw_exception_ex(compression_exception_ce, 0, "Failed to get stats: %s", error);
        free(error);
        RETURN_THROWS();
    }
    
    array_init(return_value);
    add_assoc_long(return_value, "originalSize", original_size);
    add_assoc_long(return_value, "compressedSize", compressed_size);
    add_assoc_double(return_value, "compressionRatio", compression_ratio);
    add_assoc_double(return_value, "compressionTime", (double)compression_time / 1000000000.0); // Convert nanoseconds to seconds
}

// Compressor::close
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, close)
{
    ZEND_PARSE_PARAMETERS_NONE();

    compressor_object *obj = Z_COMPRESSOR_OBJ_P(ZEND_THIS);
    
    if (obj->name) {
        char *error = go_compressor_close(obj->name);
        if (error) {
            zend_throw_exception_ex(compression_exception_ce, 0, "Failed to close compressor: %s", error);
            free(error);
            RETURN_THROWS();
        }
        
        zend_string_release(obj->name);
        obj->name = NULL;
    }
}

// Quick compress function
ZEND_FUNCTION(Phillarmonic_FrankenZstd_compress)
{
    zend_string *data;
    zend_long level = 3;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_STR(data)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(level)
    ZEND_PARSE_PARAMETERS_END();

    char *result, *error;
    int result_len;
    result = NULL;
    error = NULL;
    
    go_quick_compress(data, level, &result, &result_len, &error);
    
    if (error) {
        zend_throw_exception_ex(compression_exception_ce, 0, "Compression failed: %s", error);
        free(error);
        if (result) free(result);
        RETURN_THROWS();
    }
    
    if (result && result_len > 0) {
        RETVAL_STRINGL(result, result_len);
        free(result);
    } else {
        RETURN_EMPTY_STRING();
    }
}

// Quick decompress function
ZEND_FUNCTION(Phillarmonic_FrankenZstd_decompress)
{
    zend_string *compressed_data;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(compressed_data)
    ZEND_PARSE_PARAMETERS_END();

    char *result, *error;
    int result_len;
    result = NULL;
    error = NULL;
    
    go_quick_decompress(compressed_data, &result, &result_len, &error);
    
    if (error) {
        zend_throw_exception_ex(compression_exception_ce, 0, "Decompression failed: %s", error);
        free(error);
        if (result) free(result);
        RETURN_THROWS();
    }
    
    if (result && result_len > 0) {
        RETVAL_STRINGL(result, result_len);
        free(result);
    } else {
        RETURN_EMPTY_STRING();
    }
}

// Get zstd info function
ZEND_FUNCTION(Phillarmonic_FrankenZstd_getInfo)
{
    ZEND_PARSE_PARAMETERS_NONE();

    char *version;
    zend_long max_level, min_level, default_level;
    
    go_get_zstd_info(&version, &max_level, &min_level, &default_level);
    
    array_init(return_value);
    add_assoc_string(return_value, "version", version);
    add_assoc_long(return_value, "maxLevel", max_level);
    add_assoc_long(return_value, "minLevel", min_level);
    add_assoc_long(return_value, "defaultLevel", default_level);
    
    if (version) free(version);
}

// Object handlers
static zend_object_handlers compressor_object_handlers;

static zend_object *compressor_create_object(zend_class_entry *ce)
{
    compressor_object *obj = zend_object_alloc(sizeof(compressor_object), ce);
    
    zend_object_std_init(&obj->std, ce);
    object_properties_init(&obj->std, ce);
    
    obj->std.handlers = &compressor_object_handlers;
    obj->name = NULL;
    obj->level = 3;
    obj->window_size = 22;
    
    return &obj->std;
}

static void compressor_free_object(zend_object *object)
{
    compressor_object *obj = compressor_from_obj(object);
    
    if (obj->name) {
        // Clean up the Go compressor
        go_compressor_close(obj->name);
        zend_string_release(obj->name);
    }
    
    zend_object_std_dtor(object);
}

static HashTable *compressor_get_properties(zend_object *object)
{
    compressor_object *obj = compressor_from_obj(object);
    HashTable *props = zend_std_get_properties(object);
    
    zval level_zv, window_size_zv;
    ZVAL_LONG(&level_zv, obj->level);
    ZVAL_LONG(&window_size_zv, obj->window_size);
    
    zend_hash_str_update(props, "level", sizeof("level") - 1, &level_zv);
    zend_hash_str_update(props, "windowSize", sizeof("windowSize") - 1, &window_size_zv);
    
    return props;
}

// Module initialization
PHP_MINIT_FUNCTION(zstd)
{
    // Register CompressionException class
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Phillarmonic\\FrankenZstd", "CompressionException", NULL);
    compression_exception_ce = zend_register_internal_class_ex(&ce, zend_ce_exception);

    // Register Compressor class
    INIT_NS_CLASS_ENTRY(ce, "Phillarmonic\\FrankenZstd", "Compressor", class_Phillarmonic_FrankenZstd_Compressor_methods);
    compressor_ce = zend_register_internal_class(&ce);
    compressor_ce->create_object = compressor_create_object;
    
    // Set up object handlers
    memcpy(&compressor_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    compressor_object_handlers.free_obj = compressor_free_object;
    compressor_object_handlers.get_properties = compressor_get_properties;
    compressor_object_handlers.offset = XtOffsetOf(compressor_object, std);
    
    // Register class constants
    zend_declare_class_constant_long(compressor_ce, "LEVEL_FASTEST", sizeof("LEVEL_FASTEST") - 1, 1);
    zend_declare_class_constant_long(compressor_ce, "LEVEL_DEFAULT", sizeof("LEVEL_DEFAULT") - 1, 3);
    zend_declare_class_constant_long(compressor_ce, "LEVEL_BETTER", sizeof("LEVEL_BETTER") - 1, 6);
    zend_declare_class_constant_long(compressor_ce, "LEVEL_BEST", sizeof("LEVEL_BEST") - 1, 9);
    zend_declare_class_constant_long(compressor_ce, "LEVEL_ULTRA", sizeof("LEVEL_ULTRA") - 1, 19);
    
    zend_declare_class_constant_long(compressor_ce, "WINDOW_SIZE_MIN", sizeof("WINDOW_SIZE_MIN") - 1, 10);
    zend_declare_class_constant_long(compressor_ce, "WINDOW_SIZE_DEFAULT", sizeof("WINDOW_SIZE_DEFAULT") - 1, 22);
    zend_declare_class_constant_long(compressor_ce, "WINDOW_SIZE_MAX", sizeof("WINDOW_SIZE_MAX") - 1, 31);

    return SUCCESS;
}

// Module shutdown
PHP_MSHUTDOWN_FUNCTION(zstd)
{
    return SUCCESS;
}

// Module info
PHP_MINFO_FUNCTION(zstd)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "zstd support", "enabled");
    php_info_print_table_row(2, "Extension version", "1.1.0");
    php_info_print_table_end();
}

// Module entry
zend_module_entry zstd_module_entry = {
    STANDARD_MODULE_HEADER,
    "franken-zstd-go",
    ext_functions,
    PHP_MINIT(zstd),
    PHP_MSHUTDOWN(zstd),
    NULL,
    NULL,
    PHP_MINFO(zstd),
    "1.1.0",
    STANDARD_MODULE_PROPERTIES
};

void register_extension()
{
    // This function is called from Go to register the extension
    // The actual registration is handled by FrankenPHP
}
