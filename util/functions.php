<?php

// Mirrored from https://github.com/Danack/HexFloat

require_once __DIR__ . "/FloatInfo.php";
require_once __DIR__ . "/Float32Info.php";

use HexFloat\FloatInfo;
use HexFloat\Float32Info;

/**
 * Returns a string containing a hexadecimal representation of the given float,
 * using 64 bits of info
 *
 * @param float $number
 * @return string
 */
function floathex(float $number)
{
    return strrev(unpack('h*', pack('d', $number))[1]);
}

/**
 * Returns a string containing a hexadecimal representation of the given float,
 * using 32 bits of info
 *
 * @param float $number
 * @return string
 */
function floathex32(float $num)
{
    return strrev(unpack('h*', pack('f', $num))[1]);
}

/**
 * Convert a floating point number to a FloatInfo object,
 * which contains string representations of the float's sign,
 * exponent and mantissa
 * @param float $num
 * @return FloatInfo
 */
function float_info(float $num)
{
    $float64 = floathex($num);

    //Sign bit: 1 bit
    //Exponent: 11 bits
    //Significand precision: 53 bits (52 explicitly stored)

    $chars = str_split($float64);


    // 3 bits from this
    $byte1 = hexdec($chars[0]);
    // 4 bits from this
    $byte2 = hexdec($chars[1]);

    // 1 bit from this
    $byte3 = hexdec($chars[2]);

    $sign = '0';

    if ($byte1 >= 8) {
        $sign = '1';
    }

    $exponentString = substr($float64, 0, 3);
    $exponentValue = hexdec($exponentString) & 0x7ff;
    $exponent = sprintf("%b", $exponentValue);
    $exponent = str_pad($exponent, 11, '0', STR_PAD_LEFT);

    $mantissa = substr($float64, 2);
    $mantissa = hexdec($mantissa) & 0xfffffffffffff;
    $mantissa = sprintf("%b", $mantissa);
    $mantissa = str_pad($mantissa, 52, '0', STR_PAD_LEFT);

    return new FloatInfo(
        $sign,
        $exponent,
        $mantissa
    );
}

/**
 * Convert a floating point number to a Float32Info object,
 * which contains string representations of the float's sign,
 * exponent and mantissa
 *
 * @param float $num
 * @return Float32Info
 */
function float_info_32(float $num)
{
    $float32 = floathex32($num);
    $chars = str_split($float32);

    // 3 bits from this
    $byte1 = hexdec($chars[0]);
    // 4 bits from this
    $byte2 = hexdec($chars[1]);

    // 1 bit from this
    $byte3 = hexdec($chars[2]);

    $sign = '0';

    if ($byte1 >= 8) {
        $sign = '1';
    }
    $exponent3Bits = ($byte1 & 0x7);
    $exponent4Bits = $byte2;
    $exponent1Bit = ($byte3 & 0x8) >> 3;
    $exponent = ($exponent3Bits << 5) | ($exponent4Bits << 1) | $exponent1Bit;

    $exponent = sprintf("%b", $exponent);
    $exponent = str_pad($exponent, 8, '0', STR_PAD_LEFT);

    $mantissa = substr($float32, 2, 6);
    $mantissa = hexdec($mantissa) & 0x7fffff;
    $mantissa = sprintf("%b", $mantissa);
    $mantissa = str_pad($mantissa, 23, '0', STR_PAD_LEFT);

    return new Float32Info(
        $sign,
        $exponent,
        $mantissa
    );
}

/**
 * Produce a debug string that shows the Sign, Exponent and Mantissa for
 * two floating point numbers, using 64bit precision
 *
 *
 * @param float $value1
 * @param float $value2
 * @return string
 *
 * Example result
 * ┌──────┬─────────────┬──────────────────────────────────────────────────────┐
 * │ Sign │ Exponent    │ Mantissa                                             │
 * │    0 │ 01111111011 │ 1001100110011001100110011001100110011001100110011010 │
 * │    0 │ 10000011001 │ 0111110101111000010000000100000000000000000000000000 │
 * └──────┴─────────────┴──────────────────────────────────────────────────────┘
 *
 */
