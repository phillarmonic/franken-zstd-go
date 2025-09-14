<?php

echo "FrankenPHP Zstd Extension Registration Check\n";
echo "============================================\n\n";

$allPassed = true;

// Check if extension is loaded
echo "Checking extension loading...\n";
if (extension_loaded('franken-zstd-go')) {
    echo "✅ franken-zstd-go extension is loaded\n";
} else {
    echo "❌ franken-zstd-go extension is NOT loaded\n";
    $allPassed = false;
}

// Get extension version info
echo "\nExtension information:\n";
try {
    $info = \Phillarmonic\FrankenZstd\getInfo();
    echo "  Version: {$info['version']}\n";
    echo "  Min level: {$info['minLevel']}\n";
    echo "  Max level: {$info['maxLevel']}\n";
    echo "  Default level: {$info['defaultLevel']}\n";
    echo "✅ Extension info accessible\n";
} catch (Exception $e) {
    echo "❌ Cannot get extension info: " . $e->getMessage() . "\n";
    $allPassed = false;
}

// Check if functions exist
echo "\nChecking functions...\n";
$functions = [
    'Phillarmonic\\FrankenZstd\\compress',
    'Phillarmonic\\FrankenZstd\\decompress',
    'Phillarmonic\\FrankenZstd\\getInfo'
];

foreach ($functions as $func) {
    if (function_exists($func)) {
        echo "✅ Function $func exists\n";
    } else {
        echo "❌ Function $func does NOT exist\n";
        $allPassed = false;
    }
}

// Check if classes exist
echo "\nChecking classes...\n";
$classes = [
    'Phillarmonic\\FrankenZstd\\Compressor',
    'Phillarmonic\\FrankenZstd\\CompressionException'
];

foreach ($classes as $class) {
    if (class_exists($class)) {
        echo "✅ Class $class exists\n";
    } else {
        echo "❌ Class $class does NOT exist\n";
        $allPassed = false;
    }
}

// Check class constants
echo "\nChecking class constants...\n";
try {
    $constants = [
        'Phillarmonic\\FrankenZstd\\Compressor::LEVEL_DEFAULT',
        'Phillarmonic\\FrankenZstd\\Compressor::WINDOW_SIZE_DEFAULT'
    ];
    
    foreach ($constants as $const) {
        if (defined($const)) {
            echo "✅ Constant $const is defined\n";
        } else {
            echo "❌ Constant $const is NOT defined\n";
            $allPassed = false;
        }
    }
} catch (Exception $e) {
    echo "❌ Error checking constants: " . $e->getMessage() . "\n";
    $allPassed = false;
}

// Check method existence
echo "\nChecking class methods...\n";
try {
    $reflection = new ReflectionClass('Phillarmonic\\FrankenZstd\\Compressor');
    $methods = ['create', 'compress', 'decompress', 'getStats', 'close'];
    
    foreach ($methods as $method) {
        if ($reflection->hasMethod($method)) {
            echo "✅ Method Compressor::$method exists\n";
        } else {
            echo "❌ Method Compressor::$method does NOT exist\n";
            $allPassed = false;
        }
    }
} catch (Exception $e) {
    echo "❌ Error checking methods: " . $e->getMessage() . "\n";
    $allPassed = false;
}

// Final result
echo "\n" . str_repeat("=", 50) . "\n";
if ($allPassed) {
    echo "🎉 ALL CHECKS PASSED! Extension is properly registered.\n";
    exit(0);
} else {
    echo "❌ SOME CHECKS FAILED! Extension registration issues detected.\n";
    exit(1);
}
