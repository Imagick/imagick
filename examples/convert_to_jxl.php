<?php
/**
 * Convert an image to JPEG XL: Imagick when this ImageMagick build has
 * the libjxl delegate (probed via Imagick::queryFormats('JXL')), else
 * cjxl (the only backend that can transcode JPEG losslessly, bit-exact),
 * ffmpeg/libjxl, then magick. Success requires a JPEG XL codestream
 * (FF 0A) or container signature in the output.
 *
 *   php convert_to_jxl.php [--lossless] [--distance 1.0] [--effort 7] in.png [out.jxl]
 */
declare(strict_types=1);

function isJpegXlFile(string $path): bool
{
    if (!is_file($path)) {
        return false;
    }
    $head = file_get_contents($path, false, null, 0, 12);
    return $head !== false
        && (str_starts_with($head, "\xFF\x0A")
            || str_starts_with($head, "\x00\x00\x00\x0CJXL \x0D\x0A\x87\x0A"));
}

function convertToJxl(string $src, string $dst, bool $lossless, float $distance, int $effort): string
{
    if (extension_loaded('imagick')
        && in_array('JXL', Imagick::queryFormats('JXL'), true)) {
        try {
            $im = new Imagick($src);
            $im->setImageFormat('JXL');
            // ImageMagick's JXL coder treats quality 100 as lossless.
            $im->setImageCompressionQuality(
                $lossless ? 100 : max(10, min(100, (int) round(100 - $distance * 10))));
            $im->writeImage($dst);
            $im->clear();
            if (isJpegXlFile($dst)) {
                return $dst;
            }
        } catch (ImagickException) {
            @unlink($dst);
        }
    }
    $cjxl = ['cjxl', $src, $dst, "--effort=$effort",
             $lossless ? '--lossless=1' : "--distance=$distance"];
    $ffmpeg = ['ffmpeg', '-y', '-i', $src, '-c:v', 'libjxl', '-effort', (string) $effort,
               ...($lossless ? ['-lossless', '1'] : ['-distance', (string) $distance]), $dst];
    foreach ([$cjxl, $ffmpeg, ['magick', $src, $dst]] as $cmd) {
        $line = implode(' ', array_map('escapeshellarg', $cmd)) . ' 2>/dev/null';
        exec($line, $_out, $code);
        if ($code === 0 && isJpegXlFile($dst)) {
            return $dst;
        }
        @unlink($dst);
    }
    throw new RuntimeException('no working JPEG XL backend (install Imagick '
        . 'with JXL delegate, libjxl cjxl, or ffmpeg --enable-libjxl)');
}

$lossless = false;
$distance = 1.0;
$effort = 7;
$pos = [];
$argv = $_SERVER['argv'];
array_shift($argv);
while ($argv) {
    $a = array_shift($argv);
    if ($a === '--lossless') {
        $lossless = true;
    } elseif ($a === '--distance') {
        $distance = (float) array_shift($argv);
    } elseif ($a === '--effort') {
        $effort = (int) array_shift($argv);
    } else {
        $pos[] = $a;
    }
}
if (!$pos || count($pos) > 2) {
    fwrite(STDERR, "usage: php convert_to_jxl.php [--lossless] [--distance 1.0] [--effort 7] in [out.jxl]\n");
    exit(2);
}
$dst = $pos[1] ?? preg_replace('/\.[^.]+$/', '.jxl', $pos[0]);
try {
    echo $pos[0] . ' -> ' . convertToJxl($pos[0], $dst, $lossless, $distance, $effort) . PHP_EOL;
} catch (Throwable $e) {
    fwrite(STDERR, 'error: ' . $e->getMessage() . PHP_EOL);
    exit(1);
}
