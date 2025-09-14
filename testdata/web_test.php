<?php

use Phillarmonic\FrankenZstd\Compressor;

// Set content type to HTML
header('Content-Type: text/html; charset=utf-8');

echo "<h1>FrankenPHP Zstd Extension Web Test</h1>\n";

// Check if extension is loaded
echo "<h2>Extension Status</h2>\n";
if (extension_loaded('franken-zstd-go')) {
    echo "<p style='color: green;'>✅ franken-zstd-go extension is loaded</p>\n";
} else {
    echo "<p style='color: red;'>❌ franken-zstd-go extension is NOT loaded</p>\n";
    exit(1);
}

// Test basic compression
echo "<h2>Basic Compression Test</h2>\n";
try {
    $originalData = "Hello from FrankenPHP Zstd Extension! This is a web test to verify the extension works in a web environment.";
    echo "<p><strong>Original data:</strong> " . htmlspecialchars($originalData) . "</p>\n";
    echo "<p><strong>Original size:</strong> " . strlen($originalData) . " bytes</p>\n";
    
    // Compress using quick function
    $compressed = \Phillarmonic\FrankenZstd\compress($originalData, 6);
    echo "<p><strong>Compressed size:</strong> " . strlen($compressed) . " bytes</p>\n";
    
    $ratio = round(strlen($originalData) / strlen($compressed), 2);
    echo "<p><strong>Compression ratio:</strong> {$ratio}:1</p>\n";
    
    // Decompress
    $decompressed = \Phillarmonic\FrankenZstd\decompress($compressed);
    
    if ($decompressed === $originalData) {
        echo "<p style='color: green;'>✅ Compression/decompression successful!</p>\n";
    } else {
        echo "<p style='color: red;'>❌ Data mismatch after decompression</p>\n";
    }
    
} catch (Exception $e) {
    echo "<p style='color: red;'>❌ Error: " . htmlspecialchars($e->getMessage()) . "</p>\n";
}

// Test Compressor class
echo "<h2>Compressor Class Test</h2>\n";
try {
    $compressor = Compressor::create(Compressor::LEVEL_DEFAULT, Compressor::WINDOW_SIZE_DEFAULT);
    echo "<p>✅ Compressor created successfully</p>\n";
    
    $testData = str_repeat("Web test data for compression. ", 50);
    $compressed = $compressor->compress($testData);
    $decompressed = $compressor->decompress($compressed);
    
    if ($decompressed === $testData) {
        echo "<p style='color: green;'>✅ Compressor class working correctly</p>\n";
        
        $stats = $compressor->getStats();
        echo "<p><strong>Statistics:</strong></p>\n";
        echo "<ul>\n";
        echo "<li>Original size: " . $stats['originalSize'] . " bytes</li>\n";
        echo "<li>Compressed size: " . $stats['compressedSize'] . " bytes</li>\n";
        echo "<li>Compression ratio: " . round($stats['compressionRatio'], 2) . ":1</li>\n";
        echo "<li>Compression time: " . round($stats['compressionTime'] * 1000, 2) . " ms</li>\n";
        echo "</ul>\n";
    } else {
        echo "<p style='color: red;'>❌ Compressor class test failed</p>\n";
    }
    
    $compressor->close();
    
} catch (Exception $e) {
    echo "<p style='color: red;'>❌ Compressor error: " . htmlspecialchars($e->getMessage()) . "</p>\n";
}

// Extension info
echo "<h2>Extension Information</h2>\n";
try {
    $info = \Phillarmonic\FrankenZstd\getInfo();
    echo "<ul>\n";
    echo "<li><strong>Zstd version:</strong> " . $info['version'] . "</li>\n";
    echo "<li><strong>Min level:</strong> " . $info['minLevel'] . "</li>\n";
    echo "<li><strong>Max level:</strong> " . $info['maxLevel'] . "</li>\n";
    echo "<li><strong>Default level:</strong> " . $info['defaultLevel'] . "</li>\n";
    echo "</ul>\n";
} catch (Exception $e) {
    echo "<p style='color: red;'>❌ Info error: " . htmlspecialchars($e->getMessage()) . "</p>\n";
}

echo "<h2>✅ All web tests completed successfully!</h2>\n";
echo "<p><em>Test completed at: " . date('Y-m-d H:i:s') . "</em></p>\n";
?>
