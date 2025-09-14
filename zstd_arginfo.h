/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 478924729ffc6e4284d5980dc1e639151a00ee96 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Phillarmonic_FrankenZstd_compress, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, level, IS_LONG, 0, "Phillarmonic\\FrankenZstd\\Compressor::LEVEL_DEFAULT")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Phillarmonic_FrankenZstd_decompress, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, compressedData, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_Phillarmonic_FrankenZstd_getInfo, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Phillarmonic_FrankenZstd_Compressor_create, 0, 0, Phillarmonic\\FrankenZstd\\Compressor, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, level, IS_LONG, 0, "Phillarmonic\\FrankenZstd\\Compressor::LEVEL_DEFAULT")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, windowSize, IS_LONG, 0, "Phillarmonic\\FrankenZstd\\Compressor::WINDOW_SIZE_DEFAULT")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Phillarmonic_FrankenZstd_Compressor_compress, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Phillarmonic_FrankenZstd_Compressor_decompress arginfo_Phillarmonic_FrankenZstd_decompress

#define arginfo_class_Phillarmonic_FrankenZstd_Compressor_getStats arginfo_Phillarmonic_FrankenZstd_getInfo

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Phillarmonic_FrankenZstd_Compressor_close, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Phillarmonic_FrankenZstd_CompressionException___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, code, IS_LONG, 0, "0")
	ZEND_ARG_OBJ_INFO_WITH_DEFAULT_VALUE(0, previous, Throwable, 1, "null")
ZEND_END_ARG_INFO()

ZEND_FUNCTION(Phillarmonic_FrankenZstd_compress);
ZEND_FUNCTION(Phillarmonic_FrankenZstd_decompress);
ZEND_FUNCTION(Phillarmonic_FrankenZstd_getInfo);
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, create);
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, compress);
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, decompress);
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, getStats);
ZEND_METHOD(Phillarmonic_FrankenZstd_Compressor, close);
ZEND_METHOD(Phillarmonic_FrankenZstd_CompressionException, __construct);

static const zend_function_entry ext_functions[] = {
	ZEND_RAW_FENTRY(ZEND_NS_NAME("Phillarmonic\\FrankenZstd", "compress"), zif_Phillarmonic_FrankenZstd_compress, arginfo_Phillarmonic_FrankenZstd_compress, 0, NULL, NULL)
	ZEND_RAW_FENTRY(ZEND_NS_NAME("Phillarmonic\\FrankenZstd", "decompress"), zif_Phillarmonic_FrankenZstd_decompress, arginfo_Phillarmonic_FrankenZstd_decompress, 0, NULL, NULL)
	ZEND_RAW_FENTRY(ZEND_NS_NAME("Phillarmonic\\FrankenZstd", "getInfo"), zif_Phillarmonic_FrankenZstd_getInfo, arginfo_Phillarmonic_FrankenZstd_getInfo, 0, NULL, NULL)
	ZEND_FE_END
};

