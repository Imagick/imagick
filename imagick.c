/*
   +----------------------------------------------------------------------+
   | PHP version 4.0                                                      |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997, 1998, 1999, 2000, 2001 The PHP Group             |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Christian Stocker <chregu@bitflux.ch>                       |
   |          Andrei Nigmatulin <anight@mail.ru>                          |
   +----------------------------------------------------------------------+
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "php_imagick.h"

#define imagick_warning() MagickWarning(exception.severity, exception.reason, exception.description)
#define imagick_warning_p() MagickWarning(exception_p->severity, exception_p->reason, exception_p->description)

#define IMAGICK_RET_COPY_HANDLE()    if ( copy_handle->image == (Image *)NULL ) \
                                    { \
                                    imagick_warning(); \
                                    RETURN_FALSE; \
                                    } \
                                    ZEND_REGISTER_RESOURCE(return_value, copy_handle, le_imagick);

#define IMAGICK_FETCH_RES_AND_COPY_HANDLE()    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick); \
                                             GetExceptionInfo(&exception); \
                                             copy_handle = _php_imagick_new(); \
                                             copy_handle->info = CloneImageInfo(handle->info);

#define IMAGICK_DEPRECATED()     php_error(E_WARNING, "%s(): This function is deprecated and will be removed soon, use the corresponding imagick_copy_*() function instead", get_active_function_name(TSRMLS_C));


#define DegreesToRadians(x) ((x)*3.14159265358979323846/180.0)

ZEND_DECLARE_MODULE_GLOBALS(imagick)

/* True global resources - no need for thread safety here */
static int le_imagick;
static char *_php_imagick_client_name = "php_extension";

/* {{{ imagick_functions[]
 *
 * Every user visible function must have an entry in imagick_functions[].
 */
function_entry imagick_functions[] = {

                                        PHP_FE(imagick_new, NULL)
                                        PHP_FE(imagick_read, NULL)
                                        PHP_FE(imagick_add_resource, NULL)
                                        PHP_FE(imagick_copy_sample, NULL)
                                        PHP_FE(imagick_copy_resize, NULL)
                                        PHP_FE(imagick_copy_crop, NULL)                                                
                                        PHP_FE(imagick_copy_shear, NULL)
                                        PHP_FE(imagick_copy_rotate, NULL)
                                        PHP_FE(imagick_copy_morph, NULL)
                                                
                                        PHP_FE(imagick_sample, NULL)
                                        PHP_FE(imagick_resize, NULL)
                                        PHP_FE(imagick_crop, NULL)
                                        PHP_FE(imagick_shear, NULL)
                                        PHP_FE(imagick_rotate, NULL)
                                        PHP_FE(imagick_oilpaint, NULL)
                                        PHP_FE(imagick_annotate, NULL)
                                        PHP_FE(imagick_morph, NULL)
                                        PHP_FE(imagick_write, NULL)
                                        PHP_FE(imagick_list_magickinfo, NULL)
                                        PHP_FE(imagick_dump, NULL)
                                        PHP_FE(imagick_convert, NULL)
                                        PHP_FE(imagick_set_attribute, NULL)
                                        PHP_FE(imagick_get_attribute, NULL)
                                        PHP_FE(imagick_free, NULL)
                                        PHP_FALIAS(imagick_create, imagick_new, NULL)
                                        {NULL, NULL, NULL}	/* Must be the last line in imagick_functions[] */
                                    };
/* }}} */


/* {{{ imagick_module_entry
 */
zend_module_entry imagick_module_entry = {
#ifdef NEWER_API
                                            STANDARD_MODULE_HEADER,
#endif
                                            "imagick",
                                            imagick_functions,
                                            PHP_MINIT(imagick),
                                            PHP_MSHUTDOWN(imagick),
                                            PHP_RINIT(imagick),		/* Replace with NULL if there's nothing to do at request start */
                                            PHP_RSHUTDOWN(imagick),	/* Replace with NULL if there's nothing to do at request end */
                                            PHP_MINFO(imagick),
#ifdef NEWER_API
                                            IMAGICK_VERSION,
#endif
                                            STANDARD_MODULE_PROPERTIES
                                        };
/* }}} */


#ifdef COMPILE_DL_IMAGICK
ZEND_GET_MODULE(imagick)
#endif


/* {{{ PHP_INI
 */

