# FrankenPHP Zstd Extension - Build Configuration

# Default recipe
default:
    @just --list

# Variables
binary_output := "frankenphp"
php_lib_path := `php-config --prefix` + "/lib"
cgo_cflags := `php-config --includes`
cgo_ldflags := `php-config --ldflags` + " " + `php-config --libs` + " -L" + php_lib_path

# Generate PHP stubs
generate-stubs:
    @echo "Generating PHP stubs..."
    php `php-config --prefix`/lib/php/build/gen_stub.php zstd.stub.php

# Build the FrankenPHP binary with the zstd extension
build: generate-stubs
    @echo "Building FrankenPHP with zstd extension..."
    CGO_ENABLED=1 \
    XCADDY_GO_BUILD_FLAGS="-ldflags='-w -s'" \
    CGO_CFLAGS="{{cgo_cflags}}" \
    CGO_LDFLAGS="{{cgo_ldflags}}" \
    xcaddy build \
        --output {{binary_output}} \
        --with github.com/dunglas/frankenphp/caddy@v1.9.1 \
        --with github.com/phillarmonic/franken-zstd-go=.

# Test the extension
test: build
    @echo "Testing zstd extension..."
    @echo "Checking extension registration..."
    ./{{binary_output}} php-cli testdata/extension_check.php
    @echo ""
    @echo "Running basic functionality tests..."
    ./{{binary_output}} php-cli testdata/index.php
    @echo ""
    @echo "Running benchmark tests..."
    ./{{binary_output}} php-cli testdata/benchmark.php
    @echo ""
    @echo "✅ All tests completed successfully!"

# Check if extension is properly registered
check-extension: build
    @echo "Checking extension registration..."
    ./{{binary_output}} php-cli testdata/extension_check.php

# Clean build artifacts
clean:
    @echo "Cleaning build artifacts..."
    rm -f {{binary_output}}
    rm -f zstd_arginfo.h

# Check dependencies
check-deps:
    @command -v php-config >/dev/null 2>&1 || { echo "❌ php-config not found"; exit 1; }
    @command -v xcaddy >/dev/null 2>&1 || { echo "❌ xcaddy not found"; exit 1; }
    @command -v go >/dev/null 2>&1 || { echo "❌ go not found"; exit 1; }
    @echo "✅ All dependencies found"

# Development server
dev: build
    @echo "Starting development server..."
    ./{{binary_output}} run --config ./testdata/Caddyfile

# Benchmark the extension
benchmark: build
    @echo "Running benchmarks..."
    ./{{binary_output}} php ./testdata/benchmark.php

# Download dependencies
deps:
    @echo "Downloading Go dependencies..."
    go mod download
    go mod tidy
