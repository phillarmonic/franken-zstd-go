<?php

/** @generate-class-entries */

namespace Phillarmonic\FrankenZstd {
    final class Compressor
    {
        // Compression level constants
        public const int LEVEL_FASTEST = 1;
        public const int LEVEL_DEFAULT = 3;
        public const int LEVEL_BETTER = 6;
        public const int LEVEL_BEST = 9;
        public const int LEVEL_ULTRA = 19;
        
        // Window size constants (for advanced usage)
        public const int WINDOW_SIZE_MIN = 10;
        public const int WINDOW_SIZE_DEFAULT = 22;
        public const int WINDOW_SIZE_MAX = 31;
        
        /**
         * Creates a new zstd compressor instance.
         *
         * @param int $level Compression level (1-22, default: 3)
         * @param int $windowSize Window size for compression (10-31, default: 22)
         */
        public static function create(int $level = Compressor::LEVEL_DEFAULT, int $windowSize = Compressor::WINDOW_SIZE_DEFAULT): \Phillarmonic\FrankenZstd\Compressor {
        }
        
        /**
         * The compression level of this instance.
         */
        public readonly int $level;
        
        /**
         * The window size of this instance.
         */
        public readonly int $windowSize;
        
        /**
         * Compresses data using zstd algorithm.
         *
         * @param string $data Data to compress
         * @return string Compressed data
         * @throws \Phillarmonic\FrankenZstd\CompressionException
         */
        public function compress(string $data): string {
        }
        
        /**
         * Decompresses zstd-compressed data.
         *
         * @param string $compressedData Compressed data to decompress
         * @return string Decompressed data
         * @throws \Phillarmonic\FrankenZstd\CompressionException
         */
        public function decompress(string $compressedData): string {
        }
        
        /**
         * Gets compression statistics for the last operation.
         *
         * @return array{
         *     originalSize: int,
         *     compressedSize: int,
         *     compressionRatio: float,
         *     compressionTime: float
         * }
         */
        public function getStats(): array {
        }
        
        /**
         * Closes the compressor and releases resources.
         */
        public function close(): void {
        }
    }
    
    class CompressionException extends \Exception
    {
        public function __construct(string $message, int $code = 0, ?\Throwable $previous = null) {
        }
    }
    
    /**
     * Quick compress function for simple use cases.
     *
     * @param string $data Data to compress
     * @param int $level Compression level (1-22, default: 3)
     * @return string Compressed data
     * @throws \Phillarmonic\FrankenZstd\CompressionException
     */
    function compress(string $data, int $level = Compressor::LEVEL_DEFAULT): string {
    }
    
    /**
     * Quick decompress function for simple use cases.
     *
     * @param string $compressedData Compressed data
     * @return string Decompressed data
     * @throws \Phillarmonic\FrankenZstd\CompressionException
     */
    function decompress(string $compressedData): string {
    }
    
    /**
     * Gets information about zstd compression capabilities.
     *
     * @return array{
     *     version: string,
     *     maxLevel: int,
     *     minLevel: int,
     *     defaultLevel: int
     * }
     */
    function getInfo(): array {
    }
}