/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("imagick.global_value",      "42", PHP_INI_ALL, OnUpdateInt, global_value, zend_imagick_globals, imagick_globals)
    STD_PHP_INI_ENTRY("imagick.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_imagick_globals, imagick_globals)
PHP_INI_END()
*/
/* }}} */






/* {{{ php_imagick_init_globals
 Uncomment this function if you have INI entries
 */

static void php_imagick_init_globals(zend_imagick_globals *imagick_globals)
{

}
/*
imagick_globals->value = 0;
	imagick_globals->string = NULL;
*/
/* }}} */


static void _php_imagick_warning_handler(const ExceptionType warning,
    const char *reason, const char *description) {

    if (reason == (char *) NULL) return;

    php_error(E_WARNING, "%s(): %.1024s (%.1024s) [%.1024s]",
        get_active_function_name(TSRMLS_C),
        reason,
        (description != (char *) NULL) ? description : "",
        ((warning != OptionWarning) && errno) ? strerror(errno) : ""
    );
}


static php_imagick *_php_imagick_new() {

    php_imagick *handle;
    handle = (php_imagick *) emalloc(sizeof(php_imagick));

    if (!handle) {
        php_error(E_WARNING, "%s(): couldn't allocate PHP-Magick handle", get_active_function_name(TSRMLS_C));
        return (php_imagick *) NULL;
    }

    memset(handle, 0, sizeof(php_imagick));
    handle->info = CloneImageInfo((ImageInfo *) NULL);

    return handle;

}


static void _php_imagick_free(php_imagick *handle)
{
    if (handle->image) {
        DestroyImage(handle->image);
    }
    if (handle->info) {
        DestroyImageInfo(handle->info);
    }
    efree(handle);

}


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(imagick)
{

    ZEND_INIT_MODULE_GLOBALS(imagick, php_imagick_init_globals, NULL);
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_UNDEFINED",  UndefinedFilter,    CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_POINT",      PointFilter,        CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_BOX",        BoxFilter,          CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_TRIANGLE",   TriangleFilter,     CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_HERMITE",    HermiteFilter,      CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_HANNING",    HanningFilter,      CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_HAMMING",    HammingFilter,      CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_BLACKMAN",   BlackmanFilter,     CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_GAUSSIAN",   GaussianFilter,     CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_QUADRATIC",  QuadraticFilter,    CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_CUBIC",      CubicFilter,        CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_CATROM",     CatromFilter,       CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_MITCHELL",   MitchellFilter,      CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_LANCZOS",    LanczosFilter,      CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_BESSEL",     BesselFilter,       CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_SINC",       SincFilter,         CONST_CS | CONST_PERSISTENT);


    le_imagick = register_list_destructors(_php_imagick_free,NULL);
    InitializeMagick(_php_imagick_client_name);
    SetWarningHandler(_php_imagick_warning_handler);
    return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(imagick)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    DestroyMagick();
    return SUCCESS;
}
/* }}} */


/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(imagick)
{
    return SUCCESS;
}
/* }}} */


