--TEST--
Test Imagick, sparseColorImage
--SKIPIF--
<?php require_once(dirname(__FILE__) . '/skipif.inc'); ?>
--FILE--
<?php




function createGradientImage($width, $height, $colorPoints, $sparseMethod, $absolute = false) {

    $imagick = new \Imagick();
    $imagick->newImage($width, $height, "white");
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
    $points = [
        [0, 0, 'skyblue'],
        [-1, 1, 'skyblue'],
        [1, 1, 'black'],
    ];
    $imagick = createGradientImage(600, 200, $points, \Imagick::SPARSECOLORMETHOD_BARYCENTRIC);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
    echo "Ok".PHP_EOL;
}
function renderImageVoronoi() {
    $points = [
        [0.30, 0.10, 'red'],
        [0.10, 0.80, 'blue'],
        [0.70, 0.60, 'lime'],
        [0.80, 0.20, 'yellow'],
    ];
    $imagick = createGradientImage(500, 500, $points, \Imagick::SPARSECOLORMETHOD_VORONOI);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 
    echo "Ok".PHP_EOL;
}
function renderImageShepards() {
    $points = [
            [0.30, 0.10, 'red'],
            [0.10, 0.80, 'blue'],
            [0.70, 0.60, 'lime'],
            [0.80, 0.20, 'yellow'],
        ];
    $imagick = createGradientImage(600, 600, $points, \Imagick::SPARSECOLORMETHOD_SPEPARDS);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";}
    echo "Ok".PHP_EOL;
}

function renderImageBilinear() {
    $points = [[0.30, 0.10, 'red'], [0.10, 0.80, 'blue'], [0.70, 0.60, 'lime'], [0.80, 0.20, 'yellow'],];
    $imagick = createGradientImage(500, 500, $points, \Imagick::SPARSECOLORMETHOD_BILINEAR);
    $bytes = $imagick->getImageBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";}
    echo "Ok".PHP_EOL;
}

renderImageBilinear() ;
renderImageShepards();
renderImageVoronoi();
renderImageBarycentric();

?>
--EXPECTF--
Ok
Ok
Ok
Ok