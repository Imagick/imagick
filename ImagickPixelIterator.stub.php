<?php

/** @generate-function-entries */
class ImagickPixelIterator {
    public function __construct(Imagick $imagick) {}

    public function clear(): bool {}

    public static function getPixelIterator(Imagick $imagick) : ImagickPixelIterator {}

    public static function getPixelRegionIterator(
        Imagick $imagick,
        int $x,
        int $y,
        int $columns,
        int $rows): ImagickPixelIterator {}


    /** @alias ImagickPixelIterator::clear */
    public function destroy(): bool {}

    public function getCurrentIteratorRow(): array {}

    public function getIteratorRow(): int {}

    public function getNextIteratorRow(): array {}

    public function getPreviousIteratorRow(): array {}

    /** @alias ImagickPixelIterator::getIteratorRow */
    public function key(): int {}

    /** @alias ImagickPixelIterator::getNextIteratorRow
     *  @tentative-return-type
     */
    public function next(): void {}

    /** @alias ImagickPixelIterator::resetIterator
     *  @tentative-return-type
     */
    public function rewind(): void {}

    /** @alias ImagickPixelIterator::getCurrentIteratorRow */
    public function current(): array {}

    public function newPixelIterator(Imagick $imagick): bool {}

    public function newPixelRegionIterator(
        Imagick $imagick,
        int $x,
        int $y,
        int $columns,
        int $rows): bool {}

    public function resetIterator(): bool {}

    public function setIteratorFirstRow(): bool {}

    public function setIteratorLastRow(): bool {}

    public function setIteratorRow(int $row): bool {}

    public function syncIterator(): bool {}

    public function valid(): bool {}
}