/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(imagick)
{
    return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(imagick)
{
    unsigned int v;
    smart_str format_list = {0};
    const MagickInfo *magick_info;
    ExceptionInfo exception;
    const register MagickInfo *p;
    char version[9];

    php_info_print_table_start();
    php_info_print_table_header(2, "Imagick support", "enabled");

    php_info_print_table_row(2, "Module version", imagick_module_entry.version);

    GetMagickVersion(&v);

    sprintf(version, "%d.%d.%d", (v >> 8) & 0xf, (v >> 4) & 0xf, v & 0xf);

    php_info_print_table_row(2, "ImageMagick version", version);

    GetExceptionInfo(&exception);

    magick_info = GetMagickInfo(NULL, &exception);
    if (magick_info != (MagickInfo *) NULL) {
        for (p = magick_info; p != (MagickInfo *) NULL; p = p->next) {
            smart_str_appendl(&format_list, p->name, strlen(p->name));
            if (p->next) {
                smart_str_appendl(&format_list, ", ", 2);
            }
        }
        smart_str_0(&format_list);
        php_info_print_table_row(2, "Supported formats", format_list.c);
    }

    php_info_print_table_end();


    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}
/* }}} */


/* {{{ proto void imagick_free(resource handle)
        frees an imagick handle
*/
PHP_FUNCTION(imagick_free)
{
    zval *arg;
    int argc = ZEND_NUM_ARGS();
    int id, type;
    if (zend_parse_parameters(argc TSRMLS_CC, "r", &arg) == FAILURE) return;
    id = Z_LVAL_P(arg);
    if (zend_list_find(id, &type) && type == le_imagick)
                zend_list_delete(id);

}
/* }}} */


/* {{{ proto handle imagick_new()
        creates a new imagick-handle
*/
PHP_FUNCTION(imagick_new)
{
    php_imagick *handle = _php_imagick_new();

    if (!handle) {
        RETURN_FALSE;
    }

    ZEND_REGISTER_RESOURCE(return_value, handle, le_imagick);

}
/* }}} */


/* {{{ proto bool imagick_add_resource(resource image, resource frame)
        adds an image from another ressource
*/
PHP_FUNCTION(imagick_add_resource)
{
    zval *arg;
    zval *addarg;
    int argc = ZEND_NUM_ARGS();
    php_imagick *handle;
    php_imagick *addhandle;
    ExceptionInfo exception;
    register Image *p;


    if (zend_parse_parameters(argc TSRMLS_CC, "rr", &arg, &addarg) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);
    ZEND_FETCH_RESOURCE(addhandle, php_imagick *, &addarg, -1,  "imagick object", le_imagick);


        GetExceptionInfo(&exception);

        if ( addhandle->image == (Image *)NULL )
        {
            imagick_warning();
            RETURN_FALSE;
        }

        if (handle->image == (Image *) NULL)
          handle->image=addhandle->image;
        else
          {
            /*
              Link image into image list.
            */
            for (p=handle->image; p->next != (Image *) NULL; p=p->next);
            addhandle->image->previous=p;
            p->next=addhandle->image;
          }
}
/* }}} */


/* {{{ proto bool imagick_read(resource handle, mixed infile)
        reads an image from a file
*/
PHP_FUNCTION(imagick_read)
{
    zval *arg;
    zval *userdata;
    char *infile = NULL;
    int infile_len = 0;
    int argc = ZEND_NUM_ARGS();
    php_imagick *handle;
    ExceptionInfo exception;
    Image *next_image;
    register Image *p;
	char array_passed = 0;

    /* array stuff */

	HashTable *target_hash;
    zval **entry;				/* pointer to array entry */

    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "rs", &arg, &infile, &infile_len) == SUCCESS)
    {
        /* only one filename as a string */
        MAKE_STD_ZVAL(userdata);
        if (array_init(userdata) != SUCCESS)
        {
         php_error(E_ERROR,"%s(): Problems with creating array",get_active_function_name(TSRMLS_C));
            RETVAL_FALSE;
            return;
        }
        if (add_index_stringl(userdata,0,infile,infile_len,1) != SUCCESS)
        {
            php_error(E_ERROR,"%s(): Problems with adding string to array",get_active_function_name(TSRMLS_C));
            RETVAL_FALSE;
            return;
        }
    }
    else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "ra", &arg, &userdata) == SUCCESS)
    {
		array_passed = 1;
        /* one or more filename as an array */
    }
    else
    {
        WRONG_PARAM_COUNT;
            return;
    }


    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

	/*array stuff, i'm not very sure about that, since the docs are very mmmh... about that
        it's mostly copy&paste work from other sources

    */


    /* Set up known arguments */
    target_hash = HASH_OF(userdata);
    zend_hash_internal_pointer_reset(target_hash);

	/* Iterate through hash */
	while(zend_hash_get_current_data(target_hash, (void **)&entry) == SUCCESS)
    {
        convert_to_string_ex(entry);

        if (Z_STRLEN_PP(entry) > MaxTextExtent-1) {
            php_error(E_WARNING, "%s(): Too long file name", get_active_function_name(TSRMLS_C));
            RETURN_FALSE;
        }

        strncpy( handle->info->filename, Z_STRVAL_PP(entry), MaxTextExtent-1 );

        GetExceptionInfo(&exception);
        next_image=ReadImage(handle->info,&exception);

        if ( next_image == (Image *)NULL )
        {
            imagick_warning();
            RETURN_FALSE;
        }

        if (handle->image == (Image *) NULL)
          handle->image=next_image;
        else
          {
            /*
              Link image into image list.
            */
            for (p=handle->image; p->next != (Image *) NULL; p=p->next);
            next_image->previous=p;
            p->next=next_image;
          }


       zend_hash_move_forward(target_hash);
    }
	if (!array_passed) {
		zval_dtor(userdata);
		FREE_ZVAL(userdata);
	}
}
/* }}} */


/* {{{ proto bool imagick_write(resource handle, string outfile)
        writes an image to the file with some imageformat
*/

