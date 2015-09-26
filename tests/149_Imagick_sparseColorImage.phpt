--TEST--
Test Imagick, sparseColorImage
--SKIPIF--
<?php
$imageMagickRequiredVersion=0x653;
require_once(dirname(__FILE__) . '/skipif.inc');
?>
--FILE--
<?php




function createGradientImage($width, $height, $colorPoints, $sparseMethod, $absolute = false) {

    $imagick = new \Imagick();
    $imagick->newImage($width, $height, "rgba(255, 255, 255, 1)");
    $imagick->setImageFormat("png");

    $barycentricPoints = array();

    foreach ($colorPoints as $colorPoint) {

        if ($absolute == true) {
            $barycentricPoints[] = $colorPoint[0];
            $barycentricPoints[] = $colorPoint[1];
        }
        else {
            $barycentricPoints[] = $colorPoint[0] * $width;
            $barycentricPoints[] = $colorPoint[1] * $height;
        }

        if (is_string($colorPoint[2])) {
            $imagickPixel = new \ImagickPixel($colorPoint[2]);
        }
        else if ($colorPoint[2] instanceof \ImagickPixel) {
            $imagickPixel = $colorPoint[2];
        }
        else{
            $errorMessage = sprintf(
                "Value %s is neither a string nor an ImagickPixel class. Cannot use as a color.",
                $colorPoint[2]
            );

            throw new \InvalidArgumentException(
                $errorMessage
            );
        }

        $red = $imagickPixel->getColorValue(\Imagick::COLOR_RED);
        $green = $imagickPixel->getColorValue(\Imagick::COLOR_GREEN);
        $blue = $imagickPixel->getColorValue(\Imagick::COLOR_BLUE);
        $alpha = $imagickPixel->getColorValue(\Imagick::COLOR_ALPHA);

        $barycentricPoints[] = $red;
        $barycentricPoints[] = $green;
        $barycentricPoints[] = $blue;
        $barycentricPoints[] = $alpha;
    }

    $imagick->sparseColorImage($sparseMethod, $barycentricPoints);

    return $imagick;
}

function renderImageBarycentric() {
    $points = array(
        array(0, 0, 'skyblue'),
        array(-1, 1, 'skyblue'),
        array(1, 1, 'black'),
    );
    $imagick = createGradientImage(600, 200, $points, \Imagick::SPARSECOLORMETHOD_BARYCENTRIC);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
    echo "Ok".PHP_EOL;
}
function renderImageVoronoi() {
    $points = array(
        array(0.30, 0.10, 'red'),
        array(0.10, 0.80, 'blue'),
        array(0.70, 0.60, 'lime'),
        array(0.80, 0.20, 'yellow'),
    );
    $imagick = createGradientImage(500, 500, $points, \Imagick::SPARSECOLORMETHOD_VORONOI);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
    echo "Ok".PHP_EOL;
}
function renderImageShepards() {
    $points = array(
            array(0.30, 0.10, 'red'),
            array(0.10, 0.80, "RGBA(0, 255, 0, 0.5)"),
            array(0.70, 0.60, "RGBA(0, 255, 0, 1)"),
            array(0.80, 0.20, 'yellow'),
        );
    $imagick = createGradientImage(600, 600, $points, \Imagick::SPARSECOLORMETHOD_SPEPARDS);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";}
    echo "Ok".PHP_EOL;
}

function renderImageBilinear() {
    $points = array(
        array(0.30, 0.10, 'red'),
        array(0.10, 0.80, 'blue'),
        array(0.70, 0.60, 'lime'),
        array(0.80, 0.20, 'yellow'),
    );
    $imagick = createGradientImage(500, 500, $points, \Imagick::SPARSECOLORMETHOD_BILINEAR);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";}
    echo "Ok".PHP_EOL;
}

try {
	renderImageBilinear() ;
}
catch (\Exception $e) {
	echo "renderImageBilinear failed ".$e->getMessage().PHP_EOL;
}
try {
	renderImageShepards();
}
catch (\Exception $e) {
	echo "renderImageShepards failed ".$e->getMessage().PHP_EOL;
}
try {
	renderImageVoronoi();
}
catch (\Exception $e) {
	echo "renderImageVoronoi failed ".$e->getMessage().PHP_EOL;
}
try {
	renderImageBarycentric();
}
catch (\Exception $e) {
	echo "renderImageBarycentric failed ".$e->getMessage().PHP_EOL;
}

?>
--EXPECTF--
Ok
Ok
Ok
Ok