function float_compare(float $value1, float $value2)
{
    $float_info_1 = float_info($value1);
    $float_info_2 = float_info($value2);

    //Sign bit: 1 bit
    //Exponent: 11 bits
    //Significand precision: 53 bits (52 explicitly stored)

    $output  = "┌──────┬─────────────┬──────────────────────────────────────────────────────┐\n";
    $output .= "│ Sign │ Exponent    │ Mantissa                                             │\n";

    $format_string = "│    %s │ %s │ %s │\n";

    $output .= sprintf($format_string, $float_info_1->getSign(), $float_info_1->getExponent(), $float_info_1->getMantissa());
    $output .= sprintf($format_string, $float_info_2->getSign(), $float_info_2->getExponent(), $float_info_2->getMantissa());

    $output .= "└──────┴─────────────┴──────────────────────────────────────────────────────┘\n";

    return $output;
}


/**
 * Produce a debug string that shows the Sign, Exponent and Mantissa for
 * two floating point numbers, using 32bit precision
 *
 * @param float $value1
 * @param float $value2
 * @return string
 *
 * Example result
 * ┌──────┬──────────┬─────────────────────────┐
 * │ Sign │ Exponent │ Mantissa                │
 * │    0 │ 01111011 │ 10011001100110011001101 │
 * │    0 │ 10011001 │ 01111101011110000100000 │
 * └──────┴──────────┴─────────────────────────┘
 *
 */
function float_compare_32(float $value1, float $value2)
{
    $float_info_1 = float_info_32($value1);
    $float_info_2 = float_info_32($value2);

    $output  = "┌──────┬──────────┬─────────────────────────┐\n";
    $output .= "│ Sign │ Exponent │ Mantissa                │\n";

    $format_string = "│    %s │ %s │ %s │\n";

    $output .= sprintf($format_string, $float_info_1->getSign(), $float_info_1->getExponent(), $float_info_1->getMantissa());
    $output .= sprintf($format_string, $float_info_2->getSign(), $float_info_2->getExponent(), $float_info_2->getMantissa());

    $output .= "└──────┴──────────┴─────────────────────────┘\n";

    return $output;
}

/**
 * So. One of the disadvantages of non-HDRI compiled Image Magick
 * is that it can't accurately represent a '50%' color accurately.
 *
 * For example, if ImageMagick is compiled with 16bit color depth
 * then the two closest colors to midpoint are:
 *   32767 / 65535 = 0.5 - (1 / (2 ^ 17)) = 0.499992370...
 *  or
 *   32768 / 65535 = 0.5 + (1 / (2 ^ 17)) = 0.50000762951
 *
 * Either way there is going to be 'error' of
 * 0.00000762939453125
 *
 * The problem is even worse when ImageMagick is compiled with 8-bit
 * numbers (though this really shouldn't be used any more) and the
 * error would be 0.001953125
 *
 */
function get_epsilon_for_off_by_half_errors()
{
    // These could be defined better...
    $epsilon_values_for_non_hdri = [
        '255' => (1 / (pow(2, 8) - 1)) + 0.0000000000001,
        '65535' => (1 / (pow(2, 16) - 1)) + 0.0000000000001,
        '16777215' => (1 / (pow(2, 24) - 1) ) + 0.0000000000001,
        '4294967295' => (1 / (pow(2, 32) - 1)) + 0.0000000000001,
    ];

    // These could definitely be defined better...
    $epsilon_values_for_hdri = [
        '255' => 0.0000000000001,
        '65535' => 0.0000000000001,
        '16777215' => 0.0000000000001,
        '4294967295' => 0.0000000000001
    ];

    if (Imagick::getHdriEnabled() === false) {
        $quantum = (string)Imagick::getQuantum();
        if (array_key_exists($quantum, $epsilon_values_for_non_hdri) !== true) {
            throw new Exception(
                "Quantum values is $quantum which is not any of (2^(8|16|24|32)) - 1. Please report this as a bug."
            );
        }
        return $epsilon_values_for_non_hdri[$quantum];
    }

    $quantum = Imagick::getQuantum();
    if (array_key_exists($quantum, $epsilon_values_for_hdri) !== true) {
        throw new Exception(
            "Quantum values is $quantum which is not any of (2^(8|16|24|32)) - 1. Please report this as a bug."
        );
    }

    return $epsilon_values_for_hdri[$quantum];
}