PHP_FUNCTION(imagick_write)
{
    zval *arg;
    char * outfile = NULL;
    int outfile_len = 0;
/*    char * outformat = NULL;
    int outformat_len = 0;*/
    int er;
    int argc = ZEND_NUM_ARGS();
    php_imagick *handle;
    ExceptionInfo *exception_p;
  register Image
    *p;
  long
    scene = 0;


    if (zend_parse_parameters(argc TSRMLS_CC, "rs", &arg, &outfile, &outfile_len) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

	if (outfile_len > MaxTextExtent-1) {
        php_error(E_WARNING, "%s(): Too long file name", get_active_function_name(TSRMLS_C));
		RETURN_FALSE;
	}
  (void) strncpy(handle->info->filename,outfile,MaxTextExtent-1);
  for (p=(handle->image); p != (Image *) NULL; p=p->next)
  {
    (void) strncpy(p->filename,outfile,MaxTextExtent-1);
    p->scene=scene++;
  }
  (void) SetImageInfo(handle->info,1,&(handle->image)->exception);
  for (p=(handle->image); p != (Image *) NULL; p=p->next)
  {
    er = WriteImage(handle->info,p);
    if ( er == 0 )
    {
        exception_p = &(handle->image)->exception;
        imagick_warning_p();
        RETURN_FALSE;
    }

    if (handle->info->adjoin)
      break;
  }

    RETVAL_TRUE;

}
/* }}} */


/* {{{ proto bool imagick_sample(resource handle, int columns, int rows)
        samples an image
        WARNING: This function will be removed soon, since it's a memory leak canditate
*/
PHP_FUNCTION(imagick_sample)
{
    zval *arg;
    int columns;
    int rows;
    int argc = ZEND_NUM_ARGS();
    ExceptionInfo
    exception;
    php_imagick *handle;

    IMAGICK_DEPRECATED();

    if (zend_parse_parameters(argc TSRMLS_CC, "rll", &arg, &columns, &rows) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);
    GetExceptionInfo(&exception);
    handle->image = SampleImage(handle->image,columns,rows,&exception);
    if ( handle->image == (Image *)NULL )
    {
        imagick_warning();
        RETURN_FALSE;
    }

}
/* }}} */

/* {{{ proto handle imagick_copy_sample(resource handle, int columns, int rows)
        samples an image and returns a new handle
*/
PHP_FUNCTION(imagick_copy_sample)
{
    zval *arg;
    int columns;
    int rows;
    int argc = ZEND_NUM_ARGS();
    ExceptionInfo
    exception;
    php_imagick *handle;
    php_imagick *copy_handle;

    if (zend_parse_parameters(argc TSRMLS_CC, "rll", &arg, &columns, &rows) == FAILURE) return;

    IMAGICK_FETCH_RES_AND_COPY_HANDLE()    
            
    copy_handle->image = SampleImage(handle->image,columns,rows,&exception);
    
    IMAGICK_RET_COPY_HANDLE();   
}
/* }}} */

/* {{{ proto bool imagick_resize(resource handle, int columns, int rows, int filter, int blur)
        resizes an image with the method filter and the blurness blur
        WARNING: This function will be removed soon, since it's a memory leak canditate
*/
PHP_FUNCTION(imagick_resize)
{
    zval *arg;
    int columns;
    int rows;
    int argc = ZEND_NUM_ARGS();
    int filter = LanczosFilter;
    double blur = 0;
    ExceptionInfo
    exception;
    php_imagick *handle;

    IMAGICK_DEPRECATED();

    if (zend_parse_parameters(argc TSRMLS_CC, "rll|ld", &arg, &columns, &rows, &filter, &blur) == FAILURE) return;

   
    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);

    handle->image = ResizeImage(handle->image,columns,rows,filter,blur,&exception);
    if ( handle->image == (Image *)NULL )
    {
        imagick_warning();
        RETURN_FALSE;
    }
    RETVAL_TRUE;
}
/* }}} */

/* {{{ proto handle imagick_copy_resize(resource handle, int columns, int rows, int filter, int blur)
        resizes an image with the method filter and the blurness blur and returns a new handle
*/
PHP_FUNCTION(imagick_copy_resize)
{
    zval *arg;
    int columns;
    int rows;
    int argc = ZEND_NUM_ARGS();
    int filter = LanczosFilter;
    double blur = 0;
    ExceptionInfo
    exception;
    php_imagick *handle;
    php_imagick *copy_handle;

    if (zend_parse_parameters(argc TSRMLS_CC, "rll|ld", &arg, &columns, &rows, &filter, &blur) == FAILURE) return;

    IMAGICK_FETCH_RES_AND_COPY_HANDLE();    

    copy_handle->image = ResizeImage(handle->image,columns,rows,filter,blur,&exception);
    
    IMAGICK_RET_COPY_HANDLE();       
  
}
/* }}} */




/* {{{ proto bool imagick_crop(resource handle, int width, int height, int x, int y)
        WARNING: This function will be removed soon, since it's a memory leak canditate
        crops an image
*/
PHP_FUNCTION(imagick_crop)
{
    zval *arg;
    int width;
    int height;
    int x;
    int y;
    int argc = ZEND_NUM_ARGS();
    RectangleInfo *rect;
    ExceptionInfo exception;
    php_imagick *handle;

    IMAGICK_DEPRECATED();

    if (zend_parse_parameters(argc TSRMLS_CC, "rllll", &arg, &width, &height, &x, &y) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);
    rect = (RectangleInfo *)emalloc(sizeof(RectangleInfo));
    rect->width = width;
    rect->height = height;
    rect->x = x;
    rect->y = y;

    handle->image = CropImage(handle->image,rect,&exception);
    efree(rect);
    if ( handle->image == (Image *)NULL )
    {
        imagick_warning();
        RETURN_FALSE;
    }
     RETVAL_TRUE;
}
/* }}} */

/* {{{ proto bool imagick_copy_crop(resource handle, int width, int height, int x, int y)
        crops an image and returns a new handle
*/
PHP_FUNCTION(imagick_copy_crop)
{
    zval *arg;
    int width;
    int height;
    int x;
    int y;
    int argc = ZEND_NUM_ARGS();
    RectangleInfo *rect;
    ExceptionInfo exception;
    php_imagick *handle;
    php_imagick *copy_handle;

    if (zend_parse_parameters(argc TSRMLS_CC, "rllll", &arg, &width, &height, &x, &y) == FAILURE) return;

    IMAGICK_FETCH_RES_AND_COPY_HANDLE();    

    rect = (RectangleInfo *)emalloc(sizeof(RectangleInfo));
    rect->width = width;
    rect->height = height;
    rect->x = x;
    rect->y = y;

    copy_handle->image = CropImage(handle->image,rect,&exception);
    efree(rect);

    IMAGICK_RET_COPY_HANDLE();       

}
/* }}} */


