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
   |                                                                      |
   +----------------------------------------------------------------------+
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_imagick.h"


/* If you declare any globals in php_imagick.h uncomment this:

*/
ZEND_DECLARE_MODULE_GLOBALS(imagick)
/* True global resources - no need for thread safety here */
static int le_imagick;

/* {{{ imagick_functions[]
 *
 * Every user visible function must have an entry in imagick_functions[].
 */
function_entry imagick_functions[] = {

                                        PHP_FE(imagick_new, NULL)
                                        PHP_FE(imagick_read, NULL)
                                        PHP_FE(imagick_sample, NULL)
                                        PHP_FE(imagick_resize, NULL)                                                
                                        PHP_FE(imagick_crop, NULL)                                                
                                        PHP_FE(imagick_shear, NULL)  
                                        PHP_FE(imagick_rotate, NULL)  
                                        PHP_FE(imagick_oilpaint, NULL) 
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
                                            STANDARD_MODULE_HEADER,
                                            "imagick",
                                            imagick_functions,
                                            PHP_MINIT(imagick),
                                            PHP_MSHUTDOWN(imagick),
                                            PHP_RINIT(imagick),		/* Replace with NULL if there's nothing to do at request start */
                                            PHP_RSHUTDOWN(imagick),	/* Replace with NULL if there's nothing to do at request end */
                                            PHP_MINFO(imagick),
                                            NO_VERSION_YET,
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

static void _php_imagick_free(php_imagick *handle)
{
    if (handle->image)
    {
    DestroyImage(handle->image);
    }
    DestroyImageInfo(handle->info);
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
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_UNDEFINED",0, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_POINT",1, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_BOX",2, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_TRIANGLE",3, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_HERMITE",4, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_HANNING",5, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_HAMMING",6, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_BLACKMAN",7, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_GAUSSIAN",8, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_QUADRATIC",9, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_CUBIC",10, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_CATROM",11, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_MITCHELL",12, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_LANCZOS",13, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_BESSEL",14, CONST_CS | CONST_PERSISTENT);
REGISTER_LONG_CONSTANT("IMAGICK_FILTER_SINC",15, CONST_CS | CONST_PERSISTENT);
            
            
    le_imagick = register_list_destructors(_php_imagick_free,NULL);
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
    int *version;

    php_info_print_table_start();
    php_info_print_table_header(2, "imagick support", "enabled");
    php_info_print_table_row(2, "ImageMagickVersion", GetMagickVersion(version));
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
        php_imagick *handle;
    int argc = ZEND_NUM_ARGS();
    int id, type;
    if (zend_parse_parameters(argc TSRMLS_CC, "r", &arg) == FAILURE) return;
    id = Z_LVAL_P(arg);
    if (zend_list_find(id, &type) && type == le_imagick)
                zend_list_delete(id);

}

/* {{{ proto bool imagick_new()
        creates a new imagick-handle
*/        

PHP_FUNCTION(imagick_new)
{
    php_imagick *handle;

    handle = (php_imagick *)emalloc(sizeof(php_imagick));

    if (!handle) {
        php_error(E_WARNING, "%s() couldn't allocate PHP-Magick handle", get_active_function_name(TSRMLS_C));
        RETURN_FALSE;
    }

    memset(handle, 0, sizeof(php_imagick));
    handle->info =   CloneImageInfo((ImageInfo *) NULL);;
    ZEND_REGISTER_RESOURCE(return_value, handle, le_imagick);

}
/* {{{ proto bool imagick_read(resource handle, string infile)
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
    ExceptionInfo
    exception;
    Image *next_image;
    register Image *p;

    /* array stuff */ 

	HashTable *target_hash;
    zval **entry;				/* pointer to array entry */

    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "rs", &arg, &infile, &infile_len) == SUCCESS) 
    {
        /* only one filename as a string */
        MAKE_STD_ZVAL(userdata);
        if (array_init(userdata) != SUCCESS)
        {
         php_error(E_ERROR,"%s() Problems with creating array",get_active_function_name(TSRMLS_C));            
            RETVAL_FALSE;
            return;
        }
        if (add_index_stringl(userdata,0,infile,infile_len,1) != SUCCESS)
        {
            php_error(E_ERROR,"%s() Problems with adding string to array",get_active_function_name(TSRMLS_C));            
            RETVAL_FALSE;
            return;             
        }
    }
    else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "ra", &arg, &userdata) == SUCCESS) 
    {
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
        strncpy( handle->info->filename, Z_STRVAL_PP(entry), MaxTextExtent-1 );
        next_image=ReadImage(handle->info,&exception);
        
        if ( next_image == (Image *)NULL )
        {
         php_error(E_ERROR,"%s() Problems with opening file %s: %s",get_active_function_name(TSRMLS_C),infile,exception.reason);
         RETVAL_FALSE;
         return;
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
	zval_dtor(userdata);
    FREE_ZVAL(userdata);
}

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
  register Image
    *p;
  long
    scene = 0;


    if (zend_parse_parameters(argc TSRMLS_CC, "rs", &arg, &outfile, &outfile_len) == FAILURE) return;
/*    if (zend_parse_parameters(argc TSRMLS_CC, "rs|s", &arg, &outfile, &outfile_len, &outformat, &outformat_len) == FAILURE) return;*/

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

/*    strncpy( handle->image->filename, outfile, MaxTextExtent-1 );

    if (outformat != NULL)
    {
        strncpy( handle->image->magick, outformat, MaxTextExtent-1 );     
    }
*/
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
        php_error(E_ERROR,"%s() cannot save file %s: %s",get_active_function_name(TSRMLS_C), outfile,handle->image->exception.reason);
        RETVAL_FALSE;
        return;
    }

    if (handle->info->adjoin)
      break;
  }

    RETVAL_TRUE;

}

