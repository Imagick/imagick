/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Dan Ackroyd <danack@php.net>                                 |
   +----------------------------------------------------------------------+
*/

#include "php_imagick.h"
#include "php_imagick_defs.h"
#include "php_imagick_macros.h"
#include "php_imagick_helpers.h"

static void im_CalcKernelMetaData(KernelInfo *kernel) {
	size_t i;

	kernel->minimum = kernel->maximum = 0.0;
	kernel->negative_range = kernel->positive_range = 0.0;

	for (i=0; i < (kernel->width*kernel->height); i++) {
		if (fabs(kernel->values[i]) < MagickEpsilon) {
			kernel->values[i] = 0.0;
		}
		if (kernel->values[i] < 0) {
			kernel->negative_range += kernel->values[i];
		}
		else {
			kernel->positive_range += kernel->values[i];
		}
		if (kernel->values[i] < kernel->minimum) {
			kernel->minimum = kernel->values[i];
		}
		
		if (kernel->values[i] > kernel->maximum) {
			kernel->maximum = kernel->values[i];
		}
	}

	return;
}


#if MagickLibVersion > 0x661
KernelInfo *createKernel(double *values, size_t width, size_t height)
{
	KernelInfo *kernel_info;

	kernel_info=AcquireKernelInfo(NULL);
	if (kernel_info == (KernelInfo *) NULL) {
		return NULL;
	}

	kernel_info->width = width;
	kernel_info->height = height;
	//Need to free old values?
	if (kernel_info->values != NULL) {
		free(kernel_info->values);
	}

	kernel_info->values = values;
	im_CalcKernelMetaData(kernel_info);

	return kernel_info;
}
#endif

/* {{{ proto ImagickKernel ImagickKernel::fromArray(array)
	Returns a new Kernel from a 2d array of values. The array should be rectangular
	and contain i) float values where the kernel element should be used ii) false
	where the element should be skipped
*/
PHP_METHOD(imagickkernel, fromarray)
{
	php_imagickkernel_object *internp;
	php_imagickkernel_object *intern_return;
	zval *kernel_array;
	HashTable *inner_array;
	zval **ppzval_outer;
	zval **ppzval_inner;
	KernelInfo *kernel_info;
	long num_rows, num_columns;
	int previous_num_columns = -1;
	int row, column;
	int count = 0;

	double *values = NULL;
	double notanumber = sqrt((double)-1.0);  /* Special Value : Not A Number */

	row = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &kernel_array) == FAILURE) {
		return;
	}

	num_rows = zend_hash_num_elements(Z_ARRVAL_P(kernel_array));

	if (num_rows == 0) {
		//error - array has zero elements.
		php_imagick_throw_exception(IMAGICK_CLASS, "Matrix is empty." TSRMLS_CC);
		return;
	}

	for (zend_hash_internal_pointer_reset(Z_ARRVAL_P(kernel_array));
			zend_hash_get_current_data(Z_ARRVAL_P(kernel_array), (void **) &ppzval_outer) == SUCCESS;
			zend_hash_move_forward(Z_ARRVAL_P(kernel_array)), row++)
	{
		zval tmp_zval, *tmp_pzval;
		column = 0;

		if (Z_TYPE_PP(ppzval_outer) == IS_ARRAY ) {
			//parse this row
			inner_array = Z_ARRVAL_PP(ppzval_outer);
			num_columns = zend_hash_num_elements(inner_array);

			if (num_columns == 0) {
				php_imagick_throw_exception(IMAGICK_CLASS, "Matrix is empty." TSRMLS_CC);
				goto cleanup;
			}

			if (values == NULL) {
				values = malloc(num_rows * num_columns * sizeof(double));
			}

			if (previous_num_columns != -1) {
				if (previous_num_columns != num_columns) {
					php_imagick_throw_exception(IMAGICK_CLASS, "Values must be matrix, with the same number of columns in each row." TSRMLS_CC);
					goto cleanup;
				}
			}
			previous_num_columns = num_columns;

			for (zend_hash_internal_pointer_reset(inner_array);
					zend_hash_get_current_data(inner_array, (void **) &ppzval_inner) == SUCCESS;
					zend_hash_move_forward(inner_array), column++)
			{
				if (Z_TYPE_PP(ppzval_inner) == IS_DOUBLE) {
					//It's a float lets use it.
					values[count] = Z_DVAL_PP(ppzval_inner);
				}
				else if (Z_TYPE_PP(ppzval_inner) == IS_LONG) {
					//It's a long lets use it.
					values[count] = (float)Z_LVAL_PP(ppzval_inner);
				}
				else if (Z_TYPE_PP(ppzval_inner) == IS_BOOL && Z_BVAL_PP(ppzval_inner) == 0) { 
					//It's false, use nan
					values[count] = notanumber;
				}
				else {
					//zend_get_type_by_const(Z_TYPE_P(ppzval_inner));
					php_imagick_throw_exception(IMAGICK_CLASS, "Only numbers or false are valid values " TSRMLS_CC);
					goto cleanup;
				}
				count++;
			}
		}
		else {
			php_imagick_throw_exception(IMAGICK_CLASS, "Values must be matrix, with the same number of columns in each row." TSRMLS_CC);
			goto cleanup;
		}
	}

	kernel_info = createKernel(values, num_columns, num_rows);
	object_init_ex(return_value, php_imagickkernel_sc_entry);
	intern_return = (php_imagickkernel_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	intern_return->kernel_info = kernel_info;
	return;

cleanup:
	if (values != NULL) {
		free(values);
	}
}
/* }}} */

