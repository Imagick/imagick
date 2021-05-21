<?php

/** @generate-function-entries */
class ImagickDraw
{
#if MagickLibVersion > 0x628
    public function resetVectorGraphics(): bool {}
#endif
#if MagickLibVersion > 0x649
    public function getTextKerning(): float {}
    public function setTextKerning(float $kerning): bool {}
    public function getTextInterwordSpacing(): float {}

    public function setTextInterwordSpacing(float $spacing): bool {}
#endif
#if MagickLibVersion > 0x655
    public function getTextInterlineSpacing(): float {}
    public function setTextInterlineSpacing(float $spacing): bool {}
#endif

    public function __construct() {}

    public function setFillColor(ImagickPixel|string $fill_pixel): bool {}

    public function setFillAlpha(float $alpha): bool {}

    public function setResolution(float $resolution_x, float $resolution_): bool {}

    public function setStrokeColor(ImagickPixel|string $color): bool {}

    public function setStrokeAlpha(float $alpha): bool {}

    public function setStrokeWidth(float $width): bool {}

    public function clear(): bool {}

    public function circle(float $origin_x, float $origin_y, float $perimeter_x, float $perimeter_y): bool {}

    public function annotation(float $x, float $y, string $text): bool {}

    public function setTextAntialias(bool $antialias): bool {}

    public function setTextEncoding(string $encoding): bool {}

    public function setFont(string $font_name): bool {}

    public function setFontFamily(string $font_family): bool {}

    public function setFontSize(float $point_size): bool {}

    // StyleType
    public function setFontStyle(int $style): bool {}

    /**
     * @param int $weight font weight (valid range 100-900)
     * @return bool
     */
    public function setFontWeight(int $weight): bool {}

    public function getFont(): string {}

    public function getFontFamily(): string {}

    public function getFontSize(): float {}

    public function getFontStyle(): int {}

    public function getFontWeight(): int {}

    /** @alias ImagickDraw::clear */
    public function destroy(): bool {}

    public function rectangle(float $top_left_x, float $top_left_y, float $bottom_right_x, float $bottom_right_y): bool {}

    public function roundRectangle(float $top_left_x, float $top_left_y, float $bottom_right_x, float $bottom_right_y, float $rounding_x, float $rounding_y): bool {}


    //ox
    //origin x ordinate
    //oy
    //origin y ordinate
    //rx
    //radius in x
    //ry
    //radius in y
    //start
    //starting rotation in degrees
    //end
    //ending rotation in degrees

    public function ellipse(float $origin_x, float $origin_y, float $radius_x, float $radius_y, float $angle_start, float $angle_end): bool {}

    public function skewX(float $degrees): bool {}

    public function skewY(float $degrees): bool {}

    public function translate(float $x, float $y): bool {}

    public function line(float $start_x, float $start_y, float $end_x, float $end_y): bool {}


//sx
//starting x ordinate of bounding rectangle
//sy
//starting y ordinate of bounding rectangle
//ex
//ending x ordinate of bounding rectangle
//ey
//ending y ordinate of bounding rectangle
//sd
//starting degrees of rotation
//ed
//ending degrees of rotation

    public function arc(float $start_x, float $start_y, float $end_x, float $end_y, float $start_angle, float $end_angle): bool {}

#if MagickLibVersion >= 0x700
    public function alpha(float $x, float $y, int $paint_method): bool {}
#else
    /** @deprecated */
    public function matte(float $x, float $y, int $paint_type): bool {}
#endif

    public function polygon(array $coordinates): bool {}

    public function point(float $x, float $y): bool {}

    public function getTextDecoration(): int {}

    public function getTextEncoding(): string {}

    public function getFontStretch(): int {}

    //  StretchType font_stretch
    public function setFontStretch(int $font_stretch): bool {}

    public function setStrokeAntialias(bool $enabled): bool {}

    // AlignType
    public function setTextAlignment(int $alignment): bool {}

    // DecorationType
    public function setTextDecoration(int $decoration): bool {}

    public function setTextUnderColor(ImagickPixel|string $under_color): bool {}

    public function setViewbox(int $left_x, int $top_y, int $right_x, int $bottom_y): bool {}

    public function clone(): ImagickDraw {}

    public function affine(array $affine): bool {}

    public function bezier(array $coordinates): bool {}

    public function composite(int $compose, float $x, float $y, float $width, float $height, Imagick $imagick): bool {}

    public function color(float $x, float $y, int $paint_type): bool {}

    public function comment(string $comment): bool {}

    public function getClipPath(): string {}

    public function getClipRule(): int {}

    public function getClipUnits(): int {}

    public function getFillColor(): ImagickPixel {}

    public function getFillOpacity(): float {}

    public function getFillRule(): int {}

    public function getGravity(): int {}

    public function getStrokeAntialias(): bool {}

    public function getStrokeColor(): ImagickPixel {}

    public function getStrokeDashArray(): array {}

    public function getStrokeDashOffset(): float {}

    public function getStrokeLineCap(): int {}

    public function getStrokeLineJoin(): int {}

    public function getStrokeMiterLimit(): int {}

    public function getStrokeOpacity(): float {}

    public function getStrokeWidth(): float {}

