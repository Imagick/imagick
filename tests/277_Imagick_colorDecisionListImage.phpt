--TEST--
Test Imagick::colorDecisionListImage
--SKIPIF--
<?php

$imageMagickRequiredVersion = 0x676;

require_once(dirname(__FILE__) . '/skipif.inc');
checkClassMethods('Imagick', array('colorDecisionListImage'));

?>
--FILE--
<?php

$im = new Imagick("magick:logo");

$ccc = <<< CCC
<ColorCorrectionCollection xmlns="urn:ASC:CDL:v1.2">
    <ColorCorrection id="cc03345">
          <SOPNode>
               <Slope> 0.9 1.2 0.5 </Slope>
               <Offset> 0.4 -0.5 0.6 </Offset>
               <Power> 1.0 0.8 1.5 </Power>
          </SOPNode>
          <SATNode>
               <Saturation> 0.85 </Saturation>
          </SATNode>
    </ColorCorrection>
</ColorCorrectionCollection>
CCC;

$im->colorDecisionListImage($ccc);

echo "Ok"
?>
--EXPECT--
Ok