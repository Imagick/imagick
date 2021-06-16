<?php

/** @generate-function-entries */

#ifdef IMAGICK_WITH_KERNEL
class ImagickKernel
{
    public function addKernel(ImagickKernel $kernel): void {}

    public function addUnityKernel(float $scale): void {}

    // KERNEL_*
    public static function fromBuiltin(int $kernel, string $shape): ImagickKernel {}

    public static function fromMatrix(array $matrix, ?array $origin): ImagickKernel {}

    public function getMatrix(): array {}

    // NORMALIZE_KERNEL_
    public function scale(float $scale, ?int $normalize_kernel = null): void {}

    public function separate(): array {}
}
#endif