    public function getTextAlignment(): int {}

    public function getTextAntialias(): bool {}

    public function getVectorGraphics(): string {}

    public function getTextUnderColor(): ImagickPixel {}

    public function pathClose(): bool {}

//x1
//x ordinate of control point for curve beginning
//y1
//y ordinate of control point for curve beginning
//x2
//x ordinate of control point for curve ending
//y2
//y ordinate of control point for curve ending
//x
//x ordinate of the end of the curve
//y
//y ordinate of the end of the curve

    public function pathCurveToAbsolute(float $x1, float $y1, float $x2, float $y2, float $x_end, float $y_end): bool {}

    public function pathCurveToRelative(float $x1, float $y1, float $x2, float $y2, float $x_end, float $y_end): bool {}

//	PHP_ME(imagickdraw, pathcurvetoquadraticbezierabsolute, imagickdraw_pathcurvetoquadraticbezierabsolute_args, ZEND_ACC_PUBLIC)
    public function pathCurveToQuadraticBezierAbsolute(float $x1, float $y1, float $x_end, float $y_end): bool {}

    public function pathCurveToQuadraticBezierRelative(float $x1, float $y1, float $x_end, float $y_end): bool {}

    // TODO - should these be $x_end, float $y_end to be consistent?
    // Or should the $x_end be $x above.
    public function pathCurveToQuadraticBezierSmoothAbsolute(float $x, float $y): bool {}

    // TODO - should these be $x_end, float $y_end to be consistent
    public function pathCurveToQuadraticBezierSmoothRelative(float $x, float $y): bool {}

    public function pathCurveToSmoothAbsolute(float $x2, float $y2, float $x, float $y): bool {}

    public function pathCurveToSmoothRelative(float $x2, float $y2, float $x, float $y): bool {}

    // TODO - looks dodgy.
    public function pathEllipticArcAbsolute(float $rx, float $ry, float $x_axis_rotation, bool $large_arc_flag, bool $sweep_flag, float $x, float $y): bool {}

    public function pathEllipticArcRelative(float $rx, float $ry, float $x_axis_rotation, bool $large_arc_flag, bool $sweep_flag, float $x, float $y): bool {}

    public function pathFinish(): bool {}

    public function pathLineToAbsolute(float $x, float $y): bool {}

    public function pathLineToRelative(float $x, float $y): bool {}

    // TODO - absolutely and relative shouldn't be the same?
    public function pathLineToHorizontalAbsolute(float $x): bool {}

// TODO - absolutely and relative shouldn't be the same?
    public function pathLineToHorizontalRelative(float $x): bool {}

    public function pathLineToVerticalAbsolute(float $y): bool {}

    public function pathLineToVerticalRelative(float $y): bool {}

    public function pathMoveToAbsolute(float $x, float $y): bool {}

    public function pathMoveToRelative(float $x, float $y): bool {}

    public function pathStart(): bool {}

    public function polyline(array $coordinates): bool {}

    public function popClipPath(): bool {}

    public function popDefs(): bool {}

    public function popPattern(): bool {}


    public function pushClipPath(string $clip_mask_id): bool {}

    public function pushDefs(): bool {}

    public function pushPattern(string $pattern_id, float $x, float $y, float $width, float $height): bool {}

    public function render(): bool {}

    public function rotate(float $degrees): bool {}

    public function scale(float $x, float $y): bool {}

    public function setClipPath(string $clip_mask): bool {}

    public function setClipRule(int $fill_rule): bool {}

    public function setClipUnits(int $clip_units): bool {}

    public function setFillOpacity(float $fill_opacity): bool {}

    public function setFillPatternUrl(string $fill_url): bool {}

    public function setFillRule(int $fill_rule): bool {}

    public function setGravity(int $gravity): bool {}

    public function setStrokePatternUrl(string $stroke_url): bool {}

    public function setStrokeDashOffset(float $dash_offset): bool {}

    public function setStrokeLineCap(int $linecap): bool {}

    public function setStrokeLineJoin(int $linejoin): bool {}

    public function setStrokeMiterLimit(int $miterlimit): bool {}

    public function setStrokeOpacity(float $stroke_opacity): bool {}

    public function setVectorGraphics(string $xml): bool {}

    public function pop(): bool {}

    public function push(): bool {}

    public function setStrokeDashArray(array $dashArray): bool {}

#if MagickLibVersion >= 0x693
    public function getOpacity(): float {}

    // TODO - imply 0 to 1 range.
    public function setOpacity(float $opacity): bool {}
#endif

#if MagickLibVersion >= 0x675
    public function getFontResolution(): array {}

    public function setFontResolution(float $x, float $y): bool {}

    public function getBorderColor(): ImagickPixel {}

    public function setBorderColor(ImagickPixel|string $color): bool {}
#endif


#if MagickLibVersion >= 0x693
    public function setDensity(string $density): bool {}

    public function getDensity(): ?string {}
#endif

#if MagickLibVersion >= 0x692
//	PHP_ME(imagickdraw, gettextdirection, imagickdraw_zero_args, ZEND_ACC_PUBLIC)
    public function getTextDirection(): int {}

    // DirectionType
    public function setTextDirection(int $direction): bool {}
#endif
}