static void fiddle_with_geometry_info(ssize_t type, GeometryFlags flags, GeometryInfo *geometry_info) {

	/* special handling of missing values in input string */
	switch( type ) {
		/* Shape Kernel Defaults */
		case UnityKernel: {
			if ((flags & WidthValue) == 0)
			geometry_info->rho = 1.0;    /* Default scale = 1.0, zero is valid */
			break;
		}
		case SquareKernel:
		case DiamondKernel:
		case OctagonKernel:
		case DiskKernel:
		case PlusKernel:
		case CrossKernel: {
			if ( (flags & HeightValue) == 0 ) {
				geometry_info->sigma = 1.0;    /* Default scale = 1.0, zero is valid */
			}
			break;
		}
		case RingKernel: {
			if ((flags & XValue) == 0) {
				geometry_info->xi = 1.0;       /* Default scale = 1.0, zero is valid */
			}
			break;
		}
		case RectangleKernel: {    /* Rectangle - set size defaults */
			if ((flags & WidthValue) == 0) { /* if no width then */
				geometry_info->rho = geometry_info->sigma;         /* then  width = height */
			}
			if (geometry_info->rho < 1.0) {            /* if width too small */
				geometry_info->rho = 3;                 /* then  width = 3 */
			}
			if (geometry_info->sigma < 1.0) {          /* if height too small */
				geometry_info->sigma = geometry_info->rho;         /* then  height = width */
			}
			if ((flags & XValue) == 0) {    /* center offset if not defined */
				geometry_info->xi = (double)(((ssize_t)geometry_info->rho-1)/2);
			}
			if ((flags & YValue) == 0) {
				geometry_info->psi = (double)(((ssize_t)geometry_info->sigma-1)/2);
			}
			break;
		}
		/* Distance Kernel Defaults */
		case ChebyshevKernel:
		case ManhattanKernel:
		case OctagonalKernel:
		case EuclideanKernel: {
			if ((flags & HeightValue) == 0) {           /* no distance scale */
				geometry_info->sigma = 100.0;                       /* default distance scaling */
			}
			else if ((flags & AspectValue ) != 0) {     /* '!' flag */
				geometry_info->sigma = QuantumRange/(geometry_info->sigma+1); /* maximum pixel distance */
			}
			else if ((flags & PercentValue ) != 0) {    /* '%' flag */
				geometry_info->sigma *= QuantumRange/100.0;         /* percentage of color range */
			}
			break;
		}
		default: {
			break;
		}
	}
}

/* {{{ proto ImagickKernel ImagickKernel::fromBuiltin(type, string)
	geometryInfo needs to contain variously:
		rho, sigma, xi, psi, chi;
*/

