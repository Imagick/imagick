[![Build Status](https://travis-ci.org/mkoppanen/imagick.png)](https://travis-ci.org/mkoppanen/imagick)

# Imagick

Imagick is a PHP extension to create and modify images using the ImageMagick library. There is also a version of Imagick available for HHVM. Although the two extensions are mostly compatible in their API, and they both call the ImageMagick library, the two extensions are completely separate code-bases.

## PHP Imagick

* Source code + issues - https://github.com/mkoppanen/imagick
* Releases - http://pecl.php.net/package/imagick
* Documentation - http://php.net/manual/en/book.imagick.php
    
Bugs can also be reported at https://bugs.php.net but may have a slower response time.

## HHVM Imagick

* https://github.com/facebook/hhvm/tree/master/hphp/runtime/ext/imagick


# Installing on Windows

For Windows please install both Imagick and the underlying ImageMagick library from:
* http://windows.php.net/downloads/pecl/releases/imagick/
* http://windows.php.net/downloads/pecl/deps/

Once compiled, Imagick should be run with the same version of ImageMagick that it was compiled against. Running against a different version of ImageMagick is known to cause stability issues, and so is not recommended or supported.

# Examples

Almost all of the functions in the library have an example at [www.phpimagick.com](http://www.phpimagick.com/), where you can see the example code to call the function, as well as the generated image or output.


# ImageMagick


ImageMagick, the library that the Imagick extension exposes to PHP has had many bug fixes, that have fixed many image corruption issues. We _strongly_ recommend upgrading to the latest version (currently 6.9.1) if at all possible.


# Security

The PHP extension Imagick works by calling the ImageMagick library. Although the ImageMagick developers take good care in avoiding bugs it is inevitable that some bugs will be present in the code. ImageMagick also uses a lot of third party libraries to open, read and manipulate files. The writers of these libraries also take care when writing their code. However everyone makes mistakes and there will inevitably be some bugs present.

Because ImageMagick is used to process images it is feasibly possible for hackers to create images that contain invalid data to attempt to exploit these bugs. Because of this we recommend the following:

1) Do not run Imagick in a server that is directly accessible from outside your network. It is better to either use it as a background task using something like SupervisorD or to run it in a separate server that is not directly accessible on the internet. 

Doing this will make it more difficult for hackers to exploit a bug, if one should exist in the libraries that ImageMagick is using. 

2) Run it as a very low privileged process. As much as possible the files and system resources accessible to the PHP script that Imagick is being called from should be locked down. 

3) Verify that all image files begin with the expected "magic bytes" corresponding to the image file types you support before sending them to ImageMagick for processing. This an be be done with finfo_file() - see below.

4) Check the result of the image processing is a valid image file before displaying it to the user. In the extremely unlikely event that a hacker is able to pipe arbitrary files to the output of Imagick, checking that it is an image file, and not the source code of your application that is being sent, is a sensible precaution. This can be accomplished by the following code:


```
<?php
	$finfo = finfo_open(FILEINFO_MIME_TYPE);
	$mimeType = finfo_file($finfo, $filename);
	
	$allowedMimeTypes = [
		'image/gif',
		'image/jpeg',
		'image/jpg',
		'image/png'
	];
	
	if (in_array($mimeType, $allowedMimeTypes) == false) {
		throw new \SecurityException("Was going to send file '$filename' to the user, but it is not an image file.");
	}
```

5) NEVER directly serve any files that have been uploaded by users directly through PHP, instead either serve them through the webserver, without invoking PHP, or use [readfile](http://php.net/manual/en/function.readfile.php) to serve them within PHP.

These recommendations do not guarantee any security, but they should limit your exposure to any Imagick/ImageMagick related security issue.

# OpenMP

ImageMagick has the ability to use the Open Multi-Processing API to be able to use multiple threads to process an image at once. Some implementations of OpenMP are known to have stability issues when they are used in certain environments. 

We recommend doing one of the following:

* Disabling OpenMP support in ImageMagick by compiling it with the compile flag "--disable-openmp" set.

* Disable the use of threads in ImageMagick via Imagick by calling: `Imagick::setResourceLimit(\Imagick::RESOURCETYPE_THREAD, 1);` or `Imagick::setResourceLimit(6, 1);` if your version of Imagick does not contain the RESOURCETYPE_THREAD constant.

* Disable the use of threads in ImageMagick by setting the thread resource limit in ImageMagick' policy.xml file with `<policy domain="resource" name="thread" value="1"/>` This file is possibly located at /etc/ImageMagick-6/policy.xml or similar location.

* If you do want to use OpenMP in ImageMagick when it's called through Imagick, you should test thoroughly that it behaves correctly on your server.


# TODO

Documentation needs a lot of work. There is an online editor here: [https://edit.php.net/](https://edit.php.net/) Contributions are more than welcome.

Please refer to http://abi-laboratory.pro/tracker/timeline/imagemagick/ for exact version changes of the underlying ImageMagick library.