/* {{{ proto bool imagick_rotate(resource handle, double degrees)
        rotates an image
        WARNING: This function will be removed soon, since it's a memory leak canditate
        
*/
PHP_FUNCTION(imagick_rotate)
{
    zval *arg;
    double degrees;
    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;

    IMAGICK_DEPRECATED();

    if (zend_parse_parameters(argc TSRMLS_CC, "rd", &arg, &degrees) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);

    handle->image = RotateImage(handle->image,degrees,&exception);

    if ( handle->image == (Image *)NULL )
    {
        imagick_warning();
        RETURN_FALSE;
    }
    RETVAL_TRUE;
}
/* }}} */

/* {{{ proto bool imagick_copy_rotate(resource handle, double degrees)
        rotates an image and returns a new handle
*/
PHP_FUNCTION(imagick_copy_rotate)
{
    zval *arg;
    double degrees;
    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;
    php_imagick *copy_handle;


    if (zend_parse_parameters(argc TSRMLS_CC, "rd", &arg, &degrees) == FAILURE) return;


    IMAGICK_FETCH_RES_AND_COPY_HANDLE();    

    copy_handle->image = RotateImage(handle->image,degrees,&exception);
    
    IMAGICK_RET_COPY_HANDLE();       
}
/* }}} */

/* {{{ proto bool imagick_shear(resource handle, double x_shear, double y_shear)
        shears an image
        WARNING: This function will be removed soon, since it's a memory leak canditate

*/
PHP_FUNCTION(imagick_shear)
{
    zval *arg;
    double x_shear;
    double y_shear;
    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;
    php_imagick *copy_handle;

    IMAGICK_DEPRECATED();

    if (zend_parse_parameters(argc TSRMLS_CC, "rdd", &arg, &x_shear, &y_shear) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);

    handle->image = ShearImage(handle->image,x_shear,y_shear,&exception);

    if ( handle->image == (Image *)NULL )
    {
        imagick_warning();
        RETURN_FALSE;
    }
    RETVAL_TRUE;
}
/* }}} */

/* {{{ proto handle imagick_copy_shear(resource handle, double x_shear, double y_shear)
        shears an image and returns a new handle
*/
PHP_FUNCTION(imagick_copy_shear)
{
    zval *arg;
    double x_shear;
    double y_shear;
    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;
    php_imagick *copy_handle;

    if (zend_parse_parameters(argc TSRMLS_CC, "rdd", &arg, &x_shear, &y_shear) == FAILURE) return;

    IMAGICK_FETCH_RES_AND_COPY_HANDLE();    

    copy_handle->image = ShearImage(handle->image,x_shear,y_shear,&exception);

    IMAGICK_RET_COPY_HANDLE();       
}
/* }}} */

/* {{{ proto bool imagick_oilpaint(resource handle, double radius)
        oilpain effect on image
*/
PHP_FUNCTION(imagick_oilpaint)
{
    zval *arg;
    double radius;

    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;


    if (zend_parse_parameters(argc TSRMLS_CC, "rd", &arg, &radius) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);

    handle->image = OilPaintImage(handle->image,radius,&exception);

    if ( handle->image == (Image *)NULL )
    {
        imagick_warning();
        RETURN_FALSE;
    }
    RETVAL_TRUE;
}
/* }}} */


/* {{{ proto bool imagick_morph(resource handle, int number_frames)
        morph effect on image
*/
PHP_FUNCTION(imagick_morph)
{
    zval *arg;
    double radius;

    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;


    if (zend_parse_parameters(argc TSRMLS_CC, "rd", &arg, &radius) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);

    handle->image = MorphImages(handle->image,radius,&exception);

    if ( handle->image == (Image *)NULL )
    {
        imagick_warning();
        RETURN_FALSE;
    }
    RETVAL_TRUE;
}
/* }}} */

/* {{{ proto handle imagick_copy_morph(resource handle, int number_frames)
        morph effect on image an returns a new handle
*/
PHP_FUNCTION(imagick_copy_morph)
{
    zval *arg;
    double radius;

    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;
    php_imagick *copy_handle;


    if (zend_parse_parameters(argc TSRMLS_CC, "rd", &arg, &radius) == FAILURE) return;

    IMAGICK_FETCH_RES_AND_COPY_HANDLE();    

    copy_handle->image = MorphImages(handle->image,radius,&exception);

    IMAGICK_RET_COPY_HANDLE();   

}
/* }}} */