static const zend_function_entry class_Phillarmonic_FrankenZstd_Compressor_methods[] = {
	ZEND_ME(Phillarmonic_FrankenZstd_Compressor, create, arginfo_class_Phillarmonic_FrankenZstd_Compressor_create, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Phillarmonic_FrankenZstd_Compressor, compress, arginfo_class_Phillarmonic_FrankenZstd_Compressor_compress, ZEND_ACC_PUBLIC)
	ZEND_ME(Phillarmonic_FrankenZstd_Compressor, decompress, arginfo_class_Phillarmonic_FrankenZstd_Compressor_decompress, ZEND_ACC_PUBLIC)
	ZEND_ME(Phillarmonic_FrankenZstd_Compressor, getStats, arginfo_class_Phillarmonic_FrankenZstd_Compressor_getStats, ZEND_ACC_PUBLIC)
	ZEND_ME(Phillarmonic_FrankenZstd_Compressor, close, arginfo_class_Phillarmonic_FrankenZstd_Compressor_close, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_Phillarmonic_FrankenZstd_CompressionException_methods[] = {
	ZEND_ME(Phillarmonic_FrankenZstd_CompressionException, __construct, arginfo_class_Phillarmonic_FrankenZstd_CompressionException___construct, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_Phillarmonic_FrankenZstd_Compressor(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Phillarmonic\\FrankenZstd", "Compressor", class_Phillarmonic_FrankenZstd_Compressor_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval const_LEVEL_FASTEST_value;
	ZVAL_LONG(&const_LEVEL_FASTEST_value, 1);
	zend_string *const_LEVEL_FASTEST_name = zend_string_init_interned("LEVEL_FASTEST", sizeof("LEVEL_FASTEST") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_FASTEST_name, &const_LEVEL_FASTEST_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_FASTEST_name);

	zval const_LEVEL_DEFAULT_value;
	ZVAL_LONG(&const_LEVEL_DEFAULT_value, 3);
	zend_string *const_LEVEL_DEFAULT_name = zend_string_init_interned("LEVEL_DEFAULT", sizeof("LEVEL_DEFAULT") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_DEFAULT_name, &const_LEVEL_DEFAULT_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_DEFAULT_name);

	zval const_LEVEL_BETTER_value;
	ZVAL_LONG(&const_LEVEL_BETTER_value, 6);
	zend_string *const_LEVEL_BETTER_name = zend_string_init_interned("LEVEL_BETTER", sizeof("LEVEL_BETTER") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_BETTER_name, &const_LEVEL_BETTER_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_BETTER_name);

	zval const_LEVEL_BEST_value;
	ZVAL_LONG(&const_LEVEL_BEST_value, 9);
	zend_string *const_LEVEL_BEST_name = zend_string_init_interned("LEVEL_BEST", sizeof("LEVEL_BEST") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_BEST_name, &const_LEVEL_BEST_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_BEST_name);

	zval const_LEVEL_ULTRA_value;
	ZVAL_LONG(&const_LEVEL_ULTRA_value, 19);
	zend_string *const_LEVEL_ULTRA_name = zend_string_init_interned("LEVEL_ULTRA", sizeof("LEVEL_ULTRA") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_LEVEL_ULTRA_name, &const_LEVEL_ULTRA_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_LEVEL_ULTRA_name);

	zval const_WINDOW_SIZE_MIN_value;
	ZVAL_LONG(&const_WINDOW_SIZE_MIN_value, 10);
	zend_string *const_WINDOW_SIZE_MIN_name = zend_string_init_interned("WINDOW_SIZE_MIN", sizeof("WINDOW_SIZE_MIN") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_WINDOW_SIZE_MIN_name, &const_WINDOW_SIZE_MIN_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_WINDOW_SIZE_MIN_name);

	zval const_WINDOW_SIZE_DEFAULT_value;
	ZVAL_LONG(&const_WINDOW_SIZE_DEFAULT_value, 22);
	zend_string *const_WINDOW_SIZE_DEFAULT_name = zend_string_init_interned("WINDOW_SIZE_DEFAULT", sizeof("WINDOW_SIZE_DEFAULT") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_WINDOW_SIZE_DEFAULT_name, &const_WINDOW_SIZE_DEFAULT_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_WINDOW_SIZE_DEFAULT_name);

	zval const_WINDOW_SIZE_MAX_value;
	ZVAL_LONG(&const_WINDOW_SIZE_MAX_value, 31);
	zend_string *const_WINDOW_SIZE_MAX_name = zend_string_init_interned("WINDOW_SIZE_MAX", sizeof("WINDOW_SIZE_MAX") - 1, 1);
	zend_declare_typed_class_constant(class_entry, const_WINDOW_SIZE_MAX_name, &const_WINDOW_SIZE_MAX_value, ZEND_ACC_PUBLIC, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(const_WINDOW_SIZE_MAX_name);

	zval property_level_default_value;
	ZVAL_UNDEF(&property_level_default_value);
	zend_string *property_level_name = zend_string_init("level", sizeof("level") - 1, 1);
	zend_declare_typed_property(class_entry, property_level_name, &property_level_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_level_name);

	zval property_windowSize_default_value;
	ZVAL_UNDEF(&property_windowSize_default_value);
	zend_string *property_windowSize_name = zend_string_init("windowSize", sizeof("windowSize") - 1, 1);
	zend_declare_typed_property(class_entry, property_windowSize_name, &property_windowSize_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_windowSize_name);

	return class_entry;
}

static zend_class_entry *register_class_Phillarmonic_FrankenZstd_CompressionException(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Phillarmonic\\FrankenZstd", "CompressionException", class_Phillarmonic_FrankenZstd_CompressionException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Exception, 0);

	return class_entry;
}
