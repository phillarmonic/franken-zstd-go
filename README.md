# FrankenPHP Zstd Extension

A high-performance zstd compression extension for FrankenPHP, implemented in pure Go without CGO dependencies for the compression backend.

## Overview

This extension provides zstd (Zstandard) compression and decompression capabilities for PHP applications running on FrankenPHP. Unlike traditional C-based zstd extensions, this implementation leverages the pure Go compression library [github.com/klauspost/compress](https://github.com/klauspost/compress), eliminating memory leaks and CGO-related issues commonly found in C implementations.

## Key Advantages

### Memory Safety

- **No Memory Leaks**: Pure Go implementation eliminates memory management issues present in C-based zstd extensions
- **Garbage Collection**: Automatic memory management through Go's garbage collector
- **Thread Safety**: Built-in concurrency safety without manual lock management

### Performance Benefits

- **Pure Go Backend**: Uses `github.com/klauspost/compress` - a high-performance, pure Go zstd implementation
- **No CGO Overhead**: Eliminates the performance penalty of C-Go interop for compression operations
- **Object Pooling**: Efficient encoder/decoder reuse reduces allocation overhead
- **Optimized for FrankenPHP**: Native integration with FrankenPHP's Go runtime

### Reliability

- **Crash Resistance**: Pure Go code is less prone to segmentation faults and memory corruption
- **Better Error Handling**: Comprehensive error propagation from Go to PHP
- **Predictable Behavior**: Consistent performance across different platforms

## Installation

### Prerequisites

- FrankenPHP
- Go 1.25 or later
- PHP 8.2 or later with ZTS (Zend Thread Safety) enabled

### Building FrankenPHP with the Extension

This extension is built as a FrankenPHP module and requires building a custom FrankenPHP binary that includes the zstd extension.

#### Method 1: Manual Build with xcaddy

If you prefer to build manually or integrate into your own build process:

1. Install xcaddy (if not already installed):
   
   ```bash
   go install github.com/caddyserver/xcaddy/cmd/xcaddy@latest
   ```

2. Build FrankenPHP with the zstd extension:
   
   ```bash
   xcaddy build \
   --output frankenphp \
   --with github.com/dunglas/frankenphp/caddy@v1.9.1 \
   --with github.com/phillarmonic/franken-zstd-go=.
   ```

3. The resulting `frankenphp` binary will include the zstd extension.

#### Method 2: Using the Provided Build Script (Recommended)

1. Clone the repository:
   
   ```bash
   git clone https://github.com/phillarmonic/franken-zstd-go.git
   cd franken-zstd-go
   ```

2. Build FrankenPHP with the extension using the automated build:
   
   ```bash
   just build
   ```

This command will:

- Generate PHP stubs and argument info

- Use `xcaddy` to build FrankenPHP with the zstd extension included

- Create a `frankenphp` binary in the project directory
3. Verify the installation:
   
   ```bash
   just check-extension
   ```

#### Method 3: Integration into Existing FrankenPHP Build

To integrate this extension into an existing FrankenPHP project:

1. Add the extension to your `go.mod`:
   
   ```go
   require github.com/phillarmonic/franken-zstd-go v1.0.0
   ```

2. Import the extension in your main Go file:
   
   ```go
   import _ "github.com/phillarmonic/franken-zstd-go"
   ```

3. Build your FrankenPHP application as usual.

#### Running the Built Binary

Once built, you can use the FrankenPHP binary with the zstd extension:

```bash
# Run PHP CLI with the extension
./frankenphp php-cli script.php

# Start a web server using Caddyfile (recommended)
./frankenphp run --config Caddyfile

# Alternative: Direct PHP server (may have module limitations)
./frankenphp php-server --listen :8080 --root /path/to/webroot
```

#### Web Server Testing

The project includes a complete web testing setup:

```bash
# Start the web server for testing
just web-server

# This will start the server on http://127.0.0.1:10000 with test pages:
# - http://127.0.0.1:10000/web_test.php (Web-specific test)
# - http://127.0.0.1:10000/index.php (Full test suite)
```

**Note**: The `php-server` command may encounter module registration issues. Use the Caddyfile approach for reliable web serving.

## API Reference

### Global Functions

#### compress(string $data, int $level = 3): string

Compresses data using zstd compression.

**Parameters:**

- `$data` - The data to compress
- `$level` - Compression level (1-22, default: 3)

**Returns:** Compressed data as a binary string

**Example:**

```php
$compressed = \Phillarmonic\FrankenZstd\compress("Hello, World!");
```

#### decompress(string $compressedData): string

Decompresses zstd-compressed data.

**Parameters:**

- `$compressedData` - The compressed data to decompress

**Returns:** Original uncompressed data

**Example:**

```php
$original = \Phillarmonic\FrankenZstd\decompress($compressed);
```

#### getInfo(): array

Returns information about the zstd implementation.

**Returns:** Array containing version and level information

**Example:**

```php
$info = \Phillarmonic\FrankenZstd\getInfo();
// Returns: ['version' => '1.5.6', 'minLevel' => 1, 'maxLevel' => 22, 'defaultLevel' => 3]
```

### Compressor Class

The `Compressor` class provides stateful compression with statistics tracking.

#### Compressor::create(int $level = 3, int $windowSize = 22): Compressor

Creates a new compressor instance.

**Parameters:**

- `$level` - Compression level (1-22, default: 3)
- `$windowSize` - Window size (10-31, default: 22)

**Returns:** New Compressor instance

#### compress(string $data): string

Compresses data using the configured compressor.

#### decompress(string $compressedData): string

Decompresses data using the configured compressor.

#### getStats(): array

Returns compression statistics.

**Returns:** Array with compression metrics:

- `originalSize` - Total bytes processed
- `compressedSize` - Total compressed bytes
- `compressionRatio` - Compression ratio
- `compressionTime` - Time spent compressing (seconds)

#### close(): void

Closes the compressor and releases resources.

### Example Usage

```php
<?php

use Phillarmonic\FrankenZstd\Compressor;

// Quick compression
$data = "Large amount of data to compress...";
$compressed = \Phillarmonic\FrankenZstd\compress($data, 6);
$decompressed = \Phillarmonic\FrankenZstd\decompress($compressed);

// Stateful compression with statistics
$compressor = Compressor::create(Compressor::LEVEL_DEFAULT, Compressor::WINDOW_SIZE_DEFAULT);

$compressed1 = $compressor->compress($data1);
$compressed2 = $compressor->compress($data2);

$stats = $compressor->getStats();
echo "Compression ratio: " . $stats['compressionRatio'] . ":1\n";
echo "Total time: " . $stats['compressionTime'] . " seconds\n";

$compressor->close();
```

## Performance Benchmarks

Typical performance characteristics on modern hardware:

| Operation     | Data Size | Speed       |
| ------------- | --------- | ----------- |
| Compression   | 1MB       | 4,000+ MB/s |
| Decompression | 1MB       | 6,000+ MB/s |
| Compression   | 100KB     | 3,500+ MB/s |
| Decompression | 100KB     | 2,000+ MB/s |

## Configuration

### Compression Levels

- **1-3**: Fast compression, lower ratio
- **4-6**: Balanced compression (recommended)
- **7-15**: Higher compression, slower speed
- **16-22**: Maximum compression, slowest speed

### Window Sizes

- **10-15**: Low memory usage, faster
- **16-22**: Higher memory usage, better compression
- **22**: Default, good balance

## Development

### Building for Development

```bash
# Install dependencies
just deps

# Generate PHP stubs
just generate-stubs

# Build the extension
just build

# Run CLI tests
just test

# Check extension registration
just check-extension

# Start web server for testing
just web-server

# Run benchmarks
just benchmark
```

### Testing

The extension includes comprehensive tests:

- **Extension Registration**: Verifies proper loading and API availability
- **CLI Functionality Tests**: Tests compression/decompression with various data types
- **Web Interface Tests**: Browser-accessible test pages for web environments
- **Performance Benchmarks**: Measures compression speed and ratios
- **Memory Safety**: Validates proper resource cleanup

Run all CLI tests:

```bash
just test
```

Test web functionality:

```bash
just web-server
# Then visit http://127.0.0.1:10000/web_test.php
```

### Project Structure

```
franken-zstd-go/
├── README.md              # This file
├── LICENSE               # MIT license
├── go.mod               # Go module definition
├── justfile             # Build automation
├── zstd.stub.php        # PHP API definitions
├── zstd_arginfo.h       # Generated PHP argument info
├── franken_zstd.go      # Core Go implementation
├── convert.go           # Type conversion utilities
├── caddy.go            # FrankenPHP integration
├── zstd.h              # C header declarations
├── zstd.c              # C bridge implementation
└── testdata/           # Test files and examples
    ├── extension_check.php
    ├── index.php
    ├── benchmark.php
    └── Caddyfile
```

## Comparison with C Extensions

| Feature          | FrankenPHP Zstd (Go)        | Traditional C Extension       |
| ---------------- | --------------------------- | ----------------------------- |
| Memory Safety    | Garbage collected, no leaks | Manual management, leak-prone |
| Performance      | High (pure Go)              | High (but CGO overhead)       |
| Crash Resistance | Excellent                   | Moderate (segfaults possible) |
| Thread Safety    | Built-in                    | Manual synchronization        |
| Maintenance      | Easier (Go ecosystem)       | Complex (C dependencies)      |
| Platform Support | Consistent                  | Platform-specific issues      |

## Technical Details

### Architecture

- **Frontend**: PHP extension API implemented in C
- **Bridge**: Minimal CGO layer for PHP-Go communication
- **Backend**: Pure Go compression using `github.com/klauspost/compress`
- **Integration**: Native FrankenPHP module registration

### Memory Management

- Encoder/decoder object pooling for efficiency
- Automatic cleanup of Go resources
- Binary-safe data transfer between PHP and Go
- No manual memory management required

### Error Handling

- Comprehensive error propagation from Go to PHP
- Custom exception types for different error conditions
- Graceful handling of invalid data and parameters

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Troubleshooting

### Web Server Issues

**Problem**: `Error: module not registered: http.encoders.br`
**Solution**: Use the Caddyfile approach instead of direct `php-server`:

```bash
# Instead of: ./frankenphp php-server --listen :8080
# Use: ./frankenphp run --config Caddyfile
just web-server
```

**Problem**: 404 errors when accessing web pages
**Solution**: Ensure you're starting the server from the correct directory:

```bash
cd testdata
../frankenphp run --config Caddyfile
```

### Build Issues

**Problem**: `xcaddy` not found
**Solution**: Install xcaddy:

```bash
go install github.com/caddyserver/xcaddy/cmd/xcaddy@latest
```

**Problem**: PHP headers not found during build
**Solution**: Ensure PHP ZTS development headers are installed:

```bash
# macOS with Homebrew or Linuxbrew (use your desired version >=8.2)
brew install shivammathur/php/php@8.4-zts

# Ubuntu/Debian
sudo apt-get install php-dev

# CentOS/RHEL
sudo yum install php-devel
```

### Extension Loading Issues

**Problem**: Extension not loaded (`extension_loaded('franken-zstd-go')` returns false)
**Solution**: Verify the extension is built into the FrankenPHP binary:

```bash
just check-extension
```

## Contributing

Contributions are welcome! Please ensure that:

1. All tests pass (`just test`)
2. Code follows Go and PHP best practices
3. New features include appropriate tests
4. Documentation is updated for API changes

## Support

For issues, questions, or contributions, please visit the project repository or contact the maintainers.

## Acknowledgments

- **FrankenPHP**: For providing the Go-PHP integration framework
- **klauspost/compress**: For the excellent pure Go zstd implementation
- **Zstandard**: For the compression algorithm specification