/* {{{ proto bool imagick_sample(resource handle, int columns, int rows)
        samples an image 
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

    if (zend_parse_parameters(argc TSRMLS_CC, "rll", &arg, &columns, &rows) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);

    handle->image = SampleImage(handle->image,columns,rows,&exception);
    if ( handle->image == (Image *)NULL )
    {
        php_error(E_ERROR,"%s() error sampling image: %s",get_active_function_name(TSRMLS_C), exception.reason);
        RETVAL_FALSE;
        return;
    }

}

/* {{{ proto bool imagick_resize(resource handle, int columns, int rows, int filter, int blur)
        resizes an image with the method filter and the blurness blur
*/        


PHP_FUNCTION(imagick_resize)
{
    zval *arg;
    int columns;
    int rows;
    int argc = ZEND_NUM_ARGS();
    int filter =13; /* IMAGICK_FILTER_LANCZOS */
    double blur = 0;    
    ExceptionInfo
    exception;
    php_imagick *handle;

    if (zend_parse_parameters(argc TSRMLS_CC, "rll|ld", &arg, &columns, &rows, &filter, &blur) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);

    handle->image = ResizeImage(handle->image,columns,rows,filter,blur,&exception);
    if ( handle->image == (Image *)NULL )
    {
        php_error(E_ERROR,"%s() error resizing image: %s",get_active_function_name(TSRMLS_C), exception.reason);
        RETVAL_FALSE;
        return;
    }
    RETVAL_TRUE;
}
/* {{{ proto bool imagick_crop(resource handle, int width, int height, int x, int y)
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
        php_error(E_ERROR,"%s() error croping image: %s",get_active_function_name(TSRMLS_C), exception.reason);
        RETVAL_FALSE;
        return;
    }
     RETVAL_TRUE;
}

/* {{{ proto bool imagick_rotate(resource handle, double degrees)
        crops an image
*/        
PHP_FUNCTION(imagick_rotate)
{
    zval *arg;
    double degrees;    
    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;
    

    if (zend_parse_parameters(argc TSRMLS_CC, "rd", &arg, &degrees) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);
    
    handle->image = RotateImage(handle->image,degrees,&exception);

    if ( handle->image == (Image *)NULL )
    {
        php_error(E_ERROR,"%s() error rotating image: %s",get_active_function_name(TSRMLS_C), exception.reason);
        RETVAL_FALSE;
        return;
    }
    RETVAL_TRUE;
}

/* {{{ proto bool imagick_shear(resource handle, double x_shear, double y_shear)
        crops an image
*/        
PHP_FUNCTION(imagick_shear)
{
    zval *arg;
    double x_shear;    
    double y_shear;        
    int argc = ZEND_NUM_ARGS();
    ExceptionInfo exception;
    php_imagick *handle;
    

    if (zend_parse_parameters(argc TSRMLS_CC, "rdd", &arg, &x_shear, &y_shear) == FAILURE) return;

    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    GetExceptionInfo(&exception);
    
    handle->image = ShearImage(handle->image,x_shear,y_shear,&exception);

    if ( handle->image == (Image *)NULL )
    {
        php_error(E_ERROR,"%s() error shearing image: %s",get_active_function_name(TSRMLS_C), exception.reason);
        RETVAL_FALSE;
        return;
    }
    RETVAL_TRUE;
}

