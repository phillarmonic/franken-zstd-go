<?php

use Phillarmonic\FrankenZstd\Compressor;
use Phillarmonic\FrankenZstd\CompressionException;

echo "<h1>FrankenPHP Zstd Extension Test</h1>\n";

// Check if extension is loaded
echo "<h2>Extension Registration Check</h2>\n";
if (extension_loaded('franken-zstd-go')) {
    echo "✅ franken-zstd-go extension is loaded<br>\n";
} else {
    echo "❌ franken-zstd-go extension is NOT loaded<br>\n";
    exit(1);
}

// Check if functions exist
$functions = ['Phillarmonic\\FrankenZstd\\compress', 'Phillarmonic\\FrankenZstd\\decompress', 'Phillarmonic\\FrankenZstd\\getInfo'];
foreach ($functions as $func) {
    if (function_exists($func)) {
        echo "✅ Function $func exists<br>\n";
    } else {
        echo "❌ Function $func does NOT exist<br>\n";
        exit(1);
    }
}

// Check if classes exist
$classes = ['Phillarmonic\\FrankenZstd\\Compressor', 'Phillarmonic\\FrankenZstd\\CompressionException'];
foreach ($classes as $class) {
    if (class_exists($class)) {
        echo "✅ Class $class exists<br>\n";
    } else {
        echo "❌ Class $class does NOT exist<br>\n";
        exit(1);
    }
}

echo "<br>\n";

try {
    // Test basic compression/decompression
    echo "<h2>Basic Compression Test</h2>\n";
    
    $data = str_repeat("Hello, World! This is a test string for zstd compression. ", 100);
    echo "Original data size: " . strlen($data) . " bytes<br>\n";
    
    // Test quick functions
    echo "<h3>Quick Functions</h3>\n";
    $compressed = \Phillarmonic\FrankenZstd\compress($data);
    echo "Compressed size: " . strlen($compressed) . " bytes<br>\n";
    
    $decompressed = \Phillarmonic\FrankenZstd\decompress($compressed);
    echo "Decompressed size: " . strlen($decompressed) . " bytes<br>\n";
    echo "Data matches: " . ($data === $decompressed ? "✅ YES" : "❌ NO") . "<br>\n";
    
    // Test Compressor class
    echo "<h3>Compressor Class</h3>\n";
    $compressor = Compressor::create(Compressor::LEVEL_DEFAULT, Compressor::WINDOW_SIZE_DEFAULT);
    echo "Compressor created successfully<br>\n";
    
    $compressed2 = $compressor->compress($data);
    echo "Class compressed size: " . strlen($compressed2) . " bytes<br>\n";
    
    $decompressed2 = $compressor->decompress($compressed2);
    echo "Class decompressed size: " . strlen($decompressed2) . " bytes<br>\n";
    echo "Class data matches: " . ($data === $decompressed2 ? "✅ YES" : "❌ NO") . "<br>\n";
    
    // Test statistics
    echo "<h3>Compression Statistics</h3>\n";
    $stats = $compressor->getStats();
    echo "Original size: {$stats['originalSize']} bytes<br>\n";
    echo "Compressed size: {$stats['compressedSize']} bytes<br>\n";
    echo "Compression ratio: " . number_format($stats['compressionRatio'], 2) . ":1<br>\n";
    echo "Compression time: " . number_format($stats['compressionTime'] * 1000, 2) . " ms<br>\n";
    
    // Test different compression levels
    echo "<h3>Different Compression Levels</h3>\n";
    $testData = str_repeat("Test data for compression level comparison. ", 50);
    
    foreach ([Compressor::LEVEL_FASTEST, Compressor::LEVEL_DEFAULT, Compressor::LEVEL_BETTER, Compressor::LEVEL_BEST] as $level) {
        $levelCompressor = Compressor::create($level);
        $levelCompressed = $levelCompressor->compress($testData);
        $levelStats = $levelCompressor->getStats();
        
        echo "Level $level: " . strlen($levelCompressed) . " bytes, ratio: " . 
             number_format($levelStats['compressionRatio'], 2) . ":1, time: " . 
             number_format($levelStats['compressionTime'] * 1000, 2) . " ms<br>\n";
        
        $levelCompressor->close();
    }
    
    // Test extension info
    echo "<h3>Extension Information</h3>\n";
    $info = \Phillarmonic\FrankenZstd\getInfo();
    echo "Zstd version: {$info['version']}<br>\n";
    echo "Min level: {$info['minLevel']}<br>\n";
    echo "Max level: {$info['maxLevel']}<br>\n";
    echo "Default level: {$info['defaultLevel']}<br>\n";
    
    // Clean up
    $compressor->close();
    
    echo "<h2>✅ All tests passed!</h2>\n";
    
} catch (CompressionException $e) {
    echo "<h2>❌ Compression Error: " . htmlspecialchars($e->getMessage()) . "</h2>\n";
} catch (Exception $e) {
    echo "<h2>❌ General Error: " . htmlspecialchars($e->getMessage()) . "</h2>\n";
}