/* {{{ proto bool imagick_annotate(resource handle, int number_frames)
        draws primitives on image
*/
PHP_FUNCTION(imagick_annotate)
{
    zval *arg;
    zval *userdata;

    int argc = ZEND_NUM_ARGS();
    ExceptionInfo *exception;
    DrawInfo *drawinfo;
    php_imagick *handle;
    zval **entry;				/* pointer to array entry */
    char *key;
	HashTable *target_hash;
    AffineMatrix
      affine,
      current;

    if (zend_parse_parameters(argc TSRMLS_CC, "ra", &arg, &userdata) == FAILURE) return;
	
    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    drawinfo = CloneDrawInfo(handle->info,(DrawInfo *) NULL);
    (void) CloneString(&drawinfo->primitive,"");
    /* Set up known arguments */
    target_hash = HASH_OF(userdata);
    zend_hash_internal_pointer_reset(target_hash);

	/* Iterate through hash */
	while(zend_hash_get_current_data(target_hash, (void **)&entry) == SUCCESS)
    {
        zend_hash_get_current_key(target_hash, &key,NULL,0);

       switch (*key)
       {
        case 'a':
        case 'A':
            if (LocaleCompare(key,"antialias") == 0)
            {

              drawinfo->stroke_antialias=Z_LVAL_PP(entry) != 0;
              drawinfo->text_antialias=Z_LVAL_PP(entry) != 0;
	      break;
		    }
	 break;
        case 'f':
        case 'F':
            if (LocaleCompare(key,"font") == 0)
            {
                CloneString(&drawinfo->font, Z_STRVAL_PP(entry));
                break;
            }
            if (LocaleCompare(key,"fill") == 0)
            {
              (void) QueryColorDatabase(Z_STRVAL_PP(entry),&drawinfo->fill, exception);	    	
	      break;
            }
	
        break;
    	case 'g':
        case 'G':
		    /* no effect yet ..... */
            if (LocaleCompare(key,"geometry") == 0)
            {
                CloneString(&drawinfo->geometry, Z_STRVAL_PP(entry));
                break;
            }
        break;
    	case 'r':
        case 'R':
            if (LocaleCompare(key,"rotate") == 0)
            {
	                current=drawinfo->affine;
				IdentityAffine(&affine);
	    	    if (Z_LVAL_PP(entry) == 0.0)
                  break;
                affine.sx=cos(DegreesToRadians(fmod(Z_LVAL_PP(entry),360.0)));
                affine.rx=sin(DegreesToRadians(fmod(Z_LVAL_PP(entry),360.0)));
                affine.ry=(-sin(DegreesToRadians(fmod(Z_LVAL_PP(entry),360.0))));
                affine.sy=cos(DegreesToRadians(fmod(Z_LVAL_PP(entry),360.0)));
				drawinfo->affine.sx=current.sx*affine.sx+current.ry*affine.rx;
            	drawinfo->affine.rx=current.rx*affine.sx+current.sy*affine.rx;
            	drawinfo->affine.ry=current.sx*affine.ry+current.ry*affine.sy;
            	drawinfo->affine.sy=current.rx*affine.ry+current.sy*affine.sy;
            	drawinfo->affine.tx=
                current.sx*affine.tx+current.ry*affine.ty+current.tx;
	            drawinfo->affine.ty=
                current.rx*affine.tx+current.sy*affine.ty+current.ty;

				break;
	  	   }
	    break;
	
        case 'p':
        case 'P':
            if (LocaleCompare(key,"primitive") == 0 )
            {
                CloneString(&drawinfo->primitive, Z_STRVAL_PP(entry));
                break;
            }
            if (LocaleCompare(key,"pointsize") == 0)
            {
                drawinfo->pointsize=Z_LVAL_PP(entry);
                break;
            }
        case 's':
        case 'S':
            if (LocaleCompare(key,"stroke") == 0)
            {
		 (void) QueryColorDatabase(Z_STRVAL_PP(entry),&drawinfo->stroke,exception);	
		 break;
	    }
	  break;
        case 't':
        case 'T':
	    /* no effect yet ..... */
            if (LocaleCompare(key,"text") == 0)
            {
                CloneString(&drawinfo->text, Z_STRVAL_PP(entry));
                break;
            }
        break;
	
        break;
       }
       zend_hash_move_forward(target_hash);
	}


	(void) DrawImage(handle->image,drawinfo);


    DestroyDrawInfo(drawinfo);

}
/* }}} */


/* {{{ proto void imagick_list_magickinfo(string filename)
dumps all known imageformats to a file
*/
PHP_FUNCTION(imagick_list_magickinfo)
{

    char *outfile = NULL;
    int outfile_len = 0;

    FILE *f;
    ExceptionInfo exception;

    int argc = ZEND_NUM_ARGS();

    if (zend_parse_parameters(argc TSRMLS_CC, "s", &outfile, &outfile_len) == FAILURE) return;

    GetExceptionInfo(&exception);
    f = fopen (outfile,"w");

    ListMagickInfo ( f,&exception );

    fclose(f);
}
/* }}} */