/* {{{ proto bool imagick_oilpaint(resource handle, double radius)
        crops an image
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
        php_error(E_ERROR,"%s() error oilpainting image: %s",get_active_function_name(TSRMLS_C), exception.reason);
        RETVAL_FALSE;
        return;
    }
    RETVAL_TRUE;
}

/* {{{ proto bool imagick_morph(resource handle, int number_frames)
        crops an image
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
        php_error(E_ERROR,"%s() error morphing image: %s",get_active_function_name(TSRMLS_C), exception.reason);
        RETVAL_FALSE;
        return;
    }
    RETVAL_TRUE;
}


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
    if (zend_parse_parameters(argc TSRMLS_CC, "r|s", &arg,&outformat,&outformat_len) == FAILURE) return;

    GetExceptionInfo(&exception);


    ZEND_FETCH_RESOURCE(handle, php_imagick *, &arg, -1,  "imagick object", le_imagick);

    if (outformat != NULL)
    {
        strncpy( handle->image->magick, outformat, MaxTextExtent-1 );     
    }
    
    length=0;
    blob = ImageToBlob ( handle->info,handle->image, &length, &exception );
    php_header();
#if APACHE && defined(CHARSET_EBCDIC)
    SLS_FETCH();
    /* This is a binary file already: avoid EBCDIC->ASCII conversion */
    ap_bsetflag(php3_rqst->connection->client, B_EBCDIC2ASCII, 0);
#endif
    php_write(blob, length TSRMLS_CC);
}

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
    int intvalue = 0;
    char *strvalue = NULL;
    php_imagick *handle;
    Image *image;
    int argc = ZEND_NUM_ARGS();

	HashTable *target_hash;
    zval **entry;				/* pointer to array entry */
    char *key;        
	HashPosition pos;

    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "rsz", &arg, &attribute,&attribute_len, &value) == SUCCESS) 
    {

        MAKE_STD_ZVAL(userdata);
        if (array_init(userdata) != SUCCESS)
        {
         php_error(E_ERROR,"%s() Problems with creating array",get_active_function_name(TSRMLS_C));            
            RETVAL_FALSE;
            return;
        }
        convert_to_string(value);            
        if (add_assoc_string(userdata,attribute,Z_STRVAL_P(value),1) != SUCCESS)
        {
            php_error(E_ERROR,"%s() Problems with adding string to array",get_active_function_name(TSRMLS_C));            
            RETVAL_FALSE;
            return;             
        }
    }
        
    else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc TSRMLS_CC, "ra", &arg, &userdata) == SUCCESS) 
    {
        /* one or more attribute as an array */        
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
                for ( ; image; image=image->next)
                    strncpy( image->magick, Z_STRVAL_PP(entry), MaxTextExtent-1 );
                break;
            }

        case 'm':
        case 'M':
            if (LocaleCompare(attribute,"magick") == 0)
            {
                convert_to_string_ex(entry);                
                for ( ; image; image=image->next)
                    strncpy( image->magick, Z_STRVAL_PP(entry), MaxTextExtent-1 );
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
	zval_dtor(userdata);
    FREE_ZVAL(userdata);

}


/* {{{ proto string imagick_convert(string infile, string outfile)
 converts one fileformat to another in one go
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


    handle = (php_imagick *)emalloc(sizeof(php_imagick));

    if (!handle) {
        php_error(E_WARNING, "%s() couldn't allocate PHP-Magick handle", get_active_function_name(TSRMLS_C));
        RETURN_FALSE;
    }

    memset(handle, 0, sizeof(php_imagick));
    handle->info =   CloneImageInfo((ImageInfo *) NULL);;

    strncpy( handle->info->filename, infile, MaxTextExtent-1 );

    handle->image=ReadImage(handle->info,&exception);

    if ( handle->image == (Image *)NULL )
    {
        php_error(E_ERROR,"%s() problem with opening file %s: %s",get_active_function_name(TSRMLS_C), infile,exception.reason);
        RETVAL_FALSE;
        return;
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
        php_header();
#if APACHE && defined(CHARSET_EBCDIC)
        SLS_FETCH();
        /* This is a binary file already: avoid EBCDIC->ASCII conversion */
        ap_bsetflag(php3_rqst->connection->client, B_EBCDIC2ASCII, 0);
#endif
        php_write(blob, length TSRMLS_CC);

    }

    else
    {
        strncpy( handle->image->filename, outfile, MaxTextExtent-1 );
        er = WriteImage(handle->info,handle->image);
        if ( er == 0 )
        {
            php_error(E_ERROR,"%s() problems with saving file %s: %s",get_active_function_name(TSRMLS_C), outfile,handle->image->exception.reason);
            RETVAL_FALSE;
            return;
        }
    }
    DestroyImageInfo(handle->info);
    DestroyImage(handle->image);
    RETVAL_TRUE;


}


/*

 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