PHP_METHOD(imagickkernel, frombuiltin)
{
	php_imagickkernel_object *intern_return;
	long kernel_type;
	GeometryInfo geometry_info;
	KernelInfo *kernel_info;
	char *string;
	long string_len;
	GeometryFlags flags;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &kernel_type, &string, &string_len) == FAILURE) {
		return;
	}

	flags = ParseGeometry(string, &geometry_info);
	fiddle_with_geometry_info(kernel_type, flags, &geometry_info);

	//These ....are needed?
	//	if ((flags & PercentValue) != 0) {      /* Handle Percentage flag*/
	//		geometry_info.rho *= 0.01,  geometry_info.sigma *= 0.01;
	//	}
	//
	//	if ((flags & RhoValue) == 0) {          /* Set Defaults for missing args */
	//		geometry_info.rho = 1.0;
	//	}
	//
	//	if ((flags & SigmaValue) == 0) {
	//		geometry_info.sigma = 0.0;
	//	}

	//  /* Scale/Normalize the input kernel */
	//  ScaleKernelInfo(kernel, args.rho, flags);

	/* Add Unity Kernel, for blending with original */
	//	if ( (flags & SigmaValue) != 0 ) {
	//		UnityAddKernelInfo(kernel, args.sigma);
	//	}

	kernel_info = AcquireKernelBuiltIn(kernel_type, &geometry_info);

	object_init_ex(return_value, php_imagickkernel_sc_entry);
	intern_return = (php_imagickkernel_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	intern_return->kernel_info = kernel_info;

	return;
}

/* }}} */


/* {{{ proto ImagickKernel ImagickKernel::addKernel(ImagickKernel kernel)
    Attach a kernel to another kernel. Returns the new combined kernel 
*/
PHP_METHOD(imagickkernel, addkernel)
{
	php_imagickkernel_object *internp;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internp = (php_imagickkernel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	return;
}
/* }}} */



/* {{{ proto ImagickKernel ImagickKernel::rotateKernel(double angle)
        Description
*/
/*
PHP_METHOD(imagickkernel, rotateKernel)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internp = (php_imagickkernel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	return;
}
*/
/* }}} */




/* {{{ proto ImagickKernel[] ImagickKernel::separate(void)
	Separate a linked set of kernels into individual kernels
*/
PHP_METHOD(imagickkernel, separate)
{
	zval *separate_object;
	php_imagickkernel_object *separate_kernel;
	php_imagickkernel_object *internp;
	KernelInfo *kernel_info;
	KernelInfo *kernel_info_copy;
	int number_values;


	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internp = (php_imagickkernel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	kernel_info = internp->kernel_info;

	array_init(return_value);

	while (kernel_info != NULL) {
		MAKE_STD_ZVAL(separate_object);
		object_init_ex(separate_object, php_imagickkernel_sc_entry);
		separate_kernel = (php_imagickkernel_object *)zend_object_store_get_object(separate_object TSRMLS_CC);

		kernel_info_copy = AcquireKernelInfo(NULL);
		if (kernel_info_copy->values != NULL) {
			free(kernel_info_copy->values);
		}

		number_values = kernel_info->width * kernel_info->height;
		memcpy(kernel_info_copy, kernel_info, sizeof(KernelInfo));

		kernel_info_copy->next = NULL;
		if (kernel_info->values != NULL) {
			kernel_info_copy->values = malloc(number_values * sizeof(double));
			memcpy(kernel_info_copy->values, kernel_info->values, number_values * sizeof(double));
		}

		separate_kernel->kernel_info = kernel_info_copy;
		add_next_index_zval(return_value, separate_object);
		kernel_info = kernel_info->next;
	}

	return;
}
/* }}} */


/* {{{ proto [] ImagickKernel::getvalues(void)
	Return the values used in the kernel. The array contains floats
	for the elements used and false for the elements not used.
*/
PHP_METHOD(imagickkernel, getvalues)
{
	php_imagickkernel_object *internp;
	zval *row;
	int count;
	KernelInfo *kernel;
	double value;
	int x, y;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	internp = (php_imagickkernel_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

	kernel = internp->kernel_info;
	array_init(return_value);

	count = 0;

	for (y=0; y<kernel->height ; y++) {
		MAKE_STD_ZVAL(row);
		array_init(row);
		for (x=0; x<kernel->width ; x++) {
			value = kernel->values[count];
			count++;

			//nan is not equal to itself
			if (value != value) {
				//this will be broken by some compilers - need to investigate more...
				add_next_index_bool(row, 0);
			}
			else {
				add_next_index_double(row, value);
			}
		}

		add_next_index_zval(return_value, row);
	}

	return;
}
/* }}} */