/* {{{ proto void imagick_dump(resource handle, string imageformat)
 dumps the image to the browser
*/
PHP_FUNCTION(imagick_dump)
{
    zval *arg;
    char * outformat = NULL;
    int outformat_len = 0;
    ExceptionInfo
    exception;
    int argc = ZEND_NUM_ARGS();
    size_t length;
    void    *blob;
    php_imagick *handle;
    register Image *p;
    int scene;
    char filename[MaxTextExtent];

    if (zend_parse_parameters(argc TSRMLS_CC, "r|s", &arg,&outformat,&outformat_len) == FAILURE) return;

    GetExceptionInfo(&exception);


    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    if (outformat != NULL)
    {
        if (outformat_len > MaxTextExtent-1) {
            php_error(E_WARNING, "%s(): Too long format name", get_active_function_name(TSRMLS_C));
            RETURN_FALSE;
        }
        for (p=(handle->image); p != (Image *) NULL; p=p->next)
       {
            strncpy( p->magick, outformat, MaxTextExtent-1 );
       }
    }

    /* this multi image code is borrowed from perlMagick, but somehow it doesn't work
        with only one image it works... anyone an idea why?
    */

    (void) strncpy(filename,handle->info->filename,MaxTextExtent-1);
    scene = 0;
    for (p=(handle->image); p != (Image *) NULL; p=p->next)
    {
      (void) strncpy(p->filename,filename,MaxTextExtent-1);
        p->scene=scene++;
    }

    php_header();
#if APACHE && defined(CHARSET_EBCDIC)
    SLS_FETCH();
    /* This is a binary file already: avoid EBCDIC->ASCII conversion */
    ap_bsetflag(php3_rqst->connection->client, B_EBCDIC2ASCII, 0);
#endif

    for (p=(handle->image); p != (Image *) NULL; p=p->next)
    {
      length=0;
      if ( p == (Image *)NULL )
      {
          imagick_warning();
          RETURN_FALSE;
      }
      blob = ImageToBlob ( handle->info,p, &length, &exception );
      if ( blob == (void *)NULL )
      {
          imagick_warning();
          RETURN_FALSE;
      }
      if (blob != (char *) NULL)
      {
          php_write(blob, length TSRMLS_CC);
          LiberateMemory((void **) &blob);
      }

      if (handle->info->adjoin)
        break;

     }
}
/* }}} */


/* {{{ proto mixed imagick_get_attribute(resource handle, string attribute)
        gets an attribut
*/
PHP_FUNCTION(imagick_get_attribute)
{
    zval *arg;
    char *attribute = NULL;
    int attribute_len = 0;
    int argc = ZEND_NUM_ARGS();
    php_imagick *handle;


    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "rs", &arg, &attribute, &attribute_len) == SUCCESS)
    {
    }

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    switch (*attribute)
    {

    case 'f':
    case 'F':
        if (LocaleCompare(attribute,"format") == 0)
        {
            RETVAL_STRING(handle->image->magick,1);
            break;
        }
    case 'h':
    case 'H':
        if (LocaleCompare(attribute,"height") == 0)
        {
            RETVAL_LONG(handle->image->rows);
            break;
        }

    case 'm':
    case 'M':
        if (LocaleCompare(attribute,"magick") == 0)
        {
            RETVAL_STRING(handle->image->magick,1);
            break;
        }

    case 'q':
    case 'Q':
        if (LocaleCompare(attribute,"quality") == 0)
        {
            RETVAL_LONG(handle->info->quality);
            break;
        }

    case 's':
    case 'S':
        if (LocaleCompare(attribute,"size") == 0)
        {
            RETVAL_STRING(handle->info->size,1);
            break;
        }

    case 'w':
    case 'W':
        if (LocaleCompare(attribute,"width") == 0)
        {
            RETVAL_LONG(handle->image->columns);
        }


    }
    return;
}
/* }}} */


