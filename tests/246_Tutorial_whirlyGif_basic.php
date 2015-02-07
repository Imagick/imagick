<?php

$backgroundColor = white;
$numberDots = 40;
$numberFrames = 25;
$loopTime = 160;
$phaseMultiplier = 16;
$phaseDivider = 8;


function lerp($t, $a, $b) {
    return $a + ($t * ($b - $a));
}

class Dot {
    
    function __construct($color, $sequence, $numberDots) {
        $this->color = $color;
        $this->sequence = $sequence;
        $this->numberDots = $numberDots;
    }

    function calculateFraction($frame, $maxFrames, $timeOffset, $phaseMultiplier, $phaseDivider) {

        $frame = -$frame;
        
        $totalAngle = 2 * $phaseMultiplier;

        $fraction = ($frame / $maxFrames * 2);
        $fraction += $totalAngle * ($this->sequence / $this->numberDots);
        $fraction += (($this->sequence)) / ($phaseDivider);
        $fraction += $timeOffset;

        while ($fraction < 0) {
            //fmod does not work 'correctly' on negative numbers
            $fraction += 64;
        }

        $fraction = fmod($fraction, 2);
        
        if ($fraction > 1) {
            $unitFraction =  2 - $fraction;
        }
        else {
            $unitFraction = $fraction;
        }

        return $unitFraction * $unitFraction * (3 - 2 * $unitFraction);
    }
    
  
    
    function render(\ImagickDraw $draw, $frame, $maxFrames, $phaseMultiplier, $phaseDivider) {
        $innerDistance = 40;
        $outerDistance = 230;

        $sequenceFraction = $this->sequence / $this->numberDots;
        $angle = 2 * M_PI * $sequenceFraction;
        
        $trailSteps = 5;
        $trailLength = 0.1;
        
        $offsets = [
            100 => 0,
        ];
        
        for($i=0 ; $i<=$trailSteps ; $i++) {
            $key = intval(50 * $i / $trailSteps);
            $offsets[$key] = $trailLength * ($trailSteps - $i) / $trailSteps;
        }

        //TODO - using a pattern would make the circles look more natural
        //$draw->setFillPatternURL();

        foreach ($offsets as $alpha => $offset) {
            $distanceFraction = $this->calculateFraction($frame, $maxFrames, $offset, $phaseMultiplier, $phaseDivider);
            $distance = lerp($distanceFraction, $innerDistance, $outerDistance);
            $xOffset = $distance * sin($angle);
            $yOffset = $distance * cos($angle);
            $draw->setFillColor($this->color);
            $draw->setFillOpacity($alpha / 100);
            $draw->circle(
                $xOffset, $yOffset,
                $xOffset + 4, $yOffset + 4
            );
        }
    }
}


function whirlyGif($numberDots, $numberFrames, $loopTime, $backgroundColor, $phaseMultiplier, $phaseDivider) {
    $aniGif = new \Imagick();
    $aniGif->setFormat("gif");
    
    
    $maxFrames = $numberFrames;
    $frameDelay = ceil($loopTime / $maxFrames);

    $scale = 1;
    $startColor = new \ImagickPixel('red');
    $dots = [];

    $width = 500;
    $height = $width;
    
    for ($i=0 ; $i<$numberDots ; $i++) {
        $colorInfo = $startColor->getHSL();

        //Rotate the hue by 180 degrees
        $newHue = $colorInfo['hue'] + ($i / $numberDots);
        if ($newHue > 1) {
            $newHue = $newHue - 1;
        }

        //Set the ImagickPixel to the new color
        $color = new \ImagickPixel('#ffffff');
        $colorInfo['saturation'] *= 0.95;
        $color->setHSL($newHue, $colorInfo['saturation'], $colorInfo['luminosity']);

        $dots[] = new Dot($color, $i, $numberDots);
    }

    

    for ($frame = 0; $frame < $maxFrames; $frame++) {
        $draw = new \ImagickDraw();
        $draw->setStrokeColor('none');
        $draw->setFillColor('none');
        $draw->rectangle(0, 0, 500, 500);
        
        $draw->translate($width / 2, $height / 2);

        foreach($dots as $dot) {
            /** @var $dot Dot */
            $dot->render($draw, $frame, $maxFrames, $phaseMultiplier, $phaseDivider);
        }

        //Create an image object which the draw commands can be rendered into
        $imagick = new \Imagick();
        $imagick->newImage(500 * $scale, 500 * $scale, $backgroundColor);
        $imagick->setImageFormat("png");

        $imagick->setImageDispose(\Imagick::DISPOSE_PREVIOUS);

        //Render the draw commands in the ImagickDraw object
        //into the image.
        $imagick->drawImage($draw);
                
        $imagick->setImageDelay($frameDelay);
        $aniGif->addImage($imagick);
        $imagick->destroy();
    }

    //echo "Got here here and level is ".ob_get_level()."<br/>";
    $aniGif->setImageFormat('gif');
    
    $aniGif->setImageIterations(0); //loop forever
    $aniGif->mergeImageLayers(\Imagick::LAYERMETHOD_OPTIMIZEPLUS);


    $bytes = $aniGif->getImagesBlob();
    if (strlen($bytes) <= 0) { echo "Failed to generate image.";} 

//    $aniGif->writeImages("./smoothOut.gif", true);
}

whirlyGif($numberDots, $numberFrames, $loopTime, $backgroundColor, $phaseMultiplier, $phaseDivider) ;
echo "Ok";
?>
