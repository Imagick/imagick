// Allow the generated stubs to work on previous versions of PHP

#if PHP_VERSION_ID < 80000

#define ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(pass_by_ref, name, type_hint, allow_null, default_value) \
  ZEND_ARG_INFO(pass_by_ref, name)

#define ZEND_ARG_OBJ_TYPE_MASK(pass_by_ref, name, class_name, type_mask, default_value) \
     ZEND_ARG_INFO(pass_by_ref, name)

#define ZEND_ARG_TYPE_MASK(pass_by_ref, name, type_mask, default_value) \
     ZEND_ARG_INFO(pass_by_ref, name)

#endif

#if PHP_VERSION_ID < 70200

#define ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
        ZEND_BEGIN_ARG_INFO_EX(name, 0, return_reference, required_num_args)

#define ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, return_reference, required_num_args, class_name, allow_null) \
        ZEND_BEGIN_ARG_INFO_EX(name, 0, return_reference, required_num_args)

#endif
