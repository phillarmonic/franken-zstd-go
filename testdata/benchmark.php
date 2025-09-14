<?php

use Phillarmonic\FrankenZstd\Compressor;

echo "FrankenPHP Zstd Extension Benchmark\n";
echo "====================================\n\n";

// Quick extension registration check
if (!extension_loaded('franken-zstd-go')) {
    echo "❌ ERROR: franken-zstd-go extension is not loaded!\n";
    exit(1);
}

if (!class_exists('Phillarmonic\\FrankenZstd\\Compressor')) {
    echo "❌ ERROR: Compressor class not found!\n";
    exit(1);
}

echo "✅ Extension loaded and ready for benchmarking\n\n";

// Generate test data of different sizes
$testSizes = [
    '1KB' => 1024,
    '10KB' => 10 * 1024,
    '100KB' => 100 * 1024,
    '1MB' => 1024 * 1024,
];

$testData = [];
foreach ($testSizes as $name => $size) {
    $testData[$name] = str_repeat("This is test data for benchmarking zstd compression performance. ", $size / 64);
    $testData[$name] = substr($testData[$name], 0, $size); // Ensure exact size
}

// Test different compression levels
$levels = [
    'Fastest' => Compressor::LEVEL_FASTEST,
    'Default' => Compressor::LEVEL_DEFAULT,
    'Better' => Compressor::LEVEL_BETTER,
    'Best' => Compressor::LEVEL_BEST,
];

echo "Compression Performance Test\n";
echo "----------------------------\n";
printf("%-8s %-10s %-12s %-12s %-10s %-15s\n", "Size", "Level", "Comp.Size", "Ratio", "Time(ms)", "Speed(MB/s)");
echo str_repeat("-", 70) . "\n";

foreach ($testData as $sizeName => $data) {
    $originalSize = strlen($data);
    
    foreach ($levels as $levelName => $level) {
        $compressor = Compressor::create($level);
        
        // Warm up
        $compressor->compress($data);
        
        // Benchmark compression
        $startTime = microtime(true);
        $compressed = $compressor->compress($data);
        $endTime = microtime(true);
        
        $compressionTime = ($endTime - $startTime) * 1000; // Convert to milliseconds
        $compressedSize = strlen($compressed);
        $ratio = $originalSize / $compressedSize;
        $speed = ($originalSize / 1024 / 1024) / ($compressionTime / 1000); // MB/s
        
        printf("%-8s %-10s %-12d %-12.2f %-10.2f %-15.2f\n", 
               $sizeName, $levelName, $compressedSize, $ratio, $compressionTime, $speed);
        
        // Verify decompression works
        $decompressed = $compressor->decompress($compressed);
        if ($decompressed !== $data) {
            echo "ERROR: Decompression failed for $sizeName at level $levelName\n";
        }
        
        $compressor->close();
    }
    echo "\n";
}

echo "\nDecompression Performance Test\n";
echo "------------------------------\n";
printf("%-8s %-12s %-10s %-15s\n", "Size", "Comp.Size", "Time(ms)", "Speed(MB/s)");
echo str_repeat("-", 50) . "\n";

// Test decompression performance with default level
foreach ($testData as $sizeName => $data) {
    $compressor = Compressor::create(Compressor::LEVEL_DEFAULT);
    $compressed = $compressor->compress($data);
    $compressedSize = strlen($compressed);
    
    // Warm up
    $compressor->decompress($compressed);
    
    // Benchmark decompression
    $startTime = microtime(true);
    $decompressed = $compressor->decompress($compressed);
    $endTime = microtime(true);
    
    $decompressionTime = ($endTime - $startTime) * 1000; // Convert to milliseconds
    $speed = (strlen($decompressed) / 1024 / 1024) / ($decompressionTime / 1000); // MB/s
    
    printf("%-8s %-12d %-10.2f %-15.2f\n", 
           $sizeName, $compressedSize, $decompressionTime, $speed);
    
    $compressor->close();
}

echo "\nQuick Functions Performance Test\n";
echo "--------------------------------\n";
printf("%-8s %-12s %-10s %-15s\n", "Size", "Operation", "Time(ms)", "Speed(MB/s)");
echo str_repeat("-", 50) . "\n";

foreach ($testData as $sizeName => $data) {
    $originalSize = strlen($data);
    
    // Test quick compress
    $startTime = microtime(true);
    $compressed = \Phillarmonic\FrankenZstd\compress($data);
    $endTime = microtime(true);
    
    $compressionTime = ($endTime - $startTime) * 1000;
    $speed = ($originalSize / 1024 / 1024) / ($compressionTime / 1000);
    
    printf("%-8s %-12s %-10.2f %-15.2f\n", 
           $sizeName, "Compress", $compressionTime, $speed);
    
    // Test quick decompress
    $startTime = microtime(true);
    $decompressed = \Phillarmonic\FrankenZstd\decompress($compressed);
    $endTime = microtime(true);
    
    $decompressionTime = ($endTime - $startTime) * 1000;
    $speed = ($originalSize / 1024 / 1024) / ($decompressionTime / 1000);
    
    printf("%-8s %-12s %-10.2f %-15.2f\n", 
           $sizeName, "Decompress", $decompressionTime, $speed);
}

echo "\nMemory Usage Test\n";
echo "-----------------\n";

$memoryBefore = memory_get_usage(true);
$compressor = Compressor::create(Compressor::LEVEL_DEFAULT);
$memoryAfter = memory_get_usage(true);

echo "Memory usage for compressor creation: " . number_format(($memoryAfter - $memoryBefore) / 1024) . " KB\n";

// Test with large data
$largeData = str_repeat("Large data test for memory usage analysis. ", 100000); // ~4MB
$memoryBeforeCompress = memory_get_usage(true);
$compressed = $compressor->compress($largeData);
$memoryAfterCompress = memory_get_usage(true);

echo "Memory usage for large data compression: " . number_format(($memoryAfterCompress - $memoryBeforeCompress) / 1024) . " KB\n";
echo "Large data size: " . number_format(strlen($largeData) / 1024) . " KB\n";
echo "Compressed size: " . number_format(strlen($compressed) / 1024) . " KB\n";

$compressor->close();

echo "\nBenchmark completed!\n";
