<?php

namespace HexFloat;

// Mirrored from https://github.com/Danack/HexFloat

class FloatInfo
{
    //Sign bit: 1 bit
    private  $sign;

    //Exponent: 11 bits
    private  $exponent;

    //Mantissa precision: 53 bits (52 explicitly stored)
    private  $mantissa;

    public function __construct(
        $sign,
        $exponent,
        $mantissa
    ) {
        // TODO - check lengths
        $this->sign = $sign;
        $this->exponent = $exponent;
        $this->mantissa = $mantissa;
    }

    public function getSign()
    {
        return $this->sign;
    }

    public function getExponent()
    {
        return $this->exponent;
    }

    public function getMantissa()
    {
        return $this->mantissa;
    }
}
