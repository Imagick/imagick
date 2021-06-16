<?php

declare(strict_types = 1);


if ($argc !== 2) {
    fwrite(STDERR, "usage php fixup_arginfo.php \$arginfo_filename\n");
    exit(-1);
}

$filename = $argv[1];

$fixup_note = "file has been fixedup for different versions";

echo "Fixing $filename\n";

$input_lines = file($filename);

foreach ($input_lines as $input_line) {
    if (strpos($input_line, $fixup_note) !== false) {
        echo "File has already been fixedup.\n";
        exit(0);
    }
}

$output_lines = [];

$search = [];
$replace = [];

$search[] = "#.*Stub hash: (.*) .*/#iu";
$replace[] = "* Stub hash: regen with 'sh regen_arginfo.sh' \n* $fixup_note */";

$search[] = "#ZEND_ARG_OBJ_INFO\(0, (\w*), IMAGICK_QUANTUM_TYPE, 0\)#iu";
$replace[] = "
#if MAGICKCORE_HDRI_ENABLE 
	ZEND_ARG_TYPE_INFO(0, $1, IS_DOUBLE, 0)
#else
	ZEND_ARG_TYPE_INFO(0, $1, IS_LONG, 0)
#endif
";


// ZEND_ARG_TYPE_INFO(pass_by_ref, name, type_hint, allow_null)
// ZEND_ARG_INFO(pass_by_ref, name)

$search[] = "#ZEND_ARG_TYPE_INFO\((\w*), (\w*), (\w*), (\w*)\)#iu";
$replace[] = "
#if PHP_VERSION_ID >= 80000
    ZEND_ARG_TYPE_INFO($1, $2, $3, $4)
#else
    ZEND_ARG_INFO($1, $2)
#endif";

$search[] = "#ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX\((\w*), 0, (\w*), IMAGICK_QUANTUM_TYPE, 0\)#iu";
$replace[] = "
#if MAGICKCORE_HDRI_ENABLE 
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX($1, 0, $2, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX($1, 0, $2, IS_LONG, 0)
#endif
";


//ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null)
#define ZEND_BEGIN_ARG_INFO_EX(name, _unused, return_reference, required_num_args)
$search[] = "#ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX\((\w*), (\w*), (\w*), (\w*), (\w*)\)#iu";
$replace[] = "
#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX($1, $2, $3, $4, $5)
#else
ZEND_BEGIN_ARG_INFO_EX($1, 0, $2, $3)
#endif
";

//#define ZEND_ARG_TYPE_MASK(pass_by_ref, name, type_mask, default_value) \
$search[] = "#.*ZEND_ARG_TYPE_MASK\(([\w|\|]*), ([\w|\|]*), ([\w|\|]*), ([\w\|\"]*)\)#iu";
$replace[] = "
#if PHP_VERSION_ID >= 80000
    ZEND_ARG_TYPE_MASK($1, $2, $3, $4)
#else
    ZEND_ARG_INFO($1, $2)
#endif
";

//ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(name, return_reference, required_num_args, type)
//ZEND_BEGIN_ARG_INFO_EX(name, _unused, return_reference, required_num_args)

$search[] = "#.*ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX\(([\w|\|]*), ([\w|\|]*), ([\w|\|]*), ([\w|\|]*)\)#iu";
$replace[] = "
#if PHP_VERSION_ID >= 80000
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX($1, $2, $3, $4)
#else
    ZEND_BEGIN_ARG_INFO_EX($1, 0, $2, $3)
#endif
";

//ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, return_reference, required_num_args, class_name, allow_null)
$search[] = "#ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX\((\w*), (\w*), (\w*), (\w*), (\w*)\)#iu";
$replace[] = "
#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX($1, $2, $3, $4, $5)
#else
ZEND_BEGIN_ARG_INFO_EX($1, 0, $2, $3)
#endif
";

//ZEND_ARG_OBJ_INFO(pass_by_ref, name, classname, allow_null) \
$search[] = "#.*ZEND_ARG_OBJ_INFO\((\w*), (\w*), resource, (\w*)\)#iu";
$replace[] = "
#if PHP_VERSION_ID >= 80000
\tZEND_ARG_OBJ_INFO($1, $2, resource, $3)
#else
\tZEND_ARG_INFO($1, $2)
#endif
";

// ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, filename, IS_STRING, 1, "null")
$search[] = "#.*ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE\((\w*), (\w*), (\w*), (\w*), ([\w\"]*)\)#iu";
$replace[] = "
#if PHP_VERSION_ID >= 80000
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE($1, $2, $3, $4, $5)
#else
    ZEND_ARG_INFO($1, $2)
#endif
";


//#if PHP_VERSION_ID >= 80000
//ZEND_ARG_TYPE_MASK(0, files, MAY_BE_STRING|MAY_BE_ARRAY|MAY_BE_LONG|MAY_BE_DOUBLE|MAY_BE_NULL, NULL)
//#else
//    ZEND_ARG_INFO(0, files)
//#endif



foreach ($input_lines as $input_line) {
    $input_line = rtrim($input_line);
    $input_line = preg_replace($search, $replace, $input_line);

    $output_lines[] = $input_line;
}

file_put_contents($filename, implode("\n", $output_lines));
echo "File has now been fixedup.\n";