/* {{{ proto void imagick_set_attribute(resource handle, string attribute, mixed value)
 sets an attribute
*/
PHP_FUNCTION(imagick_set_attribute)
{
    zval *arg;
    zval *userdata;

    zval  *value;

    char *attribute = NULL;
    int attribute_len = 0;
    php_imagick *handle;
    Image *image;
    int argc = ZEND_NUM_ARGS();

	HashTable *target_hash;
    zval **entry;				/* pointer to array entry */
    char *key;
	char array_passed = 0;

    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "rsz", &arg, &attribute,&attribute_len, &value) == SUCCESS)
    {

        MAKE_STD_ZVAL(userdata);
        if (array_init(userdata) != SUCCESS)
        {
            php_error(E_ERROR,"%s(): Problems with creating array",get_active_function_name(TSRMLS_C));
            RETVAL_FALSE;
            return;
        }
        convert_to_string(value);
        if (add_assoc_string(userdata,attribute,Z_STRVAL_P(value),1) != SUCCESS)
        {
            php_error(E_ERROR,"%s(): Problems with adding string to array",get_active_function_name(TSRMLS_C));
            RETVAL_FALSE;
            return;
        }
    }

    else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "ra", &arg, &userdata) == SUCCESS)
    {
        /* one or more attribute as an array */
		array_passed = 1;
    }
    else
    {
        WRONG_PARAM_COUNT;
        RETVAL_FALSE;
            return;
    }

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);
    image = handle->image;

    /* Set up known arguments */
    target_hash = HASH_OF(userdata);
    zend_hash_internal_pointer_reset(target_hash);

	/* Iterate through hash */
	while(zend_hash_get_current_data(target_hash, (void **)&entry) == SUCCESS)
    {
        zend_hash_get_current_key(target_hash, &key,NULL,0);
        attribute=key;
       switch (*attribute)
        {
        case 'a':
        case 'A':
            if (LocaleCompare(attribute,"adjoin") == 0)
            {
                convert_to_long_ex(entry);
                handle->info->adjoin = Z_LVAL_PP(entry);
                break;
            }

        case 'd':
        case 'D':
            if (LocaleCompare(attribute,"delay") == 0)
            {
                convert_to_long_ex(entry);
                for ( ; image; image=image->next)
                    image->delay = Z_LVAL_PP(entry);
                break;
            }

        case 'f':
        case 'F':
            if (LocaleCompare(attribute,"format") == 0)
            {
                convert_to_string_ex(entry);
                if (Z_STRLEN_PP(entry) > MaxTextExtent-1) {
                    php_error(E_WARNING, "%s(): Too long 'format' attribute", get_active_function_name(TSRMLS_C));
                }
                else {
                    for ( ; image; image=image->next)
                        strncpy( image->magick, Z_STRVAL_PP(entry), MaxTextExtent-1 );
                }
                break;
            }

        case 'm':
        case 'M':
            if (LocaleCompare(attribute,"magick") == 0)
            {
                convert_to_string_ex(entry);
                if (Z_STRLEN_PP(entry) > MaxTextExtent-1) {
                    php_error(E_WARNING, "%s(): Too long 'magick' attribute", get_active_function_name(TSRMLS_C));
                }
                else {
                    for ( ; image; image=image->next)
                        strncpy( image->magick, Z_STRVAL_PP(entry), MaxTextExtent-1 );
                }
                break;
            }
        case 'q':
        case 'Q':
            if (LocaleCompare(attribute,"quality") == 0)
            {
                convert_to_long_ex(entry);
                handle->info->quality = Z_LVAL_PP(entry);
                break;
            }
        case 's':
        case 'S':
            if (LocaleCompare(attribute,"size") == 0)
            {
                convert_to_string_ex(entry);
                CloneString(&handle->info->size ,  Z_STRVAL_PP(entry));
                break;
            }
        }

	zend_hash_move_forward(target_hash);
	}

	if (!array_passed) {
		zval_dtor(userdata);
		FREE_ZVAL(userdata);
	}
}
/* }}} */


/* {{{ proto string imagick_convert(string infile, string outfile)
 converts image to another format
*/
PHP_FUNCTION(imagick_convert)
{
    char * infile = NULL;
    int infile_len = 0;
    char * outfile = NULL;
    int outfile_len = 0;
    char * outformat = NULL;
    int outformat_len = 0;

    php_imagick *handle;
    int er = 0;
    int argc = ZEND_NUM_ARGS();
    size_t length;
    void    *blob;

    ExceptionInfo
    exception;

    ExceptionInfo *exception_p;

    /* mmh, a one way conversion function... no idea how to do that with reusing the other functions...
    so for the time being, evth is duplicated
    */

    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET,argc TSRMLS_CC, "ss|s", &infile, &infile_len, &outfile, &outfile_len,&outformat, &outformat_len) == SUCCESS)
    {}
    else
    {
        WRONG_PARAM_COUNT;
        return;
    }

    handle = _php_imagick_new();

    if (!handle) {
        RETURN_FALSE;
    }

    if (infile_len > MaxTextExtent-1) {
        php_error(E_WARNING, "%s(): Too long format name", get_active_function_name(TSRMLS_C));
    }
    strncpy( handle->info->filename, infile, MaxTextExtent-1 );

    handle->image=ReadImage(handle->info,&exception);

    if ( handle->image == (Image *)NULL )
    {
        imagick_warning();
        _php_imagick_free(handle);
        RETURN_FALSE;
    }

    /* if outfilename = "-" or null, dump it to the browser */
    if (LocaleCompare(outfile,"-") == 0 || LocaleCompare(outfile,"") == 0 )
    {
        if (outformat != NULL)
        {
            strncpy( handle->image->magick, outformat, MaxTextExtent-1 );
        }
        length=0;
        blob = ImageToBlob ( handle->info,handle->image, &length, &exception );

        if (blob == NULL) {
            imagick_warning();
            RETVAL_FALSE;
        }
        else {
            php_header();
#if APACHE && defined(CHARSET_EBCDIC)
            SLS_FETCH();
            /* This is a binary file already: avoid EBCDIC->ASCII conversion */
            ap_bsetflag(php3_rqst->connection->client, B_EBCDIC2ASCII, 0);
#endif
            php_write(blob, length TSRMLS_CC);
            RETVAL_TRUE;
        }

    }

    else
    {
        strncpy( handle->image->filename, outfile, MaxTextExtent-1 );
        er = WriteImage(handle->info,handle->image);
        if ( er == 0 )
        {
            exception_p = &(handle->image)->exception;
            imagick_warning_p();
            RETVAL_FALSE;
        }
        else {
            RETVAL_TRUE;
        }
    }

    _php_imagick_free(handle);

}
/* }}} */


/*

 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

