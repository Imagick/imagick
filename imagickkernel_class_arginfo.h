/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2b1addfe7bec455d316aac0f8a51bac5c4c9c591 */

#if defined(IMAGICK_WITH_KERNEL)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ImagickKernel_addKernel, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, kernel, ImagickKernel, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(IMAGICK_WITH_KERNEL)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ImagickKernel_addUnityKernel, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, scale, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(IMAGICK_WITH_KERNEL)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_ImagickKernel_fromBuiltin, 0, 2, ImagickKernel, 0)
	ZEND_ARG_TYPE_INFO(0, kernel, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, shape, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(IMAGICK_WITH_KERNEL)
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_ImagickKernel_fromMatrix, 0, 2, ImagickKernel, 0)
	ZEND_ARG_TYPE_INFO(0, matrix, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, origin, IS_ARRAY, 1)
ZEND_END_ARG_INFO()
#endif

#if defined(IMAGICK_WITH_KERNEL)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ImagickKernel_getMatrix, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(IMAGICK_WITH_KERNEL)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ImagickKernel_scale, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, scale, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, normalize_kernel, IS_LONG, 1, "0")
ZEND_END_ARG_INFO()
#endif

#if defined(IMAGICK_WITH_KERNEL)
#define arginfo_class_ImagickKernel_separate arginfo_class_ImagickKernel_getMatrix
#endif


#if defined(IMAGICK_WITH_KERNEL)
ZEND_METHOD(ImagickKernel, addKernel);
#endif
#if defined(IMAGICK_WITH_KERNEL)
ZEND_METHOD(ImagickKernel, addUnityKernel);
#endif
#if defined(IMAGICK_WITH_KERNEL)
ZEND_METHOD(ImagickKernel, fromBuiltin);
#endif
#if defined(IMAGICK_WITH_KERNEL)
ZEND_METHOD(ImagickKernel, fromMatrix);
#endif
#if defined(IMAGICK_WITH_KERNEL)
ZEND_METHOD(ImagickKernel, getMatrix);
#endif
#if defined(IMAGICK_WITH_KERNEL)
ZEND_METHOD(ImagickKernel, scale);
#endif
#if defined(IMAGICK_WITH_KERNEL)
ZEND_METHOD(ImagickKernel, separate);
#endif


static const zend_function_entry class_ImagickKernel_methods[] = {
#if defined(IMAGICK_WITH_KERNEL)
	ZEND_ME(ImagickKernel, addKernel, arginfo_class_ImagickKernel_addKernel, ZEND_ACC_PUBLIC)
#endif
#if defined(IMAGICK_WITH_KERNEL)
	ZEND_ME(ImagickKernel, addUnityKernel, arginfo_class_ImagickKernel_addUnityKernel, ZEND_ACC_PUBLIC)
#endif
#if defined(IMAGICK_WITH_KERNEL)
	ZEND_ME(ImagickKernel, fromBuiltin, arginfo_class_ImagickKernel_fromBuiltin, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if defined(IMAGICK_WITH_KERNEL)
	ZEND_ME(ImagickKernel, fromMatrix, arginfo_class_ImagickKernel_fromMatrix, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if defined(IMAGICK_WITH_KERNEL)
	ZEND_ME(ImagickKernel, getMatrix, arginfo_class_ImagickKernel_getMatrix, ZEND_ACC_PUBLIC)
#endif
#if defined(IMAGICK_WITH_KERNEL)
	ZEND_ME(ImagickKernel, scale, arginfo_class_ImagickKernel_scale, ZEND_ACC_PUBLIC)
#endif
#if defined(IMAGICK_WITH_KERNEL)
	ZEND_ME(ImagickKernel, separate, arginfo_class_ImagickKernel_separate, ZEND_ACC_PUBLIC)
#endif
	ZEND_FE_END
};
