<?php

/** @generate-function-entries */

class ImagickPixel
{
    public function __construct(?string $color = null) {}

    public function clear(): bool {}

    /** @alias ImagickPixel::clear */
    public function destroy(): bool {}

    public function getColor(int $normalized = 0): array {}

    public function getColorAsString(): string {}

    public function getColorCount(): int {}

    public function getColorQuantum(): array {}

    // COLOR_*
    public function getColorValue(int $color): float {}

    /**
     * $color // COLOR_*
     * return
     * - Float if IM compiled with HDRI
     * - int if IM compiled with fixed point math
     */
    public function getColorValueQuantum(int $color): IMAGICK_QUANTUM_TYPE {}

    public function getHSL(): array {}

    public function getIndex(): int {}

    public function isPixelSimilar(ImagickPixel|string $color, float $fuzz): bool {}

    // The fuzz value should be in the range 0-QuantumRange. The maximum value
    // represents the longest possible distance in the colorspace. e.g. from
    // RGB(0, 0, 0) to RGB(255, 255, 255) for the RGB colorspace
    public function isPixelSimilarQuantum(ImagickPixel|string $color, float $fuzz_quantum_range_scaled_by_square_root_of_three): bool {}

    /** @alias ImagickPixel::isPixelSimilarQuantum */

    public function isSimilar(
        ImagickPixel|string $color,
        // Feel free to not use named params for this.
        float $fuzz_quantum_range_scaled_by_square_root_of_three
    ): bool{}

    public function setColor(string $color): bool {}

    public function setColorCount(int $color_count): bool {}

    // COLOR_*
    public function setColorValue(int $color, float $value): bool {}

    /**
     * $color // COLOR_*
     * $value
     *
     * Type of value depends on how ImageMagick was compiled.
     * - Float if IM compiled with HDRI
     * - int if IM compiled with fixed point math
     *
     */
    public function setColorValueQuantum(int $color, IMAGICK_QUANTUM_TYPE $value): bool{}

    public function setHSL(float $hue, float $saturation , float $luminosity): bool{}

    // This function could be described more clearly...
    // https://imagemagick.org/api/pixel-wand.php#PixelSetIndex
    public function setIndex(IMAGICK_QUANTUM_TYPE $index): bool {}

    //// Not in docs.
#if MagickLibVersion >= 0x693
    public function setColorFromPixel(ImagickPixel $pixel): bool {}
#endif
}



